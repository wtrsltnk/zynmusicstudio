#ifndef ENGINE_MGR_H
#define ENGINE_MGR_H

#include <list>
#include <string>
#include "Engine.h"
#include "InMgr.h"
#include "OutMgr.h"
#include "IMaster.h"

class MidiIn;
class AudioOut;

/**Container/Owner of the long lived Engines*/
class EngineMgr
{
    EngineMgr();
public:
    static EngineMgr &getInstance();
    ~EngineMgr();

    /**Execute a tick*/
    const Stereo<float *> tick(unsigned int frameSize);

    /**Gets requested engine
     * @param name case unsensitive name of engine
     * @return pointer to Engine or NULL
     */
    Engine *getEngine(std::string name);
    std::list<Engine*>& Engines() { return this->_engines; }

    /**Start up defaults*/
    bool start();

    /**Stop all engines*/
    void stop();

    //return false on failure
    bool setDefaultInputEngine(std::string name);
    bool setDefaultOutputEngine(std::string name);

    //Get the prefered sample rate from jack (if running)
    void preferedSampleRate(unsigned &rate);

    IMaster* GetMaster() { return this->_master; }
    void SetMaster(IMaster* master) { this->_master = master; }

    OutMgr* Output() { return this->_outputManager; }
    InMgr* Input() { return this->_inputManager; }

private:
    IMaster* _master;
    std::list<Engine *> _engines;
    Engine*_defaultOut;
    Engine*_defaultIn;

    OutMgr* _outputManager;
    InMgr* _inputManager;
};
#endif
