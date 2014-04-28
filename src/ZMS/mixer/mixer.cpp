#include "mixer.h"
#include "../../DSP/FFTwrapper.h"
#include "../../globals.h"

int Mixer::CurrentTick = 0;

Mixer::Mixer(QObject *parent) :
    QObject(parent), _engineManager(new NioEngineManager(this))
{
    for (int i = 0; i < MAX_BUS_COUNT; i++)
        this->_busses[i] = 0;

    pthread_mutex_init(&this->_mutex, NULL);
    this->_fft = new FFTwrapper(synth->oscilsize);

    this->_microtonal.defaults();
}

Mixer& Mixer::Instance()
{
    static Mixer instance;

    return instance;
}

MixerChannel* Mixer::AddChannel(const QString& name, QColor color)
{
    MixerChannel* channel = new MixerChannel();

    channel->SetName(name);
    channel->SetColor(color);
    this->_channels.push_back(channel);

    emit ChannelAdded(channel);

    return channel;
}

void Mixer::RemoveChannel(MixerChannel* channel)
{
    if (this->_channels.removeOne(channel))
        emit ChannelRemoved(channel);
}

int Mixer::ChannelIndex(MixerChannel* channel)
{
    return this->_channels.indexOf(channel);
}

QList<MixerChannel*>& Mixer::Channels()
{
    return this->_channels;
}

MixerBus* Mixer::GetBus(int index)
{
    if (this->_busses[index] == 0)
        this->_busses[index] = new MixerBus();

    return this->_busses[index];
}

MixerInstrument* Mixer::AddInstrument(const QString& name)
{
    this->Lock();

    MixerInstrument* instrument = new MixerInstrument(new Instrument(&this->_microtonal, this->_fft, &this->_mutex));

    instrument->GetInstrument()->Pname = name.toStdString();
    this->_instruments.push_back(instrument);

    this->Unlock();

    return instrument;
}

void Mixer::Lock()
{
    pthread_mutex_lock(&this->_mutex);
}

void Mixer::Unlock()
{
    pthread_mutex_unlock(&this->_mutex);
}

void Mixer::AudioOut(float *outl, float *outr)
{
    // We start a new tick
    Mixer::CurrentTick++;

    // First let all instruments compute the samples
    for (QList<MixerInstrument*>::iterator itr = this->_instruments.begin(); itr != this->_instruments.end(); ++itr)
        (*itr)->ComputeSamples();

    // Next ask MixerMaster for its samples
    this->_master.AudioOut(outl, outr);
}

void Mixer::NoteOn(char chan, char note, char velocity)
{
    for (QList<MixerInstrument*>::iterator itr = this->_instruments.begin(); itr != this->_instruments.end(); ++itr)
        (*itr)->NoteOn(chan, note, velocity);
}

void Mixer::NoteOff(char chan, char note)
{
    for (QList<MixerInstrument*>::iterator itr = this->_instruments.begin(); itr != this->_instruments.end(); ++itr)
        (*itr)->NoteOff(chan, note);
}

void Mixer::PolyphonicAftertouch(char chan, char note, char velocity)
{
    for (QList<MixerInstrument*>::iterator itr = this->_instruments.begin(); itr != this->_instruments.end(); ++itr)
        (*itr)->PolyphonicAftertouch(chan, note, velocity);
}

void Mixer::SetController(char chan, int type, int par)
{
    for (QList<MixerInstrument*>::iterator itr = this->_instruments.begin(); itr != this->_instruments.end(); ++itr)
        (*itr)->SetController(chan, type, par);
}

void Mixer::SetProgram(char chan, unsigned int pgm)
{ }
