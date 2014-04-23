#include "mixersendsource.h"
#include "mixersendsink.h"

MixerSendSource::MixerSendSource()
{ }

void MixerSendSource::AddDestination(MixerSendSink* destination)
{
    if (this->_destinations.contains(destination) == false)
        this->_destinations.push_back(destination);
    destination->AddSource(this);
}

void MixerSendSource::RemoveDestination(MixerSendSink* destination)
{
    if (this->_destinations.contains(destination))
        this->_destinations.removeOne(destination);
    destination->RemoveSource(this);
}
