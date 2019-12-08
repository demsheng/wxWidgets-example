// FileSendThread.h: interface for the CFileSendThread class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FILESENDTHREAD_H__ECEFB7AD_309F_4BF6_B245_DA142EE0CCD7__INCLUDED_)
#define AFX_FILESENDTHREAD_H__ECEFB7AD_309F_4BF6_B245_DA142EE0CCD7__INCLUDED_

#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

class CFileSendThread : public wxThread  
{
public:
    CFileSendThread(wxString Filename, wxSocketBase* Socket);
    virtual ~CFileSendThread();

    virtual void* Entry();

protected:
    wxSocketBase* m_Socket;
    wxString m_Filename;
};

#endif // !defined(AFX_FILESENDTHREAD_H__ECEFB7AD_309F_4BF6_B245_DA142EE0CCD7__INCLUDED_)
