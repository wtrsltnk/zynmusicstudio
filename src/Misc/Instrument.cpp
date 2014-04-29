/*
  ZynAddSubFX - a software synthesizer

  Part.cpp - Part implementation
  Copyright (C) 2002-2005 Nasca Octavian Paul
  Author: Nasca Octavian Paul

  This program is free software; you can redistribute it and/or modify
  it under the terms of version 2 of the GNU General Public License
  as published by the Free Software Foundation.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License (version 2 or later) for more details.

  You should have received a copy of the GNU General Public License (version 2)
  along with this program; if not, write to the Free Software Foundation,
  Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA

*/

#include "Instrument.h"
#include "Microtonal.h"
#include "Util.h"
#include "XMLwrapper.h"
#include "../Effects/EffectMgr.h"
#include "../Params/ADnoteParameters.h"
#include "../Params/SUBnoteParameters.h"
#include "../Params/PADnoteParameters.h"
#include "../Synth/ADnote.h"
#include "../Synth/SUBnote.h"
#include "../Synth/PADnote.h"
#include "../DSP/FFTwrapper.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

Instrument::Instrument(Microtonal *microtonal_, FFTwrapper *fft_, pthread_mutex_t *mutex_)
{
    microtonal = microtonal_;
    fft      = fft_;
    mutex    = mutex_;
    pthread_mutex_init(&load_mutex, NULL);
    partoutl = new float [synth->buffersize];
    partoutr = new float [synth->buffersize];

    synths.adpars  = new ADnoteParameters(fft);
    synths.subpars = new SUBnoteParameters();
    synths.padpars = new PADnoteParameters(fft, mutex);

    killallnotes = 0;
    oldfreq      = -1.0f;

    for(int i = 0; i < POLIPHONY; ++i) {
        partnote[i].status = KEY_OFF;
        partnote[i].note   = -1;
        partnote[i].itemsplaying = 0;
        partnote[i].item.adnote  = NULL;
        partnote[i].item.subnote = NULL;
        partnote[i].item.padnote = NULL;
        partnote[i].time = 0;
    }
    cleanup();

    oldvolumel = oldvolumer = 0.5f;
    lastnote   = -1;
    lastpos    = 0; // lastpos will store previously used NoteOn(...)'s pos.
    lastlegatomodevalid = false; // To store previous legatomodevalid value.

    defaults();
}

void Instrument::defaults()
{
    Penabled    = 0;
    Pminkey     = 0;
    Pmaxkey     = 127;
    Pnoteon     = 1;
    Ppolymode   = 1;
    Plegatomode = 0;
    setPvolume(96);
    Pkeyshift = 64;
    Prcvchn   = 0;
    setPpanning(64);
    Pvelsns   = 64;
    Pveloffs  = 64;
    Pkeylimit = 15;
    defaultsinstrument();
    ctl.defaults();
}

void Instrument::defaultsinstrument()
{
    this->Pname = PART_DEFAULT_NAME;

    synths.Penabled   = 1;
    synths.Padenabled = 1;
    synths.adpars->defaults();
    synths.subpars->defaults();
    synths.padpars->defaults();
}



/*
 * Cleanup the part
 */
void Instrument::cleanup(bool final)
{
    for(int k = 0; k < POLIPHONY; ++k)
        KillNotePos(k);
    for(int i = 0; i < synth->buffersize; ++i) {
        partoutl[i] = final ? 0.0f : denormalkillbuf[i];
        partoutr[i] = final ? 0.0f : denormalkillbuf[i];
    }
    ctl.resetall();
}

Instrument::~Instrument()
{
    cleanup(true);
    if(synths.adpars != NULL)
        delete (synths.adpars);
    if(synths.subpars != NULL)
        delete (synths.subpars);
    if(synths.padpars != NULL)
        delete (synths.padpars);
    synths.adpars  = NULL;
    synths.subpars = NULL;
    synths.padpars = NULL;

    delete [] partoutl;
    delete [] partoutr;
}

