/////////////////////////////////////////////////////////////////////////////
// Name:            client.cpp
// Purpose:         Socket Event Demo (client)
// Author:          Kevin Hock
// Copyright:   (c) 2005 Kevin Hock
// Licence:         wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// ==========================================================================
// declarations
// ==========================================================================

// --------------------------------------------------------------------------
// headers
// --------------------------------------------------------------------------

#if defined(__GNUG__) && !defined(__APPLE__)
#  pragma implementation "client.cpp"
#  pragma interface "client.cpp"
#endif

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#  pragma hdrstop
#endif

// for all others, include the necessary headers 
#ifndef WX_PRECOMP
#  include "wx/wx.h"
#endif

#include "wx/socket.h"

// --------------------------------------------------------------------------
// resources
// --------------------------------------------------------------------------

// the application icon
#if defined(__WXGTK__) || defined(__WXX11__) || defined(__WXMOTIF__) || defined(__WXMAC__)
#  include "mondrian.xpm"
#endif

// --------------------------------------------------------------------------
// classes
// --------------------------------------------------------------------------

// Define a new application type
class MyApp : public wxApp
{
public:
    virtual bool OnInit();
};

// Define a new frame type: this is going to be our main frame
class MyFrame : public wxFrame
{
public:
    MyFrame();
    ~MyFrame();

    // event handlers for File menu
    void OnQuit(wxCommandEvent& event);

    // event handlers for Socket menu
    void OnConnectToServer(wxCommandEvent& event);

    // socket event handler
    void OnSocketEvent(wxSocketEvent& event);

private:
    wxTextCtrl     *m_text;
    wxMenu         *m_menuFile;
    wxMenu         *m_menuSocket;
    wxMenuBar      *m_menuBar;

    // any class wishing to process wxWidgets events must use this macro
    DECLARE_EVENT_TABLE()
};

// --------------------------------------------------------------------------
// constants
// --------------------------------------------------------------------------

// IDs for the controls and the menu commands
enum
{
    // menu items
    CLIENT_QUIT = 1000,
    CLIENT_OPEN,

    // id for socket
    SOCKET_ID
};

// --------------------------------------------------------------------------
// event tables and other macros for wxWidgets
// --------------------------------------------------------------------------

BEGIN_EVENT_TABLE(MyFrame, wxFrame)
    EVT_MENU(CLIENT_QUIT,       MyFrame::OnQuit)
    EVT_MENU(CLIENT_OPEN,       MyFrame::OnConnectToServer)
    EVT_SOCKET(SOCKET_ID,       MyFrame::OnSocketEvent)
END_EVENT_TABLE()

IMPLEMENT_APP(MyApp)

// ==========================================================================
// implementation
// ==========================================================================

// --------------------------------------------------------------------------
// the application class
// --------------------------------------------------------------------------

bool MyApp::OnInit()
{
    // Create the main application window
    MyFrame *frame = new MyFrame();

    // Show it and tell the application that it's our main window
    frame->Show(true);
    SetTopWindow(frame);

    // success
    return true;
}

// --------------------------------------------------------------------------
// main frame
// --------------------------------------------------------------------------

// frame constructor
MyFrame::MyFrame() : wxFrame(NULL, wxID_ANY, _("Socket Event Demo: Client"),
                             wxDefaultPosition, wxSize(300, 200))
{
    // Give the frame an icon
    SetIcon(wxICON(mondrian));

    // Make menus
    m_menuFile = new wxMenu();
    m_menuFile->Append(CLIENT_QUIT, _("E&xit\tAlt-X"), _("Quit client"));

    m_menuSocket = new wxMenu();
    m_menuSocket->Append(CLIENT_OPEN, _("&Connect"), _("Connect to server"));

    // Append menus to the menubar
    m_menuBar = new wxMenuBar();
    m_menuBar->Append(m_menuFile, _("&File"));
    m_menuBar->Append(m_menuSocket, _("&Client"));
    SetMenuBar(m_menuBar);

    // Make a textctrl for logging
    m_text  = new wxTextCtrl(this, wxID_ANY, wxEmptyString,
                             wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE | wxTE_READONLY);

    m_text->AppendText(_("Welcome to the Socket Event Demo: Client\nClient ready\n"));
}

MyFrame::~MyFrame()
{

}

// event handlers

void MyFrame::OnQuit(wxCommandEvent& WXUNUSED(event))
{
    // true is to force the frame to close
    Close(true);
}

void MyFrame::OnConnectToServer(wxCommandEvent& WXUNUSED(event))
{
    // Ask user for server address
    wxString hostname = wxGetTextFromUser(
    _("Enter the address of the Socket Event Demo server:"),
    _("Connect ..."),
    _("localhost"));

    wxIPV4address addr;
    addr.Hostname(hostname);
    addr.Service(3000);

    // Create the socket
    wxSocketClient* Socket = new wxSocketClient();

    // Set up the event handler and subscribe to most events
    Socket->SetEventHandler(*this, SOCKET_ID);
    Socket->SetNotify(wxSOCKET_CONNECTION_FLAG |
                                        wxSOCKET_INPUT_FLAG |
                                        wxSOCKET_LOST_FLAG);
    Socket->Notify(true);

    // Wait for the connection event
    Socket->Connect(addr, false);
}

void MyFrame::OnSocketEvent(wxSocketEvent& event)
{
    // The socket that had the event
    wxSocketBase* sock = event.GetSocket();

    // Common buffer shared by the events
    char buf[10];

    switch(event.GetSocketEvent())
    {
        case wxSOCKET_CONNECTION:
        {
            m_text->AppendText(_("\nConnected to server.\n"));

            // Fill the arry with the numbers 0 through 9 as characters
            char mychar = '0';
            for (int i = 0; i < 10; i++)
            {
                buf[i] = mychar++;
            }

            // Send the characters to the server
            sock->Write(buf, sizeof(buf));

            m_text->AppendText(_("Wrote string to server.\n"));

            break;
        } 
        case wxSOCKET_INPUT:
        {
            sock->Read(buf, sizeof(buf));

            m_text->AppendText(wxString(_("Received from server: ")) + wxString(buf, wxConvUTF8, 10) + _("\n"));

            break;
        }

        // The server hangs up after sending the data
        case wxSOCKET_LOST:
        {
            sock->Destroy();

            m_text->AppendText(_("Connection lost.\n"));

            break;
        }
    }
}
