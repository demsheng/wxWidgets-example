/////////////////////////////////////////////////////////////////////////////
// Name:        wizard.cpp
// Purpose:     wxWidgets sample demonstrating wxWizard control
// Author:      Vadim Zeitlin
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

#include "wx/wx.h"
#include "wx/wizard.h"

#include "wiztest.xpm"

// ----------------------------------------------------------------------------
// constants
// ----------------------------------------------------------------------------

// ids for menu items
enum
{
    Wizard_Quit = 100,
    Wizard_Run,
    Wizard_About = 1000
};

// ----------------------------------------------------------------------------
// private classes
// ----------------------------------------------------------------------------

class MyApp : public wxApp
{
public:
    // override base class virtuals
    virtual bool OnInit();
};

class MyFrame : public wxFrame
{
public:
    // ctor(s)
    MyFrame(const wxString& title);

    // event handlers
    void OnQuit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
    void OnRunWizard(wxCommandEvent& event);
    void OnWizardCancel(wxWizardEvent& event);
    void OnWizardFinished(wxWizardEvent& event);

private:
    DECLARE_EVENT_TABLE()
};

// ----------------------------------------------------------------------------
// some pages for our wizard
// ----------------------------------------------------------------------------

// This shows how to simply control the validity of the user input by just
// overriding TransferDataFromWindow() - of course, in a real program, the
// check wouldn't be so trivial and the data will be probably saved somewhere
// too.
//
// It also shows how to use a different bitmap for one of the pages.
class wxValidationPage : public wxWizardPageSimple
{
public:
    wxValidationPage(wxWizard *parent) : wxWizardPageSimple(parent)
    {
        m_bitmap = wxBITMAP(wiztest2);

        m_checkbox = new wxCheckBox(this, wxID_ANY,
                          wxT("&Check me"));
        
        wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);
        mainSizer->Add(
            new wxStaticText(this, wxID_ANY,
                     wxT("You need to check the checkbox\n")
                     wxT("below before going to the next page\n")),
            0,
            wxALL,
            5
        );

        mainSizer->Add(
            m_checkbox,
            0, // No stretching
            wxALL,
            5 // Border
        );
        SetSizer(mainSizer);
        mainSizer->Fit(this);
    }

    virtual bool TransferDataFromWindow()
    {
        if ( !m_checkbox->GetValue() )
        {
            wxMessageBox(wxT("Check the checkbox first!"),
                         wxT("No way"),
                         wxICON_WARNING | wxOK, this);

            return false;
        }
        return true;
    }

private:
    wxCheckBox *m_checkbox;
};

// This is a more complicated example of validity checking: using events we may
// allow the user to return to the previous page, but not to proceed. It also
// demonstrates how to intercept [Cancel] button press.
class wxRadioboxPage : public wxWizardPageSimple
{
public:
    // directions in which we allow the user to proceed from this page
    enum
    {
        Forward, Backward, Both, Neither
    };

    wxRadioboxPage(wxWizard *parent) : wxWizardPageSimple(parent)
    {
        // should correspond to the enum above
        static wxString choices[] = { wxT("forward"), wxT("backward"), wxT("both"), wxT("neither") };

        m_radio = new wxRadioBox(this, wxID_ANY, wxT("Allow to proceed:"),
                                 wxDefaultPosition, wxDefaultSize,
                                 WXSIZEOF(choices), choices,
                                 1, wxRA_SPECIFY_COLS);
        m_radio->SetSelection(Both);
        
        wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);
        mainSizer->Add(
            m_radio,
            0, // No stretching
            wxALL,
            5 // Border
        );
        SetSizer(mainSizer);
        mainSizer->Fit(this);
    }

    // wizard event handlers
    void OnWizardCancel(wxWizardEvent& event)
    {
        if ( wxMessageBox(wxT("Do you really want to cancel?"), wxT("Question"),
                          wxICON_QUESTION | wxYES_NO, this) != wxYES )
        {
            // not confirmed
            event.Veto();
        }
    }

    void OnWizardPageChanging(wxWizardEvent& event)
    {
        int sel = m_radio->GetSelection();

        if ( sel == Both )
            return;

        if ( event.GetDirection() && sel == Forward )
            return;

        if ( !event.GetDirection() && sel == Backward )
            return;

        wxMessageBox(wxT("You can't go there"), wxT("Not allowed"),
                     wxICON_WARNING | wxOK, this);

        event.Veto();
    }

private:
    wxRadioBox *m_radio;

    DECLARE_EVENT_TABLE()
};

// This shows how to dynamically (i.e. during run-time) arrange the page order.
class wxCheckboxPage : public wxWizardPage
{
public:
    wxCheckboxPage(wxWizard *parent,
                   wxWizardPage *prev,
                   wxWizardPage *next)
        : wxWizardPage(parent)
    {
        m_prev = prev;
        m_next = next;
        
        wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);

        mainSizer->Add(
            new wxStaticText(this, wxID_ANY, wxT("Try checking the box below and\n")
                                       wxT("then going back and clearing it")),
            0, // No vertical stretching
            wxALL,
            5 // Border width
        );

        m_checkbox = new wxCheckBox(this, wxID_ANY, wxT("&Skip the next page"));
        mainSizer->Add(
            m_checkbox,
            0, // No vertical stretching
            wxALL,
            5 // Border width
        );
        
