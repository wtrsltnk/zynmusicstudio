#include "mixersendsink.h"
#include "mixersendsource.h"

MixerSendSink::MixerSendSink(QObject* parent)
    : QObject(parent)
{ }

MixerSendSink::~MixerSendSink()
{
    while (this->_sources.empty() == false)
    {
        MixerSendSource* source = this->_sources.back();
        this->_sources.pop_back();
        source->RemoveDestination(this);
    }
}

void MixerSendSink::AddSource(MixerSendSource* source)
{
    if (this->_sources.contains(source) == false)
    {
        this->_sources.push_back(source);
        source->AddDestination(this);
        emit SourceAdded(source);
    }
}

void MixerSendSink::RemoveSource(MixerSendSource* source)
{
    if (this->_sources.contains(source))
    {
        this->_sources.removeOne(source);
        source->RemoveDestination(this);
        emit SourceRemoved(source);
    }
}

