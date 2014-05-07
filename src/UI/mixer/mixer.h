#ifndef MIXER_H
#define MIXER_H

#include <pthread.h>
#include <QObject>
#include <QColor>
#include <QList>
#include "mixermaster.h"
#include "mixerchannel.h"
#include "mixerbus.h"
#include "mixerinstrument.h"
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
    QList<MixerChannel*>& Channels();

    MixerEffect* AddEffect(const QString& name);
    void RemoveEffect(MixerEffect* effect);
    int EffectIndex(MixerEffect* effect);
    QList<MixerEffect*>& Effects();

    MixerEffect* AddInsertEffect(const QString& name);
    void RemoveInsertEffect(MixerEffect* effect);
    int InsertEffectIndex(MixerEffect* effect);
    QList<MixerEffect*>& InsertEffects();

    MixerBus* GetBus(int index);

    MixerInstrument* AddInstrument(const QString& name);
    QList<MixerInstrument*>& Instruments() { return this->_instruments; }

    NioEngineManager* EngineManager() { return this->_engineManager; }
    MixerMaster* Master() { return &this->_master; }
signals:
    void ChannelAdded(MixerChannel* channel);
    void ChannelRemoved(MixerChannel* channel);
    void InsertEffectAdded(MixerEffect* effect);
    void InsertEffectRemoved(MixerEffect* effect);

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
    QList<MixerInstrument*> _instruments;
    QList<MixerChannel*> _channels;
    QList<MixerEffect*> _effects;
    QList<MixerEffect*> _insertEffects;
    MixerBus* _busses[MAX_BUS_COUNT];

    pthread_mutex_t _mutex;
    Microtonal _microtonal;
    class FFTwrapper* _fft;
    NioEngineManager* _engineManager;
};

#endif // MIXER_H
