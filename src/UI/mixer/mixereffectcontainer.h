#ifndef MIXEREFFECTCONTAINER_H
#define MIXEREFFECTCONTAINER_H

#include <QObject>
#include <QList>

class MixerEffect;

class MixerEffectContainer : public QObject
{
    Q_OBJECT
public:
    MixerEffectContainer(QObject *parent = 0);
    virtual ~MixerEffectContainer();

    QList<MixerEffect*>& Effects() { return this->_effects; }

signals:
    void EffectAdded(MixerEffect* effect);
    void EffectRemoved(MixerEffect* effect);

public slots:
    virtual void AddEffect(MixerEffect* effect);
    virtual void RemoveEffect(MixerEffect* effect);

private:
    QList<MixerEffect*> _effects;

};

#endif // MIXEREFFECTCONTAINER_H
