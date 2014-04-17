/*
  ZynAddSubFX - a software synthesizer

  Master.cpp - It sends Midi Messages to Parts, receives samples from parts,
             process them with system/insertion effects and mix them
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

#include "Master.h"
#include "Channel.h"

#include "../Params/LFOParams.h"
#include "../Effects/EffectMgr.h"
#include "../DSP/FFTwrapper.h"

#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <iostream>
#include <algorithm>
#include <cmath>

#ifndef WIN32
#include <unistd.h>
#endif

#include "../ZMS/sequencer/sequencer.h"

using namespace std;

vuData::vuData(void)
    :outpeakl(0.0f), outpeakr(0.0f), maxoutpeakl(0.0f), maxoutpeakr(0.0f),
      rmspeakl(0.0f), rmspeakr(0.0f), clipped(0)
{}

static Master* masterInstance = NULL;

Master::Master()
{
    swaplr = 0;
    off  = 0;
    smps = 0;
    bufl = new float[synth->buffersize];
    bufr = new float[synth->buffersize];

    pthread_mutex_init(&mutex, NULL);
    pthread_mutex_init(&vumutex, NULL);
    fft = new FFTwrapper(synth->oscilsize);

    shutup = 0;

    defaults();
}

void Master::defaults()
{
    volume = 1.0f;
    setPvolume(80);
    setPkeyshift(64);

    this->addChannel();

    microtonal.defaults();
    ShutUp();
}

Master &Master::getInstance()
{
    if (!masterInstance)
        masterInstance = new Master;

    return *masterInstance;
}

Master::~Master()
{
    delete []bufl;
    delete []bufr;

    while (this->channels.empty() == false)
    {
        Channel* part = this->channels.back();
        this->channels.pop_back();
        delete part;
    }

    delete fft;

    pthread_mutex_destroy(&mutex);
    pthread_mutex_destroy(&vumutex);
}

void Master::deleteInstance()
{
    if (masterInstance)
    {
        delete masterInstance;
        masterInstance = NULL;
    }
}

/*
 * Note On Messages (velocity=0 for NoteOff)
 */
void Master::NoteOn(char chan, char note, char velocity)
{
    if(velocity) {
        for(int npart = 0; npart < this->channels.size(); ++npart)
            if(chan == channels[npart]->Prcvchn) {
//                fakepeakpart[npart] = velocity * 2;
                if(channels[npart]->Penabled)
                    channels[npart]->NoteOn(note, velocity, keyshift);
            }
    }
    else
        this->NoteOff(chan, note);
}

/*
 * Note Off Messages
 */
void Master::NoteOff(char chan, char note)
{
    for(int npart = 0; npart < this->channels.size(); ++npart)
        if((chan == channels[npart]->Prcvchn) && channels[npart]->Penabled)
            channels[npart]->NoteOff(note);
}

/*
 * Pressure Messages (velocity=0 for NoteOff)
 */
void Master::PolyphonicAftertouch(char chan, char note, char velocity)
{
    if(velocity) {
        for(int npart = 0; npart < this->channels.size(); ++npart)
            if(chan == channels[npart]->Prcvchn)
                if(channels[npart]->Penabled)
                    channels[npart]->PolyphonicAftertouch(note, velocity, keyshift);

    }
    else
        this->NoteOff(chan, note);
}

/*
 * Controllers
 */
