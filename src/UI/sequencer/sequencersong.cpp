#include "sequencersong.h"
#include "sequencer.h"

SequencerSong::SequencerSong()
    : _currentTrack(0)
{ }

SequencerSong::~SequencerSong()
{
    while (this->_tracks.empty() == false)
    {
        SequencerTrack* track = this->_tracks.back();
        this->_tracks.pop_back();
        delete track;
    }
}

SequencerTrack* SequencerSong::AddTrack()
{
    this->_tracks.push_back(new SequencerTrack(this));
    this->_tracks.back()->SetTitle(QString("Track ") + QString::number(this->_tracks.length()));
    return this->_tracks.back();
}

SequencerTrack* SequencerSong::GetTrackAtIndex(int trackindex)
{
    if (trackindex >= 0 && trackindex < this->_tracks.size())
        return this->_tracks[trackindex];

    return 0;
}

void SequencerSong::RemoveTrack(SequencerTrack* track)
{
    this->_tracks.removeOne(track);
    if (this->_currentTrack == track)
        this->SetCurrentTrack(0);
}

void SequencerSong::SetCurrentTrack(SequencerTrack *track)
 {
    this->_currentTrack = track;
    Sequencer::Inst().SelectedTrackChanged(track);
 }

void SequencerSong::SetClipTrack(SequencerClip* clip, int trackindex)
{
    if (clip != 0 && trackindex >= 0 && trackindex < this->_tracks.size())
        clip->MoveClip(this->_tracks[trackindex]);
}
