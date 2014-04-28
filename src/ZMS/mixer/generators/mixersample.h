#ifndef MIXERSAMPLE_H
#define MIXERSAMPLE_H

/*
 * TODO : Create a MixerGenerator that uses samples to generate sound
 */

#include "../mixerchannelinput.h"

class MixerSample : public MixerChannelInput
{
    Q_OBJECT
public:
    MixerSample(QObject* parent = 0);
    virtual ~MixerSample();
};

#endif // MIXERSAMPLE_H
