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
    MixerBus(const QString& name, QObject *parent = 0);
    virtual ~MixerBus();

    void AudioOut(float *outl, float *outr);
    virtual const QString GetName();

    MixerSendSink SendSink;
signals:
    void NameChanged(const QString& name);

public slots:
    void SetName(const QString& name);

private:
    MixerBuffer _buffer;
    int _currentTick;
    QString _name;

};

#endif // MIXERBUS_H
