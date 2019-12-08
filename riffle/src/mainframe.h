/////////////////////////////////////////////////////////////////////////////
// Name:        mainframe.h
// Purpose:     Main window of the application
// Author:      Julian Smart
// Modified by: 
// Created:     03/26/05 12:36:16
// RCS-ID:      
// Copyright:   (c) Julian Smart
// Licence:     wxWindows License
/////////////////////////////////////////////////////////////////////////////

#ifndef _MAINFRAME_H_
#define _MAINFRAME_H_

#if defined(__GNUG__) && !defined(__APPLE__)
#pragma interface "mainframe.cpp"
#endif

/*!
 * Includes
 */

////@begin includes
#include "wx/frame.h"
#include "wx/toolbar.h"
#include "wx/splitter.h"
#include "wx/dirctrl.h"
#include "wx/statusbr.h"
////@end includes

/*!
 * Forward declarations
 */

////@begin forward declarations
class wxSplitterWindow;
class wxGenericDirCtrl;
class RiffleBrowserViewer;
////@end forward declarations

/*!
 * Control identifiers
 */

////@begin control identifiers
#define ID_FRAME 10000
#define SYMBOL_RIFFLEFRAME_STYLE wxDEFAULT_FRAME_STYLE
#define SYMBOL_RIFFLEFRAME_TITLE _("Riffle Frame")
#define SYMBOL_RIFFLEFRAME_IDNAME ID_FRAME
#define SYMBOL_RIFFLEFRAME_SIZE wxSize(500, 300)
#define SYMBOL_RIFFLEFRAME_POSITION wxDefaultPosition
#define ID_SHOWTHUMBNAILS 10007
#define ID_ZOOMIN 10005
#define ID_ZOOMOUT 10011
#define ID_ACTUALSIZE 10012
#define ID_FITWINDOW 10003
#define ID_ABOUT 10004
#define ID_TOOLBAR 10001
#define ID_SPLITTERWINDOW 10006
#define ID_DIRCTRL 10009
#define ID_THUMBNAILS 10010
#define ID_STATUSBAR 10008
////@end control identifiers

/*!
 * Compatibility
 */

#ifndef wxCLOSE_BOX
#define wxCLOSE_BOX 0x1000
#endif
#ifndef wxFIXED_MINSIZE
#define wxFIXED_MINSIZE 0
#endif

// Distinguish between MSW desktop and MSW WinCE
#if defined(__WXMSW__) && !defined(__WXWINCE__)
#define __WXMSW_DESKTOP__ 1
#endif

/*!
 * RiffleFrame class declaration
 */

class RiffleFrame: public wxFrame
{    
    DECLARE_CLASS( RiffleFrame )
    DECLARE_EVENT_TABLE()

public:
    /// Constructors
    RiffleFrame( );
    RiffleFrame( wxWindow* parent, wxWindowID id = SYMBOL_RIFFLEFRAME_IDNAME, const wxString& caption = SYMBOL_RIFFLEFRAME_TITLE, const wxPoint& pos = SYMBOL_RIFFLEFRAME_POSITION, const wxSize& size = SYMBOL_RIFFLEFRAME_SIZE, long style = SYMBOL_RIFFLEFRAME_STYLE );

    bool Create( wxWindow* parent, wxWindowID id = SYMBOL_RIFFLEFRAME_IDNAME, const wxString& caption = SYMBOL_RIFFLEFRAME_TITLE, const wxPoint& pos = SYMBOL_RIFFLEFRAME_POSITION, const wxSize& size = SYMBOL_RIFFLEFRAME_SIZE, long style = SYMBOL_RIFFLEFRAME_STYLE );

    /// Creates the controls and sizers
    void CreateControls();

    /// Manually added since it requires the tree control hidden inside
    /// the wxDirCtrl
    void OnDirctrlSelChanged( wxTreeEvent& event );

    /// Set the thumbnail size according to the current settings
    void UpdateThumbnailSize();

    /// Changes orientation if necessary
    void CheckOrientation();

////@begin RiffleFrame event handler declarations

    /// wxEVT_CLOSE_WINDOW event handler for ID_FRAME
    void OnCloseWindow( wxCloseEvent& event );

