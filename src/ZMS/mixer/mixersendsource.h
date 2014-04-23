#ifndef MIXERSENDSOURCE_H
#define MIXERSENDSOURCE_H

#include <QList>

class MixerSendSink;

class MixerSendSource
{
public:
    explicit MixerSendSource();

    QList<MixerSendSink*> Destinations() { return this->_destinations; }
    void AddDestination(MixerSendSink* destination);
    void RemoveDestination(MixerSendSink* destination);

private:
    QList<MixerSendSink*> _destinations;

};

#endif // MIXERSENDSOURCE_H
