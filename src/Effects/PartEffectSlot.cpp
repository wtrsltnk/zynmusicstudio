#include "PartEffectSlot.h"
#include "Effect.h"
#include "Reverb.h"
#include "Echo.h"
#include "Chorus.h"
#include "Distorsion.h"
#include "EQ.h"
#include "DynamicFilter.h"
#include "../Misc/XMLwrapper.h"
#include "../Params/FilterParams.h"

#include <iostream>
using namespace std;

PartEffectSlot::PartEffectSlot(pthread_mutex_t *mutex_)
    : efxoutl(new float[synth->buffersize]),
      efxoutr(new float[synth->buffersize]),
      filterpars(NULL),
      nefx(0),
      efx(NULL),
      mutex(mutex_)
{
    setpresettype("Peffect");
    memset(efxoutl, 0, synth->bufferbytes);
    memset(efxoutr, 0, synth->bufferbytes);
    defaults();
}


PartEffectSlot::~PartEffectSlot()
{
    delete efx;
    delete [] efxoutl;
    delete [] efxoutr;
}

void PartEffectSlot::defaults(void)
{
    changeeffect(0);
}

//Change the effect
void PartEffectSlot::changeeffect(int _nefx)
{
    cleanup();
    if(nefx == _nefx)
        return;
    nefx = _nefx;
    memset(efxoutl, 0, synth->bufferbytes);
    memset(efxoutr, 0, synth->bufferbytes);
    delete efx;
    switch(nefx) {
        case 1:
            efx = new Reverb(false, efxoutl, efxoutr);
            break;
        case 2:
            efx = new Echo(false, efxoutl, efxoutr);
            break;
        case 3:
            efx = new Chorus(false, efxoutl, efxoutr);
            break;
        case 4:
            efx = new Phaser(false, efxoutl, efxoutr);
            break;
        case 5:
            efx = new Alienwah(false, efxoutl, efxoutr);
            break;
        case 6:
            efx = new Distorsion(false, efxoutl, efxoutr);
            break;
        case 7:
            efx = new EQ(false, efxoutl, efxoutr);
            break;
        case 8:
            efx = new DynamicFilter(false, efxoutl, efxoutr);
            break;
        //put more effect here
        default:
            efx = NULL;
            break; //no effect (thru)
    }

    if(efx)
        filterpars = efx->filterpars;
}

//Obtain the effect number
int PartEffectSlot::geteffect(void)
{
    return nefx;
}

// Cleanup the current effect
void PartEffectSlot::cleanup(void)
{
    if(efx)
        efx->cleanup();
}


// Get the preset of the current effect
unsigned char PartEffectSlot::getpreset(void)
{
    if(efx)
        return efx->Ppreset;
    else
        return 0;
}

// Change the preset of the current effect
void PartEffectSlot::changepreset_nolock(unsigned char npreset)
{
    if(efx)
        efx->setpreset(npreset);
}

//Change the preset of the current effect(with thread locking)
void PartEffectSlot::changepreset(unsigned char npreset)
{
    pthread_mutex_lock(mutex);
    changepreset_nolock(npreset);
    pthread_mutex_unlock(mutex);
}


//Change a parameter of the current effect
void PartEffectSlot::seteffectpar_nolock(int npar, unsigned char value)
{
    if(!efx)
        return;
    efx->changepar(npar, value);
}

// Change a parameter of the current effect (with thread locking)
void PartEffectSlot::seteffectpar(int npar, unsigned char value)
{
    pthread_mutex_lock(mutex);
    seteffectpar_nolock(npar, value);
    pthread_mutex_unlock(mutex);
}

//Get a parameter of the current effect
unsigned char PartEffectSlot::geteffectpar(int npar)
{
    if(!efx)
        return 0;
    return efx->getpar(npar);
}

// Apply the effect
void PartEffectSlot::out(float *smpsl, float *smpsr)
{
    if(!efx) {
            for(int i = 0; i < synth->buffersize; ++i) {
                smpsl[i]   = 0.0f;
                smpsr[i]   = 0.0f;
                efxoutl[i] = 0.0f;
                efxoutr[i] = 0.0f;
            }
        return;
    }
    for(int i = 0; i < synth->buffersize; ++i) {
        smpsl[i]  += denormalkillbuf[i];
        smpsr[i]  += denormalkillbuf[i];
        efxoutl[i] = 0.0f;
        efxoutr[i] = 0.0f;
    }
    efx->out(smpsl, smpsr);

    float volume = efx->volume;

    if(nefx == 7) { //this is need only for the EQ effect
        memcpy(smpsl, efxoutl, synth->bufferbytes);
        memcpy(smpsr, efxoutr, synth->bufferbytes);
        return;
    }

    //Insertion effect
    for(int i = 0; i < synth->buffersize; ++i) {
        efxoutl[i] *= 2.0f * volume;
        efxoutr[i] *= 2.0f * volume;
        smpsl[i]    = efxoutl[i];
        smpsr[i]    = efxoutr[i];
    }
}


// Get the effect volume for the system effect
float PartEffectSlot::sysefxgetvolume(void)
{
    return (!efx) ? 1.0f : efx->outvolume;
}


// Get the EQ response
float PartEffectSlot::getEQfreqresponse(float freq)
{
    return (nefx == 7) ? efx->getfreqresponse(freq) : 0.0f;
}

void PartEffectSlot::add2XML(XMLwrapper *xml)
{
    xml->addpar("type", geteffect());

    if(!efx || !geteffect())
        return;
    xml->addpar("preset", efx->Ppreset);

    xml->beginbranch("EFFECT_PARAMETERS");
    for(int n = 0; n < 128; ++n) {
        int par = geteffectpar(n);
        if(par == 0)
            continue;
        xml->beginbranch("par_no", n);
        xml->addpar("par", par);
        xml->endbranch();
    }
    if(filterpars) {
        xml->beginbranch("FILTER");
        filterpars->add2XML(xml);
        xml->endbranch();
    }
    xml->endbranch();
}

void PartEffectSlot::getfromXML(XMLwrapper *xml)
{
    changeeffect(xml->getpar127("type", geteffect()));

    if(!efx || !geteffect())
        return;

    efx->Ppreset = xml->getpar127("preset", efx->Ppreset);

    if(xml->enterbranch("EFFECT_PARAMETERS")) {
        for(int n = 0; n < 128; ++n) {
            seteffectpar_nolock(n, 0); //erase effect parameter
            if(xml->enterbranch("par_no", n) == 0)
                continue;
            int par = geteffectpar(n);
            seteffectpar_nolock(n, xml->getpar127("par", par));
            xml->exitbranch();
        }
        if(filterpars)
            if(xml->enterbranch("FILTER")) {
                filterpars->getfromXML(xml);
                xml->exitbranch();
            }
        xml->exitbranch();
    }
    cleanup();
}
