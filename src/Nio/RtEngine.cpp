/*
  ZynAddSubFX - a software synthesizer

  RtEngine.h - Midi input through RtMidi for Windows
  Copyright (C) 2014 Wouter Saaltink
  Author: Wouter Saaltink

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
#include "RtEngine.h"
#include "NioInputManager.h"
#include "NioEngineManager.h"

RtEngine::RtEngine(NioEngineManager* mgr)
    : NioEngine(mgr), midiin(0)
{
    this->_name = "RT";
}

RtEngine::~RtEngine()
{
    this->Stop();
}

bool RtEngine::Start()
{
    this->midiin = new RtMidiIn();
    this->midiin->setCallback(RtEngine::callback, this);
    this->midiin->openPort(0);

    return true;
}

void RtEngine::Stop()
{
    if (this->midiin != 0)
        delete this->midiin;
    this->midiin = 0;
}

void RtEngine::setMidiEnabled(bool nval)
{
    if (nval)
        this->Start();
    else
        this->Stop();
}

bool RtEngine::isMidiEnabled() const
{
    return (this->midiin != 0);
}

void RtEngine::callback(double timeStamp, std::vector<unsigned char> *message, void *userData)
{
    RtEngine* engine = (RtEngine*)userData;

    Midi::Event ev;
    unsigned char chan = message->at(0) & 0x0f;
    switch(message->at(0) & 0xf0) {
        case 0x80: //Note Off
            ev.type    = Midi::M_NOTE;
            ev.channel = chan;
            ev.num     = message->at(1);
            ev.value   = 0;
            engine->_engineMgr->Input()->PutEvent(ev);
            break;
        case 0x90: //Note On
            ev.type    = Midi::M_NOTE;
            ev.channel = chan;
            ev.num     = message->at(1);
            ev.value   = message->at(2);
            engine->_engineMgr->Input()->PutEvent(ev);
            break;
        case 0xA0: /* pressure, aftertouch */
            ev.type    = Midi::M_PRESSURE;
            ev.channel = chan;
            ev.num     = message->at(1);
            ev.value   = message->at(2);
            engine->_engineMgr->Input()->PutEvent(ev);
            break;
        case 0xb0: //Controller
            ev.type    = Midi::M_CONTROLLER;
            ev.channel = chan;
            ev.num     = message->at(1);
            ev.value   = message->at(2);
            engine->_engineMgr->Input()->PutEvent(ev);
            break;
        case 0xc0: //Program Change
            ev.type    = Midi::M_PGMCHANGE;
            ev.channel = chan;
            ev.num     = message->at(1);
            ev.value   = 0;
            engine->_engineMgr->Input()->PutEvent(ev);
            break;
        case 0xe0: //Pitch Wheel
            ev.type    = Midi::M_CONTROLLER;
            ev.channel = chan;
            ev.num     = C_pitchwheel;
            ev.value   = (message->at(1) + message->at(2) * (int) 128) - 8192;
            engine->_engineMgr->Input()->PutEvent(ev);
            break;
    }
}
