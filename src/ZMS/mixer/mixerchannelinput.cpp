#include "mixerchannelinput.h"
#include "mixerchannel.h"

MixerChannelInput::MixerChannelInput(QObject* parent)
    : QObject(parent), _source(0)
{ }

MixerChannelInput::~MixerChannelInput()
{
    this->SetSource(0);
}

void MixerChannelInput::SetSource(MixerChannel* source)
{
    if (this->_source != source)
    {
        if (this->_source != 0)
            this->_source->SetChannelInput(0);

        this->_source = source;
        if (this->_source != 0)
            this->_source->SetChannelInput(this);

        emit SourceChanged(this->_source);
    }
}
