#ifndef LIBRARY_H
#define LIBRARY_H

#include <QObject>
#include <QList>
#include "libraryplugin.h"

class Library : public QObject
{
    Q_OBJECT
    Library(QObject *parent = 0);
public:
    static Library& Instance();
    virtual ~Library();

    void AddPlugin(LibraryPlugin* plugin);
    void RemovePlugin(LibraryPlugin* plugin);

signals:

public slots:

private:
    QList<LibraryPlugin*> _plugins;
};

#endif // LIBRARY_H
