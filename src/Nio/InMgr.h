#ifndef INMGR_H
#define INMGR_H

#include <string>
#include <semaphore.h>
#include "SafeQueue.h"
#include "IMaster.h"

class Engine;
class EngineMgr;
//super simple class to manage the inputs
class InMgr
{
    EngineMgr* _enginemgr;
public:
    InMgr(EngineMgr* enginemgr);
    ~InMgr();

    void PutEvent(MidiEvent ev);

    /**Flush the Midi Queue*/
    void flush();

    Engine *getInputEngine(std::string name);
    bool setSource(std::string name);
    std::string getSource() const;

    friend class EngineMgr;
private:
    SafeQueue<MidiEvent> queue;
    sem_t work;
    Engine* currentIn;

};

#endif
