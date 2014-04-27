#include "mixermaster.h"
#include "mixersource.h"

using namespace std;

MixerMaster::MixerMaster(QObject *parent) :
    MixerSink(parent)
{
    this->SetVolume(100);
}

void MixerMaster::AudioOut(float *outl, float *outr)
{
    this->_buffer.Reset();

    for (QList<MixerSource*>::iterator itr = this->_sources.begin(); itr != this->_sources.end(); ++itr)
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
