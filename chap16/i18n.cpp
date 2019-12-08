/////////////////////////////////////////////////////////////////////////////
// Name:        i18n.cpp
// Purpose:     Internationalization wxWidgets sample
// Author:      Stefan Csomor
// Modified by:
// Created:     12/12/04
// RCS-ID:      $Id: i18n.cpp,v 1.66 2004/05/25 11:18:01 JS Exp $
// Copyright:   (c) Stefan Csomor
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// ============================================================================
// declarations
// ============================================================================

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

// for all others, include the necessary headers (this file is usually all you
// need because it includes almost all "standard" wxWidgets headers)
#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

#include "wx/datetime.h"
#include "wx/image.h"

// ----------------------------------------------------------------------------
// resources
// ----------------------------------------------------------------------------

#include "sample.xpm"

// ----------------------------------------------------------------------------
// private classes
// ----------------------------------------------------------------------------

// Define a new application type, each program should derive a class from wxApp
class MyApp : public wxApp
{
public:
    ~MyApp() ;

    // Initialize the application
    virtual bool OnInit();
    
    // Recreates m_locale according to lang
    void SelectLanguage(int lang) ;
private:
    wxLocale* m_locale; // 'our' locale
};

// Define a new frame type: this is going to be our main frame
class MyFrame : public wxFrame
{
public:
    // ctor(s)
    MyFrame(const wxString& title);

    // event handlers (these functions should _not_ be virtual)
    void OnQuit(wxCommandEvent& event);
    void OnChangeLanguage(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);

    void SetupStrings() ;

private:
    wxStaticText*   m_helloString ;
    wxStaticText*   m_todayString ;
    wxStaticText*   m_thousandString ;
    wxStaticBitmap* m_flag ;
    // any class wishing to process wxWidgets events must use this macro
    DECLARE_EVENT_TABLE()
};

// ----------------------------------------------------------------------------
// event tables and other macros for wxWidgets
// ----------------------------------------------------------------------------

BEGIN_EVENT_TABLE(MyFrame, wxFrame)
    EVT_MENU(wxID_EXIT,  MyFrame::OnQuit)
    EVT_MENU(wxID_NEW,   MyFrame::OnChangeLanguage)
    EVT_MENU(wxID_ABOUT, MyFrame::OnAbout)
END_EVENT_TABLE()

// Create a new application object
IMPLEMENT_APP(MyApp)

// ============================================================================
// implementation
// ============================================================================

// ----------------------------------------------------------------------------
// the application class
// ----------------------------------------------------------------------------

// 'Main program' equivalent: the program execution "starts" here
bool MyApp::OnInit()
{
    wxImage::AddHandler( new wxPNGHandler );

    m_locale = NULL ;
    SelectLanguage( wxLANGUAGE_DEFAULT ) ;

    MyFrame *frame = new MyFrame(_("i18n wxWidgets App"));

    frame->Show(true);

    return true;
}

void MyApp::SelectLanguage(int lang)
{
    delete m_locale ;
    m_locale = new wxLocale( lang ) ;
    m_locale->AddCatalogLookupPathPrefix(wxGetCwd());
    m_locale->AddCatalog( wxT("i18n") )   ;
}

MyApp::~MyApp()
{
    delete m_locale ;
}
// ----------------------------------------------------------------------------
// main frame
// ----------------------------------------------------------------------------

// frame constructor
MyFrame::MyFrame(const wxString& title)
       : wxFrame(NULL, wxID_ANY, title , wxDefaultPosition , wxSize( 400 , 300 ))
{
    // set the frame icon
    SetIcon(sample_xpm);

    wxPanel *panel  = new wxPanel( this ) ;
    m_helloString   = new wxStaticText( panel , wxID_ANY , wxEmptyString , wxPoint( 10 , 20 ) , wxSize( 100 , 20 ) ) ;
    m_todayString   = new wxStaticText( panel , wxID_ANY , wxEmptyString , wxPoint( 10 , 50 ) , wxSize( 100 , 20 ) ) ;
    m_thousandString = new wxStaticText( panel , wxID_ANY , wxEmptyString , wxPoint( 10 , 80 ) , wxSize( 100 , 20 ) ) ;
    m_flag = new wxStaticBitmap( panel , wxID_ANY , wxBitmap( _("flag.png") , wxBITMAP_TYPE_PNG ) , wxPoint( 10 , 110 ) ) ;

    CreateStatusBar(2);

    SetupStrings() ;
}

void MyFrame::SetupStrings() 
{
    m_helloString->SetLabel( _("Welcome to International Sample") ) ;
    m_todayString->SetLabel( wxString::Format(_("Now is %s") , wxDateTime::Now().Format().c_str() ) ) ;
    m_thousandString->SetLabel( wxString::Format(_("12345 divided by 10 is written as %.1f") , 1234.5 ) ) ;
    m_flag->SetBitmap(wxBitmap( _("flag.png") , wxBITMAP_TYPE_PNG )) ;

    // create a menu bar
    wxMenu *menuFile = new wxMenu;

    // the "About" item should be in the help menu
    wxMenu *helpMenu = new wxMenu;
    helpMenu->Append(wxID_ABOUT, _("&About...\tF1"), wxT("Show about dialog"));

    menuFile->Append(wxID_NEW, _("Change language..."), wxT("Select a new language"));
    menuFile->AppendSeparator();
    menuFile->Append(wxID_EXIT, _("E&xit\tAlt-X"), wxT("Quit this program"));

    wxMenuBar *menuBar = new wxMenuBar();
    menuBar->Append(menuFile, _("&File"));
    menuBar->Append(helpMenu, _("&Help"));

    wxMenuBar* formerMenuBar = GetMenuBar() ;
    SetMenuBar(menuBar);
    delete formerMenuBar ;

    SetStatusText(_("Welcome to wxWidgets!"));
}

// event handlers

void MyFrame::OnChangeLanguage(wxCommandEvent& WXUNUSED(event))
{
    wxArrayInt languageCodes ;
    wxArrayString languageNames ;

    languageCodes.Add(wxLANGUAGE_GERMAN) ;
    languageNames.Add(_("German")) ;

    languageCodes.Add(wxLANGUAGE_FRENCH) ;
    languageNames.Add(_("French")) ;

    languageCodes.Add(wxLANGUAGE_ENGLISH) ;
    languageNames.Add(_("English")) ;

    int lang = wxGetSingleChoiceIndex( _("Select language:"), _("Language"), languageNames );

    if ( lang != -1 )
    {
        wxGetApp().SelectLanguage(languageCodes[lang]);
        SetupStrings();
    }
}

void MyFrame::OnQuit(wxCommandEvent& WXUNUSED(event))
{
    // true is to force the frame to close
    Close(true);
}

void MyFrame::OnAbout(wxCommandEvent& WXUNUSED(event))
{
    wxMessageBox( _("This is the About dialog.") , _("i18n wxWidgets App") ) ;
}
