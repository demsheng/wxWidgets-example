/////////////////////////////////////////////////////////////////////////////
// Name:        doc.cpp
// Purpose:     Implements document functionality for the Doodle sample
// Author:      Julian Smart
// Modified by:
// Created:     2004-11-07
// Copyright:   (c) Julian Smart
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "wx/txtstrm.h"
#include "wx/filename.h"

#include "doc.h"
#include "view.h"

/*
 * DoodleDocument
 */

IMPLEMENT_DYNAMIC_CLASS(DoodleDocument, wxDocument)

DoodleDocument::~DoodleDocument()
{
    WX_CLEAR_LIST(wxList, m_doodleSegments);
}

wxOutputStream& DoodleDocument::SaveObject(wxOutputStream& stream)
{
    wxDocument::SaveObject(stream);

    wxTextOutputStream textStream( stream );

    wxInt32 n = m_doodleSegments.GetCount();
    textStream << n << _T('\n');

    wxList::compatibility_iterator node = m_doodleSegments.GetFirst();
    while (node)
    {
        DoodleSegment *segment = (DoodleSegment *)node->GetData();
        segment->SaveObject(stream);
        textStream << _T('\n');

        node = node->GetNext();
    }

    return stream;
}

wxInputStream& DoodleDocument::LoadObject(wxInputStream& stream)
{
    wxDocument::LoadObject(stream);

    wxTextInputStream textStream( stream );

    wxInt32 n = 0;
    textStream >> n;

    for (int i = 0; i < n; i++)
    {
        DoodleSegment *segment = new DoodleSegment;
        segment->LoadObject(stream);
        m_doodleSegments.Append(segment);
    }

    return stream;
}

/*
 * DoodleSegment
 */

DoodleSegment::DoodleSegment(const DoodleSegment& seg)
{
    wxList::compatibility_iterator node = seg.GetLines().GetFirst();
    while (node)
    {
        DoodleLine *line = (DoodleLine *)node->GetData();
        DoodleLine *newLine = new DoodleLine(line->m_x1, line->m_y1, line->m_x2, line->m_y2);

        GetLines().Append(newLine);

        node = node->GetNext();
    }
}

DoodleSegment::~DoodleSegment()
{
    WX_CLEAR_LIST(wxList, m_lines);
}

wxOutputStream &DoodleSegment::SaveObject(wxOutputStream& stream)
{
    wxTextOutputStream textStream( stream );

    wxInt32 n = GetLines().GetCount();
    textStream << n << _T('\n');

    wxList::compatibility_iterator node = GetLines().GetFirst();
    while (node)
    {
        DoodleLine *line = (DoodleLine *)node->GetData();
        textStream
            << line->m_x1 << _T(" ")
            << line->m_y1 << _T(" ")
            << line->m_x2 << _T(" ")
            << line->m_y2 << _T("\n");
        node = node->GetNext();
    }

    return stream;
}

wxInputStream &DoodleSegment::LoadObject(wxInputStream& stream)
{
    wxTextInputStream textStream( stream );

    wxInt32 n = 0;
    textStream >> n;

    for (int i = 0; i < n; i++)
    {
        DoodleLine *line = new DoodleLine;
        textStream
            >> line->m_x1
            >> line->m_y1
            >> line->m_x2
            >> line->m_y2;
        GetLines().Append(line);
    }

    return stream;
}

void DoodleSegment::Draw(wxDC *dc)
{
    wxList::compatibility_iterator node = GetLines().GetFirst();
    while (node)
    {
        DoodleLine *line = (DoodleLine *)node->GetData();
        dc->DrawLine(line->m_x1, line->m_y1, line->m_x2, line->m_y2);
        node = node->GetNext();
    }
}

/*
 * DoodleCommand
 */

DoodleCommand::DoodleCommand(const wxString& name, int command,
                             DoodleDocument *doc, DoodleSegment *seg):
    wxCommand(true, name)
{
    m_doc = doc;
    m_segment = seg;
    m_cmd = command;
}

DoodleCommand::~DoodleCommand()
{
    if (m_segment)
        delete m_segment;
}

bool DoodleCommand::Do()
{
    return DoOrUndo(m_cmd);
}

bool DoodleCommand::Undo()
{
    switch (m_cmd)
    {
    case DOODLE_ADD:
        {
            return DoOrUndo(DOODLE_CUT);
        }
    case DOODLE_CUT:
        {
            return DoOrUndo(DOODLE_ADD);
        }
    }
    return true;
}

bool DoodleCommand::DoOrUndo(int cmd)
{
    switch (cmd)
    {
    case DOODLE_ADD:
        {
            wxASSERT( m_segment != NULL );
            
            if (m_segment)
                m_doc->GetDoodleSegments().Append(m_segment);
            
            m_segment = NULL;
            
            m_doc->Modify(true);
            m_doc->UpdateAllViews();
            break;
        }
    case DOODLE_CUT:
        {
            wxASSERT( m_segment == NULL );
            
            // Cut the last segment
            if (m_doc->GetDoodleSegments().GetCount() > 0)
            {
                wxList::compatibility_iterator node = m_doc->GetDoodleSegments().GetLast();

                m_segment = (DoodleSegment *)node->GetData();
                m_doc->GetDoodleSegments().Erase(node);

                m_doc->Modify(true);
                m_doc->UpdateAllViews();
            }
            break;
        }
    }
    return true;
}

