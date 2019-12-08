/////////////////////////////////////////////////////////////////////////////
// Name:        launch.h
// Purpose:     Launch utilities
// Author:      Julian Smart
// Modified by:
// Created:     2005-1-25
// RCS-ID:      $Id$
// Copyright:   (c) Julian Smart
// Licence:     wxWindows Licence
/////////////////////////////////////////////////////////////////////////////

#ifndef __LAUNCH_H__
#define __LAUNCH_H__

/*
 * Launch a document or executable
 */

bool wxLaunchFile(const wxString& command, const wxString& specificErrorMessage = wxEmptyString);

/*
 * View an HTML file
 */

bool wxViewHTMLFile(const wxString& url);

/*
 * View a PDF file
 */

bool wxViewPDFFile(const wxString& pdfFile);

/*
 * Play a sound file
 */

bool wxPlaySoundFile(const wxString& soundFile);

#undef
    // __LAUNCH_H__

