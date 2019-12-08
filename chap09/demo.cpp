// Name:        demo.cpp
// Purpose:     Demo for PersonalRecordDialog
// Author:      Julian Smart
// License:     wxWindows License

#include "wx/wx.h"
#include "personalrecord.h"

// Declare the application class
class MyApp : public wxApp
{
public:
    // Called on application startup
    virtual bool OnInit();
};

// Declare our main frame class
class MyFrame : public wxFrame
{
public:
    // Constructor
    MyFrame(const wxString& title);

    // Event handlers
    void OnQuit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
    void OnShowDialog(wxCommandEvent& event);

    enum {
        ID_SHOW_DIALOG
    };

private:
    // This class handles events
    DECLARE_EVENT_TABLE()
};

// Implements MyApp& GetApp()
DECLARE_APP(MyApp)

// Give wxWidgets the means to create a MyApp object
IMPLEMENT_APP(MyApp)

// Initialize the application
bool MyApp::OnInit()
{
    // Create the main application window
    MyFrame *frame = new MyFrame(wxT("PersonalRecordDialog Demo"));

    // Show it
    frame->Show(true);

    // Start the event loop
    return true;
}

// Event table for MyFrame
BEGIN_EVENT_TABLE(MyFrame, wxFrame)
    EVT_MENU(wxID_ABOUT, MyFrame::OnAbout)
    EVT_MENU(wxID_EXIT,  MyFrame::OnQuit)
    EVT_MENU(ID_SHOW_DIALOG,  MyFrame::OnShowDialog)
END_EVENT_TABLE()

void MyFrame::OnAbout(wxCommandEvent& event)
{
    wxString msg;
    msg.Printf(wxT("PersonalRecordDialog example, built with wxWidgets %s"),  
               wxVERSION_STRING);

    wxMessageBox(msg, wxT("About this program"),
                 wxOK | wxICON_INFORMATION, this);
}

void MyFrame::OnQuit(wxCommandEvent& event)
{
    // Destroy the frame
    Close();
}

void MyFrame::OnShowDialog(wxCommandEvent& event)
{
    PersonalRecordDialog dialog(this, wxID_ANY, wxT("Personal Record"),
        wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE);
    dialog.ShowModal();
}

#include "mondrian.xpm"

MyFrame::MyFrame(const wxString& title)
       : wxFrame(NULL, wxID_ANY, title)
{
    // Set the frame icon
    SetIcon(wxIcon(mondrian_xpm));

    // Create a menu bar
    wxMenu *fileMenu = new wxMenu;

    // The "About" item should be in the help menu
    wxMenu *helpMenu = new wxMenu;
    helpMenu->Append(wxID_ABOUT, wxT("&About...\tF1"),
                     wxT("Show about dialog"));

    fileMenu->Append(ID_SHOW_DIALOG, wxT("&Show Personal Record Dialog..."),
                     wxT("Show Personal Record Dialog"));
    fileMenu->Append(wxID_EXIT, wxT("E&xit\tAlt-X"),
                     wxT("Quit this program"));

    // Now append the freshly created menu to the menu bar...
    wxMenuBar *menuBar = new wxMenuBar();
    menuBar->Append(fileMenu, wxT("&File"));
    menuBar->Append(helpMenu, wxT("&Help"));

    // ... and attach this menu bar to the frame
    SetMenuBar(menuBar);
}

