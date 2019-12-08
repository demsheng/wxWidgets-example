/////////////////////////////////////////////////////////////////////////////
// Name:        doc.h
// Purpose:     Document classes for Doodle sample
// Author:      Julian Smart
// Modified by:
// Created:     2004-12-07
// Copyright:   (c) Julian Smart
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef __DOODLE_DOCH__
#define __DOODLE_DOCH__

#include "wx/docview.h"
#include "wx/cmdproc.h"

/*
 * Defines a line from one point to the other
 */

class DoodleLine: public wxObject
{
public:
    DoodleLine(wxInt32 x1 = 0, wxInt32 y1 = 0, wxInt32 x2 = 0, wxInt32 y2 = 0)
    { m_x1 = x1; m_y1 = y1; m_x2 = x2; m_y2 = y2; }

    wxInt32 m_x1;
    wxInt32 m_y1;
    wxInt32 m_x2;
    wxInt32 m_y2;
};

/*
 * Contains a list of lines: represents a mouse-down doodle
 */

class DoodleSegment: public wxObject
{
public:
    DoodleSegment(){};
    DoodleSegment(const DoodleSegment& seg);
    ~DoodleSegment();

    void Draw(wxDC *dc);

    /// Saves the segment
    wxOutputStream& SaveObject(wxOutputStream& stream);

    /// Loads the segment
    wxInputStream& LoadObject(wxInputStream& stream);

    /// Gets the lines
    wxList& GetLines() { return m_lines; }
    const wxList& GetLines() const { return m_lines; }

private:
    wxList m_lines;
};

/*
 * Represents a doodle document
 */

class DoodleDocument: public wxDocument
{
    DECLARE_DYNAMIC_CLASS(DoodleDocument)
public:
    DoodleDocument() {};
    ~DoodleDocument();

    /// Saves the document
    wxOutputStream& SaveObject(wxOutputStream& stream);

    /// Loads the document
    wxInputStream& LoadObject(wxInputStream& stream);

    inline wxList& GetDoodleSegments() { return m_doodleSegments; };

private:
    wxList m_doodleSegments;
};

/*
 * A doodle command
 */

class DoodleCommand: public wxCommand
{
public:
    DoodleCommand(const wxString& name, int cmd, DoodleDocument *doc, DoodleSegment *seg);
    ~DoodleCommand();

    /// Overrides
    virtual bool Do();
    virtual bool Undo();

    /// Combine do/undo code since the commands are symmetric
    bool DoOrUndo(int cmd);

protected:
    DoodleSegment*  m_segment;
    DoodleDocument* m_doc;
    int             m_cmd;
};

/*
 * Doodle command identifiers
 */

#define DOODLE_CUT          1
#define DOODLE_ADD          2

#endif
    // __DOODLE_DOCH__

