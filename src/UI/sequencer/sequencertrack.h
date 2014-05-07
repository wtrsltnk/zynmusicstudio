#ifndef SEQUENCERTRACK_H
#define SEQUENCERTRACK_H

#include <QObject>
#include <QList>
#include "sequencerclip.h"
#include "../mixer/mixerchannel.h"

class SequencerSong;

class SequencerTrack : public QObject
{
    Q_OBJECT
public:
    SequencerTrack(SequencerSong* song);
    virtual ~SequencerTrack();

    SequencerSong* GetSong() { return this->_song; }

    SequencerClip* AddClip();
    QList<SequencerClip*>& Clips() { return this->_clips; }
    SequencerClip* CloneClip(SequencerClip* clip);
    void RemoveClip(SequencerClip* clip);

    const QString& Title() const { return this->_title; }
    MixerChannel* GetChannel() { return this->_channel; }

    void SendNotes(long start, long end);

signals:
    void ChannelChanged(MixerChannel* channel);
    void TitleChanged(const QString& title);

public slots:
    void SetTitle(const QString& title);
    void SetChannel(MixerChannel* channel);

private:
    SequencerSong* _song;
    QList<SequencerClip*> _clips;
    QString _title;
    MixerChannel* _channel;
};

#endif // SEQUENCERTRACK_H
