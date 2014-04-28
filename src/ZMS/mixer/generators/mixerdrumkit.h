#ifndef MIXERDRUMKIT_H
#define MIXERDRUMKIT_H

#include "../mixerchannelinput.h"

class MixerDrumKit : public MixerChannelInput
{
    Q_OBJECT
public:
    MixerDrumKit(QObject* parent = 0);
    virtual ~MixerDrumKit();
};

#endif // MIXERDRUMKIT_H
