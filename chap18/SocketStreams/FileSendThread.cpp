// FileSendThread.cpp: implementation of the CFileSendThread class.
//
//////////////////////////////////////////////////////////////////////

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#include <wx/wfstream.h>
#include <wx/sckstrm.h>
#include <wx/zstream.h>

#include "FileSendThread.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFileSendThread::CFileSendThread(wxString Filename, wxSocketBase* Socket)
{
    m_Filename = Filename;
    m_Socket = Socket;

    Create();
    Run();
}

void* CFileSendThread::Entry()
{
    // If we can't write anything for 10 seconds, assume a timeout
    m_Socket->SetTimeout(10);

    // Wait for all the data to write, blocking on the socket calls
    m_Socket->SetFlags(wxSOCKET_WAITALL | wxSOCKET_BLOCK);

    // Read from the specified file
    wxFileInputStream* FileInputStream = new wxFileInputStream(m_Filename);

    // An output stream writing to the socket
    wxSocketOutputStream* SocketOutputStream = new wxSocketOutputStream(*m_Socket);
    // The results of the compression will be written to the socket stream
    wxZlibOutputStream* ZlibOutputStream = new wxZlibOutputStream(*SocketOutputStream);

    // Write the results of the zlib decompression to the file stream
    ZlibOutputStream->Write(*FileInputStream);
    
    // Write all data and send Zlib EOF
    ZlibOutputStream->Sync();
    ZlibOutputStream->Close();

    // Clean up
    delete ZlibOutputStream;
    delete SocketOutputStream;
    delete FileInputStream;

    // We can destroy the socket now
    m_Socket->Destroy();

    return NULL;
}

CFileSendThread::~CFileSendThread()
{

}

