#ifndef SEQUENCER_H
#define SEQUENCER_H

#include <QObject>
#include "sequencersong.h"
#include "../timesignature.h"

class Sequencer : public QObject
{
    Q_OBJECT

    Sequencer();
public:
    static Sequencer& Inst();
    virtual ~Sequencer();

public:
    typedef enum ePlaybackStatus
    {
        Stopped,
        Paused,
        Playing
    } PlaybackStatus;

    TimeSignature& LocalTimeSignature() { return this->_timeSignature; }
    void NewSong();
    SequencerSong* CurrentSong() { return this->_song; }

    void AddFrames(int frameCount);
    double FramesToBeats(long frames);
    long BeatsToFrames(double beats);

    PlaybackStatus Status() { return this->_status; }
    long Cursor() { return this->_frames; }
    long StartPlayAt() { return this->_startFrames; }
    long StopPlayAt() { return this->_maxFrames; }
    void SetPlayRange(long start, long end);

    void Start();
    void Pause();
    void Reset();
    void Stop();

public slots:
    void UpdateSequencer();

signals:
    void CurrentSongIsUpdated();
    void SelectedTrackChanged(SequencerTrack* track);
    void ChannelIsUpdated(Channel* channel);

private:
    TimeSignature _timeSignature;
    SequencerSong* _song;
    long _frames;
    long _startFrames;
    long _maxFrames;
    PlaybackStatus _status;

};

#endif // SEQUENCER_H
