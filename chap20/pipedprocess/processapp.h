/////////////////////////////////////////////////////////////////////////////
// Name:        processapp.h
// Purpose:     Process-aware app class
// Author:      Julian Smart
// Modified by:
// Created:     2005-1-25
// RCS-ID:      $Id$
// Copyright:   (c) Julian Smart
// Licence:	wxWindows Licence
/////////////////////////////////////////////////////////////////////////////

#ifndef __PROCESSAPP_H__
#define __PROCESSAPP_H__

#include "wx/app.h"
#include "wx/list.h"
#include "wx/process.h"
#include "wx/timer.h"

/*
 * A process-aware application class. It maintains a list of piped processes
 * and ensures that their input gets processed in idle time.
 */

class ProcessApp: public wxApp
{
DECLARE_CLASS(ProcessApp)
DECLARE_EVENT_TABLE()

public:
    ProcessApp() {}

    /// Handle any pending input, in idle time
    bool HandleProcessInput();

    /// Register a process with the application, and start
    /// idle timer if necessary
    bool RegisterProcess(wxProcess* process);

    /// UnregisterProcess
    bool UnregisterProcess(wxProcess* process);

    /// Are there processes?
    bool HasProcesses() { return m_processes.GetCount() > 0; }

    /// Handle idle events
    void OnIdle(wxIdleEvent& event);

protected:
    // List of processes that the app knows about
    wxList m_processes;

    // Timer to wake up idle processing
    wxTimer m_wakeUpTimer;
};

/*
 * Base class for a piped process
 */

class PipedProcess : public wxProcess
{
    DECLARE_CLASS(PipedProcess)
public:
    PipedProcess(wxWindow* win): wxProcess(win, wxPROCESS_REDIRECT) {}

    virtual bool HasInput() = 0;
};


#endif
  // __PROCESSAPP_H__