/*
 * Note On Messages
 */
void Instrument::NoteOn(unsigned char note,
                  unsigned char velocity,
                  int masterkeyshift)
{
    int i, pos;

    // Legato and MonoMem used vars:
    int  posb = POLIPHONY - 1; // Just a dummy initial value.
    bool legatomodevalid = false; //true when legato mode is determined applicable.
    bool doinglegato     = false; // true when we determined we do a legato note.
    bool ismonofirstnote = false; /*(In Mono/Legato) true when we determined
                  no other notes are held down or sustained.*/
    int lastnotecopy     = lastnote; //Useful after lastnote has been changed.

    if(Pnoteon == 0)
        return;
    if((note < Pminkey) || (note > Pmaxkey))
        return;

    // MonoMem stuff:
    if(Ppolymode == 0) { // If Poly is off
        monomemnotes.push_back(note); // Add note to the list.
        monomem[note].velocity  = velocity; // Store this note's velocity.
        monomem[note].mkeyshift = masterkeyshift; /* Store masterkeyshift too,
                         I'm not sure why though... */
        if((partnote[lastpos].status != KEY_PLAYING)
           && (partnote[lastpos].status != KEY_RELASED_AND_SUSTAINED))
            ismonofirstnote = true;  // No other keys are held or sustained.
    }
    else
    // Poly mode is On so just make sure the list is empty.
    if(!monomemnotes.empty())
        monomemnotes.clear();

    lastnote = note;

    pos = -1;
    for(i = 0; i < POLIPHONY; ++i)
        if(partnote[i].status == KEY_OFF) {
            pos = i;
            break;
        }

    if((Plegatomode != 0)) {
        if(Ppolymode != 0) {
            fprintf(
                stderr,
                "ZynAddSubFX WARNING: Poly and Legato modes are both On, that should not happen ! ... Disabling Legato mode ! - (Part.cpp::NoteOn(..))\n");
            Plegatomode = 0;
        }
        else {
            // Legato mode is on and applicable.
            legatomodevalid = true;
            if((!ismonofirstnote) && (lastlegatomodevalid)) {
                // At least one other key is held or sustained, and the
                // previous note was played while in valid legato mode.
                doinglegato = true; // So we'll do a legato note.
                pos  = lastpos; // A legato note uses same pos as previous..
                posb = lastposb; // .. same goes for posb.
            }
            else {
                // Legato mode is valid, but this is only a first note.
                for(i = 0; i < POLIPHONY; ++i)
                    if((partnote[i].status == KEY_PLAYING)
                       || (partnote[i].status == KEY_RELASED_AND_SUSTAINED))
                        RelaseNotePos(i);

                // Set posb
                posb = (pos + 1) % POLIPHONY; //We really want it (if the following fails)
                for(i = 0; i < POLIPHONY; ++i)
                    if((partnote[i].status == KEY_OFF) && (pos != i)) {
                        posb = i;
                        break;
                    }
            }
            lastposb = posb; // Keep a trace of used posb
        }
    }
    else     // Legato mode is either off or non-applicable.
    if(Ppolymode == 0) {   //if the mode is 'mono' turn off all other notes
        for(i = 0; i < POLIPHONY; ++i)
            if(partnote[i].status == KEY_PLAYING)
                RelaseNotePos(i);
        RelaseSustainedKeys();
    }
    lastlegatomodevalid = legatomodevalid;

    if(pos == -1)
        //test
        fprintf(stderr,
                "%s",
                "NOTES TOO MANY (> POLIPHONY) - (Part.cpp::NoteOn(..))\n");
    else {
        //start the note
        partnote[pos].status = KEY_PLAYING;
        partnote[pos].note   = note;
        if(legatomodevalid) {
            partnote[posb].status = KEY_PLAYING;
            partnote[posb].note   = note;
        }

        //this computes the velocity sensing of the part
        float vel = VelF(velocity / 127.0f, Pvelsns);

        //compute the velocity offset
        vel += (Pveloffs - 64.0f) / 64.0f;
        if(vel < 0.0f)
            vel = 0.0f;
        else
        if(vel > 1.0f)
            vel = 1.0f;

        //compute the keyshift
        int partkeyshift = (int)Pkeyshift - 64;
        int keyshift     = masterkeyshift + partkeyshift;

        //initialise note frequency
        float notebasefreq;
        notebasefreq = microtonal->getnotefreq(note, keyshift);
        if(notebasefreq < 0.0f)
            return;                  //the key is no mapped

        //Portamento
        if(oldfreq < 1.0f)
            oldfreq = notebasefreq;           //this is only the first note is played

        // For Mono/Legato: Force Portamento Off on first
        // notes. That means it is required that the previous note is
        // still held down or sustained for the Portamento to activate
        // (that's like Legato).
        int portamento = 0;
        if((Ppolymode != 0) || (!ismonofirstnote))
            // I added a third argument to the
            // ctl.initportamento(...) function to be able
            // to tell it if we're doing a legato note.
            portamento = ctl.initportamento(oldfreq, notebasefreq, doinglegato);

        if(portamento != 0)
            ctl.portamento.noteusing = pos;
        oldfreq = notebasefreq;

        lastpos = pos; // Keep a trace of used pos.

        if(doinglegato) {
            // Do Legato note
            if((synths.Padenabled != 0)
               && (partnote[pos].item.adnote != NULL)
               && (partnote[posb].item.adnote != NULL)) {
                partnote[pos].item.adnote->legatonote(notebasefreq,
                                                            vel,
                                                            portamento,
                                                            note,
                                                            true); //'true' is to tell it it's being called from here.
                partnote[posb].item.adnote->legatonote(notebasefreq,
                                                             vel,
                                                             portamento,
                                                             note,
                                                             true);
            }

            if((synths.Psubenabled != 0)
               && (partnote[pos].item.subnote != NULL)
               && (partnote[posb].item.subnote != NULL)) {
                partnote[pos].item.subnote->legatonote(
                    notebasefreq, vel, portamento, note, true);
                partnote[posb].item.subnote->legatonote(
                    notebasefreq, vel, portamento, note, true);
            }

            if((synths.Ppadenabled != 0)
               && (partnote[pos].item.padnote != NULL)
               && (partnote[posb].item.padnote != NULL)) {
                partnote[pos].item.padnote->legatonote(
                    notebasefreq, vel, portamento, note, true);
                partnote[posb].item.padnote->legatonote(
                    notebasefreq, vel, portamento, note, true);
            }
            return; // Ok, Legato note done, return.
        }

        partnote[pos].itemsplaying = 0;
        if(legatomodevalid)
            partnote[posb].itemsplaying = 0;

        if(synths.Padenabled != 0)
            partnote[pos].item.adnote = new ADnote(synths.adpars,
                                                         &ctl,
                                                         notebasefreq,
                                                         vel,
                                                         portamento,
                                                         note,
                                                         false);
        if(synths.Psubenabled != 0)
            partnote[pos].item.subnote = new SUBnote(synths.subpars,
                                                           &ctl,
                                                           notebasefreq,
                                                           vel,
                                                           portamento,
                                                           note,
                                                           false);
        if(synths.Ppadenabled != 0)
            partnote[pos].item.padnote = new PADnote(synths.padpars,
                                                           &ctl,
                                                           notebasefreq,
                                                           vel,
                                                           portamento,
                                                           note,
                                                           false);
        if((synths.Padenabled != 0) || (synths.Psubenabled != 0)
           || (synths.Ppadenabled != 0))
            partnote[pos].itemsplaying++;

        // Spawn another note (but silent) if legatomodevalid==true
        if(legatomodevalid) {
            if(synths.Padenabled != 0)
                partnote[posb].item.adnote = new ADnote(synths.adpars,
                                                              &ctl,
                                                              notebasefreq,
                                                              vel,
                                                              portamento,
                                                              note,
                                                              true);     //true for silent.
            if(synths.Psubenabled != 0)
                partnote[posb].item.subnote = new SUBnote(
                    synths.subpars,
                    &ctl,
                    notebasefreq,
                    vel,
                    portamento,
                    note,
                    true);
            if(synths.Ppadenabled != 0)
                partnote[posb].item.padnote = new PADnote(
                    synths.padpars,
                    &ctl,
                    notebasefreq,
                    vel,
                    portamento,
                    note,
                    true);
            if((synths.Padenabled != 0) || (synths.Psubenabled != 0)
               || (synths.Ppadenabled != 0))
                partnote[posb].itemsplaying++;
        }
    }

    //this only relase the keys if there is maximum number of keys allowed
    setkeylimit(Pkeylimit);
}

