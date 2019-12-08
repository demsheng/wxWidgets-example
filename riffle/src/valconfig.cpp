/////////////////////////////////////////////////////////////////////////////
// Name:        valconfig.h
// Purpose:     wxConfigValidator class
// Author:      Julian Smart
// Modified by:
// Created:     2003-04-20
// Copyright:   (c) 2003 Julian Smart
// Licence:     wxWindows License
/////////////////////////////////////////////////////////////////////////////

#ifdef __GNUG__
    #pragma implementation "valconfig.h"
#endif

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
  #pragma hdrstop
#endif

#ifndef WX_PRECOMP
  #include "wx/defs.h"
#endif

#if wxUSE_VALIDATORS

#ifndef WX_PRECOMP
  #include "wx/utils.h"
  #include "wx/intl.h"
  #include "wx/dynarray.h"
  #include "wx/choice.h"
  #include "wx/combobox.h"
  #include "wx/radiobox.h"
  #include "wx/radiobut.h"
  #include "wx/checkbox.h"
  #include "wx/scrolbar.h"
  #include "wx/gauge.h"
  #include "wx/stattext.h"
  #include "wx/textctrl.h"
  #include "wx/button.h"
  #include "wx/listbox.h"
  #include "wx/slider.h"
#endif

#if wxUSE_SPINCTRL && !defined(__WIN16__)
  #include "wx/spinctrl.h"
#endif
#if wxUSE_SPINBTN && !defined(__WIN16__)
  #include "wx/spinbutt.h"
#endif
#if wxUSE_CHECKLISTBOX && !defined(__WIN16__)
  #include "wx/checklst.h"
#endif

#include "valconfig.h"

IMPLEMENT_CLASS(wxConfigValidator, wxValidator)

wxConfigValidator::wxConfigValidator(const wxString& path, int valType, wxConfigBase* config)
{
    Initialize();
    m_type = valType;
    m_path = path;
    m_config = config;
}

wxConfigValidator::wxConfigValidator(const wxConfigValidator& val)
    : wxValidator()
{
    Copy(val);
}

bool wxConfigValidator::Copy(const wxConfigValidator& val)
{
    wxValidator::Copy(val);

    m_type = val.m_type;
    m_path = val.m_path;
    m_config = val.m_config;

    return TRUE;
}

wxConfigValidator::~wxConfigValidator()
{
}

