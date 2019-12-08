/////////////////////////////////////////////////////////////////////////////
// Name:        debugger.cpp
// Purpose:     Debugger piped process example
// Author:      Julian Smart
// Modified by:
// Created:     2005-1-25
// RCS-ID:      $Id$
// Copyright:   (c) Julian Smart
// Licence:	    wxWindows Licence
/////////////////////////////////////////////////////////////////////////////

#include "wx/wx.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#include "wx/process.h"
#include "wx/txtstrm.h"

#include "textctrlex.h"
#include "debugger.h"
#include "processapp.h"

// Implements a piped process

class DebuggerProcess : public PipedProcess
{
public:
    DECLARE_CLASS(DebuggerProcess)

    DebuggerProcess(DebuggerWindow* window): PipedProcess(window), m_debugWindow(window)
    {
        Redirect();
    }

    virtual void OnTerminate(int pid, int status);

    virtual bool HasInput();

    /// Feed it some input
    void SendInput(const wxString& text);

protected:
    DebuggerWindow*   m_debugWindow;
    wxString          m_input; // to send to process
};

IMPLEMENT_CLASS(DebuggerProcess, PipedProcess)

/*!
 * A specialisation of wxTextCtrlEx so we can process return and
 * send the line to gdb
 */

class DebuggerConsoleCtrl: public wxTextCtrlEx
{
DECLARE_CLASS(DebuggerConsoleCtrl)
public:
    DebuggerConsoleCtrl(
        wxWindow* parent, wxWindowID id, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize,
        int style = 0);

// Operations

    /// Append text setting m_writing flag
    void Append(const wxString& line);

// Event handlers

    /// Character input
    void OnCharAdded (wxStyledTextEvent &event);

// Accessors

    // Ignore character input if writing to the control
    void SetWriting(bool writing) { m_writing = writing; }
    bool GetWriting() const { return m_writing; }
protected:

    bool    m_writing;

DECLARE_EVENT_TABLE()
};


/*!
 * A debugger window class
 */

IMPLEMENT_CLASS(DebuggerWindow, wxPanel)

BEGIN_EVENT_TABLE(DebuggerWindow, wxPanel)
EVT_MENU_RANGE(DebuggerFIRST, DebuggerLAST, DebuggerWindow::OnToolButton)
EVT_UPDATE_UI_RANGE(DebuggerFIRST, DebuggerLAST, DebuggerWindow::OnUpdateToolButton)
END_EVENT_TABLE()

DebuggerWindow::DebuggerWindow(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style):
  wxPanel(parent, id, pos, size, style)
{
    m_process = NULL;
    m_pid = 0;
    m_lastConsolePosition = 0;

    wxSize toolBarSize = wxDefaultSize;

    m_toolBar = new wxToolBar(this, -1, wxDefaultPosition, toolBarSize, wxTB_FLAT|wxTB_NODIVIDER|wxTB_NOALIGN);

    // Set specified window to ancestor notebook theme colour
    // wxSetWindowToNotebookColour(m_toolBar);
    m_toolBar->SetBackgroundColour(*wxRED);

    m_consoleWindow = new DebuggerConsoleCtrl(this, -1, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER);
    m_consoleWindow->SetWrapMode (wxSTC_WRAP_WORD); // wxSTC_WRAP_NONE
    m_consoleWindow->SetMarginWidth (0,0);
    m_consoleWindow->SetMarginWidth (1,0);
    m_consoleWindow->SetMarginWidth (2,0);

    AddTools();
    m_toolBar->Realize();

    wxBoxSizer* topSizer = new wxBoxSizer(wxVERTICAL);
    SetSizer(topSizer);
    topSizer->Add(m_toolBar, 0, wxEXPAND|wxALL, 0);
    topSizer->Add(m_consoleWindow, 1, wxEXPAND|wxALL, 0);
}

