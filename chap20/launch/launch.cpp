/////////////////////////////////////////////////////////////////////////////
// Name:        launch.cpp
// Purpose:     Launch utilities
// Author:      Julian Smart
// Modified by:
// Created:     2005-1-25
// RCS-ID:      $Id$
// Copyright:   (c) Julian Smart
// Licence:     wxWindows Licence
/////////////////////////////////////////////////////////////////////////////

#include "wx/wx.h"
#include "wx/process.h"
#include "wx/mimetype.h"
#include "wx/filename.h"
#include "wx/filesys.h"
#include "wx/utils.h"

#include <math.h>

#ifdef __WXMSW__
#include <windows.h>
#include "wx/msw/winundef.h"
#include "wx/msw/private.h"
#endif

#ifdef __WXMAC__
#include "wx/mac/private.h"
#ifndef __DARWIN__
#include "InternetConfig.h"
#endif

#ifdef __DARWIN__
extern "C" {
#include <mach/mach.h>
}
#include <CoreFoundation/CFMachPort.h>
#endif

#endif

#include "launch.h"

// Separate the command-line from the arguments
bool wxSeparateCommandAndArguments(const wxString& command, wxString& cmd, wxString& args);

/*
 * View an HTML file
 */

bool wxViewHTMLFile(const wxString& url)
{
#ifdef __WXMAC__
    wxString url1(url);
    if (url1.Left(5) != wxT("file:") && url1.Left(5) != wxT("http:"))
        url1 = wxFileSystem::FileNameToURL(url1);

    OSStatus err;
    ICInstance inst;
    SInt32 startSel;
    SInt32 endSel;

    err = ICStart(&inst, 'XXXX'); // put your app creator code here
    if (err == noErr) {
#if !TARGET_CARBON
        err = ICFindConfigFile(inst, 0, nil);
#endif
        if (err == noErr) {
            startSel = 0;
            endSel = wxStrlen(url1);
            err = ICLaunchURL(inst, (ConstStr255Param) "\p", (const void*) (const wxChar*) url1, endSel, &startSel, &endSel);
        }
        ICStop(inst);
        return true;
    }
    else
        return false;
#elif defined(__WXWINCE__)
    SHELLEXECUTEINFO shellInfo;
    shellInfo.cbSize = sizeof(SHELLEXECUTEINFO);
    shellInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
    shellInfo.hwnd = 0;
    shellInfo.lpVerb = wxT("Edit");
    shellInfo.lpFile = (const wxChar*) url;
    shellInfo.lpParameters = NULL;
    shellInfo.lpDirectory = NULL;
    shellInfo.nShow = SW_SHOW;
    shellInfo.hInstApp = wxGetInstance();
    shellInfo.lpIDList = 0;
    shellInfo.lpClass = 0;
    shellInfo.hkeyClass = 0;
    shellInfo.dwHotKey = 0;
    shellInfo.hIcon = 0;
    shellInfo.hProcess = 0;

    return ShellExecuteEx(& shellInfo) != 0;
#elif defined(__WXMSW__)
    ShellExecute((HWND) (wxTheApp->GetTopWindow() ? wxTheApp->GetTopWindow()->GetHWND() : NULL),
                 NULL, url.c_str(),NULL,"",SW_SHOWNORMAL);
    return true;
#else
    bool ok = false;

    wxFileType *ft = wxTheMimeTypesManager->GetFileTypeFromExtension(wxT("html"));
    if ( ft )
    {
        wxString cmd;
        ok = ft->GetOpenCommand(&cmd,
                                 wxFileType::MessageParameters(url, _T("")));
        delete ft;

        if (ok)
        {
            cmd.Replace(wxT("file://"), wxT(""));

            ok = (wxExecute(cmd, false) != 0);
            return ok;
        }
    }

    if (!ok)
    {
        wxString command = wxGetenv(wxT("BROWSER"));
        if (command.IsEmpty())
        {
            if (wxFileExists(wxT("/usr/bin/htmlview")))
                command = wxT("/usr/bin/htmlview");
        }
        if (!command.IsEmpty())
        {
            wxString url2 = url;
            if (url2.Find(wxT("http://")) == -1 && url2.Find(wxT("file:")) == -1)
                url2 = wxT("file:") + url2;
            command = command + wxT(" \"") + url2 + wxT("\"");

            ok = (wxExecute(command, false) != 0);
            if (ok)
                return true;
        }
    }

    if (!ok)
    {
        wxMessageBox(_("Could not determine the file type for extension html. Please edit your MIME types or set the BROWSER environment variable."), wxT("Browsing Problem"), wxOK|wxICON_EXCLAMATION);
    }
    return ok;
#endif
}

