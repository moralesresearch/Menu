#ifndef DATETIME_H
#define DATETIME_H

#include "pluginsiterface.h"
#include "datetimewidget.h"
#include <QObject>

class DateTime : public QObject, MenuBarExtension
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.panda.menubar/1.0")
    Q_INTERFACES(MenuBarExtension)

public:
    explicit DateTime(QObject *parent = nullptr);

    QString pluginName() override { return "datetime"; }
    QString displayName() override { return "DateTime"; }
    QWidget *itemWidget() override { return new DateTimeWidget; }
};

#endif
