/////////////////////////////////////////////////////////////////////////////
// Name:        riffleutils.h
// Purpose:     Utility classes and functions
// Author:      Julian Smart
// Modified by:
// Created:     2002-08-15
// Copyright:   (c) Julian Smart
// Licence:     wxWindows License
/////////////////////////////////////////////////////////////////////////////

#ifndef _RIFFLEUTILS_H_
#define _RIFFLEUTILS_H_

#if defined(__GNUG__) && !defined(__APPLE__)
    #pragma interface "riffleutils.h"
#endif

/*!
 * Class for initialising config values if they're
 * not already present.
 */

class WXDLLEXPORT wxConfigBase;

class wxConfigInit
{
public:
    wxConfigInit(wxConfigBase* config) { m_config = config; }

    /// Initialise if entry not found.
    void Init(const wxString& path, long value);

    /// Initialise if entry not found.
    void Init(const wxString& path, bool value);

    /// Initialise if entry not found.
    void Init(const wxString& path, const wxString& value);

private:
    wxConfigBase*   m_config;
};

// Find the absolute path where this application has been run from.
// argv0 is wxTheApp->argv[0]
// cwd is the current working directory (at startup)
// appVariableName is the name of a variable containing the directory for this app, e.g.
//   MYAPPDIR. This is used as a last resort. Or should it be a first resort?
// appName is e.g. StoryLines, which will be used to check if cwd is the
// path above the bundle, or not.
wxString wxFindAppPath(const wxString& argv0, const wxString& cwd, const wxString& appVariableName = wxEmptyString, const wxString& appName = wxEmptyString);


#endif
  // _RIFFLEUTILS_H_