void Master::SetController(char chan, int type, int par)
{
    if((type == C_dataentryhi) || (type == C_dataentrylo)
       || (type == C_nrpnhi) || (type == C_nrpnlo)) { //Process RPN and NRPN by the Master (ignore the chan)
        ctl.setparameternumber(type, par);

//        int parhi = -1, parlo = -1, valhi = -1, vallo = -1;
//        if(ctl.getnrpn(&parhi, &parlo, &valhi, &vallo) == 0) //this is NRPN
            //fprintf(stderr,"rcv. NRPN: %d %d %d %d\n",parhi,parlo,valhi,vallo);
//            switch(parhi) {
//                case 0x04: //System Effects
//                    if(parlo < NUM_SYS_EFX)
//                        sysefx[parlo]->seteffectpar_nolock(valhi, vallo);
//                    ;
//                    break;
//                case 0x08: //Insertion Effects
//                    if(parlo < NUM_INS_EFX)
//                        insefx[parlo]->seteffectpar_nolock(valhi, vallo);
//                    ;
//                    break;
//            }
    }
    else
    if(type == C_bankselectmsb) {      // Change current bank
        if(((unsigned int)par < bank.banks.size())
           && (bank.banks[par].dir != bank.bankfiletitle))
            bank.loadbank(bank.banks[par].dir);
    }
    else {  //other controllers
        for(int npart = 0; npart < this->channels.size(); ++npart) //Send the controller to all part assigned to the channel
            if((chan == channels[npart]->Prcvchn) && (channels[npart]->Penabled != 0))
                channels[npart]->SetController(type, par);
        ;
    }
}

void Master::SetProgram(char chan, unsigned int pgm)
{
    if(config.cfg.IgnoreProgramChange)
        return;

    for(int npart = 0; npart < this->channels.size(); ++npart)
        if(chan == channels[npart]->Prcvchn) {
            bank.loadfromslot(pgm, channels[npart]);

            //Hack to get pad note parameters to update
            //this is not real time safe and makes assumptions about the calling
            //convention of this function...
            pthread_mutex_unlock(&mutex);
            channels[npart]->applyparameters();
            pthread_mutex_lock(&mutex);
        }
}

void Master::vuUpdate(const float *outl, const float *outr)
{
    //Peak computation (for vumeters)
    vu.outpeakl = 1e-12;
    vu.outpeakr = 1e-12;
    for(int i = 0; i < synth->buffersize; ++i) {
        if(fabs(outl[i]) > vu.outpeakl)
            vu.outpeakl = fabs(outl[i]);
        if(fabs(outr[i]) > vu.outpeakr)
            vu.outpeakr = fabs(outr[i]);
    }
    if((vu.outpeakl > 1.0f) || (vu.outpeakr > 1.0f))
        vu.clipped = 1;
    if(vu.maxoutpeakl < vu.outpeakl)
        vu.maxoutpeakl = vu.outpeakl;
    if(vu.maxoutpeakr < vu.outpeakr)
        vu.maxoutpeakr = vu.outpeakr;

    //RMS Peak computation (for vumeters)
    vu.rmspeakl = 1e-12;
    vu.rmspeakr = 1e-12;
    for(int i = 0; i < synth->buffersize; ++i) {
        vu.rmspeakl += outl[i] * outl[i];
        vu.rmspeakr += outr[i] * outr[i];
    }
    vu.rmspeakl = sqrt(vu.rmspeakl / synth->buffersize_f);
    vu.rmspeakr = sqrt(vu.rmspeakr / synth->buffersize_f);
}

Channel* Master::addChannel()
{
    pthread_mutex_lock(&(mutex));
    Channel* part = new Channel(&microtonal, fft, &mutex);

    part->defaults();
    part->Penabled = 1;
    part->Prcvchn = 0;
    this->channels.push_back(part);
    pthread_mutex_unlock(&(mutex));

    return part;
}

void Master::removeChannel(Channel* part)
{
    pthread_mutex_lock(&mutex);

    for (std::vector<Channel*>::iterator i = this->channels.begin(); i != this->channels.end(); ++i)
    {
        if (*i == part)
        {
            this->channels.erase(i);
            delete part;
            break;
        }
    }

    pthread_mutex_unlock(&mutex);
}

int Master::channelIndex(class Channel* part)
{
    for (int i = 0; i < this->channels.size(); i++)
        if (part == this->channels[i])
            return i;
    return -1;
}

/*
 * Master audio out (the final sound)
 */
