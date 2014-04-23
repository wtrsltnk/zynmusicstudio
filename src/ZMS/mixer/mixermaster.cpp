#include "mixermaster.h"
#include "mixersource.h"

using namespace std;

MixerMaster::MixerMaster(QObject *parent) :
    QObject(parent), _volume(100)
{ }

void MixerMaster::AudioOut(float *outl, float *outr)
{
    this->_buffer.Reset();

    for (QList<MixerSource*>::iterator itr = this->_sources.begin(); itr != this->_sources.end(); ++itr)
        this->_buffer += (*itr)->AudioOut();

    this->_buffer.CopyTo(outl, outr);
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
        emit VolumeChanged(this->_volume);
    }
}
