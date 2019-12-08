class MyTaskBarIcon: public wxTaskBarIcon
{
public:
    MyTaskBarIcon() {};

    void OnLeftButtonDClick(wxTaskBarIconEvent&);
    void OnMenuRestore(wxCommandEvent&);
    void OnMenuExit(wxCommandEvent&);
    void OnMenuSetNewIcon(wxCommandEvent&);

    virtual wxMenu *CreatePopupMenu();

DECLARE_EVENT_TABLE()
};

enum {
    PU_RESTORE = 10001,
    PU_NEW_ICON,
    PU_EXIT,
};

BEGIN_EVENT_TABLE(MyTaskBarIcon, wxTaskBarIcon)
    EVT_MENU(PU_RESTORE, MyTaskBarIcon::OnMenuRestore)
    EVT_MENU(PU_EXIT,    MyTaskBarIcon::OnMenuExit)
    EVT_MENU(PU_NEW_ICON,MyTaskBarIcon::OnMenuSetNewIcon)
    EVT_TASKBAR_LEFT_DCLICK  (MyTaskBarIcon::OnLeftButtonDClick)
END_EVENT_TABLE()

void MyTaskBarIcon::OnMenuRestore(wxCommandEvent& )
{
    dialog->Show(true);
}

void MyTaskBarIcon::OnMenuExit(wxCommandEvent& )
{
    dialog->Close(true);
}

void MyTaskBarIcon::OnMenuSetNewIcon(wxCommandEvent&)
{
    wxIcon icon(smile_xpm);

    if (!SetIcon(icon, wxT("wxTaskBarIcon Sample - a different icon")))
        wxMessageBox(wxT("Could not set new icon."));
}

// Overridables
wxMenu *MyTaskBarIcon::CreatePopupMenu()
{
    wxMenu *menu = new wxMenu;
    
    menu->Append(PU_RESTORE, wxT("&Restore TBTest"));
    menu->Append(PU_NEW_ICON,wxT("&Set New Icon"));
    menu->Append(PU_EXIT,    wxT("E&xit"));

    return menu;
}

void MyTaskBarIcon::OnLeftButtonDClick(wxTaskBarIconEvent&)
{
    dialog->Show(true);
}



// Define a new application
class MyApp: public wxApp
{
public:
    bool OnInit(void);
};

class MyDialog: public wxDialog
{
public:
    MyDialog(wxWindow* parent, const wxWindowID id, const wxString& title,
        const wxPoint& pos, const wxSize& size, const long windowStyle = wxDEFAULT_DIALOG_STYLE);
    ~MyDialog();

    void OnOK(wxCommandEvent& event);
    void OnExit(wxCommandEvent& event);
    void OnCloseWindow(wxCloseEvent& event);
    void Init(void);

protected:
    MyTaskBarIcon   *m_taskBarIcon;

DECLARE_EVENT_TABLE()
};

#include "../sample.xpm"
#include "smile.xpm"

#include "wx/taskbar.h"
#include "tbtest.h"

// Declare two frames
MyDialog   *dialog = NULL;

IMPLEMENT_APP(MyApp)

bool MyApp::OnInit(void)
{
    // Create the main frame window
    dialog = new MyDialog(NULL, wxID_ANY, wxT("wxTaskBarIcon Test Dialog"), wxDefaultPosition, wxSize(365, 290));

    dialog->Show(true);

    return true;
}


BEGIN_EVENT_TABLE(MyDialog, wxDialog)
    EVT_BUTTON(wxID_OK, MyDialog::OnOK)
    EVT_BUTTON(wxID_EXIT, MyDialog::OnExit)
    EVT_CLOSE(MyDialog::OnCloseWindow)
END_EVENT_TABLE()



MyDialog::MyDialog(wxWindow* parent, const wxWindowID id, const wxString& title,
    const wxPoint& pos, const wxSize& size, const long windowStyle):
  wxDialog(parent, id, title, pos, size, windowStyle)
{
    Init();
}

MyDialog::~MyDialog()
{
    delete m_taskBarIcon;
}

void MyDialog::OnOK(wxCommandEvent& WXUNUSED(event))
{
    Show(false);
}

void MyDialog::OnExit(wxCommandEvent& WXUNUSED(event))
{
    Close(true);
}

void MyDialog::OnCloseWindow(wxCloseEvent& WXUNUSED(event))
{
    Destroy();
}

void MyDialog::Init(void)
{
    (void)new wxStaticText(this, wxID_ANY, wxT("Press 'Hide me' to hide me, Exit to quit."),
                         wxPoint(10, 20));

    (void)new wxStaticText(this, wxID_ANY, wxT("Double-click on the taskbar icon to show me again."),
                         wxPoint(10, 40));

    (void)new wxButton(this, wxID_EXIT, wxT("Exit"), wxPoint(185, 230), wxSize(80, 25));
    (new wxButton(this, wxID_OK, wxT("Hide me"), wxPoint(100, 230), wxSize(80, 25)))->SetDefault();
    Centre(wxBOTH);
   
    m_taskBarIcon = new MyTaskBarIcon();
    if (!m_taskBarIcon->SetIcon(wxIcon(sample_xpm), wxT("wxTaskBarIcon Sample")))
        wxMessageBox(wxT("Could not set icon."));
}


