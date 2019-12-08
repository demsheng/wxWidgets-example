/////////////////////////////////////////////////////////////////////////////
// Name:        browser.cpp
// Purpose:     Implements a combined browser/image viewer window
// Author:      Julian Smart
// Modified by: 
// Created:     03/26/05 12:36:16
// RCS-ID:      
// Copyright:   (c) Julian Smart
// Licence:     wxWindows License
/////////////////////////////////////////////////////////////////////////////

#if defined(__GNUG__) && !defined(__APPLE__)
#pragma implementation "browser.h"
#endif

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "wx/image.h"
#include "wx/effects.h"
#include "wx/dir.h"
#include "wx/clipbrd.h"

#include "browser.h"

/// Returns the image type, or -1, determined from the extension.
static int DetermineImageType(const wxString& filename);


/*!
 * RiffleImageViewer
 */

IMPLEMENT_CLASS( RiffleImageViewer, wxScrolledWindow )

BEGIN_EVENT_TABLE( RiffleImageViewer, wxScrolledWindow )
    EVT_SIZE( RiffleImageViewer::OnSize )
    EVT_PAINT( RiffleImageViewer::OnPaint )
END_EVENT_TABLE()

RiffleImageViewer::RiffleImageViewer( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style )
{
    Init();

    Create( parent, id, pos, size, style );
}

bool RiffleImageViewer::Create( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style )
{
    return wxScrolledWindow::Create( parent, id, pos, size, style|wxFULL_REPAINT_ON_RESIZE );
}

/// Member initialisation
void RiffleImageViewer::Init()
{
    m_scale = 0;
}

/// Load from disk
bool RiffleImageViewer::LoadImage(const wxString& filename)
{
    m_bitmap = wxNullBitmap;

    wxWindow* p = GetParent();
    wxFrame* frame = NULL;
    while (p)
    {
        wxFrame* f = wxDynamicCast(p, wxFrame);
        if (f)
        {
            frame = f;
            break;
        }
        p = p->GetParent();
    }

    wxLogNull logNull;

    if (m_image.LoadFile(filename))
    {
        UpdateScrollbarSizes();
        Refresh();

        if (frame)
        {
            wxString name = wxFileNameFromPath(filename);
            wxString msg = wxString::Format(wxT("%s %dx%d"), (const wxChar*) name, m_image.GetWidth(), m_image.GetHeight());
            frame->SetStatusText(msg, 1);
        }

        return true;
    }
    else
        return false;
}

/// Place image
void RiffleImageViewer::OnSize(wxSizeEvent& event)
{
    event.Skip();
}

/// Paint image
void RiffleImageViewer::OnPaint(wxPaintEvent& WXUNUSED(event))
{
    wxPaintDC dc(this);
    PrepareDC(dc);

    wxRect rect;
    GetPictureRect(rect);

    if (m_image.Ok())
    {
        int borderSize = 2;
        // The picture rect actually includes the border size,
        // so subtract it for the actual image size.
        wxRect bitmapRect = rect;
        bitmapRect.width = wxMax(0, bitmapRect.width - 2*borderSize);
        bitmapRect.height = wxMax(0, bitmapRect.height - 2*borderSize);
        bitmapRect.x += borderSize;
        bitmapRect.y += borderSize;

        if (!m_bitmap.Ok() ||
            (bitmapRect.GetWidth() != m_bitmap.GetWidth()) ||
            (bitmapRect.GetHeight() != m_bitmap.GetHeight()))
        {
            // Don't do the scaling unnecesarily
            if (bitmapRect.GetWidth() == m_image.GetWidth() &&
                bitmapRect.GetHeight() == m_image.GetHeight())
            {
                m_bitmap = wxBitmap(m_image);
            }
            else
            {
                m_bitmap = wxBitmap(m_image.Scale(bitmapRect.GetWidth(), bitmapRect.GetHeight()));
            }
        }
        
        if (m_bitmap.Ok())
        {
            wxRect borderRect = rect; 
            wxEffects effects;
            effects.DrawSunkenEdge(dc, borderRect);            
            
            dc.DrawBitmap(m_bitmap, bitmapRect.x, bitmapRect.y);
        }
    }
}

