#ifndef MIXEREFFECT_H
#define MIXEREFFECT_H

#include <QObject>
#include "mixerbuffer.h"

class MixerEffectContainer;
class Effect;

class MixerEffect : public QObject
{
    Q_OBJECT

public:
    MixerEffect(const QString& name, QObject *parent = 0);
    virtual ~MixerEffect();

    QString GetName();

    virtual MixerEffectContainer* Source() { return this->_source; }

    // This will change the input buffer
    virtual void EffectOnBuffer(MixerBuffer& in) = 0;
signals:
    void NameChanged(QString name);
    void SourceChanged(MixerEffectContainer* source);

public slots:
    void SetName(QString name);
    virtual void SetSource(MixerEffectContainer* source);

protected:
    QString _name;
    MixerEffectContainer* _source;

};

#endif // MIXEREFFECT_H
