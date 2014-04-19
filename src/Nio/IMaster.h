#ifndef IMASTER_H
#define IMASTER_H

namespace Midi
{
    enum Type {
        M_NONE = 0,
        M_NOTE = 1,
        M_CONTROLLER = 2,
        M_PGMCHANGE  = 3,
        M_PRESSURE   = 4
    };

    struct Event {
        Event() : channel(0), type(M_NONE), num(0), value(0) { }

        int channel; //the midi channel for the event
        Type type;
        int num;     //note, controller or program number
        int value;   //velocity or controller value
    };
}

class IMaster
{
public:
    virtual ~IMaster() { }

    virtual void Lock() = 0;
    virtual void Unlock() = 0;

    virtual void AudioOut(float *outl, float *outr) = 0;

    virtual void NoteOn(char chan, char note, char velocity) = 0;
    virtual void NoteOff(char chan, char note) = 0;
    virtual void PolyphonicAftertouch(char chan, char note, char velocity) = 0;
    virtual void SetController(char chan, int type, int par) = 0;
    virtual void SetProgram(char chan, unsigned int pgm) = 0;

};

#endif // IMASTER_H