/*
 * View a PDF file
 */

bool wxViewPDFFile(const wxString& pdfFile)
{
#ifdef __WXMAC__
    return wxLaunchFile(pdfFile);
#elif defined(__WXMSW__)
    return wxLaunchFile(pdfFile, _("Please download and install a suitable PDF viewer, such as Adobe Acrobat Reader."));
#else
    bool ok = false;

    wxFileType *ft = wxTheMimeTypesManager->GetFileTypeFromExtension(wxT("pdf"));
    if ( ft )
    {
        wxString cmd;
        wxString params = pdfFile;
        // Unix doesn't tend to have quotes already in the
        // command, MSW does
#ifdef __UNIX__        
        if (params.GetChar(0) != wxT('"'))
            params = wxT("\"") + params + wxT("\"");
#endif
        ok = ft->GetOpenCommand(&cmd,
                                wxFileType::MessageParameters(params, _T("")));
#ifdef __UNIX__
        // Just in case we accidentally have double-quotes
        cmd.Replace(wxT("\"\""), wxT("\""));
#endif

        delete ft;

        if (ok)
        {
            ok = (wxExecute(cmd, false) != 0);
            return ok;
        }
    }

    if (!ok)
    {
        wxString command = wxGetenv(wxT("PDFVIEWER"));
        if (command.IsEmpty())
        {
            if (wxFileExists(wxT("/usr/bin/xpdf")))
                command = wxT("/usr/bin/xpdf");
        }
        if (!command.IsEmpty())
        {
            command = command + wxT(" \"") + pdfFile + wxT("\"");

            ok = (wxExecute(command, false) != 0);
            if (ok)
                return true;
        }
    }

    if (!ok)
    {
        wxMessageBox(_("Could not determine the file type for extension pdf. Please edit your MIME types, or set the PDFVIEWER environment variable."), wxT("PDF Viewing Problem"), wxOK|wxICON_EXCLAMATION);
    }
    return ok;
#endif
}

/*
 * Play a sound file
 */

bool wxPlaySoundFile(const wxString& soundFile)
{
#ifdef __WXMAC__
    return wxLaunchFile(soundFile, _("Please install a suitable sound player."));
#elif defined(__WXMSW__)
    return wxLaunchFile(soundFile, _("Please download and install a suitable sound player, such as Windows Media Player."));
#else
    bool ok = false;

    wxFileType *ft = wxTheMimeTypesManager->GetFileTypeFromExtension(wxT("mp3"));
    if ( ft )
    {
        wxString cmd;
        wxString params = soundFile;
        // Unix doesn't tend to have quotes already in the
        // command, MSW does
#ifdef __UNIX__
        if (params.GetChar(0) != wxT('"'))
            params = wxT("\"") + params + wxT("\"");
#endif
        ok = ft->GetOpenCommand(&cmd,
                                 wxFileType::MessageParameters(params, _T("")));
#ifdef __UNIX__
        // Just in case we accidentally have double-quotes
        cmd.Replace(wxT("\"\""), wxT("\""));
#endif
        delete ft;

        if (ok)
        {
            ok = (wxExecute(cmd, false) != 0);
            return ok;
        }
    }

    if (!ok)
    {
        wxString command = wxGetenv(wxT("AUDIOPLAYER"));
        if (command.IsEmpty())
        {
            if (wxFileExists(wxT("/usr/bin/xmms")))
                command = wxT("/usr/bin/xmms");
        }
        if (!command.IsEmpty())
        {
            command = command + wxT(" \"") + soundFile + wxT("\"");

            ok = (wxExecute(command, false) != 0);
            if (ok)
                return true;
        }
    }

    if (!ok)
    {
        wxMessageBox(_("Could not determine how to play sound files . Please edit your MIME types, or set the AUDIOPLAYER environment variable."), wxT("Sound Playing Problem"), wxOK|wxICON_EXCLAMATION);
    }
    return ok;
#endif
}

