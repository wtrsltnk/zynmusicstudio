#ifndef MIXER_H
#define MIXER_H

#include <pthread.h>
#include <QObject>
#include <QColor>
#include <QList>
#include "mixermaster.h"
#include "mixerchannel.h"
#include "mixerbus.h"
#include "../../Misc/Microtonal.h"
#include "../../Nio/NioEngineManager.h"

class Mixer : public QObject, public IMaster
{
    Q_OBJECT
    Mixer(QObject *parent = 0);
public:
#define MAX_BUS_COUNT 16
public:
    static Mixer& Instance();

    static int CurrentTick;

    MixerChannel* AddChannel(const QString& name, QColor color = Qt::blue);
    void RemoveChannel(MixerChannel* channel);
    int ChannelIndex(MixerChannel* channel);
    QList<MixerChannel*> Channels();

    MixerBus* GetBus(int index);
    Instrument* AddInstrument(const QString& name);

    NioEngineManager* EngineManager() { return this->_engineManager; }
    MixerMaster* Master() { return &this->_master; }
signals:
    void ChannelAdded(MixerChannel* channel);
    void ChannelRemoved(MixerChannel* channel);

public slots:

public: // IMaster Interface
    virtual void Lock();
    virtual void Unlock();

    virtual void AudioOut(float *outl, float *outr);

    virtual void NoteOn(char chan, char note, char velocity);
    virtual void NoteOff(char chan, char note);
    virtual void PolyphonicAftertouch(char chan, char note, char velocity);
    virtual void SetController(char chan, int type, int par);
    virtual void SetProgram(char chan, unsigned int pgm);

private:
    MixerMaster _master;
    QList<Instrument*> _instruments;
    QList<MixerChannel*> _channels;
    MixerBus* _busses[MAX_BUS_COUNT];

    pthread_mutex_t _mutex;
    Microtonal _microtonal;
    class FFTwrapper* _fft;
    NioEngineManager* _engineManager;
};

#endif // MIXER_H
