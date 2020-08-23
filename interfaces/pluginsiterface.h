#ifndef PLUGINSINTERFACE_H
#define PLUGINSINTERFACE_H

#include <QtPlugin>

class StatusBarExtension
{
public:
    virtual ~StatusBarExtension() {}

    virtual QString pluginName() = 0;
    virtual QString popupText() { return QString(); }
    virtual QWidget *itemWidget() = 0;
    virtual QWidget *popupWindow() { return nullptr; }
};

Q_DECLARE_INTERFACE(StatusBarExtension, "org.panda.statusbar/1.0")

#endif