#ifdef __WXMAC__

/*
Below FinderLaunch function comes from:
http://developer.apple.com/technotes/tn/tn1002.html#fndrask
*/
    /* FinderLaunch converts a list of nTargets FSSpec records
    pointed to by the targetList parameter and converts the
    list to an Apple Event.  It then sends that event to the
    Finder.  The array of FSSpec records pointed to by the
    targetList parameter may contain references to files,
    folders, or applications.  The net effect of this command
    is equivalent to the user selecting an icon in one of the
    Finder's windows and then choosing the open command from
    the Finder's file menu. */

static OSErr FinderLaunch(long nTargets, FSSpec *targetList) {
    OSErr err;
    AppleEvent theAEvent, theReply;
    AEAddressDesc fndrAddress;
    AEDescList targetListDesc;
    OSType fndrCreator;
    Boolean wasChanged;
    AliasHandle targetAlias;
    long index;

        /* set up locals  */
    AECreateDesc(typeNull, NULL, 0, &theAEvent);
    AECreateDesc(typeNull, NULL, 0, &fndrAddress);
    AECreateDesc(typeNull, NULL, 0, &theReply);
    AECreateDesc(typeNull, NULL, 0, &targetListDesc);
    targetAlias = NULL;
    fndrCreator = 'MACS';

        /* verify parameters */
    if ((nTargets == 0) || (targetList == NULL)) {
        err = paramErr;
        goto bail;
    }

        /* create an open documents event targeting the
        finder */
    err = AECreateDesc(typeApplSignature, (Ptr) &fndrCreator,
        sizeof(fndrCreator), &fndrAddress);
    if (err != noErr) goto bail;
    err = AECreateAppleEvent(kCoreEventClass, kAEOpenDocuments,
        &fndrAddress, kAutoGenerateReturnID,
        kAnyTransactionID, &theAEvent);
    if (err != noErr) goto bail;

        /* create the list of files to open */
    err = AECreateList(NULL, 0, false, &targetListDesc);
    if (err != noErr) goto bail;
    for ( index=0; index < nTargets; index++) {
        if (targetAlias == NULL)
            err = NewAlias(NULL, (targetList + index),
                  &targetAlias);
        else err = UpdateAlias(NULL, (targetList + index),
                   targetAlias, &wasChanged);
        if (err != noErr) goto bail;
        HLock((Handle) targetAlias);
        err = AEPutPtr(&targetListDesc, (index + 1),
              typeAlias, *targetAlias,
              GetHandleSize((Handle) targetAlias));
        HUnlock((Handle) targetAlias);
        if (err != noErr) goto bail;
    }

        /* add the file list to the Apple Event */
    err = AEPutParamDesc(&theAEvent, keyDirectObject,
          &targetListDesc);
    if (err != noErr) goto bail;

        /* send the event to the Finder */
    err = AESend(&theAEvent, &theReply, kAENoReply,
        kAENormalPriority, kAEDefaultTimeout, NULL, NULL);

        /* clean up and leave */
bail:
    if (targetAlias != NULL) DisposeHandle((Handle) targetAlias);
    AEDisposeDesc(&targetListDesc);
    AEDisposeDesc(&theAEvent);
    AEDisposeDesc(&fndrAddress);
    AEDisposeDesc(&theReply);
    return err;
}
#endif

