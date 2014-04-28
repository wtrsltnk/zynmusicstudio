#include "mixerbus.h"
#include "mixer.h"

MixerBus::MixerBus(QObject *parent) :
    MixerChannelInput(parent)
{ }

MixerBus::~MixerBus()
{ }

void MixerBus::AudioOut(float *outl, float *outr)
{
    if (this->_currentTick < Mixer::CurrentTick)
    {

        // And finnally we progress the local tick
        this->_currentTick = Mixer::CurrentTick;
    }
}
