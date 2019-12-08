/////////////////////////////////////////////////////////////////////////////
// Name:        debugger.h
// Purpose:     Debugger piped process example
// Author:      Julian Smart
// Modified by:
// Created:     2005-1-25
// RCS-ID:      $Id$
// Copyright:   (c) Julian Smart
// Licence:     wxWindows Licence
/////////////////////////////////////////////////////////////////////////////

#ifndef __DEBUGGER_H__
#define __DEBUGGER_H__

class DebuggerProcess;
class DebuggerConsoleCtrl;

/*!
 * A debugger window class (gdb)
 */

class DebuggerWindow: public wxPanel
{
DECLARE_CLASS(DebuggerWindow)
  public:
    DebuggerWindow(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style);

// Accessors

    /// Input/output window
    DebuggerConsoleCtrl* GetConsoleWindow() const { return m_consoleWindow; }

    /// Debugger process
    void SetDebuggerProcess(DebuggerProcess* process) { m_process = process; }
    DebuggerProcess* GetDebuggerProcess() const { return m_process; }

    /// Last known console (text) position
    int GetLastConsolePosition() const { return m_lastConsolePosition; }

// Operations

    /// *** CONSOLE WINDOW FUNCTIONS ***

    /// Adds a line to the output window (appends newline)
    void AddLine(const wxString& line, bool appendNewline = true);

    /// Clears output log, and errors and warnings.
    void ClearConsole();

    /// Gets the number of lines in the log window
    size_t GetConsoleLineCount() ;

    /// Gets the given line in the log window
    wxString GetConsoleLineText(size_t i) ;

    /// Gets the line of text at the current caret position
    wxString GetConsoleLineAtCaret();

    /// Have got a line of text from the user
    void UserCommand(const wxString& line);

    /// *** OTHER FUNCTIONS ***

    /// Clears console
    void Clear();

// Debugger process handling

    /// Notification that a line has been read from the debugger output
    void ReadDebuggerOutput(const wxString& line, bool isErrorStream);

    /// Notification that the debugger process has terminates
    void DebuggerTerminated();

    /// Start debugger
    bool StartDebugger(const wxString& executable, const wxArrayString& dirs);

    /// Kill debugger
    bool KillDebugger();

    /// Are we debugging?
    bool IsDebuggerRunning();

    /// Send a command to the debugger
    bool SendDebugCommand(const wxString& cmd, bool needEcho = false);

// Tools

    /// Add all standard tools
    void AddTools();

    /// Add a tool. If prompt is present, will prompt for an argument for the command.
    int AddTool(int id, const wxBitmap& bitmap, const wxString& label, const wxString& cmd, const wxString& prompt = wxEmptyString);

    /// Finds a tool position
    int FindToolIndex(int id);
    int FindToolIndexByLabel(const wxString& label);

    /// Finds a command for the given tool id
    wxString FindToolCommand(int id);

    /// Finds a prompt for the given tool id
    wxString FindToolPrompt(int id);

    /// Finds a label for the given tool id
    wxString FindToolLabel(int id);

    /// Hands a tool button press
    bool HandleToolButton(int id);

// Event handling

    /// Detect a tool button press
    void OnToolButton(wxCommandEvent& event);

    /// Update toolbar buttons
    void OnUpdateToolButton(wxUpdateUIEvent& event);


DECLARE_EVENT_TABLE()

protected:
    wxToolBar*          m_toolBar;
    DebuggerConsoleCtrl* m_consoleWindow;
    DebuggerProcess*    m_process;
    long                m_pid;
    int                 m_lastConsolePosition;

// TOOLS

    /// Tool labels
    wxArrayString       m_toolLabels;
    wxArrayString       m_toolCommands;
    wxArrayInt          m_toolIds;
    wxArrayString       m_toolPrompts;
};

enum {
    DebuggerFIRST = 100,
    DebuggerStep,
    DebuggerContinue,
    DebuggerNext,
    DebuggerQuit,
    DebuggerPrint,
    DebuggerBreak,
    DebuggerBreakpoints,
    DebuggerTracepoints,
    DebuggerDelete,
    DebuggerBacktrace,
    DebuggerUp,
    DebuggerDown,
    DebuggerReturn,
    DebuggerHelp,
    DebuggerWatch,
    DebuggerRun,
    DebuggerFinish,
    DebuggerLAST
};

#endif
  // __DEBUGGER_H__

