#include "mixerchannel.h"
#include "mixer.h"
#include "mixereffect.h"

MixerChannel::MixerChannel(QObject *parent) :
    QObject(parent), _volume(100), _sink(0), _generator(0)
{ }

MixerChannel::~MixerChannel()
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

QString MixerChannel::GetName()
{
    return this->_name;
}

QColor MixerChannel::GetColor()
{
    return this->_color;
}

int MixerChannel::GetVolume()
{
    return this->_volume;
}

void MixerChannel::SetName(QString name)
{
    if (this->_name != name)
    {
        this->_name = name;
        emit NameChanged(this->_name);
    }
}

void MixerChannel::SetColor(QColor color)
{
    if (this->_color != color)
    {
        this->_color = color;
        emit ColorChanged(this->_color);
    }
}

void MixerChannel::SetVolume(int volume)
{
    if (this->_volume != volume)
    {
        this->_volume = volume;
        if (this->_volume < 0) this->_volume = 0;
        if (this->_volume > 128) this->_volume = 128;
        emit VolumeChanged(this->_volume);
    }
}

MixerBuffer& MixerChannel::AudioOut()
{
    // Make sure we do not do this multiple times per tick
    if (this->_currentTick < Mixer::CurrentTick)
    {
        this->_buffer.Reset();
        if (this->ChannelInput() != 0)
        {
            MixerInstrument* instrument = dynamic_cast<MixerInstrument*>(this->ChannelInput());
            if (instrument != 0)
                this->_buffer.CopyFrom(instrument->GetInstrument()->partoutl, instrument->GetInstrument()->partoutr);

            // Only when there is an instrument, and potentially sound, we do effects
            for (QList<MixerEffect*>::iterator itr = this->_effects.begin(); itr != this->_effects.end(); ++itr)
                (*itr)->EffectOnBuffer(this->_buffer);
        }

        // And finnally we progress the local tick
        this->_currentTick = Mixer::CurrentTick;
    }
    return this->_buffer;
}

void MixerChannel::SetSink(MixerMaster* sink)
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

void MixerChannel::SetChannelInput(MixerChannelInput* generator)
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

void MixerChannel::AddEffect(MixerEffect* effect)
{
    if (this->_effects.contains(effect) == false)
    {
        this->_effects.push_back(effect);
        if (effect != 0)
            effect->SetSource(this);
    }
}

void MixerChannel::RemoveEffect(MixerEffect* effect)
{
    if (this->_effects.contains(effect))
    {
        this->_effects.removeOne(effect);
        if (effect != 0)
            effect->SetSource(0);
    }
}
