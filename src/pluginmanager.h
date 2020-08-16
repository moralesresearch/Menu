#ifndef PLUGINMANAGER_H
#define PLUGINMANAGER_H

#include <QObject>
#include <QMap>
#include "../interfaces/pluginsiterface.h"

class PluginManager : public QObject
{
    Q_OBJECT

public:
    explicit PluginManager(QObject *parent = 0);

    void start();
    MenuBarExtension *plugin(const QString &pluginName);

private:
    QMap<QString, MenuBarExtension *> m_map;
};

#endif
