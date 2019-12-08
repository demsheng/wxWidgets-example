// FileReceiveThread.h: interface for the CFileReceiveThread class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FILERECEIVETHREAD_H__E8A33FF3_1701_49A6_BFBE_A33792674E8B__INCLUDED_)
#define AFX_FILERECEIVETHREAD_H__E8A33FF3_1701_49A6_BFBE_A33792674E8B__INCLUDED_

#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

class CFileReceiveThread : public wxThread  
{
public:
    CFileReceiveThread(wxString Filename, wxSocketBase* Socket);
    virtual ~CFileReceiveThread();

    virtual void* Entry();

protected:
    wxSocketBase* m_Socket;
    wxString m_Filename;
};

#endif // !defined(AFX_FILERECEIVETHREAD_H__E8A33FF3_1701_49A6_BFBE_A33792674E8B__INCLUDED_)
