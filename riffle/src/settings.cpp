/////////////////////////////////////////////////////////////////////////////
// Name:        settings.cpp
// Purpose:     Settings dialog for Riffle
// Author:      Julian Smart
// Modified by: 
// Created:     03/26/05 12:36:16
// RCS-ID:      
// Copyright:   (c) Julian Smart
// Licence:     wxWindows License
/////////////////////////////////////////////////////////////////////////////

#if defined(__GNUG__) && !defined(__APPLE__)
#pragma implementation "settings.h"
#endif

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "settings.h"
#include "riffle.h"
#include "generalsettingsdlg.h"

IMPLEMENT_CLASS(SettingsDialog, wxPropertySheetDialog)

BEGIN_EVENT_TABLE(SettingsDialog, wxPropertySheetDialog)
END_EVENT_TABLE()

SettingsDialog::SettingsDialog(wxWindow* win)
{
    SetExtraStyle(wxDIALOG_EX_CONTEXTHELP|wxWS_EX_VALIDATE_RECURSIVELY);

    Create(win, -1, _("Settings"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER);
    CreateButtons(wxOK|wxCANCEL|wxHELP);

    wxBookCtrlBase* notebook = GetBookCtrl();

    m_generalSettingsPage = CreateGeneralSettingsPage(notebook);

    notebook->AddPage(m_generalSettingsPage, _("General"));

    LayoutDialog();
}

wxPanel* SettingsDialog::CreateGeneralSettingsPage(wxWindow* parent)
{
    wxPanel* page = new GeneralSettingsPage(parent);
    return page;
}

