#ifndef MIXERMASTER_H
#define MIXERMASTER_H

#include <QObject>
#include <QList>
#include "mixerbuffer.h"
#include "../../Nio/NioEngineManager.h"

class MixerChannel;

class MixerMaster : public QObject
{
    Q_OBJECT
public:
    MixerMaster(QObject *parent = 0);
    virtual ~MixerMaster();

    void AudioOut(float *outl, float *outr);

    int GetVolume();
    virtual QString Title() { return "Master"; }

    void AddSource(MixerChannel* source);
    void RemoveSource(MixerChannel* source);

signals:
    void VolumeChanged(int volume);

public slots:
    void SetVolume(int volume);

private:
    MixerBuffer _buffer;
    int _volume;
    float _volumeScale;

    QList<MixerChannel*> _sources;

};

#endif // MIXERMASTER_H
