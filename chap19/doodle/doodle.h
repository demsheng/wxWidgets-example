/////////////////////////////////////////////////////////////////////////////
// Name:        doodle.h
// Purpose:     Document/view Doodle sample
// Author:      Julian Smart
// Modified by:
// Created:     2004-11-07
// Copyright:   (c) Julian Smart
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef __DOODLEH__
#define __DOODLEH__

#include "wx/docview.h"

class wxDocManager;

/*
 * Declare an application class
 */

class DoodleApp: public wxApp
{
public:
    DoodleApp();

    /// Called on app initialization
    virtual bool OnInit();

    /// Called on app exit
    virtual int OnExit();

private:
    wxDocManager* m_docManager;
};

DECLARE_APP(DoodleApp)

// Define a new frame
class DoodleCanvas;
class DoodleFrame: public wxDocParentFrame
{
    DECLARE_CLASS(DoodleFrame)
    DECLARE_EVENT_TABLE()
public:
    DoodleFrame(wxDocManager *manager, wxFrame *frame, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size,
        long type);

    /// Show About box
    void OnAbout(wxCommandEvent& event);

    /// Get edit menu
    wxMenu* GetEditMenu() const { return m_editMenu; }

    /// Get canvas
    DoodleCanvas* GetCanvas() const { return m_canvas; }

private:
    wxMenu *        m_editMenu;
    DoodleCanvas*   m_canvas;
};

extern DoodleFrame *GetMainFrame();

#define DOCVIEW_CUT     1
#define DOCVIEW_ABOUT   2

#endif
    // __DOODLEH__

