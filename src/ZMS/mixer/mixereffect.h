#ifndef MIXEREFFECT_H
#define MIXEREFFECT_H

#include <QObject>
#include "mixerbuffer.h"

class MixerSource;

class MixerEffect : public QObject
{
    Q_OBJECT
public:
    explicit MixerEffect(QObject *parent = 0);
    virtual ~MixerEffect();

    virtual MixerSource* Source() { return this->_source; }

    // This will not change the input buffer
    virtual const MixerBuffer& EffectFromBuffer(const MixerBuffer& in) = 0;

    // This will change the input buffer
    virtual void EffectOnBuffer(MixerBuffer& in) = 0;
signals:
    void SourceChanged(MixerSource* source);

public slots:
    virtual void SetSource(MixerSource* source);

protected:
    MixerSource* _source;
    MixerBuffer _buffer;
};

#endif // MIXEREFFECT_H
