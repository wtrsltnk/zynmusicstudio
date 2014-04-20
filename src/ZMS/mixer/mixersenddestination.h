#ifndef MIXERSENDDESTINATION_H
#define MIXERSENDDESTINATION_H

class MixerSendSource;

class MixerSendDestination
{
public:
    explicit MixerSendDestination();

    MixerSendSource* Source() { return this->_source; }
    void SetSource(MixerSendSource* source);

private:
    MixerSendSource* _source;

};

#endif // MIXERSENDDESTINATION_H
