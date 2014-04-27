#ifndef MIXERSOURCE_H
#define MIXERSOURCE_H

#include <QObject>
#include "mixerbuffer.h"

class MixerSink;
class MixerGenerator;
class MixerEffect;

class MixerSource : public QObject
{
    Q_OBJECT
public:
    MixerSource(QObject* parent = 0);
    virtual ~MixerSource();

    virtual MixerBuffer& AudioOut() = 0;

    virtual MixerSink* Sink() { return this->_sink; }
    virtual MixerGenerator* Generator() { return this->_generator; }
    virtual QList<MixerEffect*>& Effects() { return this->_effects; }

signals:
    void SinkChanged(MixerSink* sink);
    void GeneratorChanged(MixerGenerator* generator);

public slots:
    virtual void SetSink(MixerSink* sink);
    virtual void SetGenerator(MixerGenerator* generator);
    virtual void AddEffect(MixerEffect* effect);
    virtual void RemoveEffect(MixerEffect* effect);

protected:
    MixerSink* _sink;
    MixerGenerator* _generator;
    QList<MixerEffect*> _effects;
};

#endif // MIXERSOURCE_H
