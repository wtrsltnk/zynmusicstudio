#ifndef MIXERSINK_H
#define MIXERSINK_H

#include <QList>

class MixerSource;

class MixerSink
{
public:
    MixerSink();
    virtual ~MixerSink();

    void AddSource(MixerSource* source);
    void RemoveSource(MixerSource* source);

protected:
    QList<MixerSource*> _sources;

};

#endif // MIXERSINK_H