/*
 * Note Off Messages
 */
void Instrument::NoteOff(unsigned char note) //relase the key
{
    int i;

    // This note is released, so we remove it from the list.
    if(!monomemnotes.empty())
        monomemnotes.remove(note);

    for(i = POLIPHONY - 1; i >= 0; i--) //first note in, is first out if there are same note multiple times
        if((partnote[i].status == KEY_PLAYING) && (partnote[i].note == note)) {
            if(ctl.sustain.sustain == 0) { //the sustain pedal is not pushed
                if((Ppolymode == 0) && (!monomemnotes.empty()))
                    MonoMemRenote();  // To play most recent still held note.
                else
                    RelaseNotePos(i);
                /// break;
            }
            else    //the sustain pedal is pushed
                partnote[i].status = KEY_RELASED_AND_SUSTAINED;
        }
}

void Instrument::PolyphonicAftertouch(unsigned char note,
                                unsigned char velocity,
                                int masterkeyshift)
{
    (void) masterkeyshift;
    if(!Pnoteon || (note < Pminkey) || (note > Pmaxkey))
        return;

    // MonoMem stuff:
    if(!Ppolymode)   // if Poly is off

        monomem[note].velocity = velocity;       // Store this note's velocity.


    for(int i = 0; i < POLIPHONY; ++i)
        if((partnote[i].note == note) && (partnote[i].status == KEY_PLAYING)) {
            /* update velocity */
            // compute the velocity offset
            float vel =
                VelF(velocity / 127.0f, Pvelsns) + (Pveloffs - 64.0f) / 64.0f;
            vel = (vel < 0.0f) ? 0.0f : vel;
            vel = (vel > 1.0f) ? 1.0f : vel;

            if(synths.Padenabled && partnote[i].item.adnote)
                partnote[i].item.adnote->setVelocity(vel);
            if(synths.Psubenabled && partnote[i].item.subnote)
                partnote[i].item.subnote->setVelocity(vel);
            if(synths.Ppadenabled && partnote[i].item.padnote)
                partnote[i].item.padnote->setVelocity(vel);

        }
}

