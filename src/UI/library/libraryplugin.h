#ifndef LIBRARYPLUGIN_H
#define LIBRARYPLUGIN_H

#include <QObject>

typedef struct {
    char id[4];
} LibraryPluginID;

namespace LibraryPluginType
{
enum eType
{
    NoType,
    Synthesizer,
    Effect,

    eTypeCount    // This value always last
};
}

class MixerChannelInput;
class MixerEffect;

class LibraryPlugin : public QObject
{
    Q_OBJECT

public:
    LibraryPlugin(QObject* parent = 0);
    virtual ~LibraryPlugin();

    virtual const LibraryPluginID PluginID() = 0;

    virtual MixerChannelInput* CreateChannelInput() { return 0; }
    virtual void DestroyChannelInput(MixerChannelInput* channelinput) = 0;

    virtual MixerEffect* CreateEffect() { return 0; }
    virtual void DestroyEffect(MixerEffect* effectg) = 0;

    virtual QWidget* CreateQtInterface() = 0;

    LibraryPluginType::eType Type() { return this->_type; }

public slots:
    void SetType(LibraryPluginType::eType type);

private:
    LibraryPluginType::eType _type;

};

#endif // LIBRARYPLUGIN_H
