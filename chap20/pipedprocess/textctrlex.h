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
#pragma interface "textctrlex.cpp"
#endif

#ifndef __TEXTCTRLEX_H__
#define __TEXTCTRLEX_H__

#include <wx/stc/stc.h>

/*!
 * wxTextCtrlEx
 */

class wxTextCtrlEx: public wxStyledTextCtrl
{
DECLARE_CLASS(wxTextCtrlEx)
public:
    wxTextCtrlEx(
        wxWindow* parent, wxWindowID id, const wxString& value = wxEmptyString,
       const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxTE_MULTILINE);

    ~wxTextCtrlEx();

// Event handlers

    void OnCut(wxCommandEvent& event);
    void OnCopy(wxCommandEvent& event);
    void OnPaste(wxCommandEvent& event);
    void OnUndo(wxCommandEvent& event);
    void OnRedo(wxCommandEvent& event);

    void OnUpdateCut(wxUpdateUIEvent& event);
    void OnUpdateCopy(wxUpdateUIEvent& event);
    void OnUpdatePaste(wxUpdateUIEvent& event);
    void OnUpdateUndo(wxUpdateUIEvent& event);
    void OnUpdateRedo(wxUpdateUIEvent& event);

// Operations

// Compatibility with wxTextCtrl

    /// Sets the string value.
    void SetValue(const wxString& value);

    /// Gets the string value.
    wxString GetValue() ;

    /// Sets the insertion point.
    void SetInsertionPoint(long i);

    /// Loads the file.
    bool LoadFile(const wxString& filename);

    /// Saves the file.
    bool SaveFile(const wxString& filename);

    /// Can we cut text?
    bool CanCut();

    /// Can we copy text?
    bool CanCopy();

    /// Gets the last position in the control.
    long GetLastPosition() ;

    /// Gets the text in the given range.
    wxString GetRange(long start, long end) ;

    /// Displays content at the given position.
    void ShowPosition(long pos);

    /// Replaces text in the given range.
    void Replace(long from, long to, const wxString& value);

    /// Returns the insertion point.
    long GetInsertionPoint() ;

    /// Returns the range of the current selection.
    void GetSelection(long* from, long* to) ;

    /// Has the text been modified?
    bool IsModified() ;

    /// Discards edits
    void DiscardEdits();

    /// Returns the number of lines
    int GetNumberOfLines() ;

    static wxList& GetEditorCtrls() { return sm_editorCtrls; }

protected:
    static wxList sm_editorCtrls;

    bool    m_loading;


DECLARE_EVENT_TABLE()
};

#endif
    // __STYLEDTEXT_H__