/*
 * Controllers
 */
void Instrument::SetController(unsigned int type, int par)
{
    switch(type) {
        case C_pitchwheel:
            ctl.setpitchwheel(par);
            break;
        case C_expression:
            ctl.setexpression(par);
            setPvolume(Pvolume); //update the volume
            break;
        case C_portamento:
            ctl.setportamento(par);
            break;
        case C_panning:
            ctl.setpanning(par);
            setPpanning(Ppanning); //update the panning
            break;
        case C_filtercutoff:
            ctl.setfiltercutoff(par);
            break;
        case C_filterq:
            ctl.setfilterq(par);
            break;
        case C_bandwidth:
            ctl.setbandwidth(par);
            break;
        case C_modwheel:
            ctl.setmodwheel(par);
            break;
        case C_fmamp:
            ctl.setfmamp(par);
            break;
        case C_volume:
            ctl.setvolume(par);
            if(ctl.volume.receive != 0)
                volume = ctl.volume.volume;
            else
                setPvolume(Pvolume);
            break;
        case C_sustain:
            ctl.setsustain(par);
            if(ctl.sustain.sustain == 0)
                RelaseSustainedKeys();
            break;
        case C_allsoundsoff:
            AllNotesOff(); //Panic
            break;
        case C_resetallcontrollers:
            ctl.resetall();
            RelaseSustainedKeys();
            if(ctl.volume.receive != 0)
                volume = ctl.volume.volume;
            else
                setPvolume(Pvolume);
            setPvolume(Pvolume); //update the volume
            setPpanning(Ppanning); //update the panning

            if(synths.adpars != NULL)
            {
                synths.adpars->GlobalPar.Reson->
                sendcontroller(C_resonance_center, 1.0f);

                synths.adpars->GlobalPar.Reson->
                sendcontroller(C_resonance_bandwidth, 1.0f);
            }
            //more update to add here if I add controllers
            break;
        case C_allnotesoff:
            RelaseAllKeys();
            break;
        case C_resonance_center:
            ctl.setresonancecenter(par);
            if(synths.adpars != NULL)
            {
                synths.adpars->GlobalPar.Reson->
                sendcontroller(C_resonance_center,
                               ctl.resonancecenter.relcenter);
            }
            break;
        case C_resonance_bandwidth:
            ctl.setresonancebw(par);
            synths.adpars->GlobalPar.Reson->
            sendcontroller(C_resonance_bandwidth, ctl.resonancebandwidth.relbw);
            break;
    }
}
/*
 * Relase the sustained keys
 */