void Master::AudioOut(float *outl, float *outr)
{
    //Swaps the Left channel with Right Channel
    if(swaplr)
        swap(outl, outr);

    //clean up the output samples (should not be needed?)
    memset(outl, 0, synth->bufferbytes);
    memset(outr, 0, synth->bufferbytes);

    // When the sequences is paused, we should not make any sound
    if (Sequencer::Inst().Status() == Sequencer::Paused)
        return;

    //Compute part samples and store them part[npart]->partoutl,partoutr
    for(int npart = 0; npart < this->channels.size(); ++npart) {
        if(channels[npart]->Penabled != 0 && !pthread_mutex_trylock(&channels[npart]->load_mutex)) {
            channels[npart]->ComputePartSmps();
            pthread_mutex_unlock(&channels[npart]->load_mutex);
        }
    }

    //Apply the part volumes and pannings (after insertion effects)
    for(int npart = 0; npart < this->channels.size(); ++npart) {
        if(channels[npart]->Penabled == 0)
            continue;

        Stereo<float> newvol(channels[npart]->volume),
        oldvol(channels[npart]->oldvolumel,
               channels[npart]->oldvolumer);

        float pan = channels[npart]->panning;
        if(pan < 0.5f)
            newvol.l *= pan * 2.0f;
        else
            newvol.r *= (1.0f - pan) * 2.0f;

        //the volume or the panning has changed and needs interpolation
        if(ABOVE_AMPLITUDE_THRESHOLD(oldvol.l, newvol.l)
           || ABOVE_AMPLITUDE_THRESHOLD(oldvol.r, newvol.r)) {
            for(int i = 0; i < synth->buffersize; ++i) {
                Stereo<float> vol(INTERPOLATE_AMPLITUDE(oldvol.l, newvol.l,
                                                        i, synth->buffersize),
                                  INTERPOLATE_AMPLITUDE(oldvol.r, newvol.r,
                                                        i, synth->buffersize));
                channels[npart]->partoutl[i] *= vol.l;
                channels[npart]->partoutr[i] *= vol.r;
            }
            channels[npart]->oldvolumel = newvol.l;
            channels[npart]->oldvolumer = newvol.r;
        }
        else
            for(int i = 0; i < synth->buffersize; ++i) { //the volume did not changed
                channels[npart]->partoutl[i] *= newvol.l;
                channels[npart]->partoutr[i] *= newvol.r;
            }
    }


    //Mix all parts
    for(int npart = 0; npart < this->channels.size(); ++npart)
        if(channels[npart]->Penabled)   //only mix active parts
            for(int i = 0; i < synth->buffersize; ++i) { //the volume did not changed
                outl[i] += channels[npart]->partoutl[i];
                outr[i] += channels[npart]->partoutr[i];
            }

    //Master Volume
    for(int i = 0; i < synth->buffersize; ++i) {
        outl[i] *= volume;
        outr[i] *= volume;
    }

    if(!pthread_mutex_trylock(&vumutex)) {
        vuUpdate(outl, outr);
        pthread_mutex_unlock(&vumutex);
    }

    //Shutup if it is asked (with fade-out)
    if(shutup) {
        for(int i = 0; i < synth->buffersize; ++i) {
            float tmp = (synth->buffersize_f - i) / synth->buffersize_f;
            outl[i] *= tmp;
            outr[i] *= tmp;
        }
        ShutUp();
    }

    //update the LFO's time
    LFOParams::time++;

    Sequencer::Inst().AddFrames(config.cfg.SoundBufferSize);
}


/*
 * Parameter control
 */
void Master::setPvolume(char Pvolume_)
{
    Pvolume = Pvolume_;
    volume  = dB2rap((Pvolume - 96.0f) / 96.0f * 40.0f);
}

void Master::setPkeyshift(char Pkeyshift_)
{
    Pkeyshift = Pkeyshift_;
    keyshift  = (int)Pkeyshift - 64;
}

