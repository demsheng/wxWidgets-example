/////////////////////////////////////////////////////////////////////////////
// Name:        riffleutils.cpp
// Purpose:     Misc utilities
// Author:      Julian Smart
// Modified by:
// Created:     2002-08-15
// Copyright:   (c) Julian Smart
// Licence:     wxWindows License
/////////////////////////////////////////////////////////////////////////////

#ifdef __GNUG__
    #pragma implementation "riffleutils.h"
#endif

#include "wx/wx.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#include "wx/config.h"
#include "wx/filename.h"
#include "wx/filesys.h"

#include "riffleutils.h"

// Find the absolute path where this application has been run from.
// argv0 is wxTheApp->argv[0]
// cwd is the current working directory (at startup)
// appVariableName is the name of a variable containing the directory for this app, e.g.
// MYAPPDIR. This is checked first.

wxString wxFindAppPath(const wxString& argv0, const wxString& cwd, const wxString& appVariableName,
                       const wxString& appName)
{
    wxString str;

    // Try appVariableName
    if (!appVariableName.IsEmpty())
    {
        str = wxGetenv(appVariableName);
        if (!str.IsEmpty())
            return str;
    }

    if (wxIsAbsolutePath(argv0))
        return wxPathOnly(argv0);
    else
    {
        // Is it a relative path?
        wxString currentDir(cwd);
        if (currentDir.Last() != wxFILE_SEP_PATH)
            currentDir += wxFILE_SEP_PATH;

        str = currentDir + argv0;
        if (wxFileExists(str))
            return wxPathOnly(str);
#ifdef __WXMAC__
            // The current directory may be above the actual
            // bundle. So if we find the bundle below it,
            // we know where we are.
            if (!appName.IsEmpty())
            {
                wxString p = currentDir + appName + wxT(".app");
                if (wxDirExists(p))
                {
                    p += wxFILE_SEP_PATH;
                    p += wxT("Content/MacOS");
                    return p;
                }
            }
#endif
    }

    // OK, it's neither an absolute path nor a relative path.
    // Search PATH.

    wxPathList pathList;
    pathList.AddEnvList(wxT("PATH"));
    str = pathList.FindAbsoluteValidPath(argv0);
    if (!str.IsEmpty())
        return wxPathOnly(str);

    // Failed
    return wxEmptyString;
}

/*!
 * Class for initialising config values if they're
 * not already present.
 */

/// Initialise if entry not found.
void wxConfigInit::Init(const wxString& path, long value)
{
    if (!m_config->Exists(path))
        m_config->Write(path, value);
}

/// Initialise if entry not found.
void wxConfigInit::Init(const wxString& path, bool value)
{
    if (!m_config->Exists(path))
        m_config->Write(path, value);
}

/// Initialise if entry not found.
void wxConfigInit::Init(const wxString& path, const wxString& value)
{
    if (!m_config->Exists(path))
        m_config->Write(path, value);
}
