#ifndef SEQUENCERTRACK_H
#define SEQUENCERTRACK_H

#include <QList>
#include "sequencerclip.h"
#include "../Misc/Channel.h"

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

    Channel* GetChannel() { return this->_channel; }
    void SetChannel(Channel* channel) { this->_channel = channel; }

    void SendNotes(long start, long end);

private:
    SequencerSong* _song;
    QList<SequencerClip*> _clips;
    QString _title;
    Channel* _channel;
};

#endif // SEQUENCERTRACK_H
