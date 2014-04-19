#ifndef OUTMGR_H
#define OUTMGR_H

#include "../Misc/Stereo.h"
#include <list>
#include <string>
#include <semaphore.h>

class Engine;
class EngineMgr;
class OutMgr
{
    EngineMgr* enginemgr;
public:
    OutMgr(EngineMgr* mgr);
    virtual ~OutMgr();

    /**Execute a tick*/
    const Stereo<float *> tick(unsigned int frameSize);

    /**Request a new set of samples
     * @param n number of requested samples (defaults to 1)
     * @return -1 for locking issues 0 for valid request*/
    void requestSamples(unsigned int n = 1);

    /**Gets requested driver
     * @param name case unsensitive name of driver
     * @return pointer to Audio Out or NULL
     */
    Engine *getOutputEngine(std::string name);

    bool setSink(std::string name);
    std::string getSink() const;

    class WavEngine * wave;     /**<The Wave Recorder*/
    friend class EngineMgr;
private:
    void addSamples(float *l, float *r);
    unsigned int  storedSmps() const { return (unsigned int)(priBuffCurrent.l - priBuf.l); }
    void removeStaleSamples();

    Engine* currentOut; /**<The current output driver*/

    sem_t requested;

    /**Buffer*/
    Stereo<float *> priBuf;          //buffer for primary drivers
    Stereo<float *> priBuffCurrent; //current array accessor

    float *outl;
    float *outr;

    int stales;
};

#endif
