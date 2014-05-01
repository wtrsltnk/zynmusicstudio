#include "mixereffect.h"
#include "mixerchannel.h"

MixerEffect::MixerEffect(QObject *parent) :
    QObject(parent), _source(0)
{ }

MixerEffect::~MixerEffect()
{
    this->SetSource(0);
}

void MixerEffect::SetSource(MixerEffectContainer *source)
{
    if (this->_source != source)
    {
        if (this->_source != 0)
            this->_source->RemoveEffect(this);
        this->_source = source;
        if (this->_source != 0)
            this->_source->AddEffect(this);
        emit SourceChanged(source);
    }
}
