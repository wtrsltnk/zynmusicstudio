#ifndef MIXERSENDSOURCE_H
#define MIXERSENDSOURCE_H

#include <QObject>

class MixerSendDestination;

class MixerSendSource
{
public:
    explicit MixerSendSource();

    MixerSendDestination* Destination() { return this->_destination; }
    void SetDestination(MixerSendDestination* destination);

private:
    MixerSendDestination* _destination;

};

#endif // MIXERSENDSOURCE_H
