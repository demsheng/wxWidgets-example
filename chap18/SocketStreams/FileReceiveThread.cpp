// FileReceiveThread.cpp: implementation of the CFileReceiveThread class.
//
//////////////////////////////////////////////////////////////////////

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#include <wx/wfstream.h>
#include <wx/sckstrm.h>
#include <wx/zstream.h>

#include "FileReceiveThread.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFileReceiveThread::CFileReceiveThread(wxString Filename, wxSocketBase* Socket)
{
    m_Filename = Filename;
    m_Socket = Socket;

    Create();
    Run();
}

void* CFileReceiveThread::Entry()
{
    // If we don't receive anything for 10 seconds, assume a timeout
    m_Socket->SetTimeout(10);
    
    // Wait for some data to come in, or for an error and block on the socket calls
    m_Socket->SetFlags(wxSOCKET_WAITALL | wxSOCKET_BLOCK);

    // Output to the specified file
    wxFileOutputStream FileOutputStream(m_Filename);
    // Stream data in from the socket
    wxSocketInputStream SocketInputStream(*m_Socket);
    // The zlib decompression will decompress data from the socket stream
    wxZlibInputStream ZlibInputStream(SocketInputStream);

    // Write to the file stream the results of reading from the zlib input stream
    FileOutputStream.Write(ZlibInputStream);

    // We can destroy the socket now
    m_Socket->Destroy();

    return NULL;
}

CFileReceiveThread::~CFileReceiveThread()
{

}
