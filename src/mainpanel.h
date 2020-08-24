#ifndef MAINPANEL_H
#define MAINPANEL_H

#include <QWidget>
#include <QHBoxLayout>
#include "../interfaces/pluginsiterface.h"
#include "pluginmanager.h"
#include "appmenuwidget.h"

class MainPanel : public QWidget
{
    Q_OBJECT

public:
    explicit MainPanel(QWidget *parent = nullptr);

    void loadModules();
    void loadModule(const QString &pluginName, QHBoxLayout *layout);

protected:
    void mouseDoubleClickEvent(QMouseEvent *e) override;

private:
    QHBoxLayout *m_globalMenuLayout;
    QHBoxLayout *m_statusnotifierLayout;
    QHBoxLayout *m_controlCenterLayout;
    QHBoxLayout *m_dateTimeLayout;
    AppMenuWidget *m_appMenuWidget;

    PluginManager *m_pluginManager;
};

#endif // MAINPANEL_H
