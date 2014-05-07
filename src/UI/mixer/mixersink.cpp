#include "mixersink.h"
#include "mixersource.h"

MixerSink::MixerSink(QObject *parent)
    : QObject(parent)
{ }

MixerSink::~MixerSink()
{
    while (this->_sources.empty() == false)
    {
        MixerSource* source = this->_sources.back();
        this->_sources.pop_back();
        source->SetSink(0);
    }
}

void MixerSink::AddSource(MixerSource* source)
{
    if (this->_sources.contains(source) == false)
    {
        this->_sources.push_back(source);
        source->SetSink(this);
    }
}

void MixerSink::RemoveSource(MixerSource* source)
{
    if (this->_sources.contains(source))
    {
        this->_sources.removeOne(source);
        source->SetSink(0);
    }
}
