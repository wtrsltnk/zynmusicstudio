/*
  ZynAddSubFX - a software synthesizer

  Master.h - It sends Midi Messages to Parts, receives samples from parts,
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

#ifndef MASTER_H
#define MASTER_H
#include <pthread.h>

#include "../globals.h"
#include "Microtonal.h"

#include "Bank.h"
#include "Dump.h"
#include "XMLwrapper.h"

#include "../Params/Controller.h"
#include "../Nio/IMaster.h"

struct vuData {
    vuData(void);
    float outpeakl, outpeakr, maxoutpeakl, maxoutpeakr,
          rmspeakl, rmspeakr;
    int clipped;
};


/** It sends Midi Messages to Parts, receives samples from parts,
 *  process them with system/insertion effects and mix them */
class Master : public IMaster
{
public:
    /** Constructor TODO make private*/
    Master();
    /** Destructor*/
    ~Master();

    static Master &getInstance();
    static void deleteInstance();

    /**Saves all settings to a XML file
     * @return 0 for ok or <0 if there is an error*/
    int saveXML(const char *filename);

    /**This adds the parameters to the XML data*/
    void add2XML(XMLwrapper *xml);

    void defaults();


    /**loads all settings from a XML file
     * @return 0 for ok or -1 if there is an error*/
    int loadXML(const char *filename);
    void applyparameters(bool lockmutex = true);

    void getfromXML(XMLwrapper *xml);

    /**get all data to a newly allocated array (used for VST)
     * @return the datasize*/
    int getalldata(char **data);
    /**put all data from the *data array to zynaddsubfx parameters (used for VST)*/
    void putalldata(char *data, int size);

    //Midi IN
    virtual void NoteOn(char chan, char note, char velocity);
    virtual void NoteOff(char chan, char note);
    virtual void PolyphonicAftertouch(char chan, char note, char velocity);
    virtual void SetController(char chan, int type, int par);
    virtual void SetProgram(char chan, unsigned int pgm);
    //void NRPN...


    void ShutUp();
    int shutup;

    void vuUpdate(const float *outl, const float *outr);

    /**Audio Output*/
    virtual void AudioOut(float *outl, float *outr);

    class Channel* addChannel();
    void removeChannel(class Channel* channel);

    std::vector<class Channel *> channels;
    int channelIndex(class Channel* channel);

    //parameters

    unsigned char Pvolume;
    unsigned char Pkeyshift;

    //parameters control
    void setPvolume(char Pvolume_);
    void setPkeyshift(char Pkeyshift_);

    //part that's apply the insertion effect; -1 to disable, -2 for master out
    void* Pinsparts[NUM_INS_EFX];


    //peaks for VU-meter
    void vuresetpeaks();
    //get VU-meter data
    vuData getVuData();

    Controller ctl;
    bool       swaplr; //if L and R are swapped

    //other objects
    Microtonal microtonal;
    Bank       bank;

    class FFTwrapper * fft;

    virtual void Lock() { pthread_mutex_lock(&this->mutex); }
    virtual void Unlock() { pthread_mutex_unlock(&this->mutex); }
private:
    bool   nullRun;
    vuData vu;
    float  volume;
    int    keyshift;

    //information relevent to generating plugin audio samples
    float *bufl;
    float *bufr;
    off_t  off;
    size_t smps;

    pthread_mutex_t mutex;
    pthread_mutex_t vumutex;
};

#endif
