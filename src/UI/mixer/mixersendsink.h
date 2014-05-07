#ifndef MIXERSENDSINK_H
#define MIXERSENDSINK_H

#include <QObject>
#include <QList>

class MixerSendSource;

class MixerSendSink : public QObject
{
    Q_OBJECT

public:
    MixerSendSink(QObject* parent = 0);
    virtual ~MixerSendSink();

    QList<MixerSendSource*> Sources() { return this->_sources; }
    void AddSource(MixerSendSource* source);
    void RemoveSource(MixerSendSource* source);

signals:
    void SourceAdded(MixerSendSource* source);
    void SourceRemoved(MixerSendSource* source);

private:
    QList<MixerSendSource*> _sources;

};

#endif // MIXERSENDSINK_H
