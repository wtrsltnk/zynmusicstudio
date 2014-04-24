#include "sequencertrack.h"
#include "sequencersong.h"

static int track_counter = 0;

SequencerTrack::SequencerTrack(SequencerSong* song)
    : QObject(0), _song(song), _title(QString("Track ") + QString::number(track_counter++)), _channel(0)
{ }

SequencerTrack::~SequencerTrack()
{
    while (this->_clips.empty() == false)
    {
        SequencerClip* clip = this->_clips.back();
        this->_clips.pop_back();
        delete clip;
    }
}

void SequencerTrack::SetTitle(const QString& title)
{
    if (this->_title != title)
    {
        this->_title = title;
        emit TitleChanged(this->_title);
    }
}

void SequencerTrack::SetChannel(MixerChannel* channel)
{
    if (this->_channel != channel)
    {
        this->_channel = channel;
        emit ChannelChanged(this->_channel);
    }
}

SequencerClip* SequencerTrack::AddClip()
{
    this->_clips.push_back(new SequencerClip(this));

    return this->_clips.back();
}

SequencerClip* SequencerTrack::CloneClip(SequencerClip* clip)
{
    this->_clips.push_back(new SequencerClip(this));

    this->_clips.back()->SetColor(clip->Color());
    this->_clips.back()->SetStart(clip->Start());

    for (QList<SequencerNote*>::iterator i = clip->Notes().begin(); i != clip->Notes().end(); ++i)
    {
        SequencerNote* note = *i;
        this->_clips.back()->Notes().push_back(new SequencerNote(this->_clips.back(), note->Note(), note->Start(), note->Length()));
    }

    return this->_clips.back();
}

void SequencerTrack::RemoveClip(SequencerClip* clip)
{
    this->_clips.removeOne(clip);
}

void SequencerTrack::SendNotes(long start, long end)
{
    for (QList<SequencerClip*>::iterator i = this->Clips().begin(); i != this->Clips().end(); ++i)
        (*i)->SendNotes(start, end);
}
