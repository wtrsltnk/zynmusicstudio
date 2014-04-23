#include "mixerchannel.h"
#include "mixer.h"

MixerChannel::MixerChannel(QObject *parent) :
    QObject(parent), MixerSendSource(), MixerSendSink(), _instrument(0), _volume(100)
{ }

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

char MixerChannel::GetVolume()
{
    return this->_volume;
}

void MixerChannel::SetInstrument(Instrument* instrument)
{
    this->_instrument = instrument;
    emit InstrumentChanged(this->_instrument);
}

void MixerChannel::SetName(QString name)
{
    this->_name = name;
    emit NameChanged(this->_name);
}

void MixerChannel::SetColor(QColor color)
{
    this->_color = color;
    emit ColorChanged(this->_color);
}

void MixerChannel::SetVolume(char volume)
{
    this->_volume = volume;
    emit VolumeChanged(this->_volume);
}

MixerBuffer& MixerChannel::AudioOut()
{
    // Make sure we do not do this multiple times per tick
    if (this->_currentTick < Mixer::CurrentTick)
    {
        this->_buffer.Reset();
        if (this->_instrument != 0)
            this->_buffer.CopyFrom(this->_instrument->partoutl, this->_instrument->partoutr);

        // And finnally we progress the local tick
        this->_currentTick = Mixer::CurrentTick;
    }
    return this->_buffer;
}
