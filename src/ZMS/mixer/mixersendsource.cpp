#include "mixersendsource.h"
#include "mixersendsink.h"

MixerSendSource::MixerSendSource(QObject* parent)
    : QObject(parent)
{ }

MixerSendSource::~MixerSendSource()
{
    while (this->_sinks.empty() == false)
    {
        MixerSendSink* destination = this->_sinks.back();
        this->_sinks.pop_back();
        destination->RemoveSource(this);
    }
}

void MixerSendSource::AddDestination(MixerSendSink* sink)
{
    if (this->_sinks.contains(sink) == false)
    {
        this->_sinks.push_back(sink);
        if (sink!= 0)
            sink->AddSource(this);
        emit SinkAdded(sink);
    }
}

void MixerSendSource::RemoveDestination(MixerSendSink* sink)
{
    if (this->_sinks.contains(sink))
    {
        this->_sinks.removeOne(sink);
        if (sink != 0)
            sink->RemoveSource(this);
        emit SinkRemoved(sink);
    }
}
