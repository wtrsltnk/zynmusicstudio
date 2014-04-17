/*
  ZynAddSubFX - a software synthesizer

  Engine.h - Audio Driver base class
  Copyright (C) 2009-2010 Mark McCurry
  Author: Mark McCurry

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

#ifndef ENGINE_H
#define ENGINE_H

#include "../Misc/Stereo.h"
#include "../globals.h"
#include <string>

class EngineMgr;

class Engine
{
public:
    Engine(EngineMgr* mgr);
    virtual ~Engine();

    /**Start the Driver with all capabilities
     * @return true on success*/
    virtual bool Start() = 0;
    /**Completely stop the Driver*/
    virtual void Stop() = 0;

    std::string& Name() { return this->_name; }
protected:
    std::string _name;
    EngineMgr* _engineMgr;

public:
    virtual bool IsMidiIn() = 0;

    /**Enables or disables driver based upon value*/
    virtual void setMidiEnabled(bool nval) = 0;
    /**Returns if driver is initialized*/
    virtual bool isMidiEnabled() const = 0;

public:
    virtual bool IsAudioOut() = 0;

    void setBufferSize(int _bufferSize);

    /**Sets the Frame Size for output*/
    void bufferingSize(int nBuffering);
    int bufferingSize();

    virtual void setAudioEnabled(bool nval) = 0;
    virtual bool isAudioEnabled() const    = 0;

protected:
    /**Get the next sample for output.
     * (has nsamples sampled at a rate of samplerate)*/
    const Stereo<float *> getNext();

//        int samplerate;
    int bufferSize;

};
#endif
