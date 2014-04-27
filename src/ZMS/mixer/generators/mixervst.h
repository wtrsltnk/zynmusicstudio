#ifndef MIXERVST_H
#define MIXERVST_H

/*
 * TODO : Create a MixerGenerator that uses Vst instrument plugins to generate sound
 */

#include "../mixergenerator.h"

class MixerVst : public MixerGenerator
{
    Q_OBJECT
public:
    MixerVst(QObject* parent = 0);
    virtual ~MixerVst();
};

#endif // MIXERVST_H
