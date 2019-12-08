/////////////////////////////////////////////////////////////////////////////
// Name:        fontctrl.cpp
// Purpose:     Font selector control
// Author:      Julian Smart
// Modified by:
// Created:     2002-10-17
// RCS-ID:      $Id: fontctrl.cpp,v 1.2 2004/10/25 15:07:19 js Exp $
// Copyright:   (c) Julian Smart
// Licence:   	wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifdef __GNUG__
#pragma implementation "fontctrl.h"
#endif

#include "wx/wx.h"
#include "wx/fontdlg.h"

#include "fontctrl.h"

/*
 * A control for selecting font styles and colours
 */

BEGIN_EVENT_TABLE(wxFontSelectorCtrl, wxControl)
    EVT_PAINT(wxFontSelectorCtrl::OnPaint)
    EVT_MOUSE_EVENTS(wxFontSelectorCtrl::OnMouseEvent)
END_EVENT_TABLE()

DEFINE_EVENT_TYPE(wxEVT_COMMAND_FONT_SELECTION_CHANGED)
IMPLEMENT_DYNAMIC_CLASS(wxFontSelectorCtrlEvent, wxNotifyEvent)

IMPLEMENT_DYNAMIC_CLASS(wxFontSelectorCtrl, wxControl)

bool wxFontSelectorCtrl::Create(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style,
                                             const wxValidator& validator)
{
    if (!wxControl::Create(parent, id, pos, size, style, validator))
        return false;

    SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_WINDOW));

    m_fontData.SetInitialFont(GetFont());
    m_fontData.SetChosenFont(GetFont());
    m_fontData.SetColour(GetForegroundColour());

    // Tell the sizers to use the given or best size    
    SetBestFittingSize(size);
    
    return true;
}

void wxFontSelectorCtrl::OnPaint(wxPaintEvent& event)
{
    wxPaintDC dc(this);

    wxRect rect = GetClientRect();

    int topMargin = 2;
    int leftMargin = 2;

    dc.SetFont(m_fontData.GetChosenFont());
    wxCoord width, height;
    dc.GetTextExtent(m_sampleText, & width, & height);

    int x = wxMax(leftMargin, ((rect.GetWidth() - width) / 2)) ;
    int y = wxMax(topMargin, ((rect.GetHeight() - height) / 2)) ;

    dc.SetBackgroundMode(wxTRANSPARENT);
    dc.SetTextForeground(m_fontData.GetColour());
    dc.DrawText(m_sampleText, x, y);
    dc.SetFont(wxNullFont);
}

void wxFontSelectorCtrl::OnMouseEvent(wxMouseEvent& event)
{
    if (event.LeftDown())
    {
        // Get a parent for the font dialog
        wxWindow* parent = GetParent();
        while (parent != NULL && !parent->IsKindOf(CLASSINFO(wxDialog)) && !parent->IsKindOf(CLASSINFO(wxFrame)))
            parent = parent->GetParent();

        wxFontDialog dialog(parent, m_fontData);
        dialog.SetTitle(_("Please choose a font"));

        if (dialog.ShowModal() == wxID_OK)
        {
            m_fontData = dialog.GetFontData();
            m_fontData.SetInitialFont(dialog.GetFontData().GetChosenFont());

            Refresh();

            wxFontSelectorCtrlEvent event(wxEVT_COMMAND_FONT_SELECTION_CHANGED, GetId());
            event.SetEventObject(this);
            GetEventHandler()->ProcessEvent(event);
        }
    }
}

IMPLEMENT_DYNAMIC_CLASS(wxFontSelectorValidator, wxValidator)

BEGIN_EVENT_TABLE(wxFontSelectorValidator, wxValidator)
END_EVENT_TABLE()

wxFontSelectorValidator::wxFontSelectorValidator(wxFontData *val)
{
    m_fontDataValue = val ;
    m_fontValue = NULL;
    m_colourValue = NULL;
}

wxFontSelectorValidator::wxFontSelectorValidator(wxFont *fontVal, wxColour* colourVal)
{
    m_fontDataValue = NULL;
    m_fontValue = fontVal ;
    m_colourValue = colourVal ;
}

wxFontSelectorValidator::wxFontSelectorValidator(const wxFontSelectorValidator& val)
{
    Copy(val);
}

bool wxFontSelectorValidator::Copy(const wxFontSelectorValidator& val)
{
    wxValidator::Copy(val);

    m_fontDataValue = val.m_fontDataValue ;
    m_fontValue = val.m_fontValue ;
    m_colourValue = val.m_colourValue ;

    return TRUE;
}

wxFontSelectorValidator::~wxFontSelectorValidator()
{
}

// Called when the value in the window must be validated.
// This function can pop up an error message.
bool wxFontSelectorValidator::Validate(wxWindow *parent)
{
    if( !CheckValidator() )
        return FALSE;

    wxFontSelectorCtrl *control = (wxFontSelectorCtrl *) m_validatorWindow ;

    // If window is disabled, simply return
    if ( !control->IsEnabled() )
        return TRUE;

    if ((m_fontDataValue && !m_fontDataValue->GetChosenFont().Ok()) ||
        (m_fontValue && !m_fontValue->Ok()))
    {
        wxString errormsg(_("The font is invalid."));

        wxMessageBox(errormsg, _("Validation conflict"),
                     wxOK | wxICON_EXCLAMATION, parent);
        return FALSE;
    }
    if (m_colourValue && !m_colourValue->Ok())
    {
        wxString errormsg(_("The text colour is invalid."));

        wxMessageBox(errormsg, _("Validation conflict"),
                     wxOK | wxICON_EXCLAMATION, parent);
        return FALSE;
    }

    return TRUE;
}

// Called to transfer data to the window
bool wxFontSelectorValidator::TransferToWindow(void)
{
    if( !CheckValidator() )
        return FALSE;

    wxFontSelectorCtrl* control = (wxFontSelectorCtrl*) m_validatorWindow ;

    if (m_fontDataValue)
        control->SetFontData(* m_fontDataValue);
    if (m_fontValue)
    {
        wxFontData fontData = control->GetFontData();
        fontData.SetChosenFont(* m_fontValue);
        fontData.SetInitialFont(* m_fontValue);
        control->SetFontData(fontData);
    }
    if (m_colourValue)
    {
        wxFontData fontData = control->GetFontData();
        fontData.SetColour(* m_colourValue);
        control->SetFontData(fontData);
    }

    control->Refresh();

    return TRUE;
}

// Called to transfer data to the window
bool wxFontSelectorValidator::TransferFromWindow(void)
{
    if( !CheckValidator() )
        return FALSE;

    wxFontSelectorCtrl* control = (wxFontSelectorCtrl*) m_validatorWindow ;
    if (m_fontDataValue)
        (* m_fontDataValue) = control->GetFontData() ;
    if (m_fontValue)
        (* m_fontValue) = control->GetFontData().GetChosenFont();
    if (m_colourValue)
    {
        wxFontData fontData(control->GetFontData());
        (* m_colourValue) = fontData.GetColour();
    }

    return TRUE;
}

// Checks that the validator is set up correctly
bool wxFontSelectorValidator::CheckValidator() const
{
    wxCHECK_MSG( m_validatorWindow, FALSE,
                 _T("No window associated with validator") );
    wxCHECK_MSG( m_validatorWindow->IsKindOf(CLASSINFO(wxFontSelectorCtrl)), FALSE,
                 _T("wxFontSelectorValidator is only for wxFontSelectorCtrl") );
    wxCHECK_MSG( (m_fontDataValue || m_fontValue), FALSE,
                 _T("No variable storage for validator") );

    return TRUE;
}

