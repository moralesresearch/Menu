#ifndef PLUGINMANAGER_H
#define PLUGINMANAGER_H

#include <QObject>
#include <QMap>
#include "extensionwidget.h"
#include "../interfaces/pluginsiterface.h"

class PluginManager : public QObject
{
    Q_OBJECT

public:
    explicit PluginManager(QObject *parent = 0);

    void start();
    ExtensionWidget *plugin(const QString &pluginName);

private:
    QMap<QString, ExtensionWidget *> m_extensions;
};

#endif
