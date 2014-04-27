#ifndef MIXERGENERATOR_H
#define MIXERGENERATOR_H

#include <QObject>

class MixerSource;

class MixerGenerator : public QObject
{
    Q_OBJECT
public:
    MixerGenerator(QObject* parent = 0);
    virtual ~MixerGenerator();

    virtual MixerSource* Source() { return this->_source; }

signals:
    void SourceChanged(MixerSource* source);

public slots:
    virtual void SetSource(MixerSource* source);

protected:
    MixerSource* _source;
};

#endif // MIXERGENERATOR_H
