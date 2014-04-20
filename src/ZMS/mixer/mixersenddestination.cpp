#include "mixersenddestination.h"
#include "mixersendsource.h"

MixerSendDestination::MixerSendDestination()
    : _source(0)
{ }

void MixerSendDestination::SetSource(MixerSendSource* source)
{
    if (source == (MixerSendSource*)this)
        return;

    if (this->_source != 0) this->_source->SetDestination(0);
    this->_source = source;
    if (this->_source != 0 && this->_source->Destination() != this) this->_source->SetDestination(this);
}
