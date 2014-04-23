#include "mixersendsink.h"
#include "mixersendsource.h"

MixerSendSink::MixerSendSink()
{ }

void MixerSendSink::AddSource(MixerSendSource* source)
{
    if (this->_sources.contains(source) == false)
        this->_sources.push_back(source);
    source->AddDestination(this);
}

void MixerSendSink::RemoveSource(MixerSendSource* source)
{
    if (this->_sources.contains(source))
        this->_sources.removeOne(source);
    source->RemoveDestination(this);
}

