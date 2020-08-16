#ifndef DATETIME_H
#define DATETIME_H

#include "pluginsiterface.h"
#include "controlwidget.h"
#include <QObject>

class Plugin : public QObject, MenuBarExtension
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.panda.menubar/1.0")
    Q_INTERFACES(MenuBarExtension)

public:
    explicit Plugin(QObject *parent = nullptr) : QObject(parent) {}

    QString pluginName() override { return "controlcenter"; }
    QString displayName() override { return "ControlCenter"; }
    QWidget *itemWidget() override { return new ControlWidget; }
};

#endif
