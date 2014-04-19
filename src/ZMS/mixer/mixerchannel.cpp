#include "mixerchannel.h"

MixerChannel::MixerChannel(QObject *parent) :
    QObject(parent)
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
