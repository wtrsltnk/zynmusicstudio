#include "mixersendsource.h"
#include "mixersenddestination.h"

MixerSendSource::MixerSendSource()
    : _destination(0)
{ }

void MixerSendSource::SetDestination(MixerSendDestination* destination)
{
    if (destination == (MixerSendDestination*)this)
        return;

    if (this->_destination != 0) this->_destination->SetSource(0);
    this->_destination = destination;
    if (this->_destination != 0 && this->_destination->Source() != this) this->_destination->SetSource(this);
}

