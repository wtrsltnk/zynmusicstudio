#include "mixerchannel.h"
#include "mixer.h"
#include "mixereffect.h"

MixerChannel::MixerChannel(QObject *parent) :
    MixerSource(parent), MixerSendSource(), MixerSendSink(), _instrument(0), _volume(100)
{ }

MixerChannel::~MixerChannel()
{
    this->SetInstrument(0);
}

Instrument* MixerChannel::GetInstrument()
{
    return this->_instrument;
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

void MixerChannel::SetInstrument(Instrument* instrument)
{
    if (this->_instrument != instrument)
    {
        this->_instrument = instrument;
        emit InstrumentChanged(this->_instrument);
    }
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
        if (this->_instrument != 0)
        {
            this->_buffer.CopyFrom(this->_instrument->partoutl, this->_instrument->partoutr);

            // Only when there is an instrument, and potentially sound, we do effects
            for (QList<MixerEffect*>::iterator itr = this->_effects.begin(); itr != this->_effects.end(); ++itr)
                (*itr)->EffectOnBuffer(this->_buffer);
        }

        // And finnally we progress the local tick
        this->_currentTick = Mixer::CurrentTick;
    }
    return this->_buffer;
}
