#ifndef LIBRARYEQEFFECTPLUGIN_H
#define LIBRARYEQEFFECTPLUGIN_H

#include "libraryplugin.h"

class LibraryEQEffectPlugin : public LibraryPlugin
{
    Q_OBJECT
public:
    LibraryEQEffectPlugin(QObject *parent = 0);
    virtual ~LibraryEQEffectPlugin();

    virtual const LibraryPluginID PluginID();

    virtual void DestroyChannelInput(MixerChannelInput* channelinput) { }

    virtual MixerEffect* CreateEffect();
    virtual void DestroyEffect(MixerEffect* effectg);

    virtual QWidget* CreateQtInterface();

signals:

public slots:

};

#endif // LIBRARYEQEFFECTPLUGIN_H
