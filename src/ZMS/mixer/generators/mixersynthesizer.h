#ifndef MIXERSYNTHESIZER_H
#define MIXERSYNTHESIZER_H

#include "../mixergenerator.h"

class MixerSynthesizer : public MixerGenerator
{
    Q_OBJECT
public:
    MixerSynthesizer(QObject* parent = 0);
    virtual ~MixerSynthesizer();
};

#endif // MIXERSYNTHESIZER_H
