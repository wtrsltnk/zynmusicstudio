#ifndef MIXERINSTRUMENT_H
#define MIXERINSTRUMENT_H

#include "mixerchannelinput.h"
#include "../../Misc/Instrument.h"

class MixerInstrument : public MixerChannelInput
{
    Q_OBJECT
public:
    MixerInstrument(Instrument* instrument);
    virtual ~MixerInstrument();

    Instrument* GetInstrument() { return this->_instrument; }
    virtual const QString GetName();

    void ComputeSamples();
    virtual void NoteOn(char chan, char note, char velocity);
    virtual void NoteOff(char chan, char note);
    virtual void PolyphonicAftertouch(char chan, char note, char velocity);
    virtual void SetController(char chan, int type, int par);

private:
    Instrument* _instrument;
};

#endif // MIXERINSTRUMENT_H
