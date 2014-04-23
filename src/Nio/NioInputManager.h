#ifndef _NIOINPUTMANAGER_H_
#define _NIOINPUTMANAGER_H_

#include <string>
#include <semaphore.h>
#include "SafeQueue.h"
#include "IMaster.h"

class NioEngine;
class NioEngineManager;
//super simple class to manage the inputs
class NioInputManager
{
    NioEngineManager* _enginemgr;
public:
    NioInputManager(NioEngineManager* enginemgr);
    ~NioInputManager();

    void PutEvent(Midi::Event ev);

    void Tick();

    NioEngine *GetInputEngine(std::string name);
    bool SetSource(std::string name);
    std::string GetSource() const;

    friend class NioEngineManager;
private:
    SafeQueue<Midi::Event> queue;
    sem_t work;
    NioEngine* currentIn;

};

#endif // _NIOINPUTMANAGER_H_
