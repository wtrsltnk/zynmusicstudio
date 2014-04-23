#include "mixersource.h"
#include "mixersink.h"

MixerSource::MixerSource()
    : _sink(0)
{ }

MixerSource::~MixerSource()
{
    this->SetSink(0);
}

void MixerSource::SetSink(MixerSink* sink)
{
    if (this->_sink != sink)
    {
        if (this->_sink != 0)
            this->_sink->RemoveSource(this);

        this->_sink = sink;
        if (this->_sink != 0)
            this->_sink->AddSource(this);
    }
}
