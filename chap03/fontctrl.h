/////////////////////////////////////////////////////////////////////////////
// Name:        fontctrl.h
// Purpose:     Font selector control
// Author:      Julian Smart
// Modified by:
// Created:     2002-10-17
// RCS-ID:      $Id: fontctrl.h,v 1.2 2004/10/25 15:07:19 js Exp $
// Copyright:   (c) Julian Smart
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

#ifdef __GNUG__
#pragma interface "fontctrl.cpp"
#endif

#ifndef _WX_FONTCTRL_H_
#define _WX_FONTCTRL_H_

#include "wx/cmndata.h"

/*!
 * A control for displaying a font preview.
 */

class wxFontSelectorCtrl: public wxControl
{
    DECLARE_DYNAMIC_CLASS(wxFontSelectorCtrl)
    DECLARE_EVENT_TABLE()

public:

    // Constructors
    wxFontSelectorCtrl() { Init(); }

    wxFontSelectorCtrl(wxWindow* parent, wxWindowID id,
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
        long style = wxSUNKEN_BORDER,
        const wxValidator& validator = wxDefaultValidator)
    {
        Init();
        Create(parent, id, pos, size, style, validator);
    }

    // Creation
    bool Create(wxWindow* parent, wxWindowID id,
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
        long style = wxSUNKEN_BORDER,
        const wxValidator& validator = wxDefaultValidator);

    // Common initialization
    void Init() { m_sampleText = wxT("abcdeABCDE"); }

    // Overrides
    wxSize DoGetBestSize() const { return wxSize(200, 40); }

    // Event handlers
    void OnPaint(wxPaintEvent& event);
    void OnMouseEvent(wxMouseEvent& event);

    // Accessors
    void SetFontData(const wxFontData& fontData) { m_fontData = fontData; };
    const wxFontData& GetFontData() const { return m_fontData; };
    wxFontData& GetFontData() { return m_fontData; };

    void SetSampleText(const wxString& sample);
    const wxString& GetSampleText() const { return m_sampleText; };

protected:
    wxFontData  m_fontData;
    wxString    m_sampleText;
};

/*!
 * Font selector event class
 */

class wxFontSelectorCtrlEvent : public wxNotifyEvent
{
public:
    wxFontSelectorCtrlEvent(wxEventType commandType = wxEVT_NULL, int id = 0):
                 wxNotifyEvent(commandType, id)
    {}

    wxFontSelectorCtrlEvent(const wxFontSelectorCtrlEvent& event): wxNotifyEvent(event)
    {}

    virtual wxEvent *Clone() const { return new wxFontSelectorCtrlEvent(*this); }

    DECLARE_DYNAMIC_CLASS(wxFontSelectorCtrlEvent);
};

typedef void (wxEvtHandler::*wxFontSelectorCtrlEventFunction)(wxFontSelectorCtrlEvent&);

/*!
 * Font selector control events and macros for handling them
 */

BEGIN_DECLARE_EVENT_TYPES()
    DECLARE_EVENT_TYPE(wxEVT_COMMAND_FONT_SELECTION_CHANGED, 801)
END_DECLARE_EVENT_TYPES()

#define EVT_FONT_SELECTION_CHANGED(id, fn) DECLARE_EVENT_TABLE_ENTRY( wxEVT_COMMAND_FONT_SELECTION_CHANGED, id, -1, (wxObjectEventFunction) (wxEventFunction) (wxFontSelectorCtrlEventFunction) & fn, (wxObject *) NULL ),

/*!
 * Validator for wxFontSelectorCtrl
 */

class wxFontSelectorValidator: public wxValidator
{
DECLARE_DYNAMIC_CLASS(wxFontSelectorValidator)
public:
    // Constructors
    wxFontSelectorValidator(wxFontData *val = NULL);
    wxFontSelectorValidator(wxFont *fontVal, wxColour* colourVal = NULL);
    wxFontSelectorValidator(const wxFontSelectorValidator& val);

    // Destructor
    ~wxFontSelectorValidator();

    // Make a clone of this validator
    virtual wxObject *Clone() const { return new wxFontSelectorValidator(*this); }

    // Copies val to this object
    bool Copy(const wxFontSelectorValidator& val);

    // Called when the value in the window must be validated.
    // This function can pop up an error message.
    virtual bool Validate(wxWindow *parent);

    // Called to transfer data to the window
    virtual bool TransferToWindow();

    // Called to transfer data to the window
    virtual bool TransferFromWindow();

    wxFontData* GetFontData() { return m_fontDataValue; }

DECLARE_EVENT_TABLE()

protected:
    wxFontData*     m_fontDataValue;
    wxFont*         m_fontValue;
    wxColour*       m_colourValue;

    // Checks that the validator is set up correctly
    bool CheckValidator() const ;
};

#endif
    // _WX_FONTCTRL_H_

