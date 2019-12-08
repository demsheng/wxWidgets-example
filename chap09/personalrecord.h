/////////////////////////////////////////////////////////////////////////////
// Name:        personalrecord.h
// Purpose:     Dialog to get name, age, sex, and voting preference
// Author:      Julian Smart
// Created:     02/28/04 06:52:49
// Copyright:   (c) 2004, Julian Smart
// Licence:     wxWindows license
/////////////////////////////////////////////////////////////////////////////

#ifndef _PERSONALRECORD_H_
#define _PERSONALRECORD_H_

#ifdef __GNUG__
#pragma interface "personalrecord.cpp"
#endif

/*!
 * Includes
 */

#include "wx/spinctrl.h"
#include "wx/statline.h"

/*!
 * Control identifiers
 */

enum {
    ID_PERSONAL_RECORD = 10000,
    ID_NAME = 10001,
    ID_AGE = 10002,
    ID_SEX = 10003,
    ID_VOTE = 10006,
    ID_RESET = 10004
};

/*!
 * PersonalRecordDialog class declaration
 */

class PersonalRecordDialog: public wxDialog
{    
    DECLARE_CLASS( PersonalRecordDialog )
    DECLARE_EVENT_TABLE()

public:

//// Constructors

    PersonalRecordDialog( );

    PersonalRecordDialog( wxWindow* parent,
      wxWindowID id = ID_PERSONAL_RECORD,
      const wxString& caption = wxT("Personal Record"),
      const wxPoint& pos = wxDefaultPosition,
      const wxSize& size = wxDefaultSize,
      long style = wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU );

    /// Member initialisation
    void Init();

    /// Creation
    bool Create( wxWindow* parent,
      wxWindowID id = ID_PERSONAL_RECORD,
      const wxString& caption = wxT("Personal Record"),
      const wxPoint& pos = wxDefaultPosition,
      const wxSize& size = wxDefaultSize,
      long style = wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU );

    /// Creates the controls and sizers
    void CreateControls();

    /// Sets the validators for the dialog controls
    void SetDialogValidators();

    /// Sets the help text for the dialog controls
    void SetDialogHelp();

    /// Name accessors
    void SetName(const wxString& name) { m_name = name; }
    wxString GetName() const { return m_name; }

    /// Age accessors
    void SetAge(int age) { m_age = age; }
    int GetAge() const { return m_age; }

    /// Sex accessors (male = false, female = true)
    void SetSex(bool sex) { sex ? m_sex = 1 : m_sex = 0; }
    bool GetSex() const { return m_sex == 1; }

    /// Does the person vote?
    void SetVote(bool vote) { m_vote = vote; }
    bool GetVote() const { return m_vote; }

//// PersonalRecordDialog event handler declarations

    /// wxEVT_UPDATE_UI event handler for ID_VOTE
    void OnVoteUpdate( wxUpdateUIEvent& event );

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_RESET
    void OnResetClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_HELP
    void OnHelpClick( wxCommandEvent& event );

//// PersonalRecordDialog member variables

    /// Data members
    wxString    m_name;
    int         m_age;
    int         m_sex;
    bool        m_vote;
};

#endif
    // _PERSONALRECORD_H_

