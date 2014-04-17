#include "trackareaselection.h"

TrackAreaSelection::TrackAreaSelection()
{ }

TrackAreaSelection::~TrackAreaSelection()
{ }

void TrackAreaSelection::Reset()
{
    for (QList<TrackAreaClip*>::iterator i = this->_clips.begin(); i != this->_clips.end(); ++i)
        (*i)->Deselect();

    this->_clips.clear();
    this->SelectionChanged(this->_clips);
}

void TrackAreaSelection::StartSelection(TrackAreaClip* item)
{
    if (item != 0)
    {
        for (QList<TrackAreaClip*>::iterator i = this->_clips.begin(); i != this->_clips.end(); ++i)
            (*i)->Deselect();

        this->_clips.clear();

        item->Select();
        this->_clips.push_back(item);
        this->SelectionChanged(this->_clips);
    }
}

void TrackAreaSelection::AddToSelection(TrackAreaClip* item)
{
    if (item != 0 && this->_clips.indexOf(item) == -1)
    {
        item->Select();
        this->_clips.push_back(item);
        this->SelectionChanged(this->_clips);
    }
}

bool TrackAreaSelection::IsSelected(TrackAreaClip* item)
{
    return (this->_clips.indexOf(item) != -1);
}

void TrackAreaSelection::ToggleSelection(TrackAreaClip* item)
{
    if (this->IsSelected(item) == false)
        this->AddToSelection(item);
    else
        this->RemoveFromSelection(item);
}

void TrackAreaSelection::RemoveFromSelection(TrackAreaClip* item)
{
    if (this->_clips.removeOne(item))
    {
        item->Deselect();
        this->SelectionChanged(this->_clips);
    }
}

const QList<TrackAreaClip*>& TrackAreaSelection::Clips() const
{
    return this->_clips;
}
