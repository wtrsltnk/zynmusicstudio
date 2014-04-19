#ifndef MIXERCHANNEL_H
#define MIXERCHANNEL_H

#include <QObject>
#include <QColor>
#include "../Misc/Instrument.h"

class MixerChannel : public QObject
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
    QString _name;
    QColor _color;

};

#endif // MIXERCHANNEL_H
