#ifndef MIXERSOURCE_H
#define MIXERSOURCE_H

#include "mixerbuffer.h"

class MixerSink;

class MixerSource
{
public:
    MixerSource();
    virtual ~MixerSource();

    virtual MixerBuffer& AudioOut() = 0;

    virtual MixerSink* Sink() { return this->_sink; }
    virtual void SetSink(MixerSink* sink);
protected:
    MixerSink* _sink;
};

#endif // MIXERSOURCE_H
