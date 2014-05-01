#include "mixereffectcontainer.h"
#include "mixereffect.h"

MixerEffectContainer::MixerEffectContainer(QObject *parent) :
    QObject(parent)
{ }

MixerEffectContainer::~MixerEffectContainer()
{
    while (this->_effects.empty() == false)
    {
        MixerEffect* effect = this->_effects.back();
        this->_effects.pop_back();
        effect->SetSource(0);
    }
}

void MixerEffectContainer::AddEffect(MixerEffect* effect)
{
    if (this->_effects.contains(effect) == false)
    {
        this->_effects.push_back(effect);
        if (effect != 0)
            effect->SetSource(this);
        emit EffectAdded(effect);
    }
}

void MixerEffectContainer::RemoveEffect(MixerEffect* effect)
{
    if (this->_effects.contains(effect))
    {
        this->_effects.removeOne(effect);
        if (effect != 0)
            effect->SetSource(0);
        emit EffectRemoved(effect);
    }
}