void Instrument::RelaseSustainedKeys()
{
    // Let's call MonoMemRenote() on some conditions:
    if((Ppolymode == 0) && (!monomemnotes.empty()))
        if(monomemnotes.back() != lastnote) // Sustain controller manipulation would cause repeated same note respawn without this check.
            MonoMemRenote();  // To play most recent still held note.

    for(int i = 0; i < POLIPHONY; ++i)
        if(partnote[i].status == KEY_RELASED_AND_SUSTAINED)
            RelaseNotePos(i);
}

/*
 * Relase all keys
 */

void Instrument::RelaseAllKeys()
{
    for(int i = 0; i < POLIPHONY; ++i)
        if((partnote[i].status != KEY_RELASED)
           && (partnote[i].status != KEY_OFF)) //thanks to Frank Neumann
            RelaseNotePos(i);
}

// Call NoteOn(...) with the most recent still held key as new note
// (Made for Mono/Legato).
void Instrument::MonoMemRenote()
{
    unsigned char mmrtempnote = monomemnotes.back(); // Last list element.
    monomemnotes.pop_back(); // We remove it, will be added again in NoteOn(...).
    if(Pnoteon == 0)
        RelaseNotePos(lastpos);
    else
        NoteOn(mmrtempnote, monomem[mmrtempnote].velocity,
               monomem[mmrtempnote].mkeyshift);
}

/*
 * Release note at position
 */
void Instrument::RelaseNotePos(int pos)
{
        if(partnote[pos].item.adnote != NULL)
            if(partnote[pos].item.adnote)
                partnote[pos].item.adnote->relasekey();

        if(partnote[pos].item.subnote != NULL)
            if(partnote[pos].item.subnote != NULL)
                partnote[pos].item.subnote->relasekey();

        if(partnote[pos].item.padnote != NULL)
            if(partnote[pos].item.padnote)
                partnote[pos].item.padnote->relasekey();
    partnote[pos].status = KEY_RELASED;
}


/*
 * Kill note at position
 */
void Instrument::KillNotePos(int pos)
{
    partnote[pos].status = KEY_OFF;
    partnote[pos].note   = -1;
    partnote[pos].time   = 0;
    partnote[pos].itemsplaying = 0;

    if(partnote[pos].item.adnote != NULL) {
        delete (partnote[pos].item.adnote);
        partnote[pos].item.adnote = NULL;
    }
    if(partnote[pos].item.subnote != NULL) {
        delete (partnote[pos].item.subnote);
        partnote[pos].item.subnote = NULL;
    }
    if(partnote[pos].item.padnote != NULL) {
        delete (partnote[pos].item.padnote);
        partnote[pos].item.padnote = NULL;
    }
    if(pos == ctl.portamento.noteusing) {
        ctl.portamento.noteusing = -1;
        ctl.portamento.used      = 0;
    }
}


