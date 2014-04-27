#ifndef MIXERSINK_H
#define MIXERSINK_H

#include <QObject>
#include <QList>

class MixerSource;

class MixerSink : public QObject
{
    Q_OBJECT
public:
    MixerSink(QObject *parent = 0);
    virtual ~MixerSink();

    void AddSource(MixerSource* source);
    void RemoveSource(MixerSource* source);

    virtual QString Title() = 0;
protected:
    QList<MixerSource*> _sources;

};

#endif // MIXERSINK_H
