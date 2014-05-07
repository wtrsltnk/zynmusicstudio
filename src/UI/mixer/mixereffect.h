#ifndef MIXEREFFECT_H
#define MIXEREFFECT_H

#include <QObject>
#include "mixerbuffer.h"

class MixerEffectContainer;

class MixerEffect : public QObject
{
    Q_OBJECT

    friend class Mixer;
    MixerEffect(const QString& name, QObject *parent = 0);
public:
    virtual ~MixerEffect();

    QString GetName();

    virtual MixerEffectContainer* Source() { return this->_source; }

    // This will not change the input buffer
//    virtual const MixerBuffer& EffectFromBuffer(const MixerBuffer& in) = 0;

    // This will change the input buffer
    virtual void EffectOnBuffer(MixerBuffer& in) { }
signals:
    void NameChanged(QString name);
    void SourceChanged(MixerEffectContainer* source);

public slots:
    void SetName(QString name);
    virtual void SetSource(MixerEffectContainer* source);

protected:
    QString _name;
    MixerEffectContainer* _source;
    MixerBuffer _buffer;

};

#endif // MIXEREFFECT_H
