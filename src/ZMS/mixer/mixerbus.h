#ifndef MIXERBUS_H
#define MIXERBUS_H

#include <QObject>
#include "mixersenddestination.h"
#include "mixersendsource.h"

class MixerBus : public QObject, public MixerSendDestination, public MixerSendSource
{
    Q_OBJECT
public:
    explicit MixerBus(QObject *parent = 0);

signals:

public slots:

};

#endif // MIXERBUS_H
