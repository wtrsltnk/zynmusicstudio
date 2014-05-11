#ifndef LIBRARYEQEFFECT_H
#define LIBRARYEQEFFECT_H

#include "mixer/mixereffect.h"
#include "../Effects/EQ.h"

class LibraryEQEffect : public MixerEffect
{
    Q_OBJECT
public:
    LibraryEQEffect(QObject *parent = 0);
    virtual ~LibraryEQEffect();

    virtual void EffectOnBuffer(MixerBuffer& in);

signals:

public slots:

private:
    EQ* _eq;

};

#endif // LIBRARYEQEFFECT_H
