#include "NioInputManager.h"
#include "NioEngineManager.h"
#include <iostream>

using namespace std;

NioInputManager::NioInputManager(NioEngineManager* enginemgr)
    : _enginemgr(enginemgr), queue(100)
{
    this->currentIn = NULL;
    sem_init(&this->work, PTHREAD_PROCESS_PRIVATE, 0);
}

NioInputManager::~NioInputManager()
{
    //lets stop the consumer thread
    sem_destroy(&this->work);
}

void NioInputManager::PutEvent(Midi::Event ev)
{
    if(this->queue.push(ev)) //check for error
        cerr << "ERROR: Midi Ringbuffer is FULL" << endl;
    else
        sem_post(&this->work);
}

void NioInputManager::Tick()
{
    this->_enginemgr->GetMaster()->Lock();

    Midi::Event ev;
    while(!sem_trywait(&this->work)) {
        this->queue.pop(ev);

        switch(ev.type) {
            case Midi::M_NOTE:
                if(ev.value)
                    this->_enginemgr->GetMaster()->NoteOn(ev.channel, ev.num, ev.value);
                else
                    this->_enginemgr->GetMaster()->NoteOff(ev.channel, ev.num);
                break;

            case Midi::M_CONTROLLER:
                this->_enginemgr->GetMaster()->SetController(ev.channel, ev.num, ev.value);
                break;

            case Midi::M_PGMCHANGE:
                this->_enginemgr->GetMaster()->SetProgram(ev.channel, ev.num);
                break;
            case Midi::M_PRESSURE:
                this->_enginemgr->GetMaster()->PolyphonicAftertouch(ev.channel, ev.num, ev.value);
                break;
        }
    }

    this->_enginemgr->GetMaster()->Unlock();
}

bool NioInputManager::SetSource(string name)
{
    NioEngine *src = this->GetInputEngine(name);

    if(!src)
        return false;

    if(this->currentIn)
        this->currentIn->setMidiEnabled(false);
    this->currentIn = src;
    this->currentIn->setMidiEnabled(true);

    bool success = this->currentIn->isMidiEnabled();

    //Keep system in a valid state (aka with a running driver)
    if(!success)
        (this->currentIn = this->GetInputEngine("NULL"))->setMidiEnabled(true);

    return success;
}

string NioInputManager::GetSource() const
{
    if(this->currentIn)
        return this->currentIn->Name();
    else
        return "ERROR";
}

NioEngine* NioInputManager::GetInputEngine(string name)
{
    NioEngine* e = this->_enginemgr->getEngine(name);
    if (e != 0 && e->IsMidiIn())
        return e;
    return 0;
}
