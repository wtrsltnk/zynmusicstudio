#ifndef _NIOOUTPUTMANAGER_H_
#define _NIOOUTPUTMANAGER_H_

#include "../Misc/Stereo.h"
#include <list>
#include <string>
#include <semaphore.h>

class NioEngine;
class NioEngineManager;
class NioOutputManager
{
    NioEngineManager* enginemgr;
public:
    NioOutputManager(NioEngineManager* mgr);
    virtual ~NioOutputManager();

    /**Execute a tick*/
    const Stereo<float *> Tick(unsigned int frameSize);

    /**Request a new set of samples
     * @param n number of requested samples (defaults to 1)
     * @return -1 for locking issues 0 for valid request*/
    void requestSamples(unsigned int n = 1);

    /**Gets requested driver
     * @param name case unsensitive name of driver
     * @return pointer to Audio Out or NULL
     */
    NioEngine *GetOutputEngine(std::string name);

    bool SetSink(std::string name);
    std::string GetSink() const;

    class WavEngine * wave;     /**<The Wave Recorder*/
    friend class NioEngineManager;
private:
    void addSamples(float *l, float *r);
    unsigned int  storedSmps() const { return (unsigned int)(priBuffCurrent.l - priBuf.l); }
    void removeStaleSamples();

    NioEngine* currentOut; /**<The current output driver*/

    sem_t requested;

    /**Buffer*/
    Stereo<float *> priBuf;          //buffer for primary drivers
    Stereo<float *> priBuffCurrent; //current array accessor

    float *outl;
    float *outr;

    int stales;
};

#endif // _NIOOUTPUTMANAGER_H_
