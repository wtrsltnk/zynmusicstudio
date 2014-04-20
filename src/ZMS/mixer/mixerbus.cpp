#include "mixerbus.h"

MixerBus::MixerBus(QObject *parent) :
    QObject(parent), MixerSendDestination(), MixerSendSource()
{ }
