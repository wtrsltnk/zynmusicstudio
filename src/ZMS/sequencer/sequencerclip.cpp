#include "sequencerclip.h"
#include "sequencersong.h"

SequencerClip::SequencerClip(SequencerTrack* track)
    : _track(track), _start(2), _color(QColor::fromRgb(20, 120, 220))
{ }

SequencerNote* SequencerClip::CloneNote(SequencerNote* item)
{
    this->_notes.push_back(new SequencerNote(item));
    return this->_notes.back();
}

void SequencerClip::RemoveNote(SequencerNote* note)
{
    this->_notes.removeOne(note);
}

double SequencerClip::Length()
{
    double len = 1;

    for (QList<SequencerNote*>::iterator i = this->_notes.begin(); i != this->_notes.end(); ++i)
        if (len < (*i)->Start() + (*i)->Length())
            len = (*i)->Start() + (*i)->Length();

    return len;
}

void SequencerClip::MoveClip(SequencerTrack* track)
{
    this->_track->Clips().removeOne(this);
    this->_track = track;
    this->_track->Clips().push_back(this);
}

void SequencerClip::SendNotes(long start, long end)
{

}