/*
 * Set Part's key limit
 */
void Instrument::setkeylimit(unsigned char Pkeylimit)
{
    this->Pkeylimit = Pkeylimit;
    int keylimit = Pkeylimit;
    if(keylimit == 0)
        keylimit = POLIPHONY - 5;

    //release old keys if the number of notes>keylimit
    if(Ppolymode != 0) {
        int notecount = 0;
        for(int i = 0; i < POLIPHONY; ++i)
            if((partnote[i].status == KEY_PLAYING)
               || (partnote[i].status == KEY_RELASED_AND_SUSTAINED))
                notecount++;

        int oldestnotepos = -1;
        if(notecount > keylimit)   //find out the oldest note
            for(int i = 0; i < POLIPHONY; ++i) {
                int maxtime = 0;
                if(((partnote[i].status == KEY_PLAYING)
                    || (partnote[i].status == KEY_RELASED_AND_SUSTAINED))
                   && (partnote[i].time > maxtime)) {
                    maxtime = partnote[i].time;
                    oldestnotepos = i;
                }
            }
        if(oldestnotepos != -1)
            RelaseNotePos(oldestnotepos);
    }
}


/*
 * Prepare all notes to be turned off
 */
void Instrument::AllNotesOff()
{
    killallnotes = 1;
}

void Instrument::RunNote(unsigned int k)
{
    unsigned noteplay = 0;
    int sendcurrenttofx = partnote[k].item.sendtoparteffect;

    for(unsigned type = 0; type < 3; ++type) {
        //Select a note
        SynthNote **note = NULL;
        if(type == 0)
            note = &partnote[k].item.adnote;
        else if(type == 1)
            note = &partnote[k].item.subnote;
        else if(type == 2)
            note = &partnote[k].item.padnote;

        //Process if it exists
        if(!(*note))
            continue;
        noteplay++;

        float *tmpoutr = getTmpBuffer();
        float *tmpoutl = getTmpBuffer();
        (*note)->noteout(&tmpoutl[0], &tmpoutr[0]);

        if((*note)->finished()) {
            delete (*note);
            (*note) = NULL;
        }
        for(int i = 0; i < synth->buffersize; ++i) { //add the note to part(mix)
            partoutl[i] += tmpoutl[i];
            partoutr[i] += tmpoutr[i];
        }
        returnTmpBuffer(tmpoutr);
        returnTmpBuffer(tmpoutl);
    }

    //Kill note if there is no synth on that note
    if(noteplay == 0)
        KillNotePos(k);
}

/*
 * Compute Part samples and store them in the partoutl[] and partoutr[]
 */
void Instrument::ComputeSamples()
{
    for(int i = 0; i < synth->buffersize; ++i) {
        partoutl[i] = 0.0f;
        partoutr[i] = 0.0f;
    }

    for(unsigned k = 0; k < POLIPHONY; ++k) {
        if(partnote[k].status == KEY_OFF)
            continue;
        partnote[k].time++;
        //get the sampledata of the note and kill it if it's finished
        RunNote(k);
    }

    //Kill All Notes if killallnotes!=0
    if(killallnotes != 0) {
        for(int i = 0; i < synth->buffersize; ++i) {
            float tmp = (synth->buffersize_f - i) / synth->buffersize_f;
            partoutl[i] *= tmp;
            partoutr[i] *= tmp;
        }
        for(int k = 0; k < POLIPHONY; ++k)
            KillNotePos(k);
        killallnotes = 0;
    }
    ctl.updateportamento();
}

/*
 * Parameter control
 */
void Instrument::setPvolume(char Pvolume_)
{
    Pvolume = Pvolume_;
    volume  =
        dB2rap((Pvolume - 96.0f) / 96.0f * 40.0f) * ctl.expression.relvolume;
}

