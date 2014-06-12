#include "mixereffect.h"
#include "mixerchannel.h"
#include "../Effects/Effect.h"

MixerEffect::MixerEffect(const QString& name, QObject *parent) :
    QObject(parent), _source(0), _effect(0)
{
    this->SetName(name);
}

MixerEffect::~MixerEffect()
{
    this->SetSource(0);
}

QString MixerEffect::GetName()
{
    return this->_name;
}

void MixerEffect::SetName(QString name)
{
    if (this->_name != name)
    {
        this->_name = name;
        emit NameChanged(this->_name);
    }
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

void MixerEffect::SetEffect(int index)
{

}

void MixerEffect::EffectOnBuffer(MixerBuffer &in)
{
    if (this->_effect != 0)
    {
        this->_effect->out(in.Left(), in.Right());
    }
}
