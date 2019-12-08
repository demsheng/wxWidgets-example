/////////////////////////////////////////////////////////////////////////////
// Name:        settings.h
// Purpose:     Settings dialog for Riffle
// Author:      Julian Smart
// Modified by: 
// Created:     03/26/05 12:36:16
// RCS-ID:      
// Copyright:   (c) Julian Smart
// Licence:     wxWindows License
/////////////////////////////////////////////////////////////////////////////

#ifndef _SETTINGS_H_
#define _SETTINGS_H_

#if defined(__GNUG__) && !defined(__APPLE__)
#pragma interface "settings.cpp"
#endif

#include "wx/propdlg.h"

// Property sheet dialog
class SettingsDialog: public wxPropertySheetDialog
{
DECLARE_CLASS(SettingsDialog)
public:
    SettingsDialog(wxWindow* parent);

    wxPanel* CreateGeneralSettingsPage(wxWindow* parent);

protected:

    wxPanel*    m_generalSettingsPage;

DECLARE_EVENT_TABLE()
};


#endif
    // _SETTINGS_H_
