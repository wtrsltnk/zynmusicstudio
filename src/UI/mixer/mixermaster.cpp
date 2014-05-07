#include "mixermaster.h"
#include "mixerchannel.h"

using namespace std;

MixerMaster::MixerMaster(QObject *parent) :
    QObject(parent)
{
    this->SetVolume(100);
}

MixerMaster::~MixerMaster()
{
    while (this->_sources.empty() == false)
    {
        MixerChannel* source = this->_sources.back();
        this->_sources.pop_back();
        source->SetSink(0);
    }
}

void MixerMaster::AudioOut(float *outl, float *outr)
{
    this->_buffer.Reset();

    for (QList<MixerChannel*>::iterator itr = this->_sources.begin(); itr != this->_sources.end(); ++itr)
        this->_buffer += (*itr)->AudioOut();

    for (int i = 0; i < synth->buffersize; i++)
    {
        outl[i] = this->_buffer.Left()[i] * this->_volumeScale;
        outr[i] = this->_buffer.Right()[i] * this->_volumeScale;
    }
}

int MixerMaster::GetVolume()
{
    return this->_volume;
}

void MixerMaster::SetVolume(int volume)
{
    if (this->_volume != volume)
    {
        this->_volume = volume;
        if (this->_volume < 0) this->_volume = 0;
        if (this->_volume > 128) this->_volume = 128;
        this->_volumeScale = float(this->_volume) / 128.0f;
        emit VolumeChanged(this->_volume);
    }
}

void MixerMaster::AddSource(MixerChannel* source)
{
    if (this->_sources.contains(source) == false)
    {
        this->_sources.push_back(source);
        source->SetSink(this);
    }
}

void MixerMaster::RemoveSource(MixerChannel* source)
{
    if (this->_sources.contains(source))
    {
        this->_sources.removeOne(source);
        source->SetSink(0);
    }
}
