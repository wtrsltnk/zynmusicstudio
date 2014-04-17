#include "timesignature.h"

TimeSignature::TimeSignature()
    : _numBeats(4), _beatUnit(4)
{ }

int TimeSignature::numBeats() const
{
    return this->_numBeats;
}

void TimeSignature::setNumBeats(int num)
{
    this->_numBeats = num;
    emit numBeatsChanged(num);
}

int TimeSignature::beatUnit() const
{
    return this->_beatUnit;
}

void TimeSignature::setBeatUnit(int unit)
{
    this->_beatUnit = unit;
    emit beatUnitChanged(unit);
}

int TimeSignature::beatsPerMinute() const
{
    return this->_beatsPerMinute;
}

void TimeSignature::setBeatsPerMinute(int bpm)
{
    this->_beatsPerMinute = bpm;
    emit beatsPerMinuteChanged(bpm);
}
