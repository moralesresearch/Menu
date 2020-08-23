#ifndef DATETIME_H
#define DATETIME_H

#include "pluginsiterface.h"
#include "controlwidget.h"
#include <QObject>

class Plugin : public QObject, StatusBarExtension
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.panda.statusbar/1.0")
    Q_INTERFACES(StatusBarExtension)

public:
    explicit Plugin(QObject *parent = nullptr) : QObject(parent) {}

    QString pluginName() override { return "controlcenter"; }
    QWidget *itemWidget() override { return new ControlWidget; }
};

#endif
