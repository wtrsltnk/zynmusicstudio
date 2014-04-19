/*
    AlsaEngine.h

    Copyright 2009, Alan Calvert
              2010, Mark McCurry

    This file is part of ZynAddSubFX, which is free software: you can
    redistribute it and/or modify it under the terms of the GNU General
    Public License as published by the Free Software Foundation, either
    version 3 of the License, or (at your option) any later version.

    ZynAddSubFX is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with ZynAddSubFX.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef ALSA_ENGINE_H
#define ALSA_ENGINE_H

#include <pthread.h>
#include <string>
#include <alsa/asoundlib.h>
#include <queue>

#include "Engine.h"
#include "OutMgr.h"
#include "../Misc/Stereo.h"

class AlsaEngine : public Engine
{
    public:
        AlsaEngine(EngineMgr* mgr);
        virtual ~AlsaEngine();

        virtual bool IsMidiIn() { return true; }
        virtual bool IsAudioOut() { return true; }

        virtual bool Start();
        virtual void Stop();

        virtual void setAudioEnabled(bool nval);
        virtual bool isAudioEnabled() const;
        virtual void setMidiEnabled(bool nval);
        virtual bool isMidiEnabled() const;

    protected:
        void *AudioThread();
        static void *_AudioThread(void *arg);
        void *MidiThread();
        static void *_MidiThread(void *arg);

    private:
        bool openMidi();
        void stopMidi();
        bool openAudio();
        void stopAudio();

        short *interleave(const Stereo<float *> &smps);

        struct {
            std::string device;
            snd_seq_t  *handle;
            int alsaId;
            pthread_t pThread;
        } midi;

        struct {
            snd_pcm_t *handle;
            snd_pcm_hw_params_t *params;
            unsigned int      sampleRate;
            snd_pcm_uframes_t frames;
            unsigned int      periods;
            short    *buffer;
            pthread_t pThread;
        } audio;

        void *processAudio();
};

#endif
