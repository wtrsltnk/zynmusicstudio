#ifndef MIXEREFFECT_H
#define MIXEREFFECT_H

#include <QObject>
#include "mixerbuffer.h"

class MixerEffectContainer;
class Effect;

namespace EffectTypes
{
    enum eTypes
    {
        None = 0,
        Alien,
        Chorus,
        Distortion,
        Dynamic,
        Echo,
        EQ,
        Phaser,
        Reverb,
        _Count
    };
}

class MixerEffect : public QObject
{
    Q_OBJECT

public:
    MixerEffect(const QString& name, QObject *parent = 0);
    virtual ~MixerEffect();

    QString GetName();

    virtual MixerEffectContainer* Source() { return this->_source; }

    // This will change the input buffer
    virtual void EffectOnBuffer(MixerBuffer& in);
signals:
    void NameChanged(QString name);
    void SourceChanged(MixerEffectContainer* source);

public slots:
    void SetName(QString name);
    virtual void SetSource(MixerEffectContainer* source);
    void SetEffect(int index);

protected:
    QString _name;
    MixerEffectContainer* _source;
    Effect* _effect;

};

#endif // MIXEREFFECT_H
