#ifndef _NIOENGINEMANAGER_H_
#define _NIOENGINEMANAGER_H_

#include <list>
#include <string>
#include "NioEngine.h"
#include "NioInputManager.h"
#include "NioOutputManager.h"
#include "IMaster.h"

class MidiIn;
class AudioOut;

/**Container/Owner of the long lived Engines*/
class NioEngineManager
{
public:
    NioEngineManager(IMaster* master);
    virtual ~NioEngineManager();

    /**Execute a tick*/
    const Stereo<float *> tick(unsigned int frameSize);

    /**Gets requested engine
     * @param name case unsensitive name of engine
     * @return pointer to Engine or NULL
     */
    NioEngine *getEngine(std::string name);
    std::list<NioEngine*>& Engines() { return this->_engines; }

    /**Start up defaults*/
    bool start();

    /**Stop all engines*/
    void stop();

    //return false on failure
    bool setDefaultInputEngine(std::string name);
    bool setDefaultOutputEngine(std::string name);

    //Get the prefered sample rate from jack (if running)
    static void preferedSampleRate(unsigned &rate);

    IMaster* GetMaster() { return this->_master; }

    NioOutputManager* Output() { return this->_outputManager; }
    NioInputManager* Input() { return this->_inputManager; }

private:
    IMaster* _master;
    std::list<NioEngine *> _engines;
    NioEngine*_defaultOut;
    NioEngine*_defaultIn;

    NioOutputManager* _outputManager;
    NioInputManager* _inputManager;
};
#endif // _NIOENGINEMANAGER_H_
