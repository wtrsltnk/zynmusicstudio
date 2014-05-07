#ifndef CLIP_H
#define CLIP_H

#include <QList>
#include <QColor>
#include "sequencernote.h"
#include "sequencerautomatedproperty.h"

class SequencerTrack;

class SequencerClip
{
public:
    SequencerClip(SequencerTrack* track);

    SequencerTrack* GetTrack() { return this->_track; }
    QList<SequencerNote*>& Notes() { return this->_notes; }
    QList<SequencerAutomatedProperty*>& AutomatedProperties() { return this->_automatedProperties; }
    QColor Color() { return this->_color; }
    void SetColor(const QColor& color) { this->_color = color; }
    SequencerNote* CloneNote(SequencerNote* item);
    void RemoveNote(SequencerNote* note);

    double Start() { return this->_start; }
    void SetStart(double start) { this->_start = start; }
    double Length();
    void MoveClip(SequencerTrack* track);

    void SendNotes(long start, long end);

private:
    SequencerTrack* _track;
    QList<SequencerNote*> _notes;
    QList<SequencerAutomatedProperty*> _automatedProperties;
    QColor _color;
    double _start;

};

#endif // CLIP_H
