// XPMs with the art
#include "bitmaps/helpbook.xpm"
#include "bitmaps/helppage.xpm"
#include "bitmaps/helpback.xpm"
#include "bitmaps/helpdown.xpm"
#include "bitmaps/helpforward.xpm"
#include "bitmaps/helpoptions.xpm"
#include "bitmaps/helpsidepanel.xpm"
#include "bitmaps/helpup.xpm"
#include "bitmaps/helpuplevel.xpm"
#include "bitmaps/helpicon.xpm"

#include "wx/artprov.h"

// The art provider class
class MyArtProvider : public wxArtProvider
{
protected:
    virtual wxBitmap CreateBitmap(const wxArtID& id, const wxArtClient& client,
                                  const wxSize& size);
};

// CreateBitmap function
wxBitmap MyArtProvider::CreateBitmap(const wxArtID& id,
                                     const wxArtClient& client,
                                     const wxSize& size)
{
    if (id == wxART_HELP_SIDE_PANEL)
        return wxBitmap(helpsidepanel_xpm);
    if (id == wxART_HELP_SETTINGS)
        return wxBitmap(helpoptions_xpm);
    if (id == wxART_HELP_BOOK)
        return wxBitmap(helpbook_xpm);
    if (id == wxART_HELP_FOLDER)
        return wxBitmap(helpbook_xpm);
    if (id == wxART_HELP_PAGE)
        return wxBitmap(helppage_xpm);
    if (id == wxART_GO_BACK)
        return wxBitmap(helpback_xpm);
    if (id == wxART_GO_FORWARD)
        return wxBitmap(helpforward_xpm);
    if (id == wxART_GO_UP)
        return wxBitmap(helpup_xpm);
    if (id == wxART_GO_DOWN)
        return wxBitmap(helpdown_xpm);
    if (id == wxART_GO_TO_PARENT)
        return wxBitmap(helpuplevel_xpm);
    if (id == wxART_FRAME_ICON)
        return wxBitmap(helpicon_xpm);
    if (id == wxART_HELP)
        return wxBitmap(helpicon_xpm);

    // Any wxWidgets icons not implemented here
    // will be provided by the default art provider.
    return wxNullBitmap;
}

// Initialization
bool MyApp::OnInit()
{
    ...

    wxArtProvider::PushProvider(new MyArtProvider);

    ...
    return true;
}

