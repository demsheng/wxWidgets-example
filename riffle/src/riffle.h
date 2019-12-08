/////////////////////////////////////////////////////////////////////////////
// Name:        riffle.h
// Purpose:     An image viewer application
// Author:      Julian Smart
// Modified by: 
// Created:     03/26/05 12:33:53
// RCS-ID:      
// Copyright:   (c) Julian Smart
// Licence:     wxWindows License
/////////////////////////////////////////////////////////////////////////////

#ifndef _RIFFLE_H_
#define _RIFFLE_H_

#if defined(__GNUG__) && !defined(__APPLE__)
#pragma interface "riffle.cpp"
#endif

/*!
 * Includes
 */

////@begin includes
#include "wx/image.h"
#include "mainframe.h"
////@end includes

#include "wx/help.h"
#include "wx/config.h"

/*!
 * Forward declarations
 */

////@begin forward declarations
////@end forward declarations

/*!
 * Control identifiers
 */

////@begin control identifiers
////@end control identifiers

/*!
 * RiffleApp class declaration
 */

class RiffleApp: public wxApp
{    
    DECLARE_CLASS( RiffleApp )
    DECLARE_EVENT_TABLE()

public:
    /// Constructor
    RiffleApp();

    /// Initialises the application
    virtual bool OnInit();

    /// Called on exit
    virtual int OnExit();
    
    /// Show help
    void ShowHelp(const wxString& helpTopic = wxEmptyString);

    /// Initialize config values
    void InitConfig();

//// Accessors

    /// Returns the application directory
    wxString GetAppDir() const { return m_appDir; }

    // Prepend the current app program directory to the name
    wxString GetFullAppPath(const wxString& filename) const;

    // Returns the wxConfig object
    wxConfig* GetConfig() const { return m_config; }

////@begin RiffleApp event handler declarations

////@end RiffleApp event handler declarations

////@begin RiffleApp member function declarations

////@end RiffleApp member function declarations

////@begin RiffleApp member variables
////@end RiffleApp member variables

    wxHelpControllerBase*   m_helpController;
    wxConfig*               m_config;
    wxString                m_appDir;
};

/*!
 * Application instance declaration 
 */

////@begin declare app
DECLARE_APP(RiffleApp)
////@end declare app

#endif
    // _RIFFLE_H_
