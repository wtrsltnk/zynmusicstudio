#ifndef MIXERMASTER_H
#define MIXERMASTER_H

#include <QObject>
#include "./Nio/EngineMgr.h"
#include "./Nio/InMgr.h"
#include "./Nio/OutMgr.h"

class MixerMaster : public QObject
{
    Q_OBJECT
public:
    explicit MixerMaster(QObject *parent = 0);

signals:

public slots:

private:

};

#endif // MIXERMASTER_H
