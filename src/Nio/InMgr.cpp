#include "InMgr.h"
#include "MidiIn.h"
#include "EngineMgr.h"
#include <iostream>

using namespace std;

InMgr::InMgr(EngineMgr* enginemgr)
    : _enginemgr(enginemgr), queue(100)
{
    this->currentIn = NULL;
    sem_init(&this->work, PTHREAD_PROCESS_PRIVATE, 0);
}

InMgr::~InMgr()
{
    //lets stop the consumer thread
    sem_destroy(&this->work);
}

void InMgr::PutEvent(MidiEvent ev)
{
    if(this->queue.push(ev)) //check for error
        cerr << "ERROR: Midi Ringbuffer is FULL" << endl;
    else
        sem_post(&this->work);
}

void InMgr::flush()
{
    this->_enginemgr->GetMaster()->Lock();

    MidiEvent ev;
    while(!sem_trywait(&this->work)) {
        this->queue.pop(ev);

        switch(ev.type) {
            case M_NOTE:
                if(ev.value)
                    this->_enginemgr->GetMaster()->NoteOn(ev.channel, ev.num, ev.value);
                else
                    this->_enginemgr->GetMaster()->NoteOff(ev.channel, ev.num);
                break;

            case M_CONTROLLER:
                this->_enginemgr->GetMaster()->SetController(ev.channel, ev.num, ev.value);
                break;

            case M_PGMCHANGE:
                this->_enginemgr->GetMaster()->SetProgram(ev.channel, ev.num);
                break;
            case M_PRESSURE:
                this->_enginemgr->GetMaster()->PolyphonicAftertouch(ev.channel, ev.num, ev.value);
                break;
        }
    }

    this->_enginemgr->GetMaster()->Unlock();
}

bool InMgr::setSource(string name)
{
    Engine *src = this->getInputEngine(name);

    if(!src)
        return false;

    if(this->currentIn)
        this->currentIn->setMidiEnabled(false);
    this->currentIn = src;
    this->currentIn->setMidiEnabled(true);

    bool success = this->currentIn->isMidiEnabled();

    //Keep system in a valid state (aka with a running driver)
    if(!success)
        (this->currentIn = this->getInputEngine("NULL"))->setMidiEnabled(true);

    return success;
}

string InMgr::getSource() const
{
    if(this->currentIn)
        return this->currentIn->Name();
    else
        return "ERROR";
}

Engine* InMgr::getInputEngine(string name)
{
    Engine* e = this->_enginemgr->getEngine(name);
    if (e != 0 && e->IsMidiIn())
        return e;
    return 0;
}