static wxString wbStripNewlines(const wxString& line)
{
    wxString l = line.BeforeFirst(wxT('\n'));
    l = l.BeforeFirst(wxT('\r'));
    return l;
}

/// Adds a line to the output window (appends newline)
void DebuggerWindow::AddLine(const wxString& line, bool appendNewLine)
{
    wxString l = line;
    if (appendNewLine)
        l += wxT("\n");
    m_consoleWindow->Append(l);
    int pos = m_consoleWindow->PositionFromLine(m_consoleWindow->GetLineCount()-1);
    pos += m_consoleWindow->GetLine(m_consoleWindow->GetLineCount()-1).Length();
    m_consoleWindow->SetSelectionStart(pos);
    m_consoleWindow->SetCurrentPos(pos);
    m_consoleWindow->EnsureCaretVisible();
    m_lastConsolePosition = pos;
}

/// Adds errors and warnings from the given list of output lines
void DebuggerWindow::Clear()
{
    m_consoleWindow->ClearAll();
}

/// Gets the number of lines in the log window
size_t DebuggerWindow::GetConsoleLineCount()
{
    return m_consoleWindow->GetLineCount();
}

/// Gets the given line in the log window
wxString DebuggerWindow::GetConsoleLineText(size_t i)
{
    return m_consoleWindow->GetLine(i);
}

/// Gets the line of text at the current caret position
wxString DebuggerWindow::GetConsoleLineAtCaret()
{
    int l = m_consoleWindow->LineFromPosition(m_consoleWindow->GetCurrentPos());
    if (l >= 0)
    {
        wxString line = m_consoleWindow->GetLine(l);
        return line;
    }
    return wxEmptyString;
}

/// Notification that a line has been read from the debugger output
void DebuggerWindow::ReadDebuggerOutput(const wxString& line, bool isErrorStream)
{
    AddLine(line, false);
}

/// Notification that the debugger process has terminates
void DebuggerWindow::DebuggerTerminated()
{
    wxGetApp().UnregisterProcess(m_process);
    m_process = NULL;
}

/// Are we debugging?
bool DebuggerWindow::IsDebuggerRunning()
{
    return (m_process != NULL);
}

/// Start debugger
bool DebuggerWindow::StartDebugger(const wxString& executable, const wxArrayString& searchDirs)
{
    if (m_process)
    {
        Clear();

        if (!KillDebugger() || IsDebuggerRunning())
        {
            wxMessageBox(_("Sorry, could not stop the debugger. Please stop it manually."), _("DialogBlocks"), wxOK|wxICON_EXCLAMATION, this);
            return false;
        }

        return false;
    }

    wxString exe = executable;

    wxString cmd = wxT("gdb -nw ");
    cmd += wxT("\"");
    cmd += exe;
    cmd += wxT("\"");

    size_t i;
    for (i = 0; i < searchDirs.GetCount(); i++)
    {
        wxString dir = searchDirs[i];
        // Spaces in directories cause an error on Windows
#ifdef __WXMSW__
        if (dir.Find(wxT(" ")) != wxNOT_FOUND)
        {
            wxFileName fname(dir);
            dir = fname.GetShortPath();
        }
#endif
        cmd += wxT(" --directory=\"");
        cmd += dir;
        cmd += wxT("\"");
    }

    AddLine(cmd);

    DebuggerProcess *process = new DebuggerProcess (this);
    m_pid = wxExecute(cmd, wxEXEC_ASYNC, process);

    if ( !m_pid )
    {
        wxLogError( _T("Execution of '%s' failed. gdb may not be installed."), cmd.c_str() );

        delete process;
        return false;
    }
    else
    {
        m_process = process;

        wxGetApp().RegisterProcess(process);

        return true;
    }
    return true;
}

/// Kill debugger
bool DebuggerWindow::KillDebugger()
{
    if (!m_process)
        return true;

    // Fourth parameter exists in 2.5.4 and above
    return wxKill(m_pid, wxSIGKILL, NULL, wxKILL_CHILDREN) == 0;
}

