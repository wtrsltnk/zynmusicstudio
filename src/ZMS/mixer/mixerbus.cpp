#include "mixerbus.h"
#include "mixer.h"

MixerBus::MixerBus(const QString& name, QObject *parent) :
    MixerChannelInput(parent), _name(name)
{ }

MixerBus::~MixerBus()
{ }

const QString MixerBus::GetName()
{
    return this->_name;
}

void MixerBus::SetName(const QString& name)
{
    if (this->_name != name)
    {
        this->_name = name;
        emit NameChanged(name);
    }
}

void MixerBus::AudioOut(float *outl, float *outr)
{
    if (this->_currentTick < Mixer::CurrentTick)
    {

        // And finnally we progress the local tick
        this->_currentTick = Mixer::CurrentTick;
    }
}
