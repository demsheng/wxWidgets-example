/////////////////////////////////////////////////////////////////////////////
// Name:        textctrlex.h
// Purpose:     Styled text control with wxTextCtrl compatibility functions
// Author:      Julian Smart
// Modified by:
// Created:     2005-01-25
// RCS-ID:      $Id$
// Copyright:   (c) Julian Smart
// Licence:		wxWindows Licence
/////////////////////////////////////////////////////////////////////////////

#ifdef __GNUG__
    #pragma implementation "textctrlex.h"
#endif

// For compilers that support precompilation, includes "wx.h".
#include "wx/wx.h"

#include <wx/file.h>
#include <wx/wfstream.h>
#include <wx/variant.h>
#include <wx/filename.h>
#include <wx/file.h>

#include "textctrlex.h"

/*
 * wxTextCtrlEx
 */

IMPLEMENT_CLASS(wxTextCtrlEx, wxStyledTextCtrl)
BEGIN_EVENT_TABLE(wxTextCtrlEx, wxStyledTextCtrl)
    EVT_MENU(wxID_CUT, wxTextCtrlEx::OnCut)
    EVT_MENU(wxID_PASTE, wxTextCtrlEx::OnPaste)
    EVT_MENU(wxID_COPY, wxTextCtrlEx::OnCopy)
    EVT_MENU(wxID_UNDO, wxTextCtrlEx::OnUndo)
    EVT_MENU(wxID_REDO, wxTextCtrlEx::OnRedo)

    EVT_UPDATE_UI(wxID_CUT, wxTextCtrlEx::OnUpdateCut)
    EVT_UPDATE_UI(wxID_COPY, wxTextCtrlEx::OnUpdateCopy)
    EVT_UPDATE_UI(wxID_PASTE, wxTextCtrlEx::OnUpdatePaste)
    EVT_UPDATE_UI(wxID_UNDO, wxTextCtrlEx::OnUpdateUndo)
    EVT_UPDATE_UI(wxID_REDO, wxTextCtrlEx::OnUpdateRedo)

END_EVENT_TABLE()

wxList wxTextCtrlEx::sm_editorCtrls;

wxTextCtrlEx::wxTextCtrlEx(
        wxWindow* parent, wxWindowID id, const wxString& value,
       const wxPoint& pos, const wxSize& size, long style):
    wxStyledTextCtrl(parent, id, pos, size, style)
{
    sm_editorCtrls.Append(this);

    m_loading = false;

    SetTabWidth(4);
    SetUseTabs(FALSE);

    // Put line numbers in the margin ( ie the gutter )
    SetMarginType( 0, wxSTC_MARGIN_NUMBER );

    // Format the font of the line numbers, and the margin width
#ifdef __WXMSW__
    StyleSetSpec( wxSTC_STYLE_LINENUMBER, wxT("size:7,face:Arial,fore:#666666") );
    SetMarginWidth( 0, 18 );
#else
    StyleSetSpec( wxSTC_STYLE_LINENUMBER, wxT("size:9,face:Helvetica") );
    SetMarginWidth( 0, 18 );
#endif
}

wxTextCtrlEx::~wxTextCtrlEx()
{
    sm_editorCtrls.DeleteObject(this);
}


void wxTextCtrlEx::OnPaste(wxCommandEvent& event)
{
    if (CanPaste())
    {
        Paste();
    }
}

void wxTextCtrlEx::OnCut(wxCommandEvent& event)
{
    if (CanCut())
    {
        Cut();
    }
}

void wxTextCtrlEx::OnCopy(wxCommandEvent& WXUNUSED(event))
{
    Copy();
}

void wxTextCtrlEx::OnUndo(wxCommandEvent& WXUNUSED(event))
{
    Undo();
}

void wxTextCtrlEx::OnRedo(wxCommandEvent& WXUNUSED(event))
{
    Redo();
}

void wxTextCtrlEx::OnUpdateCut(wxUpdateUIEvent& event)
{
    event.Enable( CanCut() );
}

void wxTextCtrlEx::OnUpdateCopy(wxUpdateUIEvent& event)
{
    event.Enable( CanCopy() );
}

void wxTextCtrlEx::OnUpdatePaste(wxUpdateUIEvent& event)
{
    event.Enable( CanPaste() );
}

void wxTextCtrlEx::OnUpdateUndo(wxUpdateUIEvent& event)
{
    event.Enable( CanUndo() );
}

void wxTextCtrlEx::OnUpdateRedo(wxUpdateUIEvent& event)
{
    event.Enable( CanRedo() );
}

void wxTextCtrlEx::SetValue(const wxString& value)
{
    m_loading = true;

    ClearAll();
    AddText(value);
    EmptyUndoBuffer();

    m_loading = false;
}

wxString wxTextCtrlEx::GetValue()
{
    int lastPos = wxMax(0, GetLineEndPosition(GetLineCount()));
    if (lastPos > 0)
        lastPos ++;

    return GetTextRange(0, lastPos);
}

void wxTextCtrlEx::SetInsertionPoint(long i)
{
    GotoPos(i);
}

long wxTextCtrlEx::GetLastPosition()
{
    return GetLineEndPosition(GetLineCount());
}

wxString wxTextCtrlEx::GetRange(long start, long end)
{
    return GetTextRange(start, end);
}

void wxTextCtrlEx::ShowPosition(long pos)
{
    int line = LineFromPosition(pos);

    EnsureVisible(line);
}

void wxTextCtrlEx::Replace(long from, long to, const wxString& value)
{
    BeginUndoAction();
    SetSelection(from, to);
    ReplaceSelection(value);
    EndUndoAction();
}

long wxTextCtrlEx::GetInsertionPoint()
{
    return GetCurrentPos();
}

void wxTextCtrlEx::GetSelection(long* from, long* to)
{
    *from = GetSelectionStart();
    *to = GetSelectionEnd();
}

bool wxTextCtrlEx::LoadFile(const wxString& filename)
{
    // give it some text to play with
    wxFile file(filename);
    if (!file.IsOpened())
        return FALSE;

    m_loading = true;

    wxString st;

    char* buf = new char[file.Length() + 1];
    file.Read(buf, file.Length());
    buf[file.Length()] = 0;
    st = wxString::FromAscii(buf);
    delete[] buf;

    ClearAll();
    InsertText(0, st);
    EmptyUndoBuffer();

    m_loading = false;

    return TRUE;
}

/// Saves the file.
bool wxTextCtrlEx::SaveFile(const wxString& filename)
{
    wxString str = GetValue();
    if (!filename.IsEmpty())
    {
        wxFile file(filename, wxFile::write);
        if (file.IsOpened())
        {
            file.Write(str.mb_str(), str.Length());
            return TRUE;
        }
        return FALSE;
    }
    else
        return FALSE;
}

bool wxTextCtrlEx::CanCut()
{
    return (GetSelectionStart() != GetSelectionEnd());
}

bool wxTextCtrlEx::CanCopy()
{
    return (GetSelectionStart() != GetSelectionEnd());
}

bool wxTextCtrlEx::IsModified()
{
    return GetModify();
}

void wxTextCtrlEx::DiscardEdits()
{
    EmptyUndoBuffer();
}

int wxTextCtrlEx::GetNumberOfLines()
{
    return GetLineCount();
}
