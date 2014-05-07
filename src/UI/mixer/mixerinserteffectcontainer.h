#ifndef MIXERINSERTEFFECTCONTAINER_H
#define MIXERINSERTEFFECTCONTAINER_H

#include <QObject>
#include <QList>

class MixerEffect;
class MixerInsertEffect;

class MixerInsertEffectContainer : public QObject
{
    Q_OBJECT
public:
    MixerInsertEffectContainer(QObject *parent = 0);
    virtual ~MixerInsertEffectContainer();

    QList<MixerInsertEffect*>& InsertEffects() { return this->_effects; }
    bool ContainsEffect(MixerEffect* effect);

signals:
    void InsertEffectAdded(MixerInsertEffect* effect);
    void InsertEffectRemoved(MixerInsertEffect* effect);

public slots:
    virtual void AddInsertEffect(MixerInsertEffect* effect);
    virtual void RemoveInsertEffect(MixerInsertEffect* effect);

private:
    QList<MixerInsertEffect*> _effects;

};

#endif // MIXERINSERTEFFECTCONTAINER_H
