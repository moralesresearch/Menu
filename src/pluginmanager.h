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
    StatusBarExtension *plugin(const QString &pluginName);

private:
    QMap<QString, StatusBarExtension *> m_map;
};

#endif
