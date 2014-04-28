#ifndef MIXERSYNTHESIZER_H
#define MIXERSYNTHESIZER_H

#include "../mixerchannelinput.h"

class MixerSynthesizer : public MixerChannelInput
{
    Q_OBJECT
public:
    MixerSynthesizer(QObject* parent = 0);
    virtual ~MixerSynthesizer();
};

#endif // MIXERSYNTHESIZER_H
