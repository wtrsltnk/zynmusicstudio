#include "libraryplugin.h"

LibraryPlugin::LibraryPlugin(QObject* parent)
    : QObject(parent), _type(LibraryPluginType::NoType)
{ }

LibraryPlugin::~LibraryPlugin()
{ }

void LibraryPlugin::SetType(LibraryPluginType::eType type)
{
    this->_type = type;
}