// Execute the command.
// 1. If HTML file, call wxViewHTMLFile.
// 2. If there's an extension and an associated
//    application, run the app + document.
// 3. If the application exists, run the application.
bool wxLaunchFile(const wxString& command, const wxString& specificErrorMessage)
{
    // First see if there are any arguments
    wxString actualCommand, arguments;
    wxSeparateCommandAndArguments(command, actualCommand, arguments);

    wxString path, file, ext;
    wxSplitPath(actualCommand, & path, & file, & ext);

    wxString extLower(ext.Lower());
    if (extLower == wxT("html") || 
        extLower == wxT("htm") || 
        extLower == wxT("sgml"))
    {
        wxViewHTMLFile(command);
        return true;
    }
#ifdef __WXMAC__
    else
    {
        wxFileName fn(command);
        fn.Normalize(wxPATH_NORM_DOTS|wxPATH_NORM_TILDE|wxPATH_NORM_ABSOLUTE);
        wxString cmd = fn.GetFullPath();
        wxSplitPath(command, & path, & file, & ext);

        if (ext.IsEmpty() || ext == wxT(".app"))
        {
              if (cmd.GetChar(0) != wxT('"'))
                  cmd = wxT("\"") + cmd + wxT("\"");
              return (wxExecute(cmd) != 0);
        }

        FSSpec fsSpec;
        wxMacFilename2FSSpec(command, &fsSpec);

        return ( FinderLaunch(1 /*one file*/, &fsSpec) == noErr ) ;
    }
#else
    else
    {
        bool isApp = false;
#ifdef __WXMSW__
        if (ext.Lower() == wxT("exe") ||
            ext.Lower() == wxT("com") ||
            ext.Lower() == wxT("cmd") ||
            ext.Lower() == wxT("bat"))
            isApp = true;
#endif
        if (ext.IsEmpty())
            isApp = true;

        if (isApp)
        {
            wxString command2 = actualCommand ;
            if (command2.GetChar(0) != wxT('"'))
                command2 = wxT("\"") + actualCommand + wxT("\"");
            return (wxExecute(command2) != 0);
        }

        wxString msg;
        wxString errMsg = specificErrorMessage;
        if (errMsg.IsEmpty())
        {
#ifdef __WXMSW__
            errMsg = _("Please install a suitable application.");
#else
            errMsg = _("Please install a suitable application and/or edit your MIME types.");
#endif
        }

        if (!ext.IsEmpty())
        {
            wxFileType *ft = wxTheMimeTypesManager->GetFileTypeFromExtension(ext);
            if ( !ft )
            {
                wxString msg;
                msg.Printf(_("Could not determine the application for extension %s.\n%s"), (const wxChar*) ext, (const wxChar*) errMsg);
                wxMessageBox(msg, wxT("Run command"), wxOK|wxICON_EXCLAMATION);
                return false;
            }
            
            wxString cmd;
            wxString params = command;
#ifdef __UNIX__
            if (params.GetChar(0) != wxT('"'))
                params = wxT("\"") + params + wxT("\"");
#endif
            bool ok = ft->GetOpenCommand(&cmd,
                wxFileType::MessageParameters(params));
#ifdef __UNIX__
            // Just in case we accidentally have double-quotes
            cmd.Replace(wxT("\"\""), wxT("\""));
#endif
            delete ft;
            
            if (!ok)
            {
                // TODO: some kind of configuration dialog here.
                wxString msg;
                msg.Printf(wxT("Could not determine the command for running %s.\n%s"), (const wxChar*) command, (const wxChar*) errMsg);

                wxMessageBox(msg, wxT("Run command"), wxOK|wxICON_EXCLAMATION);
                return false;
            }

            ok = (wxExecute(cmd, false) != 0);
            return ok;
        }
        return false;
    }
#endif
    return true;
}

// Separate the command-line from the arguments
bool wxSeparateCommandAndArguments(const wxString& command, wxString& cmd, wxString& args)
{
    wxString lowerCaseCommand(command.Lower());
    cmd = command;
    args = wxEmptyString;

#ifdef __WXMSW__
    wxString command2 = command;
    command2.Replace(wxT("\""), wxT(""));
    wxArrayString toFind;
    toFind.Add(wxT(".exe "));
    toFind.Add(wxT(".com "));
    toFind.Add(wxT(".bat "));
    toFind.Add(wxT(".cmd "));
    size_t i;
    size_t sz = toFind.GetCount();
    for (i = 0; i < sz; i++)
    {
        int pos = command2.Find(toFind[i]);
        if (pos > -1)
        {
            cmd = command2.Mid(0, pos + toFind[i].Length() - 1);
            args = command2.Mid(pos + toFind[i].Length());
            return true;
        }
    }
    return true;
#else
    // Assume no spaces on other platforms
    cmd = command.BeforeFirst(wxT(' '));
    if (cmd != command)
    {
        if (wxFileExists(cmd))
            args = command.AfterFirst(wxT(' '));
        else
        {
            // In fact there were spaces (the command didn't exist)
            // so restore the command.
            cmd = command;
        }
    }
#endif
    return true;
}

