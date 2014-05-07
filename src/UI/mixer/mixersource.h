#ifndef MIXERSOURCE_H
#define MIXERSOURCE_H

#include <QObject>
#include "mixerbuffer.h"

class MixerSink;
class MixerChannelInput;
class MixerEffect;

class MixerSource : public QObject
{
    Q_OBJECT
public:
    MixerSource(QObject* parent = 0);
    virtual ~MixerSource();

    virtual MixerBuffer& AudioOut() = 0;

signals:

public slots:

protected:
};

#endif // MIXERSOURCE_H
