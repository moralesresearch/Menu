#ifndef PLUGINSINTERFACE_H
#define PLUGINSINTERFACE_H

#include <QtPlugin>

class MenuBarExtension
{
public:
    virtual ~MenuBarExtension() {}

    virtual QString pluginName() = 0;
    virtual QString displayName() { return QString(); }
    virtual QWidget *itemWidget() = 0;
};

Q_DECLARE_INTERFACE(MenuBarExtension, "org.panda.menubar/1.0")

#endif