void Instrument::setPpanning(char Ppanning_)
{
    Ppanning = Ppanning_;
    panning  = Ppanning / 127.0f + ctl.panning.pan;
    if(panning < 0.0f)
        panning = 0.0f;
    else
    if(panning > 1.0f)
        panning = 1.0f;
}

/*
 * Enable or disable a kit item
 */
void Instrument::setstatus(int Penabled_)
{
    synths.Penabled = Penabled_;

    bool resetallnotes = false;
    if(Penabled_ == 0) {
        if(synths.adpars != NULL)
            delete (synths.adpars);
        if(synths.subpars != NULL)
            delete (synths.subpars);
        if(synths.padpars != NULL) {
            delete (synths.padpars);
            resetallnotes = true;
        }
        synths.adpars   = NULL;
        synths.subpars  = NULL;
        synths.padpars  = NULL;
    }
    else {
        if(synths.adpars == NULL)
            synths.adpars = new ADnoteParameters(fft);
        if(synths.subpars == NULL)
            synths.subpars = new SUBnoteParameters();
        if(synths.padpars == NULL)
            synths.padpars = new PADnoteParameters(fft, mutex);
    }

    if(resetallnotes)
        for(int k = 0; k < POLIPHONY; ++k)
            KillNotePos(k);
}

void Instrument::add2XMLinstrument(XMLwrapper *xml)
{
    xml->beginbranch("INFO");
    xml->addparstr("name", Pname.c_str());
    xml->addparstr("author", Pauthor.c_str());
    xml->addparstr("comments", Pcomments.c_str());
    xml->endbranch();


    xml->beginbranch("INSTRUMENT_KIT");

    xml->beginbranch("INSTRUMENT_KIT_ITEM", 0);
    xml->addparbool("enabled", synths.Penabled);
    if(synths.Penabled != 0) {
        xml->addparbool("muted", synths.Pmuted);
        xml->addpar("min_key", synths.Pminkey);
        xml->addpar("max_key", synths.Pmaxkey);

        xml->addparbool("add_enabled", synths.Padenabled);
        if((synths.Padenabled != 0) && (synths.adpars != NULL)) {
            xml->beginbranch("ADD_SYNTH_PARAMETERS");
            synths.adpars->add2XML(xml);
            xml->endbranch();
        }

        xml->addparbool("sub_enabled", synths.Psubenabled);
        if((synths.Psubenabled != 0) && (synths.subpars != NULL)) {
            xml->beginbranch("SUB_SYNTH_PARAMETERS");
            synths.subpars->add2XML(xml);
            xml->endbranch();
        }

        xml->addparbool("pad_enabled", synths.Ppadenabled);
        if((synths.Ppadenabled != 0) && (synths.padpars != NULL)) {
            xml->beginbranch("PAD_SYNTH_PARAMETERS");
            synths.padpars->add2XML(xml);
            xml->endbranch();
        }
    }
    xml->endbranch();
    xml->endbranch();
}

void Instrument::add2XML(XMLwrapper *xml)
{
    xml->addpar("volume", Pvolume);
    xml->addpar("panning", Ppanning);

    xml->addpar("min_key", Pminkey);
    xml->addpar("max_key", Pmaxkey);
    xml->addpar("key_shift", Pkeyshift);
    xml->addpar("rcv_chn", Prcvchn);

    xml->addpar("velocity_sensing", Pvelsns);
    xml->addpar("velocity_offset", Pveloffs);

    xml->addparbool("note_on", Pnoteon);
    xml->addparbool("poly_mode", Ppolymode);
    xml->addpar("legato_mode", Plegatomode);
    xml->addpar("key_limit", Pkeylimit);

    xml->beginbranch("INSTRUMENT");
    add2XMLinstrument(xml);
    xml->endbranch();

    xml->beginbranch("CONTROLLER");
    ctl.add2XML(xml);
    xml->endbranch();
}

