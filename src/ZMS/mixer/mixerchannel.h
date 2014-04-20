#ifndef MIXERCHANNEL_H
#define MIXERCHANNEL_H

#include <QObject>
#include <QColor>
#include "../Misc/Instrument.h"
#include "mixersenddestination.h"
#include "mixersendsource.h"

class MixerChannel : public QObject, public MixerSendSource, public MixerSendDestination
{
    Q_OBJECT
public:
    explicit MixerChannel(QObject *parent = 0);

    Instrument* GetInstrument();

    QString GetName();
    QColor GetColor();

signals:
    void InstrumentChanged(Instrument* instrument);

    void NameChanged(QString name);
    void ColorChanged(QColor color);

public slots:
    void SetInstrument(Instrument* instrument);

    void SetName(QString name);
    void SetColor(QColor color);

private:
    Instrument* _instrument;
    QList<MixerSendDestination*> _sends;

    QString _name;
    QColor _color;

};

#endif // MIXERCHANNEL_H
