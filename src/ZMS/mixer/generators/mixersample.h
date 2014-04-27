#ifndef MIXERSAMPLE_H
#define MIXERSAMPLE_H

/*
 * TODO : Create a MixerGenerator that uses samples to generate sound
 */

#include "../mixergenerator.h"

class MixerSample : public MixerGenerator
{
    Q_OBJECT
public:
    MixerSample(QObject* parent = 0);
    virtual ~MixerSample();
};

#endif // MIXERSAMPLE_H