        SetSizer(mainSizer);
        mainSizer->Fit(this);
    }

    // implement wxWizardPage functions
    virtual wxWizardPage *GetPrev() const { return m_prev; }
    virtual wxWizardPage *GetNext() const
    {
        return m_checkbox->GetValue() ? m_next->GetNext() : m_next;
    }

private:
    wxWizardPage *m_prev,
                 *m_next;

    wxCheckBox *m_checkbox;
};

// ============================================================================
// implementation
// ============================================================================

// ----------------------------------------------------------------------------
// event tables and such
// ----------------------------------------------------------------------------

BEGIN_EVENT_TABLE(MyFrame, wxFrame)
    EVT_MENU(Wizard_Quit,  MyFrame::OnQuit)
    EVT_MENU(Wizard_About, MyFrame::OnAbout)
    EVT_MENU(Wizard_Run,   MyFrame::OnRunWizard)

    EVT_WIZARD_CANCEL(wxID_ANY, MyFrame::OnWizardCancel)
    EVT_WIZARD_FINISHED(wxID_ANY, MyFrame::OnWizardFinished)
END_EVENT_TABLE()

BEGIN_EVENT_TABLE(wxRadioboxPage, wxWizardPageSimple)
    EVT_WIZARD_PAGE_CHANGING(wxID_ANY, wxRadioboxPage::OnWizardPageChanging)
    EVT_WIZARD_CANCEL(wxID_ANY, wxRadioboxPage::OnWizardCancel)
END_EVENT_TABLE()

IMPLEMENT_APP(MyApp)

// ----------------------------------------------------------------------------
// the application class
// ----------------------------------------------------------------------------

bool MyApp::OnInit()
{
    MyFrame *frame = new MyFrame(wxT("wxWizard Sample"));

    frame->Show(true);

    // we're done
    return true;
}

// ----------------------------------------------------------------------------
// MyFrame
// ----------------------------------------------------------------------------

MyFrame::MyFrame(const wxString& title)
       : wxFrame((wxFrame *)NULL, wxID_ANY, title,
                  wxDefaultPosition, wxSize(250, 150))  // small frame
{
    wxMenu *fileMenu = new wxMenu;
    fileMenu->Append(Wizard_Run, wxT("&Run wizard...\tCtrl-R"));
    fileMenu->AppendSeparator();
    fileMenu->Append(Wizard_Quit, wxT("E&xit\tAlt-X"), wxT("Quit this program"));

    wxMenu *helpMenu = new wxMenu;
    helpMenu->Append(Wizard_About, wxT("&About...\tF1"), wxT("Show about dialog"));

    // now append the freshly created menu to the menu bar...
    wxMenuBar *menuBar = new wxMenuBar();
    menuBar->Append(fileMenu, wxT("&File"));
    menuBar->Append(helpMenu, wxT("&Help"));

    // ... and attach this menu bar to the frame
    SetMenuBar(menuBar);

    // also create status bar which we use in OnWizardCancel
    CreateStatusBar();
}

void MyFrame::OnQuit(wxCommandEvent& WXUNUSED(event))
{
    // true is to force the frame to close
    Close(true);
}

void MyFrame::OnAbout(wxCommandEvent& WXUNUSED(event))
{
    wxMessageBox(wxT("Demo of wxWizard class\n")
                 wxT("(c) 1999, 2000 Vadim Zeitlin"),
                 wxT("About wxWizard sample"), wxOK | wxICON_INFORMATION, this);
}

void MyFrame::OnRunWizard(wxCommandEvent& WXUNUSED(event))
{
    wxWizard *wizard = new wxWizard(this, wxID_ANY,
                    wxT("Absolutely Useless Wizard"),
                    wxBitmap(wiztest),
                    wxDefaultPosition,
                    wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER);

    // a wizard page may be either an object of predefined class
    wxWizardPageSimple *page1 = new wxWizardPageSimple(wizard);
    wxStaticText *text = new wxStaticText(page1, wxID_ANY,
         wxT("This wizard doesn't help you\nto do anything at all.\n")
         wxT("\n")
         wxT("The next pages will present you\nwith more useless controls."),
         wxPoint(5,5)
        );

    // ... or a derived class
    wxRadioboxPage *page3 = new wxRadioboxPage(wizard);
    wxValidationPage *page4 = new wxValidationPage(wizard);

    // set the page order using a convenience function - could also use
    // SetNext/Prev directly as below
    wxWizardPageSimple::Chain(page3, page4);

    // this page is not a wxWizardPageSimple, so we use SetNext/Prev
    // to insert it into the chain of pages
    wxCheckboxPage *page2 = new wxCheckboxPage(wizard, page1, page3);
    page1->SetNext(page2);
    page3->SetPrev(page2);

    // allow the wizard to size itself around the pages
    wizard->GetPageAreaSizer()->Add(page1);
    
    if ( wizard->RunWizard(page1) )
    {
        wxMessageBox(wxT("The wizard successfully completed"),
         wxT("That's all"), wxICON_INFORMATION | wxOK);
    }

    wizard->Destroy();
}

void MyFrame::OnWizardFinished(wxWizardEvent& WXUNUSED(event))
{
    wxLogStatus(this, wxT("The wizard finished successfully."));
}

void MyFrame::OnWizardCancel(wxWizardEvent& WXUNUSED(event))
{
    wxLogStatus(this, wxT("The wizard was cancelled."));
}

