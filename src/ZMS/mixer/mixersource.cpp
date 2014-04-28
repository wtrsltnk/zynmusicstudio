#include "mixersource.h"
#include "mixersink.h"
#include "mixerchannelinput.h"
#include "mixereffect.h"

MixerSource::MixerSource(QObject* parent)
    : QObject(parent), _sink(0), _generator(0)
{ }

MixerSource::~MixerSource()
{
    this->SetSink(0);
    this->SetChannelInput(0);
    while (this->_effects.empty() == false)
    {
        MixerEffect* effect = this->_effects.back();
        this->_effects.pop_back();
        effect->SetSource(0);
    }
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

        emit SinkChanged(this->_sink);
    }
}

void MixerSource::SetChannelInput(MixerChannelInput* generator)
{
    if (this->_generator != generator)
    {
        if (this->_generator != 0)
            this->_generator->SetSource(0);

        this->_generator = generator;
        if (this->_generator != 0)
            this->_generator->SetSource(this);

        emit ChannelInputChanged(this->_generator);
    }
}

void MixerSource::AddEffect(MixerEffect* effect)
{
    if (this->_effects.contains(effect) == false)
    {
        this->_effects.push_back(effect);
        if (effect != 0)
            effect->SetSource(this);
    }
}

void MixerSource::RemoveEffect(MixerEffect* effect)
{
    if (this->_effects.contains(effect))
    {
        this->_effects.removeOne(effect);
        if (effect != 0)
            effect->SetSource(0);
    }
}