/// Have got a line of text from the user
void DebuggerWindow::UserCommand(const wxString& line)
{
    wxString cmd = line;
    int pos = cmd.Find(wxT("(gdb)"));
    if (pos != wxNOT_FOUND)
        cmd = cmd.Mid(pos + 5);

    cmd = cmd.BeforeFirst(wxT('\n'));
    cmd.Trim(true);
    cmd.Trim(false);

    SendDebugCommand(cmd);
}

/// Send a command to the debugger
bool DebuggerWindow::SendDebugCommand(const wxString& cmd, bool needEcho)
{
    if (m_process && m_process->GetOutputStream())
    {
        wxString c = cmd;
        c += wxT("\n");

        if (needEcho)
            AddLine(cmd);

        m_process->SendInput(c);
        return true;
    }
    return false;
}

// Bitmaps not supplied: please supply your own.

#include "bitmaps/debugger_run.xpm"
#include "bitmaps/debugger_continue.xpm"
#include "bitmaps/debugger_backtrace.xpm"
#include "bitmaps/debugger_breakpoint.xpm"
#include "bitmaps/debugger_stepinto.xpm"
#include "bitmaps/debugger_next.xpm"
#include "bitmaps/debugger_stepout.xpm"
#include "bitmaps/help_small.xpm"
#include "bitmaps/delete.xpm"

/// Add all standard tools
void DebuggerWindow::AddTools()
{
    AddTool(DebuggerBreak, wxBitmap(debugger_breakpoint_xpm), wxT("Breakpoint"), wxT("break"), wxT("Please enter <function> or <sourcefile>:<line>"));
    AddTool(DebuggerRun, wxBitmap(debugger_run_xpm), wxT("Run"), wxT("run"));
    AddTool(DebuggerContinue, wxBitmap(debugger_continue_xpm), wxT("Continue"), wxT("continue"));
    AddTool(DebuggerStep, wxBitmap(debugger_stepinto_xpm), wxT("Step"), wxT("step"));
    AddTool(DebuggerNext, wxBitmap(debugger_next_xpm), wxT("Next"), wxT("next"));
    AddTool(DebuggerFinish, wxBitmap(debugger_stepout_xpm), wxT("Finish"), wxT("finish"));
    AddTool(DebuggerBacktrace, wxBitmap(debugger_backtrace_xpm), wxT("Backtrace"), wxT("backtrace"));
    AddTool(DebuggerQuit, wxBitmap(delete_xpm), wxT("Quit"), wxT("quit"));
    AddTool(DebuggerHelp, wxBitmap(help_small_xpm), wxT("Help"), wxT("help"));
}

/// Add a tool
int DebuggerWindow::AddTool(int id, const wxBitmap& bitmap, const wxString& label, const wxString& cmd, const wxString& prompt)
{
    int pos = m_toolLabels.Add(label);
    m_toolIds.Add(id);
    m_toolCommands.Add(cmd);
    m_toolPrompts.Add(prompt);

    m_toolBar->AddTool(id, bitmap, wxNullBitmap, FALSE, -1, -1, (wxObject *) NULL, label);

    return pos;
}

/// Finds a tool position
int DebuggerWindow::FindToolIndex(int id)
{
    size_t i;
    for (i = 0; i < m_toolIds.GetCount(); i++)
    {
        if (m_toolIds[i] == id)
            return i;
    }
    return -1;
}

int DebuggerWindow::FindToolIndexByLabel(const wxString& label)
{
    size_t i;
    for (i = 0; i < m_toolLabels.GetCount(); i++)
    {
        if (m_toolLabels[i] == label)
            return i;
    }
    return -1;
}

/// Finds a command for the given tool id
wxString DebuggerWindow::FindToolCommand(int id)
{
    return m_toolCommands[FindToolIndex(id)];
}

