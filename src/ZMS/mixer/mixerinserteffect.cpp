#include "mixerinserteffect.h"
#include "mixereffect.h"

MixerInsertEffect::MixerInsertEffect(MixerEffect* effect, QObject* parent)
    : QObject(parent), _effect(effect), _value(1.0f)
{ }

MixerInsertEffect::~MixerInsertEffect()
{ }

void MixerInsertEffect::SetValue(float value)
{
    if (this->_value != value)
    {
        this->_value = value;
        emit ValueChanged(value);
    }
}
