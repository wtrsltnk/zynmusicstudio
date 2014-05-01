#ifndef MIXEREFFECT_H
#define MIXEREFFECT_H

#include <QObject>
#include "mixerbuffer.h"

class MixerChannel;

class MixerEffect : public QObject
{
    Q_OBJECT
public:
    explicit MixerEffect(QObject *parent = 0);
    virtual ~MixerEffect();

    virtual MixerChannel* Source() { return this->_source; }

    // This will not change the input buffer
//    virtual const MixerBuffer& EffectFromBuffer(const MixerBuffer& in) = 0;

    // This will change the input buffer
    virtual void EffectOnBuffer(MixerBuffer& in) { }
signals:
    void SourceChanged(MixerChannel* source);

public slots:
    virtual void SetSource(MixerChannel* source);

protected:
    MixerChannel* _source;
    MixerBuffer _buffer;
};

#endif // MIXEREFFECT_H
