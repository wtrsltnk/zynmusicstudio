#ifndef TIMESIGNATURE_H
#define TIMESIGNATURE_H

#include <QObject>

class TimeSignature : public QObject
{
    Q_OBJECT

public:
    TimeSignature();

    int numBeats() const;
    int beatUnit() const;
    int beatsPerMinute() const;

public slots:
    void setNumBeats(int num);
    void setBeatUnit(int unit);
    void setBeatsPerMinute(int bpm);

signals:
    void numBeatsChanged(int num);
    void beatUnitChanged(int unit);
    void beatsPerMinuteChanged(int bpm);

protected:
    int _numBeats;
    int _beatUnit;
    int _beatsPerMinute;

};

#endif // TIMESIGNATURE_H
