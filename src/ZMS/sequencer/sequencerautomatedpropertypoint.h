#ifndef SEQUENCERAUTOMATEDPROPERTYPOINT_H
#define SEQUENCERAUTOMATEDPROPERTYPOINT_H

class SequencerAutomatedProperty;

class SequencerAutomatedPropertyPoint
{
public:
    SequencerAutomatedPropertyPoint(SequencerAutomatedProperty* property);
    virtual ~SequencerAutomatedPropertyPoint();

    SequencerAutomatedProperty* GetAutomatedProperty() { return this->_property; }

    double Position() { return this->_position; }
    void SetPosition(double position) { this->_position = position; }

    double Value() { return this->_value; }
    void SetValue(double value) { this->_value = value; }

protected:
    SequencerAutomatedProperty* _property;

    double _position;  // between 0.0 and 1.0 meaning 0.0 begin of clip, 1.0 end of clip
    double _value;  // between 0.0 and 1.0
};

#endif // SEQUENCERAUTOMATEDPROPERTYPOINT_H
