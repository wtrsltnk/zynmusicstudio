#ifndef MIXERSENDSOURCE_H
#define MIXERSENDSOURCE_H

#include <QObject>
#include <QList>

class MixerSendSink;

class MixerSendSource : public QObject
{
    Q_OBJECT

public:
    MixerSendSource(QObject* parent = 0);
    virtual ~MixerSendSource();

    QList<MixerSendSink*> Sinks() { return this->_sinks; }
    void AddDestination(MixerSendSink* sink);
    void RemoveDestination(MixerSendSink* sink);

signals:
    void SinkAdded(MixerSendSink* sink);
    void SinkRemoved(MixerSendSink* sink);

private:
    QList<MixerSendSink*> _sinks;

};

#endif // MIXERSENDSOURCE_H
