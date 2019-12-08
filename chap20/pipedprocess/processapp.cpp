/////////////////////////////////////////////////////////////////////////////
// Name:        processapp.cpp
// Purpose:     Process-aware app class
// Author:      Julian Smart
// Modified by:
// Created:     2005-1-25
// RCS-ID:      $Id$
// Copyright:   (c) Julian Smart
// Licence:	wxWindows Licence
/////////////////////////////////////////////////////////////////////////////

#include "wx/wx.h"
#include "processapp.h"

IMPLEMENT_CLASS(PipedProcess, wxProcess)
IMPLEMENT_CLASS(ProcessApp, wxApp)

BEGIN_EVENT_TABLE(ProcessApp, wxApp)
	EVT_IDLE(ProcessApp::OnIdle)
END_EVENT_TABLE()

/// Register a process with the application, and start
/// idle timer if necessary
bool ProcessApp::RegisterProcess(wxProcess* process)
{
    if (!m_processes.Find(process))
        m_processes.Append(process);

    if (!m_wakeUpTimer.IsRunning())
        m_wakeUpTimer.Start(100);
    return true;
}

/// UnregisterProcess
bool ProcessApp::UnregisterProcess(wxProcess* process)
{
    bool success = m_processes.DeleteObject(process);
    if (!HasProcesses())
    {
        if (m_wakeUpTimer.IsRunning())
            m_wakeUpTimer.Stop();
    }
    return success;
}

void ProcessApp::OnTimer(wxTimerEvent& WXUNUSED(event))
{
    wxWakeUpIdle();
}

/// Handle any pending input, in idle time
bool ProcessApp::HandleProcessInput()
{
    if (!HasProcesses())
        return false;

    bool hasInput = false;

    wxNode* node = m_processes.GetFirst();
    while (node)
    {
        PipedProcess* process = wxDynamicCast(node->GetData(), PipedProcess);
        if (process && process->HasInput())
            hasInput = true;
        node = node->GetNext();
    }
    return hasInput;
}

void ProcessApp::OnIdle(wxIdleEvent& event)
{
    if (HandleProcessInput())
        event.RequestMore();
    event.Skip();
}


