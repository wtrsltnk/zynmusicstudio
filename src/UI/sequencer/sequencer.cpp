#include "sequencer.h"
#include "globals.h"
#include "../Misc/Master.h"

Sequencer::Sequencer()
    : _frames(0), _startFrames(0), _maxFrames(0)
{ }

Sequencer::~Sequencer()
{ }

Sequencer& Sequencer::Inst()
{
    static Sequencer instance;

    return instance;
}

void Sequencer::NewSong()
{
    if (this->_song != 0)
        delete this->_song;

    this->_song = new SequencerSong();
}

void Sequencer::Start()
{
    this->_status = Sequencer::Playing;
}

void Sequencer::Pause()
{
    this->_status = Sequencer::Paused;
}

void Sequencer::Reset()
{
    this->_frames = this->_startFrames;
    Master::getInstance().ShutUp();
}

void Sequencer::Stop()
{
    this->_status = Sequencer::Stopped;
    this->Reset();
}

void Sequencer::SetPlayRange(long start, long end)
{
    this->_startFrames = start;
    this->_maxFrames = end;
}

void Sequencer::AddFrames(int frameCount)
{
    if (this->_status == Sequencer::Playing)
    {
        long start = this->_frames;
        this->_frames += frameCount;

        // Special case where we go past the _endframes
        if (this->_frames > this->_maxFrames)
        {
            // Recalculate the minimum from startframe
            start = this->_startFrames;
            this->_frames = this->_startFrames + (this->_frames - this->_maxFrames);

            // and shutup everything from before.
            Master::getInstance().ShutUp();
        }

        // Send alle notes from all clips between our calculated min/max frame
        for (QList<SequencerTrack*>::iterator i = this->_song->Tracks().begin(); i != this->_song->Tracks().end(); ++i)
            (*i)->SendNotes(start, this->_frames);
    }
}

void Sequencer::UpdateSequencer()
{
    this->_maxFrames = 0;
    for (QList<SequencerTrack*>::iterator i = this->_song->Tracks().begin(); i != this->_song->Tracks().end(); ++i)
    {
        for (QList<SequencerClip*>::iterator j = (*i)->Clips().begin(); j != (*i)->Clips().end(); ++j)
        {
            int tmp = this->BeatsToFrames((*j)->Start() + (*j)->Length());
            if (tmp > this->_maxFrames)
                this->_maxFrames = tmp;
        }
    }
}

double Sequencer::FramesToBeats(long frames)
{
    return double(frames) * ((double(this->_timeSignature.beatsPerMinute()) / 60.0) / double(synth->samplerate_f));
}

long Sequencer::BeatsToFrames(double beats)
{
    return beats / ((double(this->_timeSignature.beatsPerMinute()) / 60.0) / double(synth->samplerate_f));
}
