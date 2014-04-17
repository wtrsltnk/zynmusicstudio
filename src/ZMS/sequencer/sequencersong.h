#ifndef SONG_H
#define SONG_H

#include <QList>
#include "sequencertrack.h"

class SequencerSong
{
public:
    SequencerSong();
    virtual ~SequencerSong();

    SequencerTrack* AddTrack();
    QList<SequencerTrack*>& Tracks() { return this->_tracks; }
    SequencerTrack* GetTrackAtIndex(int index);
    void RemoveTrack(SequencerTrack* track);

    SequencerTrack* CurrentTrack() { return this->_currentTrack; }
    void SetCurrentTrack(SequencerTrack* track);

    void SetClipTrack(SequencerClip* clip, int trackindex);

private:
    QList<SequencerTrack*> _tracks;
    SequencerTrack* _currentTrack;
};

#endif // SONG_H