// Called to transfer data to the window
bool wxConfigValidator::TransferToWindow(void)
{
    if ( !m_validatorWindow )
        return FALSE;

    // bool controls
#if wxUSE_CHECKBOX
    if (m_validatorWindow->IsKindOf(CLASSINFO(wxCheckBox)) )
    {
        wxCheckBox* pControl = (wxCheckBox*) m_validatorWindow;
        if (m_type == wxVAL_BOOL)
        {
            bool value = FALSE;
            m_config->Read(m_path, & value);
            pControl->SetValue(value);
            return TRUE;
        }
    } else
#endif
#if wxUSE_RADIOBTN
    if (m_validatorWindow->IsKindOf(CLASSINFO(wxRadioButton)) )
    {
        wxRadioButton* pControl = (wxRadioButton*) m_validatorWindow;
        if (m_type == wxVAL_BOOL)
        {
            bool value = FALSE;
            m_config->Read(m_path, & value);
            pControl->SetValue(value) ;
            return TRUE;
        }
    } else
#endif

    // int controls
#if wxUSE_GAUGE
    if (m_validatorWindow->IsKindOf(CLASSINFO(wxGauge)) )
    {
        wxGauge* pControl = (wxGauge*) m_validatorWindow;
        if (m_type == wxVAL_LONG)
        {
            long value = 0;
            m_config->Read(m_path, & value);
            pControl->SetValue((int) value);
            return TRUE;
        }
    } else
#endif
#if wxUSE_RADIOBOX
    if (m_validatorWindow->IsKindOf(CLASSINFO(wxRadioBox)) )
    {
        wxRadioBox* pControl = (wxRadioBox*) m_validatorWindow;
        if (m_type == wxVAL_LONG)
        {
            long value = 0;
            m_config->Read(m_path, & value);
            pControl->SetSelection((int) value) ;
            return TRUE;
        }
    } else
#endif
#if wxUSE_SCROLLBAR
    if (m_validatorWindow->IsKindOf(CLASSINFO(wxScrollBar)) )
    {
        wxScrollBar* pControl = (wxScrollBar*) m_validatorWindow;
        if (m_type == wxVAL_LONG)
        {
            long value = 0;
            m_config->Read(m_path, & value);
            pControl->SetThumbPosition((int) value) ;
            return TRUE;
        }
    } else
#endif
#if wxUSE_SPINCTRL && !defined(__WIN16__) && !defined(__WXMOTIF__)
    if (m_validatorWindow->IsKindOf(CLASSINFO(wxSpinCtrl)) )
    {
        wxSpinCtrl* pControl = (wxSpinCtrl*) m_validatorWindow;
        if (m_type == wxVAL_LONG)
        {
            long value = 0;
            m_config->Read(m_path, & value);
            pControl->SetValue((int) value);
            return TRUE;
        }
    } else
#endif
#if wxUSE_SPINBTN && !defined(__WIN16__)
    if (m_validatorWindow->IsKindOf(CLASSINFO(wxSpinButton)) )
    {
        wxSpinButton* pControl = (wxSpinButton*) m_validatorWindow;
        if (m_type == wxVAL_LONG)
        {
            long value = 0;
            m_config->Read(m_path, & value);
            pControl->SetValue((int) value) ;
            return TRUE;
        }
    } else
#endif
#if wxUSE_SLIDER
    if (m_validatorWindow->IsKindOf(CLASSINFO(wxSlider)) )
    {
        wxSlider* pControl = (wxSlider*) m_validatorWindow;
        if (m_type == wxVAL_LONG)
        {
            long value = 0;
            m_config->Read(m_path, & value);
            pControl->SetValue((int) value) ;
            return TRUE;
        }
    } else
#endif

    // string controls
    if (m_validatorWindow->IsKindOf(CLASSINFO(wxButton)) )
    {
        wxButton* pControl = (wxButton*) m_validatorWindow;
        if (m_type == wxVAL_STRING)
        {
            wxString value;
            m_config->Read(m_path, & value);
            pControl->SetLabel(value) ;
            return TRUE;
        }
    } else
#if wxUSE_COMBOBOX
    if (m_validatorWindow->IsKindOf(CLASSINFO(wxComboBox)) )
    {
        wxComboBox* pControl = (wxComboBox*) m_validatorWindow;
        if (m_type == wxVAL_LONG)
        {
            long value = 0;
            m_config->Read(m_path, & value);
            pControl->SetSelection((int) value) ;
            return TRUE;
        }
        else if (m_type == wxVAL_STRING)
        {
            wxString value;
            m_config->Read(m_path, & value);
            if (pControl->FindString(value) > -1)
            {
                pControl->SetStringSelection(value);
            }
            else
            {
                pControl->SetValue(value);
            }
            return TRUE;
        }
    } else
#endif
#if wxUSE_CHOICE
    if (m_validatorWindow->IsKindOf(CLASSINFO(wxChoice)) )
    {
        wxChoice* pControl = (wxChoice*) m_validatorWindow;
        if (m_type == wxVAL_LONG)
        {
            long value = 0;
            m_config->Read(m_path, & value);
            pControl->SetSelection((int) value) ;
            return TRUE;
        }
        else if (m_type == wxVAL_STRING)
        {
            wxString value;
            m_config->Read(m_path, & value);
            if (pControl->FindString(value) > -1)
            {
                pControl->SetStringSelection(value);
            }
            return TRUE;
        }
    } else
#endif
    if (m_validatorWindow->IsKindOf(CLASSINFO(wxStaticText)) )
    {
        wxStaticText* pControl = (wxStaticText*) m_validatorWindow;
        if (m_type == wxVAL_STRING)
        {
            wxString value;
            m_config->Read(m_path, & value);
            pControl->SetLabel(value) ;
            return TRUE;
        }
    } else
    if (m_validatorWindow->IsKindOf(CLASSINFO(wxTextCtrl)) )
    {
        wxTextCtrl* pControl = (wxTextCtrl*) m_validatorWindow;
        if (m_type == wxVAL_STRING)
        {
            wxString value;
            m_config->Read(m_path, & value);
            pControl->SetValue(value) ;
            return TRUE;
        }
        else if (m_type == wxVAL_LONG)
        {
            long value;
            m_config->Read(m_path, & value);
            wxString str;
            str.Printf(wxT("%d"), (int) value);
            pControl->SetValue(str);
            return TRUE;
        }
    } else
    // array controls
#if 0
#if wxUSE_CHECKLISTBOX && !defined(__WIN16__)
    // NOTE: wxCheckListBox is a wxListBox, so wxCheckListBox MUST come first:
    if (m_validatorWindow->IsKindOf(CLASSINFO(wxCheckListBox)) )
    {
        wxCheckListBox* pControl = (wxCheckListBox*) m_validatorWindow;
        if (m_pArrayInt)
        {
            // clear all selections
            size_t i,
                   count = pControl->GetCount();
            for ( i = 0 ; i < count; i++ )
                pControl->Check(i, FALSE);

            // select each item in our array
            count = m_pArrayInt->GetCount();
            for ( i = 0 ; i < count; i++ )
                pControl->Check(m_pArrayInt->Item(i));

            return TRUE;
        }
        else
            return FALSE;
    } else
#endif
#if wxUSE_LISTBOX
    if (m_validatorWindow->IsKindOf(CLASSINFO(wxListBox)) )
    {
        wxListBox* pControl = (wxListBox*) m_validatorWindow;
        if (m_pArrayInt)
        {
            // clear all selections
            size_t i,
                   count = pControl->GetCount();
            for ( i = 0 ; i < count; i++ )
                pControl->Deselect(i);

            // select each item in our array
            count = m_pArrayInt->GetCount();
            for ( i = 0 ; i < count; i++ )
                pControl->SetSelection(m_pArrayInt->Item(i));

            return TRUE;
        }
    } else
#endif
#endif
        ;   // to match the last 'else' above

  // unrecognized control, or bad pointer
  return FALSE;
}

