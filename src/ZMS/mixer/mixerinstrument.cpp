#include "mixerinstrument.h"

MixerInstrument::MixerInstrument(Instrument* instrument)
    : MixerChannelInput(), _instrument(instrument)
{ }

MixerInstrument::~MixerInstrument()
{ }

void MixerInstrument::ComputeSamples()
{
    if (this->_instrument != 0)
        this->_instrument->ComputeSamples();
}

void MixerInstrument::NoteOn(char chan, char note, char velocity)
{
    if (this->_instrument != 0 && this->_instrument->Prcvchn == chan)
        this->_instrument->NoteOn(note, velocity, 0);
}

void MixerInstrument::NoteOff(char chan, char note)
{
    if (this->_instrument != 0 && this->_instrument->Prcvchn == chan)
        this->_instrument->NoteOff(note);
}

void MixerInstrument::PolyphonicAftertouch(char chan, char note, char velocity)
{
    if (this->_instrument != 0 && this->_instrument->Prcvchn == chan)
        this->_instrument->PolyphonicAftertouch(note, velocity, 0);
}

void MixerInstrument::SetController(char chan, int type, int par)
{
    if (this->_instrument != 0 && this->_instrument->Prcvchn == chan)
        this->_instrument->SetController(type, par);
}
