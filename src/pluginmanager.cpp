#include "pluginmanager.h"
#include <QDir>
#include <QPluginLoader>
#include <QDebug>

PluginManager::PluginManager(QObject *parent)
  : QObject(parent)
{

}

void PluginManager::start()
{
    QDir pluginsDir("/usr/lib/panda-statusbar/plugins");
    const QFileInfoList files = pluginsDir.entryInfoList(QDir::Files);
    for (const QFileInfo file : files) {
        const QString filePath = file.filePath();
        if (!QLibrary::isLibrary(filePath))
            continue;

        QPluginLoader *loader = new QPluginLoader(filePath);
        StatusBarExtension *plugin = qobject_cast<StatusBarExtension *>(loader->instance());

        if (plugin) {
            qDebug() << "loaded " << plugin->pluginName() << " !!!";
            ExtensionWidget *widget = new ExtensionWidget(plugin);
            m_extensions.insert(plugin->pluginName(), widget);
        } else {
            qDebug() << filePath << loader->errorString();
        }
    }
}

ExtensionWidget* PluginManager::plugin(const QString &pluginName)
{
    return m_extensions.value(pluginName, nullptr);
}
