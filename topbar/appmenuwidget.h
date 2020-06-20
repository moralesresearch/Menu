#ifndef APPMENUWIDGET_H
#define APPMENUWIDGET_H

#include <QWidget>
#include <QMenuBar>
#include "appmenu/appmenumodel.h"
#include "appmenu/menuimporter.h"

class AppMenuWidget : public QWidget
{
    Q_OBJECT

public:
    explicit AppMenuWidget(QWidget *parent = nullptr);

    void updateMenu();

private:
    AppMenuModel *m_appMenuModel;
    MenuImporter *m_menuImporter;
    QMenuBar *m_menuBar;
};

#endif // APPMENUWIDGET_H