/// Get the size and position for the image
bool RiffleImageViewer::GetPictureRect(wxRect& rect)
{
    if (m_image.Ok())
    {
        wxSize sz = GetClientSize();

        int w = m_image.GetWidth();
        int h = m_image.GetHeight();
        int scaledWindowW;
        int scaledWindowH;
        int x, y;
        int borderSize = 2;
            
        if (m_scale == 0)
        {
            double scale = 0;
            double scaleX = 1.0;
            double scaleY = 1.0;
            if (w > sz.x)
                scaleX = ((double) sz.x - 2*borderSize)/(double) w;
            if (h > sz.y)
                scaleY = ((double) sz.y - 2*borderSize)/(double) h;
            scale = wxMin(scaleX, scaleY);

            scaledWindowW = int((m_image.GetWidth()) * scale);
            scaledWindowH = int((m_image.GetHeight()) * scale);
        }
        else
        {
            if (m_scale <= 0)
                m_scale = 100;
            if (m_scale > 500)
                m_scale = 100;

            double scale = ((double) m_scale) / 100.0 ;

            scaledWindowW = int((m_image.GetWidth() * scale));
            scaledWindowH = int((m_image.GetHeight() * scale));
        }
        scaledWindowW += 2*borderSize;
        scaledWindowH += 2*borderSize;

        x = wxMax(0, (sz.x - scaledWindowW)/2);
        y = wxMax(0, (sz.y - scaledWindowH)/2);

        rect.x = x;
        rect.y = y;
        rect.width = scaledWindowW;
        rect.height = scaledWindowH;
        
        return true;
    }
    return false;
}

/// Update the scrollbars according to current image and scale
void RiffleImageViewer::UpdateScrollbarSizes()
{
    if (m_image.Ok())
    {
        wxRect pictureRect; 
        if (m_scale == 0 || !GetPictureRect(pictureRect))
        {
            SetScrollbars(0, 0, 0, 0);
            return;
        }
        wxSize sz = GetClientSize();

        // int borderSize = 2;

        int horzScrollLength, vertScrollLength;
        int ppuX = 1, ppuY = 1;

        if (pictureRect.GetWidth() > sz.x)
        {
            horzScrollLength = (pictureRect.GetWidth() / ppuX) ;
            if (horzScrollLength * ppuX < pictureRect.GetWidth())
                horzScrollLength ++;
        }
        else
            horzScrollLength = pictureRect.GetWidth();

        if (pictureRect.GetHeight() > sz.y)
        {
            vertScrollLength = (pictureRect.GetHeight() / ppuY);
            if (vertScrollLength * ppuY < pictureRect.GetHeight())
                vertScrollLength ++;
        }
        else
            vertScrollLength = pictureRect.GetHeight();

        SetScrollbars(ppuX, ppuY, horzScrollLength, vertScrollLength);
    }
    else
        SetScrollbars(0, 0, 0, 0);
}

/// Zoom in/out
void RiffleImageViewer::Zoom(bool zoomIn, int inc)
{
    if (m_scale == 0)
        m_scale = 100;

    if (zoomIn)
    {
        if (m_scale < 1000)
            m_scale += inc;
    }
    else
    {
        if (m_scale >= inc)
            m_scale -= inc;
    }
    UpdateScrollbarSizes();
    Refresh();
}

/// Fit to window
void RiffleImageViewer::ZoomFitToWindow()
{
    m_scale = 0;
    UpdateScrollbarSizes();
    Refresh();
}

/// Actual size
void RiffleImageViewer::ZoomActualSize()
{
    m_scale = 100;
    UpdateScrollbarSizes();
    Refresh();
}

/// Copies the image to the clipboard
bool RiffleImageViewer::CopyToClipboard()
{
    if (!m_image.Ok())
        return false;

    wxBitmap bitmap = m_image;
    if (!bitmap.Ok())
        return false;

    wxBitmapDataObject dataObj(bitmap);

    if (wxTheClipboard->Open())
    {
        wxTheClipboard->SetData( new wxBitmapDataObject(bitmap) );
        wxTheClipboard->Close();

        return true;
    }
    else
        return false;
}


/*!
 * RiffleBrowserViewer
 */

IMPLEMENT_CLASS( RiffleBrowserViewer, wxWindow )

BEGIN_EVENT_TABLE( RiffleBrowserViewer, wxWindow )
    EVT_SIZE( RiffleBrowserViewer::OnSize )
    EVT_THUMBNAIL_LEFT_CLICK(wxID_ANY, RiffleBrowserViewer::OnLeftClick)
END_EVENT_TABLE()