int Instrument::saveXML(const char *filename)
{
    XMLwrapper *xml;
    xml = new XMLwrapper();

    xml->beginbranch("INSTRUMENT");
    add2XMLinstrument(xml);
    xml->endbranch();

    int result = xml->saveXMLfile(filename);
    delete (xml);
    return result;
}

int Instrument::loadXMLinstrument(const char *filename) /*{*/
{
    XMLwrapper *xml = new XMLwrapper();
    if(xml->loadXMLfile(filename) < 0) {
        delete (xml);
        return -1;
    }

    if(xml->enterbranch("INSTRUMENT") == 0)
        return -10;
    getfromXMLinstrument(xml);
    xml->exitbranch();

    delete (xml);
    return 0;
} /*}*/

void Instrument::applyparameters(bool lockmutex) /*{*/
{
    for(int n = 0; n < NUM_KIT_ITEMS; ++n)
        if((synths.padpars != NULL) && (synths.Ppadenabled != 0))
            synths.padpars->applyparameters(lockmutex);
} /*}*/

void Instrument::getfromXMLinstrument(XMLwrapper *xml)
{
    if(xml->enterbranch("INFO")) {
        this->Pname = xml->getparstr("name", "default");
        this->Pauthor = xml->getparstr("author", "");
        this->Pcomments = xml->getparstr("comments", "");

        xml->exitbranch();
    }

    if(xml->enterbranch("INSTRUMENT_KIT")) {

        setstatus(0);
        synths.Pmuted  = xml->getparbool("muted", synths.Pmuted);
        synths.Pminkey = xml->getpar127("min_key", synths.Pminkey);
        synths.Pmaxkey = xml->getpar127("max_key", synths.Pmaxkey);

        synths.Padenabled = xml->getparbool("add_enabled",
                                            synths.Padenabled);
        if(xml->enterbranch("ADD_SYNTH_PARAMETERS")) {
            synths.adpars->getfromXML(xml);
            xml->exitbranch();
        }

        synths.Psubenabled = xml->getparbool("sub_enabled",
                                             synths.Psubenabled);
        if(xml->enterbranch("SUB_SYNTH_PARAMETERS")) {
            synths.subpars->getfromXML(xml);
            xml->exitbranch();
        }

        synths.Ppadenabled = xml->getparbool("pad_enabled",
                                             synths.Ppadenabled);
        if(xml->enterbranch("PAD_SYNTH_PARAMETERS")) {
            synths.padpars->getfromXML(xml);
            xml->exitbranch();
        }

        xml->exitbranch();

        xml->exitbranch();
    }
}

void Instrument::getfromXML(XMLwrapper *xml)
{
    Penabled = xml->getparbool("enabled", Penabled);

    setPvolume(xml->getpar127("volume", Pvolume));
    setPpanning(xml->getpar127("panning", Ppanning));

    Pminkey   = xml->getpar127("min_key", Pminkey);
    Pmaxkey   = xml->getpar127("max_key", Pmaxkey);
    Pkeyshift = xml->getpar127("key_shift", Pkeyshift);
    Prcvchn   = xml->getpar127("rcv_chn", Prcvchn);

    Pvelsns  = xml->getpar127("velocity_sensing", Pvelsns);
    Pveloffs = xml->getpar127("velocity_offset", Pveloffs);

    Pnoteon     = xml->getparbool("note_on", Pnoteon);
    Ppolymode   = xml->getparbool("poly_mode", Ppolymode);
    Plegatomode = xml->getparbool("legato_mode", Plegatomode); //older versions
    if(!Plegatomode)
        Plegatomode = xml->getpar127("legato_mode", Plegatomode);
    Pkeylimit = xml->getpar127("key_limit", Pkeylimit);


    if(xml->enterbranch("INSTRUMENT")) {
        getfromXMLinstrument(xml);
        xml->exitbranch();
    }

    if(xml->enterbranch("CONTROLLER")) {
        ctl.getfromXML(xml);
        xml->exitbranch();
    }
}
