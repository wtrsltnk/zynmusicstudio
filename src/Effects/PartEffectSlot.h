#ifndef PARTEFFECTSLOT_H
#define PARTEFFECTSLOT_H

#include <pthread.h>

#include "Alienwah.h"
#include "Phaser.h"
#include "../Params/Presets.h"

class Effect;
class FilterParams;
class XMLwrapper;

#include "Distorsion.h"
#include "EQ.h"
#include "DynamicFilter.h"
#include "../Misc/XMLwrapper.h"
#include "../Params/FilterParams.h"
#include "../Params/Presets.h"

class PartEffectSlot:public Presets
{
public:
    PartEffectSlot(pthread_mutex_t *mutex_);
    virtual ~PartEffectSlot();

    void add2XML(XMLwrapper *xml);
    void defaults(void);
    void getfromXML(XMLwrapper *xml);

    void out(float *smpsl, float *smpsr);

    /**get the output(to speakers) volume of the systemeffect*/
    float sysefxgetvolume(void);

    void cleanup(void);

    void changeeffect(int nefx_);
    int geteffect(void);
    void changepreset(unsigned char npreset);
    void changepreset_nolock(unsigned char npreset);
    unsigned char getpreset(void);
    void seteffectpar(int npar, unsigned char value);
    void seteffectpar_nolock(int npar, unsigned char value);
    unsigned char geteffectpar(int npar);

    float     *efxoutl, *efxoutr;

    // used by UI
    float getEQfreqresponse(float freq);

    FilterParams *filterpars;

private:
    int     nefx;
    Effect *efx;
    pthread_mutex_t *mutex;

};

#endif // PARTEFFECTSLOT_H