RiffleBrowserViewer::RiffleBrowserViewer( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style )
{
    Init();

    Create( parent, id, pos, size, style );
}

bool RiffleBrowserViewer::Create( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style )
{
    wxWindow::Create( parent, id, pos, size, style );

    CreateControls();
    
    return true;
}

/// Member initialisation
void RiffleBrowserViewer::Init()
{
    m_thumbnailCtrl = NULL;
    m_imageViewer = NULL;
    m_showingThumbnails = true;
}


/// Control creation
void RiffleBrowserViewer::CreateControls()
{
    m_thumbnailCtrl = new wxThumbnailCtrl(this, ID_THUMBNAILCTRL);
    m_imageViewer = new RiffleImageViewer(this, ID_IMAGEVIEWER);
    m_imageViewer->Show(false);
}

/// Show the thumbnails, or image
void RiffleBrowserViewer::ShowThumbnails(bool showThumbnails)
{
    if (showThumbnails)
    {
        if (m_showingThumbnails)
            return;
        m_imageViewer->Show(false);
        m_thumbnailCtrl->Show(true);
        m_thumbnailCtrl->SetFocus();
    }
    else
    {
        if (!m_showingThumbnails)
            return;
        m_thumbnailCtrl->Show(false);
        m_imageViewer->Show(true);
        m_imageViewer->SetFocus();
    }

    m_showingThumbnails = showThumbnails;

    UpdateWindowSize();
}

/// Show an image
bool RiffleBrowserViewer::ShowImage(const wxString& imageFilename)
{
    bool success = m_imageViewer->LoadImage(imageFilename);
    ShowThumbnails(false);

    return success;
}

/// Show thumbnails for the given a folder
bool RiffleBrowserViewer::ShowFolder(const wxString& path)
{
    wxBusyCursor busy;
    m_thumbnailCtrl->Clear();
    
    m_thumbnailCtrl->Freeze();

    {
        wxLogNull logNull;
        wxDir dir;
        if (dir.Open(path))
        {
            wxString filename;
            
            bool cont = dir.GetFirst(&filename, wxT("*.*"), wxDIR_FILES);
            while ( cont )
            {
                wxString file = path + wxFILE_SEP_PATH + filename;
                if (wxFileExists(file) && DetermineImageType(file) != -1)
                {
                    m_thumbnailCtrl->Append(new wxImageThumbnailItem(file));
                }
                
                cont = dir.GetNext(&filename);
            }                
        }
    }

    m_thumbnailCtrl->Thaw();

    ShowThumbnails(true);

    return true;
}

/// Sizes the visible window to this window
void RiffleBrowserViewer::OnSize(wxSizeEvent& WXUNUSED(event))
{
    UpdateWindowSize();
}

/// Set the size
void RiffleBrowserViewer::UpdateWindowSize()
{
    wxSize sz = GetClientSize();
    if (m_showingThumbnails && m_thumbnailCtrl)
        m_thumbnailCtrl->SetSize(0, 0, sz.x, sz.y);
    if (!m_showingThumbnails && m_imageViewer)
        m_imageViewer->SetSize(0, 0, sz.x, sz.y);
}

/// Respond to left click on an image
void RiffleBrowserViewer::OnLeftClick(wxThumbnailEvent& event)
{
    int index = event.GetIndex();
    wxThumbnailItem* item = m_thumbnailCtrl->GetItem(index);
    if (item)
    {
        wxString filename(item->GetFilename());
        ShowImage(filename);
    }
}


// Returns the image type, or -1, determined from the extension.
static int DetermineImageType(const wxString& filename)
{
    wxString path, name, ext;

    wxSplitPath(filename, & path, & name, & ext);

    ext.MakeLower();
    if (ext == wxT("jpg") || ext == wxT("jpeg"))
        return wxBITMAP_TYPE_JPEG;
    else if (ext == wxT("gif"))
        return wxBITMAP_TYPE_GIF;
    else if (ext == wxT("bmp"))
        return wxBITMAP_TYPE_BMP;
    else if (ext == wxT("png"))
        return wxBITMAP_TYPE_PNG;
    else if (ext == wxT("pcx"))
        return wxBITMAP_TYPE_PCX;
    else if (ext == wxT("xpm"))
        return wxBITMAP_TYPE_XPM;
    else if (ext == wxT("tif") || ext == wxT("tiff"))
        return wxBITMAP_TYPE_TIF;
    else
        return -1;
}
