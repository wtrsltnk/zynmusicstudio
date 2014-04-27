#ifndef MIXERDRUMKIT_H
#define MIXERDRUMKIT_H

#include "../mixergenerator.h"

class MixerDrumKit : public MixerGenerator
{
    Q_OBJECT
public:
    MixerDrumKit(QObject* parent = 0);
    virtual ~MixerDrumKit();
};

#endif // MIXERDRUMKIT_H
