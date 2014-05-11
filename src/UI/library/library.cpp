#include "library.h"

Library::Library(QObject *parent)
    : QObject(parent)
{ }

Library& Library::Instance()
{
    static Library library;
    return library;
}

Library::~Library()
{ }

void Library::AddPlugin(LibraryPlugin* plugin)
{
    this->_plugins.push_back(plugin);
}

void Library::RemovePlugin(LibraryPlugin* plugin)
{
    if (this->_plugins.contains(plugin))
        this->_plugins.removeOne(plugin);
}
