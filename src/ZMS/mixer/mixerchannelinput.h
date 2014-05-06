#ifndef MIXERCHANNELINPUT_H
#define MIXERCHANNELINPUT_H

#include <QObject>

class MixerChannel;

class MixerChannelInput : public QObject
{
    Q_OBJECT
public:
    MixerChannelInput(QObject* parent = 0);
    virtual ~MixerChannelInput();

    virtual MixerChannel* Source() { return this->_source; }
    virtual const QString GetName() = 0;

signals:
    void SourceChanged(MixerChannel* source);

public slots:
    virtual void SetSource(MixerChannel* source);

protected:
    MixerChannel* _source;
};

#endif // MIXERCHANNELINPUT_H