/*
 * Panic! (Clean up all parts and effects)
 */
void Master::ShutUp()
{
    for(int npart = 0; npart < this->channels.size(); ++npart) {
        channels[npart]->cleanup();
//        fakepeakpart[npart] = 0;
    }
    vuresetpeaks();
    shutup = 0;
}


/*
 * Reset peaks and clear the "cliped" flag (for VU-meter)
 */
void Master::vuresetpeaks()
{
    pthread_mutex_lock(&vumutex);
    vu.outpeakl    = 1e-9;
    vu.outpeakr    = 1e-9;
    vu.maxoutpeakl = 1e-9;
    vu.maxoutpeakr = 1e-9;
    vu.clipped     = 0;
    pthread_mutex_unlock(&vumutex);
}

vuData Master::getVuData()
{
    vuData tmp;
    pthread_mutex_lock(&vumutex);
    tmp = vu;
    pthread_mutex_unlock(&vumutex);
    return tmp;
}

void Master::applyparameters(bool lockmutex)
{
    for(int npart = 0; npart < this->channels.size(); ++npart)
        channels[npart]->applyparameters(lockmutex);
}

void Master::add2XML(XMLwrapper *xml)
{
    xml->addpar("volume", Pvolume);
    xml->addpar("key_shift", Pkeyshift);
    xml->addparbool("nrpn_receive", ctl.NRPN.receive);

    xml->beginbranch("MICROTONAL");
    microtonal.add2XML(xml);
    xml->endbranch();

    for(int npart = 0; npart < NUM_MIDI_PARTS; ++npart) {
        xml->beginbranch("PART", npart);
        channels[npart]->add2XML(xml);
        xml->endbranch();
    }
}


int Master::getalldata(char **data)
{
    XMLwrapper *xml = new XMLwrapper();

    xml->beginbranch("MASTER");

    pthread_mutex_lock(&mutex);
    add2XML(xml);
    pthread_mutex_unlock(&mutex);

    xml->endbranch();

    *data = xml->getXMLdata();
    delete (xml);
    return strlen(*data) + 1;
}

void Master::putalldata(char *data, int /*size*/)
{
    XMLwrapper *xml = new XMLwrapper();
    if(!xml->putXMLdata(data)) {
        delete (xml);
        return;
    }

    if(xml->enterbranch("MASTER") == 0)
        return;

    pthread_mutex_lock(&mutex);
    getfromXML(xml);
    pthread_mutex_unlock(&mutex);

    xml->exitbranch();

    delete (xml);
}

int Master::saveXML(const char *filename)
{
    XMLwrapper *xml = new XMLwrapper();

    xml->beginbranch("MASTER");
    add2XML(xml);
    xml->endbranch();

    int result = xml->saveXMLfile(filename);
    delete (xml);
    return result;
}



int Master::loadXML(const char *filename)
{
    XMLwrapper *xml = new XMLwrapper();
    if(xml->loadXMLfile(filename) < 0) {
        delete (xml);
        return -1;
    }

    if(xml->enterbranch("MASTER") == 0)
        return -10;
    getfromXML(xml);
    xml->exitbranch();

    delete (xml);
    return 0;
}

void Master::getfromXML(XMLwrapper *xml)
{
    setPvolume(xml->getpar127("volume", Pvolume));
    setPkeyshift(xml->getpar127("key_shift", Pkeyshift));
    ctl.NRPN.receive = xml->getparbool("nrpn_receive", ctl.NRPN.receive);


    channels[0]->Penabled = 0;
    for(int npart = 0; npart < NUM_MIDI_PARTS; ++npart) {
        if(xml->enterbranch("PART", npart) == 0)
            continue;
        channels[npart]->getfromXML(xml);
        xml->exitbranch();
    }

    if(xml->enterbranch("MICROTONAL")) {
        microtonal.getfromXML(xml);
        xml->exitbranch();
    }
}
