/////////////////////////////////////////////////////////////////////////////
// Name:            server.cpp
// Purpose:         Socket Stream Demo (server)
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
#  pragma implementation "server.cpp"
#  pragma interface "server.cpp"
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

#include "FileSendThread.h"

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

	// event handlers (these functions should _not_ be virtual)
	void OnQuit(wxCommandEvent& event);
	void OnServerEvent(wxSocketEvent& event);

private:
	wxSocketServer *m_server;
	wxTextCtrl     *m_text;
	wxMenu         *m_menuFile;
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
	SERVER_QUIT = 1000,

	// id for sockets
	SERVER_ID,
	SOCKET_ID
};

// --------------------------------------------------------------------------
// event tables and other macros for wxWidgets
// --------------------------------------------------------------------------

BEGIN_EVENT_TABLE(MyFrame, wxFrame)
	EVT_MENU(SERVER_QUIT, MyFrame::OnQuit)
	EVT_SOCKET(SERVER_ID, MyFrame::OnServerEvent)
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

	// Success
	return true;
}

// --------------------------------------------------------------------------
// main frame
// --------------------------------------------------------------------------

// frame constructor

MyFrame::MyFrame() : wxFrame(NULL, wxID_ANY, _("Socket Stream Demo: Server"),
							 wxDefaultPosition, wxSize(300, 200))
{
	// Give the frame an icon
	SetIcon(wxICON(mondrian));

	// Make menus
	m_menuFile = new wxMenu();
	m_menuFile->Append(SERVER_QUIT, _("E&xit\tAlt-X"), _("Quit server"));

	// Append menus to the menubar
	m_menuBar = new wxMenuBar();
	m_menuBar->Append(m_menuFile, _("&File"));
	SetMenuBar(m_menuBar);

	// Make a textctrl for logging
	m_text = new wxTextCtrl(this, wxID_ANY, wxEmptyString,
							 wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE | wxTE_READONLY);

	m_text->AppendText(_("Welcome to the Socket Stream Demo: Server\n"));

	// Create the address - defaults to localhost:0 initially
	wxIPV4address addr;
	addr.Service(3000);

	// Create the socket
	m_server = new wxSocketServer(addr);

	// We use Ok() here to see if the server is really listening
	if (!m_server->Ok())
	{
		m_text->AppendText(_("Could not listen at the specified port!\n\n"));
	}
	else
	{
		m_text->AppendText(_("Server listening.\n\n"));
	}

	// Setup the event handler and subscribe to connection events
	m_server->SetEventHandler(*this, SERVER_ID);
	m_server->SetNotify(wxSOCKET_CONNECTION_FLAG);
	m_server->Notify(true);
}

MyFrame::~MyFrame()
{
	// No delayed deletion here, as the frame is dying anyway
	delete m_server;
}

// event handlers

void MyFrame::OnQuit(wxCommandEvent& WXUNUSED(event))
{
	// true is to force the frame to close
	Close(true);
}

void MyFrame::OnServerEvent(wxSocketEvent& WXUNUSED(event))
{
	// Accept the connection
	wxSocketBase* sock = m_server->Accept(false);

	m_text->AppendText(_("\nAccepted incoming connection.\n"));

	m_text->AppendText(_("\nBeginning file transfer."));

	// Start the file transfer
	new CFileSendThread(wxT("ReadFrom.jpg"), sock);
}
