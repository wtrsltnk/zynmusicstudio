#ifndef MIXERBUS_H
#define MIXERBUS_H

#include <QObject>
#include "mixerbuffer.h"
#include "mixersendsink.h"
#include "mixerchannelinput.h"

class MixerBus : public MixerChannelInput
{
    Q_OBJECT
public:
    explicit MixerBus(QObject *parent = 0);
    virtual ~MixerBus();

    void AudioOut(float *outl, float *outr);

    MixerSendSink SendSink;
signals:

public slots:

private:
    MixerBuffer _buffer;
    int _currentTick;

};

#endif // MIXERBUS_H