/// Finds a prompt for the given tool id
wxString DebuggerWindow::FindToolPrompt(int id)
{
    return m_toolPrompts[FindToolIndex(id)];
}

/// Finds a label for the given tool id
wxString DebuggerWindow::FindToolLabel(int id)
{
    return m_toolLabels[FindToolIndex(id)];
}

/// Hands a tool button press
bool DebuggerWindow::HandleToolButton(int id)
{
    if (!m_process)
        return false;

    wxString label = FindToolLabel(id);
    wxString cmd = FindToolCommand(id);
    wxString prompt = FindToolPrompt(id);

    if (!prompt.IsEmpty())
    {
        wxString args = wxGetTextFromUser(prompt, label + _(" command"), wxEmptyString, this);
        if (!args.IsEmpty())
        {
            cmd += wxT(" ");
            cmd += args;
        }
    }

    SendDebugCommand(cmd, true);

    return true;
}

/// Detect a tool button press
void DebuggerWindow::OnToolButton(wxCommandEvent& event)
{
    HandleToolButton(event.GetId());
}

/// Update toolbar buttons
void DebuggerWindow::OnUpdateToolButton(wxUpdateUIEvent& event)
{
    event.Enable(m_process != NULL);
}

/*!
 * Debugger process
 */

void DebuggerProcess::OnTerminate(int pid, int status)
{
    // show the rest of the output
    while ( HasInput() )
        ;

    m_debugWindow->DebuggerTerminated();

    delete this;
}

bool DebuggerProcess::HasInput()
{
    bool hasInput = false;
    static wxChar buffer[4096];

    if ( !m_input.IsEmpty() )
    {
        wxTextOutputStream os(*GetOutputStream());
        os.WriteString(m_input);
        m_input.Empty();

        hasInput = true;
    }

    if ( IsErrorAvailable() )
    {
        buffer[GetErrorStream()->Read(buffer, WXSIZEOF(buffer) - 1).LastRead()] = _T('\0');
        wxString msg(buffer);

        m_debugWindow->ReadDebuggerOutput(msg, true);

        hasInput = true;
    }

    if ( IsInputAvailable() )
    {
        buffer[GetInputStream()->Read(buffer, WXSIZEOF(buffer) - 1).LastRead()] = _T('\0');
        wxString msg(buffer);

        m_debugWindow->ReadDebuggerOutput(buffer, false);

        hasInput = true;
    }

    return hasInput;
}

/// Feed it some input
void DebuggerProcess::SendInput(const wxString& text)
{
    m_input = text;
}

/*!
 * A specialisation of wxTextCtrlEx so we can process return and
 * send the line to gdb
 */

IMPLEMENT_CLASS(DebuggerConsoleCtrl, wxTextCtrlEx)

BEGIN_EVENT_TABLE(DebuggerConsoleCtrl, wxTextCtrlEx)
    EVT_STC_CHARADDED (-1, DebuggerConsoleCtrl::OnCharAdded)
END_EVENT_TABLE()

DebuggerConsoleCtrl::DebuggerConsoleCtrl(
  wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, int style):
    wxTextCtrlEx(parent, id, wxEmptyString, pos, size, style)
{
    m_writing = false;
}

/// Append text setting m_writing flag
void DebuggerConsoleCtrl::Append(const wxString& line)
{
    m_writing = true;
    AppendText(line.Length(), line);
    m_writing = false;
}

/// Character input
void DebuggerConsoleCtrl::OnCharAdded(wxStyledTextEvent &event)
{
    if (m_writing)
    {
        event.Skip();
        return; // do nothing during writing
    }
    char chr = event.GetKey();
    if (chr == wxT('\n'))
    {
        DebuggerWindow* parent = wxDynamicCast(GetParent(), DebuggerWindow);
        wxString text = GetTextRange(parent->GetLastConsolePosition(), GetCurrentPos());
        parent->UserCommand(text);
        return;
    }
    event.Skip();
}

