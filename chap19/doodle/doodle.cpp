/////////////////////////////////////////////////////////////////////////////
// Name:        doodle.cpp
// Purpose:     Document/view sample
// Author:      Julian Smart
// Modified by:
// Created:     2004-11-07
// Copyright:   (c) Julian Smart
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "wx/docview.h"
#include "wx/filename.h"

#include "doodle.h"
#include "doc.h"
#include "view.h"

#include "doodle.xpm"

IMPLEMENT_APP(DoodleApp)

DoodleApp::DoodleApp()
{
    m_docManager = NULL;
}

bool DoodleApp::OnInit()
{
    // Create a document manager
    m_docManager = new wxDocManager;

    // Create a template relating drawing documents to their views
    (void) new wxDocTemplate(m_docManager, wxT("Doodle"), wxT("*.drw"), wxT(""), wxT("drw"), wxT("Doodle Doc"), wxT("Doodle View"),
        CLASSINFO(DoodleDocument), CLASSINFO(DoodleView));

    // Register the drawing document type on Mac
#ifdef __WXMAC__
    wxFileName::MacRegisterDefaultTypeAndCreator( wxT("drw") , 'WXMB' , 'WXMA' ) ;
#endif
    
    // If we've only got one window, we only get to edit
    // one document at a time.
    m_docManager->SetMaxDocsOpen(1);
    
    // Create the main frame window
    DoodleFrame* frame = new DoodleFrame(m_docManager, NULL, wxID_ANY, wxT("Doodle Sample"), wxPoint(0, 0), wxSize(500, 400), wxDEFAULT_FRAME_STYLE);    
    
    frame->Centre(wxBOTH);
    frame->Show(true);
    
    SetTopWindow(frame);
    return true;
}

int DoodleApp::OnExit()
{
    delete m_docManager;
    return 0;
}

/*
* This is the top-level window of the application.
*/

IMPLEMENT_CLASS(DoodleFrame, wxDocParentFrame)

BEGIN_EVENT_TABLE(DoodleFrame, wxDocParentFrame)
    EVT_MENU(DOCVIEW_ABOUT, DoodleFrame::OnAbout)
END_EVENT_TABLE()

DoodleFrame::DoodleFrame(wxDocManager *manager, wxFrame *parent, wxWindowID id, const wxString& title,
                 const wxPoint& pos, const wxSize& size, long type):
wxDocParentFrame(manager, parent, id, title, pos, size, type)
{
    m_editMenu = NULL;

    m_canvas = new DoodleCanvas(this, wxDefaultPosition, wxDefaultSize, 0);
    m_canvas->SetCursor(wxCursor(wxCURSOR_PENCIL));
    
    // Give it scrollbars
    m_canvas->SetScrollbars(20, 20, 50, 50);
    m_canvas->SetBackgroundColour(*wxWHITE);
    m_canvas->ClearBackground();    

    // Give it an icon
    SetIcon(wxIcon(doodle_xpm));
    
    // Make a menubar
    wxMenu *fileMenu = new wxMenu;
    wxMenu *editMenu = (wxMenu *) NULL;
    
    fileMenu->Append(wxID_NEW, wxT("&New..."));
    fileMenu->Append(wxID_OPEN, wxT("&Open..."));
    
    fileMenu->Append(wxID_CLOSE, wxT("&Close"));
    fileMenu->Append(wxID_SAVE, wxT("&Save"));
    fileMenu->Append(wxID_SAVEAS, wxT("Save &As..."));
    fileMenu->AppendSeparator();
    fileMenu->Append(wxID_PRINT, wxT("&Print..."));
    fileMenu->Append(wxID_PRINT_SETUP, wxT("Print &Setup..."));
    fileMenu->Append(wxID_PREVIEW, wxT("Print Pre&view"));
        
    editMenu = new wxMenu;
    editMenu->Append(wxID_UNDO, wxT("&Undo"));
    editMenu->Append(wxID_REDO, wxT("&Redo"));
    editMenu->AppendSeparator();
    editMenu->Append(DOCVIEW_CUT, wxT("&Cut last segment"));
        
    m_editMenu = editMenu;
    
    fileMenu->AppendSeparator();
    fileMenu->Append(wxID_EXIT, wxT("E&xit"));
    
    wxMenu *helpMenu = new wxMenu;
    helpMenu->Append(DOCVIEW_ABOUT, wxT("&About"));
    
    wxMenuBar *menuBar = new wxMenuBar;
    
    menuBar->Append(fileMenu, wxT("&File"));
    menuBar->Append(editMenu, wxT("&Edit"));
    menuBar->Append(helpMenu, wxT("&Help"));
    
    // Associate the menu bar with the frame
    SetMenuBar(menuBar);
    
    // A nice touch: a history of files visited. Use this menu.
    manager->FileHistoryUseMenu(fileMenu);
}

void DoodleFrame::OnAbout(wxCommandEvent& WXUNUSED(event) )
{
    (void)wxMessageBox(wxT("Doodle Sample\n(c) 2005, Julian Smart"), wxT("About Doodle"));
}

DoodleFrame *GetMainFrame()
{
    return (DoodleFrame*) wxGetApp().GetTopWindow();
}

