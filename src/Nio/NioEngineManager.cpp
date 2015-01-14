#include "NioEngineManager.h"
#include "NioInputManager.h"
#include "NioOutputManager.h"
#include <algorithm>
#include <iostream>

#include "NulEngine.h"
#if OSS
#include "OssEngine.h"
#endif
#if ALSA
#include "AlsaEngine.h"
#endif
#if JACK
#include "JackEngine.h"
#endif
#if PORTAUDIO
#include "PaEngine.h"
#endif
#include "RtEngine.h"

using namespace std;

NioEngineManager::NioEngineManager(IMaster* master)
    : _master(master),
      _inputManager(new NioInputManager(this)),
      _outputManager(new NioOutputManager(this))
{
    NioEngine *defaultEng = new NulEngine(this);

    //conditional compiling mess (but contained)
    _engines.push_back(defaultEng);
#if OSS
    this->_engines.push_back(new OssEngine(this));
#endif
#if ALSA
    this->_engines.push_back(new AlsaEngine(this));
#endif
#if JACK
    this->_engines.push_back(new JackEngine(this));
#endif
#if PORTAUDIO
    this->_engines.push_back(new PaEngine(this));
#endif
    this->_engines.push_back(new RtEngine(this));

    this->_defaultOut = defaultEng;
    this->_defaultIn = defaultEng;

    this->setDefaultOutputEngine("NULL");
    this->setDefaultInputEngine("NULL");
}

NioEngineManager::~NioEngineManager()
{
    for(list<NioEngine *>::iterator itr = _engines.begin();
        itr != _engines.end(); ++itr)
        delete *itr;
}

/* Sequence of a tick
 * 1) lets see if we have any stuff to do via midi
 * 2) Lets do that stuff
 * 3) Lets see if the event queue has anything for us
 * 4) Lets empty that out
 * 5) Lets remove old/stale samples
 * 6) Lets see if we need to generate samples
 * 7) Lets generate some
 * 8) Lets return those samples to the primary and secondary outputs
 * 9) Lets wait for another tick
 */
const Stereo<float *> NioEngineManager::tick(unsigned int frameSize)
{
    this->_inputManager->Tick();

    return this->_outputManager->Tick(frameSize);
}

NioEngine *NioEngineManager::getEngine(string name)
{
    transform(name.begin(), name.end(), name.begin(), ::toupper);
    for(list<NioEngine *>::iterator itr = _engines.begin();
        itr != _engines.end(); ++itr)
        if((*itr)->Name() == name)
            return *itr;
    return NULL;
}

bool NioEngineManager::start()
{
    bool expected = true;
    if(!(_defaultOut && _defaultIn)) {
        cerr << "ERROR: It looks like someone broke the Nio Output\n"
             << "       Attempting to recover by defaulting to the\n"
             << "       Null Engine." << endl;
        _defaultOut = this->getEngine("NULL");
        _defaultIn  = this->getEngine("NULL");
    }

    this->_outputManager->currentOut = _defaultOut;
    this->_inputManager->currentIn    = _defaultIn;

    //open up the default output(s)
    cout << "Starting Audio: " << _defaultOut->Name() << endl;
    _defaultOut->setAudioEnabled(true);
    if(_defaultOut->isAudioEnabled())
        cout << "Audio Started" << endl;
    else {
        expected = false;
        cerr << "ERROR: The default audio output failed to open!" << endl;
        this->_outputManager->currentOut = this->getEngine("NULL");
        this->_outputManager->currentOut->setAudioEnabled(true);
    }

    cout << "Starting MIDI: " << _defaultIn->Name() << endl;
    _defaultIn->setMidiEnabled(true);
    if(_defaultIn->isMidiEnabled())
        cout << "MIDI Started" << endl;
    else { //recover
        expected = false;
        cerr << "ERROR: The default MIDI input failed to open!" << endl;
        this->_inputManager->currentIn = this->getEngine("NULL");
        this->_inputManager->currentIn->setMidiEnabled(true);
    }

    //Show if expected drivers were booted
    return expected;
}

void NioEngineManager::stop()
{
    for(list<NioEngine *>::iterator itr = _engines.begin();
        itr != _engines.end(); ++itr)
        (*itr)->Stop();
}

bool NioEngineManager::setDefaultInputEngine(string name)
{
    NioEngine* chosen;
    if((chosen = this->getEngine(name)) && chosen->IsMidiIn()) {    //got the input
        _defaultIn = chosen;
        return true;
    }

    //Warn user
    cerr << "Error: " << name << " is not a recognized MIDI input source"
         << endl;
    cerr << "       Defaulting to the NULL input source" << endl;

    return false;
}

bool NioEngineManager::setDefaultOutputEngine(string name)
{
    NioEngine* chosen;
    if((chosen = this->getEngine(name)) && chosen->IsAudioOut()) {    //got the output
        _defaultOut = chosen;
        return true;
    }

    //Warn user
    cerr << "Error: " << name << " is not a recognized audio backend" << endl;
    cerr << "       Defaulting to the NULL audio backend" << endl;
    return false;
}

#if JACK
#include <jack/jack.h>
void NioEngineManager::preferedSampleRate(unsigned &rate)
{
    jack_client_t *client = jack_client_open("temp-client",
                                             JackNoStartServer, 0);
    if(client) {
        rate = jack_get_sample_rate(client);
        jack_client_close(client);
    }
}
#else
void NioEngineManager::preferedSampleRate(unsigned &)
{}
#endif
