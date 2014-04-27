#include "mixersendsource.h"
#include "mixersendsink.h"

MixerSendSource::MixerSendSource()
{ }

MixerSendSource::~MixerSendSource()
{
    while (this->_destinations.empty() == false)
    {
        MixerSendSink* destination = this->_destinations.back();
        this->_destinations.pop_back();
        destination->RemoveSource(this);
    }
}

void MixerSendSource::AddDestination(MixerSendSink* destination)
{
    if (this->_destinations.contains(destination) == false)
    {
        this->_destinations.push_back(destination);
        if (destination != 0)
            destination->AddSource(this);
    }
}

void MixerSendSource::RemoveDestination(MixerSendSink* destination)
{
    if (this->_destinations.contains(destination))
    {
        this->_destinations.removeOne(destination);
        if (destination != 0)
            destination->RemoveSource(this);
    }
}
