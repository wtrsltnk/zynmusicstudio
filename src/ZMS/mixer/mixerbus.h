#ifndef MIXERBUS_H
#define MIXERBUS_H

#include <QObject>
#include "mixerbuffer.h"
#include "mixersendsink.h"
#include "mixersendsource.h"

class MixerBus : public QObject, public MixerSendSink, public MixerSendSource
{
    Q_OBJECT
public:
    explicit MixerBus(QObject *parent = 0);

    void AudioOut(float *outl, float *outr);
signals:

public slots:

private:
    MixerBuffer _buffer;
    int _currentTick;

};

#endif // MIXERBUS_H
