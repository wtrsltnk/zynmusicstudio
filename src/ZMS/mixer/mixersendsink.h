#ifndef MIXERSENDSINK_H
#define MIXERSENDSINK_H

#include <QList>

class MixerSendSource;

class MixerSendSink
{
public:
    explicit MixerSendSink();

    QList<MixerSendSource*> Sources() { return this->_sources; }
    void AddSource(MixerSendSource* source);
    void RemoveSource(MixerSendSource* source);

private:
    QList<MixerSendSource*> _sources;

};

#endif // MIXERSENDSINK_H
