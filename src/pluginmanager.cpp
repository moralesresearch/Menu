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
    QDir pluginsDir("/usr/lib/panda-menubar/plugins");
    const QFileInfoList files = pluginsDir.entryInfoList(QDir::Files);
    for (const QFileInfo file : files) {
      const QString filePath = file.filePath();
      if (!QLibrary::isLibrary(filePath))
          continue;

      QPluginLoader *loader = new QPluginLoader(filePath);
      MenuBarExtension *plugin = qobject_cast<MenuBarExtension *>(loader->instance());

      if (plugin) {
          qDebug() << "load " << plugin->pluginName() << " !!!";
          m_map.insert(plugin->pluginName(), plugin);
      } else {
          qDebug() << filePath << loader->errorString();
      }
    }
}

MenuBarExtension* PluginManager::plugin(const QString &pluginName)
{
    return m_map.value(pluginName, nullptr);
}
