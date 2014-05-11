#include "libraryeqeffectplugin.h"
#include "library.h"
#include "libraryeqeffect.h"

LibraryEQEffectPlugin::LibraryEQEffectPlugin(QObject *parent)
    : LibraryPlugin(parent)
{ }

LibraryEQEffectPlugin::~LibraryEQEffectPlugin()
{ }

const LibraryPluginID LibraryEQEffectPlugin::PluginID()
{
    static LibraryPluginID id = { 'E', 'Q', '0', '0' };
    return id;
}

MixerEffect* LibraryEQEffectPlugin::CreateEffect()
{
    return new LibraryEQEffect(this);
}

void LibraryEQEffectPlugin::DestroyEffect(MixerEffect* effectg)
{

}

QWidget* LibraryEQEffectPlugin::CreateQtInterface()
{
    return 0;
}
