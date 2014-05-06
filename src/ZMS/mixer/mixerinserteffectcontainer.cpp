#include "mixerinserteffectcontainer.h"
#include "mixerinserteffect.h"

MixerInsertEffectContainer::MixerInsertEffectContainer(QObject *parent) :
    QObject(parent)
{ }

MixerInsertEffectContainer::~MixerInsertEffectContainer()
{
    while (this->_effects.empty() == false)
    {
        MixerInsertEffect* effect = this->_effects.back();
        this->_effects.pop_back();
    }
}

void MixerInsertEffectContainer::AddInsertEffect(MixerInsertEffect* effect)
{
    if (this->_effects.contains(effect) == false)
    {
        this->_effects.push_back(effect);
        emit InsertEffectAdded(effect);
    }
}

void MixerInsertEffectContainer::RemoveInsertEffect(MixerInsertEffect* effect)
{
    if (this->_effects.contains(effect))
    {
        this->_effects.removeOne(effect);
        emit InsertEffectRemoved(effect);
    }
}
