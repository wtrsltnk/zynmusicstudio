#include "OutMgr.h"
#include <algorithm>
#include <iostream>
#include <cassert>
#include "AudioOut.h"
#include "Engine.h"
#include "EngineMgr.h"
#include "InMgr.h"
#include "WavEngine.h"
#include "../Misc/Master.h"
#include "../Misc/Util.h" //for set_realtime()

using namespace std;

OutMgr::OutMgr(EngineMgr* mgr)
    : enginemgr(mgr), wave(new WavEngine(mgr)),
      priBuf(new float[4096],
             new float[4096]), priBuffCurrent(priBuf)
{
    this->currentOut = NULL;
    this->stales     = 0;

    //init samples
    this->outr = new float[synth->buffersize];
    this->outl = new float[synth->buffersize];
    memset(this->outl, 0, synth->bufferbytes);
    memset(this->outr, 0, synth->bufferbytes);
}

OutMgr::~OutMgr()
{
    delete this->wave;
    delete [] this->priBuf.l;
    delete [] this->priBuf.r;
    delete [] this->outr;
    delete [] this->outl;
}

const Stereo<float *> OutMgr::tick(unsigned int frameSize)
{
    this->removeStaleSamples();
    while(frameSize > this->storedSmps())
    {
        this->enginemgr->GetMaster()->Lock();
        this->enginemgr->GetMaster()->AudioOut(this->outl, this->outr);
        this->enginemgr->GetMaster()->Unlock();

        this->addSamples(outl, outr);
    }
    this->stales = frameSize;
    return this->priBuf;
}

Engine *OutMgr::getOutputEngine(string name)
{
    Engine* e = this->enginemgr->getEngine(name);
    if (e != 0 && e->IsAudioOut())
        return e;
    return 0;
}

bool OutMgr::setSink(string name)
{
    Engine* sink = this->getOutputEngine(name);

    if(!sink)
        return false;

    if(this->currentOut)
        this->currentOut->setAudioEnabled(false);

    this->currentOut = sink;
    this->currentOut->setAudioEnabled(true);

    bool success = this->currentOut->isAudioEnabled();

    //Keep system in a valid state (aka with a running driver)
    if(!success)
        (this->currentOut = this->getOutputEngine("NULL"))->setAudioEnabled(true);

    return success;
}

string OutMgr::getSink() const
{
    if(this->currentOut)
        return this->currentOut->Name();
    else {
        cerr << "BUG: No current output in OutMgr " << __LINE__ << endl;
        return "ERROR";
    }
    return "ERROR";
}

void OutMgr::addSamples(float *l, float *r)
{
    //allow wave file to syphon off stream
    this->wave->push(Stereo<float *>(l, r), synth->buffersize);

    memcpy(this->priBuffCurrent.l, l, synth->bufferbytes);
    memcpy(this->priBuffCurrent.r, r, synth->bufferbytes);
    this->priBuffCurrent.l += synth->buffersize;
    this->priBuffCurrent.r += synth->buffersize;
}

void OutMgr::removeStaleSamples()
{
    if(!this->stales)
        return;

    const int leftover = storedSmps() - this->stales;

    assert(leftover > -1);

    //leftover samples [seen at very low latencies]
    if(leftover) {
        memmove(this->priBuf.l, this->priBuffCurrent.l - leftover, leftover * sizeof(float));
        memmove(this->priBuf.r, this->priBuffCurrent.r - leftover, leftover * sizeof(float));
        this->priBuffCurrent.l = this->priBuf.l + leftover;
        this->priBuffCurrent.r = this->priBuf.r + leftover;
    }
    else
        this->priBuffCurrent = this->priBuf;

    this->stales = 0;
}