    /// wxEVT_IDLE event handler for ID_FRAME
    void OnIdle( wxIdleEvent& event );

#if defined(__WXWINCE__)
    /// wxEVT_COMMAND_MENU_SELECTED event handler for wxID_HELP_CONTENTS
    void OnHelpContentsClick( wxCommandEvent& event );

#endif
#if defined(__WXWINCE__)
    /// wxEVT_COMMAND_MENU_SELECTED event handler for wxID_ABOUT
    void OnAboutClick( wxCommandEvent& event );

#endif
#if defined(__WXWINCE__)
    /// wxEVT_COMMAND_MENU_SELECTED event handler for wxID_COPY
    void OnCopyClick( wxCommandEvent& event );

    /// wxEVT_UPDATE_UI event handler for wxID_COPY
    void OnCopyUpdate( wxUpdateUIEvent& event );

#endif
    /// wxEVT_COMMAND_MENU_SELECTED event handler for wxID_EXIT
    void OnExitClick( wxCommandEvent& event );

#if defined(__WXMAC__)
    /// wxEVT_COMMAND_MENU_SELECTED event handler for wxID_CLOSE
    void OnCloseClick( wxCommandEvent& event );

#endif
#if defined(__WXMSW_DESKTOP__) || defined(__WXMAC__) || defined(__WXGTK__)
    /// wxEVT_COMMAND_MENU_SELECTED event handler for wxID_COPY
    void OnCopyClick( wxCommandEvent& event );

    /// wxEVT_UPDATE_UI event handler for wxID_COPY
    void OnCopyUpdate( wxUpdateUIEvent& event );

#endif
    /// wxEVT_COMMAND_MENU_SELECTED event handler for ID_ZOOMIN
    void OnZoominClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_MENU_SELECTED event handler for ID_ZOOMOUT
    void OnZoomoutClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_MENU_SELECTED event handler for ID_ACTUALSIZE
    void OnActualsizeClick( wxCommandEvent& event );

    /// wxEVT_UPDATE_UI event handler for ID_ACTUALSIZE
    void OnActualsizeUpdate( wxUpdateUIEvent& event );

    /// wxEVT_COMMAND_MENU_SELECTED event handler for ID_FITWINDOW
    void OnFitwindowClick( wxCommandEvent& event );

    /// wxEVT_UPDATE_UI event handler for ID_FITWINDOW
    void OnFitwindowUpdate( wxUpdateUIEvent& event );

    /// wxEVT_COMMAND_MENU_SELECTED event handler for wxID_PREFERENCES
    void OnPreferencesClick( wxCommandEvent& event );

#if defined(__WXMAC__) || defined(__WXGTK__) || defined(__WXMSW_DESKTOP__)
    /// wxEVT_COMMAND_MENU_SELECTED event handler for wxID_HELP_CONTENTS
    void OnHelpContentsClick( wxCommandEvent& event );

#endif
#if defined(__WXMSW_DESKTOP__) || defined(__WXMAC__) || defined(__WXGTK__)
    /// wxEVT_COMMAND_MENU_SELECTED event handler for wxID_ABOUT
    void OnAboutClick( wxCommandEvent& event );

#endif
#if defined(__WXMAC__)
    /// wxEVT_COMMAND_MENU_SELECTED event handler for ID_ABOUT
    void OnMacAboutClick( wxCommandEvent& event );

#endif
    /// wxEVT_COMMAND_MENU_SELECTED event handler for ID_SHOWTHUMBNAILS
    void OnShowthumbnailsClick( wxCommandEvent& event );

    /// wxEVT_UPDATE_UI event handler for ID_SHOWTHUMBNAILS
    void OnShowthumbnailsUpdate( wxUpdateUIEvent& event );

////@end RiffleFrame event handler declarations

////@begin RiffleFrame member function declarations

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end RiffleFrame member function declarations

    /// Should we show tooltips?
    static bool ShowToolTips();

////@begin RiffleFrame member variables
    wxSplitterWindow* m_splitterWindow;
    wxGenericDirCtrl* m_dirCtrl;
    RiffleBrowserViewer* m_browserCtrl;
////@end RiffleFrame member variables

    // Work around calling selection callback twice
    int m_selectionCount;
};

#endif
    // _MAINFRAME_H_
