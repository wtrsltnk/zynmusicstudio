#include "mixerbus.h"
#include "mixer.h"

MixerBus::MixerBus(QObject *parent) :
    QObject(parent), MixerSendSink(), MixerSendSource()
{ }

void MixerBus::AudioOut(float *outl, float *outr)
{
    if (this->_currentTick < Mixer::CurrentTick)
    {

        // And finnally we progress the local tick
        this->_currentTick = Mixer::CurrentTick;
    }
}