// Called to transfer data from the window
bool wxConfigValidator::TransferFromWindow(void)
{
  if ( !m_validatorWindow )
    return FALSE;

  // bool controls
#if wxUSE_CHECKBOX
  if (m_validatorWindow->IsKindOf(CLASSINFO(wxCheckBox)) )
  {
    wxCheckBox* pControl = (wxCheckBox*) m_validatorWindow;
    if (m_type == wxVAL_BOOL)
    {
      m_config->Write(m_path, pControl->GetValue()) ;
      return TRUE;
    }
  } else
#endif
#if wxUSE_RADIOBTN
  if (m_validatorWindow->IsKindOf(CLASSINFO(wxRadioButton)) )
  {
    wxRadioButton* pControl = (wxRadioButton*) m_validatorWindow;
    if (m_type == wxVAL_BOOL)
    {
      m_config->Write(m_path, pControl->GetValue()) ;
      return TRUE;
    }
  } else
#endif
  // int controls
#if wxUSE_GAUGE
  if (m_validatorWindow->IsKindOf(CLASSINFO(wxGauge)) )
  {
    wxGauge* pControl = (wxGauge*) m_validatorWindow;
    if (m_type == wxVAL_LONG)
    {
      m_config->Write(m_path, (long) pControl->GetValue()) ;
      return TRUE;
    }
  } else
#endif
#if wxUSE_RADIOBOX
  if (m_validatorWindow->IsKindOf(CLASSINFO(wxRadioBox)) )
  {
    wxRadioBox* pControl = (wxRadioBox*) m_validatorWindow;
        if (m_type == wxVAL_LONG)
    {
      m_config->Write(m_path, (long) pControl->GetSelection()) ;
      return TRUE;
    }
  } else
#endif
#if wxUSE_SCROLLBAR
  if (m_validatorWindow->IsKindOf(CLASSINFO(wxScrollBar)) )
  {
    wxScrollBar* pControl = (wxScrollBar*) m_validatorWindow;
    if (m_type == wxVAL_LONG)
    {
      m_config->Write(m_path, (long) pControl->GetThumbPosition()) ;
      return TRUE;
    }
  } else
#endif
#if wxUSE_SPINCTRL && !defined(__WIN16__) && !defined(__WXMOTIF__)
    if (m_validatorWindow->IsKindOf(CLASSINFO(wxSpinCtrl)) )
    {
        wxSpinCtrl* pControl = (wxSpinCtrl*) m_validatorWindow;
        if (m_type == wxVAL_LONG)
        {
            m_config->Write(m_path, (long) pControl->GetValue());
            return TRUE;
        }
    } else
#endif
#if wxUSE_SPINBTN && !defined(__WIN16__)
  if (m_validatorWindow->IsKindOf(CLASSINFO(wxSpinButton)) )
  {
    wxSpinButton* pControl = (wxSpinButton*) m_validatorWindow;
    if (m_type == wxVAL_LONG)
    {
      m_config->Write(m_path, (long) pControl->GetValue()) ;
      return TRUE;
    }
  } else
#endif
#if wxUSE_SLIDER
  if (m_validatorWindow->IsKindOf(CLASSINFO(wxSlider)) )
  {
    wxSlider* pControl = (wxSlider*) m_validatorWindow;
    if (m_type == wxVAL_LONG)
    {
      m_config->Write(m_path, (long) pControl->GetValue()) ;
      return TRUE;
    }
  } else
#endif
  // string controls
  if (m_validatorWindow->IsKindOf(CLASSINFO(wxButton)) )
  {
    wxButton* pControl = (wxButton*) m_validatorWindow;
    if (m_type == wxVAL_STRING)
    {
      m_config->Write(m_path, pControl->GetLabel()) ;
      return TRUE;
    }
  }
  else
#if wxUSE_COMBOBOX
  if (m_validatorWindow->IsKindOf(CLASSINFO(wxComboBox)) )
  {
    wxComboBox* pControl = (wxComboBox*) m_validatorWindow;
    if (m_type == wxVAL_LONG)
    {
      m_config->Write(m_path, (long) pControl->GetSelection()) ;
      return TRUE;
    }
    else if (m_type == wxVAL_STRING)
    {
        m_config->Write(m_path, pControl->GetValue());
        return TRUE;
    }
  } else
#endif
#if wxUSE_CHOICE
 if (m_validatorWindow->IsKindOf(CLASSINFO(wxChoice)) )
  {
    wxChoice* pControl = (wxChoice*) m_validatorWindow;
    if (m_type == wxVAL_LONG)
    {
      m_config->Write(m_path, (long) pControl->GetSelection()) ;
      return TRUE;
    }
    else if (m_type == wxVAL_STRING)
    {
        m_config->Write(m_path, pControl->GetStringSelection());
        return TRUE;
    }
  } else
#endif
  if (m_validatorWindow->IsKindOf(CLASSINFO(wxStaticText)) )
  {
    wxStaticText* pControl = (wxStaticText*) m_validatorWindow;
    if (m_type == wxVAL_STRING)
    {
      m_config->Write(m_path, pControl->GetLabel()) ;
      return TRUE;
    }
  } else
  if (m_validatorWindow->IsKindOf(CLASSINFO(wxTextCtrl)) )
  {
    wxTextCtrl* pControl = (wxTextCtrl*) m_validatorWindow;
    if (m_type == wxVAL_STRING)
    {
      m_config->Write(m_path, pControl->GetValue()) ;
      return TRUE;
    }
    else if (m_type == wxVAL_LONG)
    {
        m_config->Write(m_path, (long) wxAtoi(pControl->GetValue()));
        return TRUE;
    }
  } else
  // array controls
#if 0
#if wxUSE_CHECKLISTBOX
#ifndef __WIN16__
  // NOTE: wxCheckListBox isa wxListBox, so wxCheckListBox MUST come first:
  if (m_validatorWindow->IsKindOf(CLASSINFO(wxCheckListBox)) )
  {
    wxCheckListBox* pControl = (wxCheckListBox*) m_validatorWindow;
    if (m_pArrayInt)
    {
      // clear our array
      m_pArrayInt->Clear();

      // add each selected item to our array
      size_t i,
             count = pControl->GetCount();
      for ( i = 0; i < count; i++ )
      {
        if (pControl->IsChecked(i))
          m_pArrayInt->Add(i);
      }

      return TRUE;
    }
    else
      return FALSE;
  } else
#endif
#endif
#if wxUSE_LISTBOX
  if (m_validatorWindow->IsKindOf(CLASSINFO(wxListBox)) )
  {
    wxListBox* pControl = (wxListBox*) m_validatorWindow;
    if (m_pArrayInt)
    {
      // clear our array
      m_pArrayInt->Clear();

      // add each selected item to our array
      size_t i,
             count = pControl->GetCount();
      for ( i = 0; i < count; i++ )
      {
        if (pControl->Selected(i))
          m_pArrayInt->Add(i);
      }

      return TRUE;
    }
  } else
#endif
#endif

  // unrecognized control, or bad pointer
    return FALSE;
  return FALSE;
}

/*
  Called by constructors to initialize ALL data members
*/
void wxConfigValidator::Initialize()
{
    m_path = wxEmptyString;
    m_config = NULL;
    m_type = wxVAL_UNKNOWN;

//    m_pArrayInt = 0;
}

#endif
  // wxUSE_VALIDATORS

