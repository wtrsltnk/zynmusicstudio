/*
  ZynAddSubFX - a software synthesizer

  OSSaudiooutput.C - Audio output for Open Sound System
  Copyright (C) 2002-2005 Nasca Octavian Paul
  Author: Nasca Octavian Paul

  This program is free software; you can redistribute it and/or modify
  it under the terms of version 2 of the GNU General Public License
  as published by the Free Software Foundation.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License (version 2 or later) for more details.

  You should have received a copy of the GNU General Public License (version 2)
  along with this program; if not, write to the Free Software Foundation,
  Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA

*/

#include "NulEngine.h"
#include "../globals.h"

#ifndef WIN32
#include <unistd.h>
#else
#include <Windows.h>
#endif
#include <iostream>

using namespace std;

NulEngine::NulEngine(NioEngineManager* mgr)
    : NioEngine(mgr), pThread(NULL)
{
    this->_name = "NULL";
}

void *NulEngine::_AudioThread(void *arg)
{
    return (static_cast<NulEngine *>(arg))->AudioThread();
}

void *NulEngine::AudioThread()
{
    while(this->pThread) {
        getNext();

#ifndef WIN32
                usleep(10000);
#else
                Sleep(1);
#endif
    }
    return NULL;
}

NulEngine::~NulEngine()
{}

bool NulEngine::Start()
{
    setAudioEnabled(true);
    return isAudioEnabled();
}

void NulEngine::Stop()
{
    setAudioEnabled(false);
}

void NulEngine::setAudioEnabled(bool nval)
{
    if(nval) {
        if(!isAudioEnabled()) {
            pthread_t     *thread = new pthread_t;
            pthread_attr_t attr;
            pthread_attr_init(&attr);
            pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
            this->pThread = thread;
            pthread_create(this->pThread, &attr, _AudioThread, this);
        }
    }
    else
    if(isAudioEnabled()) {
        pthread_t *thread = this->pThread;
        this->pThread = NULL;
        pthread_join(*thread, NULL);
        delete thread;
    }
}

bool NulEngine::isAudioEnabled() const
{
    return (this->pThread != 0);
}
