#ifndef MIXERCHANNEL_H
#define MIXERCHANNEL_H

#include <QObject>
#include <QColor>
#include "mixerbuffer.h"
#include "mixersource.h"
#include "mixersendsink.h"
#include "mixersendsource.h"
#include "../Misc/Instrument.h"

class MixerChannel : public MixerSource, public MixerSendSource, public MixerSendSink
{
    Q_OBJECT
public:
    explicit MixerChannel(QObject *parent = 0);
    virtual ~MixerChannel();

    Instrument* GetInstrument();

    QString GetName();
    QColor GetColor();
    int GetVolume();

    virtual MixerBuffer& AudioOut();
signals:
    void InstrumentChanged(Instrument* instrument);

    void NameChanged(QString name);
    void ColorChanged(QColor color);
    void VolumeChanged(int volume);

public slots:
    void SetInstrument(Instrument* instrument);

    void SetName(QString name);
    void SetColor(QColor color);
    void SetVolume(int volume);

private:
    MixerBuffer _buffer;
    Instrument* _instrument;

    QString _name;
    QColor _color;
    int _volume;

    int _currentTick;

};

#endif // MIXERCHANNEL_H
