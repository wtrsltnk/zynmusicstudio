#ifndef MIXERINSERTEFFECT_H
#define MIXERINSERTEFFECT_H

#include <QObject>

class MixerEffect;

class MixerInsertEffect : public QObject
{
    Q_OBJECT

public:
    MixerInsertEffect(MixerEffect* effect, QObject* parent = 0);
    virtual ~MixerInsertEffect();

    float GetValue() { return this->_value; }
    MixerEffect* GetEffect() { return this->_effect; }

signals:
    void ValueChanged(float value);

public slots:
    void SetValue(float value);

private:
    MixerEffect* _effect;
    float _value;
};

#endif // MIXERINSERTEFFECT_H
