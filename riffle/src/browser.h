/////////////////////////////////////////////////////////////////////////////
// Name:        browser.h
// Purpose:     Implements a combined browser/image viewer window
// Author:      Julian Smart
// Modified by: 
// Created:     03/26/05 12:36:16
// RCS-ID:      
// Copyright:   (c) Julian Smart
// Licence:     wxWindows License
/////////////////////////////////////////////////////////////////////////////

#ifndef _BROWSER_H_
#define _BROWSER_H_

#if defined(__GNUG__) && !defined(__APPLE__)
#pragma interface "browser.cpp"
#endif

/*!
 * Includes
 */

#include "wx/image.h"
#include "thumbnailctrl.h"

#define ID_THUMBNAILCTRL    2000
#define ID_IMAGEVIEWER      2001

/*!
 * RiffleImageViewer is a window that displays an image at a selected scale.
 */

class RiffleImageViewer: public wxScrolledWindow
{    
    DECLARE_CLASS( RiffleImageViewer )
    DECLARE_EVENT_TABLE()

public:
    /// Constructors
    RiffleImageViewer( ) { Init(); }
    RiffleImageViewer( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = 0 );

    /// Initialisation
    void Init();

    /// Creation
    bool Create( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = 0 );

    /// Load from disk
    bool LoadImage(const wxString& filename);

    /// Set/get image
    void SetImage(const wxImage& image) { m_image = image; }
    wxImage GetImage() const { return m_image; }
    bool HasImage() const { return m_image.Ok(); }

    /// Set/get bitmap
    void SetBitmap(const wxBitmap& bitmap) { m_bitmap = bitmap; }
    wxBitmap GetBitmap() const { return m_bitmap; }

    /// Set/get scale
    void SetScale(int scale) { m_scale = scale; }
    int GetScale() const { return m_scale; }

    /// Zoom in/out
    void Zoom(bool zoomIn, int inc = 10) ;

    /// Fit to window
    void ZoomFitToWindow() ;

    /// Actual size
    void ZoomActualSize() ;

    /// Copies the image to the clipboard
    bool CopyToClipboard();

    /// Place image
    void OnSize(wxSizeEvent& event);

    /// Paint image
    void OnPaint(wxPaintEvent& event);

    /// Get the size and position for the image
    bool GetPictureRect(wxRect& rect) ;

    /// Update the scrollbars according to current image and scale
    void UpdateScrollbarSizes();

protected:
    /// Original image
    wxImage     m_image;

    /// Scaled bitmap
    wxBitmap    m_bitmap;

    /// Scale percent. zero means fit to window
    int         m_scale;
};

/*!
 * RiffleBrowserViewer shows either the thumbnail browser or
 * the image viewer.
 */

class RiffleBrowserViewer: public wxWindow
{    
    DECLARE_CLASS( RiffleBrowserViewer )
    DECLARE_EVENT_TABLE()

public:
    /// Constructors
    RiffleBrowserViewer( ) { Init(); }
    RiffleBrowserViewer( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = 0 );

    /// Member initialisation
    void Init();

    /// Creation
    bool Create( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = 0 );

    /// Creates the controls
    void CreateControls();

    /// Show the thumbnails, or image
    void ShowThumbnails(bool showThumbnails);

    /// Show an image
    bool ShowImage(const wxString& imageFilename);

    /// Show thumbnails for the given a folder
    bool ShowFolder(const wxString& path);

    /// Showing thumbnails?
    bool ShowingThumbnails() const { return m_showingThumbnails; }

    /// Showing image?
    bool ShowingImage() const { return !m_showingThumbnails; }

    /// Gets the thumbnail control
    wxThumbnailCtrl* GetThumbnailCtrl() const { return m_thumbnailCtrl; }

    /// Gets the image viewer window
    RiffleImageViewer* GetImageViewerWindow() const { return m_imageViewer; }

/// Event handlers

    /// Sizes the visible window to this window
    void OnSize(wxSizeEvent& event);

    /// Respond to left click on an image
    void OnLeftClick(wxThumbnailEvent& event);

/// Helpers

    /// Set the size
    void UpdateWindowSize();

protected:
    wxThumbnailCtrl*    m_thumbnailCtrl;
    RiffleImageViewer*  m_imageViewer;
    bool                m_showingThumbnails;
};

#endif
    // _BROWSER_H_
