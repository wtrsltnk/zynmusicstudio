#ifndef SEQUENCERTRACK_H
#define SEQUENCERTRACK_H

#include <QList>
#include "sequencerclip.h"
#include "../../Misc/Instrument.h"

class SequencerSong;

class SequencerTrack
{
public:
    SequencerTrack(SequencerSong* song);
    virtual ~SequencerTrack();

    SequencerSong* GetSong() { return this->_song; }
    SequencerClip* AddClip();
    QList<SequencerClip*>& Clips() { return this->_clips; }
    SequencerClip* CloneClip(SequencerClip* clip);
    void RemoveClip(SequencerClip* clip);

    const QString& Title() const { return this->_title; }
    void SetTitle(const QString& title) { this->_title = title; }

    Instrument* GetChannel() { return this->_channel; }
    void SetChannel(Instrument* channel) { this->_channel = channel; }

    void SendNotes(long start, long end);

private:
    SequencerSong* _song;
    QList<SequencerClip*> _clips;
    QString _title;
    Instrument* _channel;
};

#endif // SEQUENCERTRACK_H
