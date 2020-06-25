#ifndef APPMENUWIDGET_H
#define APPMENUWIDGET_H

#include <QWidget>
#include <QMenuBar>
#include <QToolButton>
#include <QPropertyAnimation>
#include "appmenu/appmenumodel.h"
#include "appmenu/menuimporter.h"

class AppMenuWidget : public QWidget
{
    Q_OBJECT

public:
    explicit AppMenuWidget(QWidget *parent = nullptr);

    void updateMenu();
    void toggleMaximizeWindow();

private:
    void delayUpdateActiveWindow();
    void onActiveWindowChanged();
    void onWindowChanged(WId id, NET::Properties properties, NET::Properties2 properties2);
    void minimizeWindow();
    void clsoeWindow();
    void maxmizeWindow();
    void restoreWindow();

private:
    AppMenuModel *m_appMenuModel;
    MenuImporter *m_menuImporter;
    QMenuBar *m_menuBar;
    QWidget *m_buttonsWidget;
    QToolButton *m_minButton;
    QToolButton *m_restoreButton;
    QToolButton *m_closeButton;
    QPropertyAnimation *m_buttonsAnimation;
    WId m_windowID;

    int m_buttonsWidth;
};

#endif // APPMENUWIDGET_H
