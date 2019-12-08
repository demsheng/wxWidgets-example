/////////////////////////////////////////////////////////////////////////////
// Name:        valconfig.h
// Purpose:     wxConfigValidator class
// Author:      Julian Smart
// Modified by:
// Created:     2003-04-20
// RCS-ID:      $Id: valconfig.h,v 1.1 2003/05/09 08:16:14 js Exp $
// Copyright:   (c) 2003 Julian Smart
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_VALCONFIGH__
#define _WX_VALCONFIGH__

#if defined(__GNUG__) && !defined(__APPLE__)
    #pragma interface "valconfig.h"
#endif

#include "wx/validate.h"
#include "wx/confbase.h"

#if wxUSE_VALIDATORS

#define wxVAL_UNKNOWN   0
#define wxVAL_LONG      1
#define wxVAL_STRING    2
#define wxVAL_BOOL      3
#define wxVAL_REAL      4

class wxConfigValidator: public wxValidator
{
DECLARE_CLASS(wxConfigValidator)
public:
  wxConfigValidator(const wxString& path, int valType, wxConfigBase* config);
  wxConfigValidator(const wxConfigValidator& copyFrom);

  ~wxConfigValidator();

  // Make a clone of this validator (or return NULL) - currently necessary
  // if you're passing a reference to a validator.
  // Another possibility is to always pass a pointer to a new validator
  // (so the calling code can use a copy constructor of the relevant class).
  virtual wxObject *Clone() const { return new wxConfigValidator(*this); }
  bool Copy(const wxConfigValidator& val);

  // Called when the value in the window must be validated.
  // This function can pop up an error message.
  virtual bool Validate(wxWindow * WXUNUSED(parent)) { return TRUE; }

  // Called to transfer data to the window
  virtual bool TransferToWindow();

  // Called to transfer data to the window
  virtual bool TransferFromWindow();

protected:
  void Initialize();

  int         m_type;
  wxString    m_path;
  wxConfigBase* m_config;

private:
// Cannot use
//  DECLARE_NO_COPY_CLASS(wxConfigValidator)
// because copy constructor is explicitly declared above;
// but no copy assignment operator is defined, so declare
// it private to prevent the compiler from defining it:
    wxConfigValidator& operator=(const wxConfigValidator&);
};

#endif
  // wxUSE_VALIDATORS

#endif    
  // _WX_VALCONFIGH__
