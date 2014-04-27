#ifndef MIXERMASTER_H
#define MIXERMASTER_H

#include <QObject>
#include "mixersink.h"
#include "mixerbuffer.h"
#include "../../Nio/NioEngineManager.h"

class MixerMaster : public MixerSink
{
    Q_OBJECT
public:
    explicit MixerMaster(QObject *parent = 0);

    void AudioOut(float *outl, float *outr);

    int GetVolume();
    virtual QString Title() { return "Master"; }
signals:
    void VolumeChanged(int volume);

public slots:
    void SetVolume(int volume);

private:
    MixerBuffer _buffer;
    int _volume;
    float _volumeScale;

};

#endif // MIXERMASTER_H
