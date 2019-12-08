/////////////////////////////////////////////////////////////////////////////
// Name:        sample.cpp
// Purpose:     Thumbnail sample
// Author:      Julian Smart
// Modified by:
// Created:     2005-03-01
// RCS-ID:      $Id$
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

#include "wx/image.h"
#include "thumbnaildlg.h"

#include "sample.xpm"

class MyApp : public wxApp
{
public:
    virtual bool OnInit();
};

class MyFrame : public wxFrame
{
public:
    // ctor(s)
    MyFrame(const wxString& title);

    void OnQuit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
    void OnBrowse(wxCommandEvent& event);

private:
    // any class wishing to process wxWidgets events must use this macro
    DECLARE_EVENT_TABLE()
};

// IDs for the controls and the menu commands
enum
{
    Sample_Quit = wxID_EXIT,

    Sample_About = wxID_ABOUT,
    
    // Show the image browser
    Sample_Browse
};

BEGIN_EVENT_TABLE(MyFrame, wxFrame)
    EVT_MENU(Sample_Quit,  MyFrame::OnQuit)
    EVT_MENU(Sample_About, MyFrame::OnAbout)
    EVT_MENU(Sample_Browse, MyFrame::OnBrowse)
END_EVENT_TABLE()

IMPLEMENT_APP(MyApp)

bool MyApp::OnInit()
{
    MyFrame *frame = new MyFrame(_T("Thumbnail Browser"));

    frame->Show(true);

    return true;
}

MyFrame::MyFrame(const wxString& title)
       : wxFrame(NULL, wxID_ANY, title)
{
    // set the frame icon
    SetIcon(wxIcon(sample_xpm));
    
    wxInitAllImageHandlers();

    wxMenu *fileMenu = new wxMenu;

    wxMenu *helpMenu = new wxMenu;
    helpMenu->Append(Sample_About, _T("&About...\tF1"), _T("Show about dialog"));
    fileMenu->Append(Sample_Browse, _T("&Browse..."), _T("Browse images"));
    fileMenu->Append(Sample_Quit, _T("E&xit\tAlt-X"), _T("Quit this program"));

    wxMenuBar *menuBar = new wxMenuBar();
    menuBar->Append(fileMenu, _T("&File"));
    menuBar->Append(helpMenu, _T("&Help"));

    SetMenuBar(menuBar);

    CreateStatusBar(2);
    SetStatusText(_T("Welcome to wxWidgets!"));
    
    Centre();
}

void MyFrame::OnQuit(wxCommandEvent& WXUNUSED(event))
{
    // true is to force the frame to close
    Close(true);
}

void MyFrame::OnAbout(wxCommandEvent& WXUNUSED(event))
{
    wxString msg;
    msg.Printf( _T("Thumbnail Browser Sample.\n")
                _T("Welcome to %s"), wxVERSION_STRING);

    wxMessageBox(msg, _T("About This App"), wxOK | wxICON_INFORMATION, this);
}

void MyFrame::OnBrowse(wxCommandEvent& WXUNUSED(event))
{
    wxThumbnailBrowserDialog dlg(this, wxID_ANY);
    
    wxString initialPath = wxGetCwd() + wxFILE_SEP_PATH + wxT("images");
    if (wxDirExists(initialPath))
    {
        dlg.SetSelection(initialPath);
    }
    if (wxID_OK == dlg.ShowModal())
    {
        wxString sel = dlg.GetSelection();
        wxMessageBox(sel);
    }
}

