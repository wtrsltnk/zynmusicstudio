#ifndef MIXERCHANNEL_H
#define MIXERCHANNEL_H

#include <QObject>
#include <QColor>
#include "mixerbuffer.h"
#include "mixersendsource.h"
#include "mixereffectcontainer.h"
#include "../Misc/Instrument.h"

class MixerMaster;
class MixerChannelInput;
class MixerEffect;

class MixerChannel : public QObject
{
    Q_OBJECT
public:
    explicit MixerChannel(QObject *parent = 0);
    virtual ~MixerChannel();

    QString GetName();
    QColor GetColor();
    int GetVolume();

    virtual MixerBuffer& AudioOut();

    virtual MixerMaster* Sink() { return this->_sink; }
    virtual MixerChannelInput* ChannelInput() { return this->_generator; }

    MixerSendSource SendSource;
    MixerEffectContainer Effects;
signals:
    void NameChanged(QString name);
    void ColorChanged(QColor color);
    void VolumeChanged(int volume);

    void SinkChanged(MixerMaster* sink);
    void ChannelInputChanged(MixerChannelInput* input);

public slots:
    void SetName(QString name);
    void SetColor(QColor color);
    void SetVolume(int volume);

    virtual void SetSink(MixerMaster* sink);
    virtual void SetChannelInput(MixerChannelInput* input);

private:
    MixerBuffer _buffer;

    QString _name;
    QColor _color;
    int _volume;

    MixerMaster* _sink;
    MixerChannelInput* _generator;

    int _currentTick;

};

#endif // MIXERCHANNEL_H
