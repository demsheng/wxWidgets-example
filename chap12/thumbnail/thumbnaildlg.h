/////////////////////////////////////////////////////////////////////////////
// Name:        thumbnaildlg.h
// Purpose:     
// Author:      Julian Smart
// Modified by: 
// Created:     03/16/04 16:08:52
// RCS-ID:      
// Copyright:   (c) Julian Smart
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#ifndef _THUMBNAILDLG_H_
#define _THUMBNAILDLG_H_

#if defined(__GNUG__) && !defined(__APPLE__)
#pragma interface "thumbnaildlg.cpp"
#endif

/*!
 * Includes
 */

////@begin includes
#include "wx/wx.h"
#include "wx/splitter.h"
#include "wx/dirctrl.h"
////@end includes

#include "thumbnailctrl.h"

/*!
 * Forward declarations
 */

////@begin forward declarations
class wxGenericDirCtrl;
class wxThumbnailCtrl;
////@end forward declarations

/*!
 * Control identifiers
 */

////@begin control identifiers
#define ID_IMAGE_BROWSER_DIALOG 21500
#define SYMBOL_WXTHUMBNAILBROWSERDIALOG_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX
#define SYMBOL_WXTHUMBNAILBROWSERDIALOG_TITLE _("Image Browser Dialog")
#define SYMBOL_WXTHUMBNAILBROWSERDIALOG_IDNAME ID_IMAGE_BROWSER_DIALOG
#define SYMBOL_WXTHUMBNAILBROWSERDIALOG_SIZE wxSize(400, 300)
#define SYMBOL_WXTHUMBNAILBROWSERDIALOG_POSITION wxPoint(0, 0)
#define ID_IMAGE_BROWSER_SPLITTER 21501
#define ID_IMAGE_BROWSER_DIRCTRL 21502
#define ID_IMAGE_BROWSER_BROWSER 21503
////@end control identifiers

/*!
 * Compatibility
 */

#ifndef wxCLOSE_BOX
#define wxCLOSE_BOX 0x1000
#endif

/*!
 * wxThumbnailBrowserDialog class declaration
 */

class wxThumbnailBrowserDialog: public wxDialog
{    
    DECLARE_CLASS( wxThumbnailBrowserDialog )
    DECLARE_EVENT_TABLE()

public:
    /// Constructors
    wxThumbnailBrowserDialog( );
    wxThumbnailBrowserDialog( wxWindow* parent, wxWindowID id = SYMBOL_WXTHUMBNAILBROWSERDIALOG_IDNAME, const wxString& caption = SYMBOL_WXTHUMBNAILBROWSERDIALOG_TITLE, const wxPoint& pos = SYMBOL_WXTHUMBNAILBROWSERDIALOG_POSITION, const wxSize& size = SYMBOL_WXTHUMBNAILBROWSERDIALOG_SIZE, long style = SYMBOL_WXTHUMBNAILBROWSERDIALOG_STYLE );

    /// Creation
    bool Create( wxWindow* parent, wxWindowID id = SYMBOL_WXTHUMBNAILBROWSERDIALOG_IDNAME, const wxString& caption = SYMBOL_WXTHUMBNAILBROWSERDIALOG_TITLE, const wxPoint& pos = SYMBOL_WXTHUMBNAILBROWSERDIALOG_POSITION, const wxSize& size = SYMBOL_WXTHUMBNAILBROWSERDIALOG_SIZE, long style = SYMBOL_WXTHUMBNAILBROWSERDIALOG_STYLE );

    /// Creates the controls and sizers
    void CreateControls();

    /// Shows the images in the given dir
    void ShowFolder(const wxString& path);

    /// Accessors
    
    void SetSelection(const wxString& filename);
    wxString GetSelection() const ;
    wxArrayString GetSelections() const ;

    /// Custom event handlers
    void OnLeftClick(wxThumbnailEvent& event);
    void OnLeftDClick(wxThumbnailEvent& event);

////@begin wxThumbnailBrowserDialog event handler declarations

    /// wxEVT_COMMAND_TREE_SEL_CHANGED event handler for ID_IMAGE_BROWSER_DIRCTRL
    void OnImageBrowserDirctrlSelChanged( wxTreeEvent& event );

////@end wxThumbnailBrowserDialog event handler declarations

////@begin wxThumbnailBrowserDialog member function declarations

////@end wxThumbnailBrowserDialog member function declarations

    /// Should we show tooltips?
    static bool ShowToolTips();

////@begin wxThumbnailBrowserDialog member variables
    wxGenericDirCtrl* m_dirCtrl;
    wxThumbnailCtrl* m_imageBrowser;
////@end wxThumbnailBrowserDialog member variables

    // Work around calling selection callback twice
    int m_selectionCount;
    
    // Initial path or filename, if set
    wxString m_initialSelection;
};

#endif
    // _THUMBNAILDLG_H_
