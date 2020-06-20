#include "appmenuwidget.h"
#include "appmenu/menuimporteradaptor.h"
#include <QHBoxLayout>
#include <QDebug>
#include <QMenu>

#include <QDBusConnection>
#include <QDBusConnectionInterface>
#include <QDBusServiceWatcher>

#include <KWindowSystem>

AppMenuWidget::AppMenuWidget(QWidget *parent)
    : QWidget(parent)
{
    QHBoxLayout *layout = new QHBoxLayout;
    setLayout(layout);

    m_menuBar = new QMenuBar(this);
    m_menuBar->setAttribute(Qt::WA_TranslucentBackground);
    m_menuBar->setStyleSheet("background: transparent");
    layout->addWidget(m_menuBar, 0, Qt::AlignVCenter);
    layout->setContentsMargins(0, 0, 0, 0);

    MenuImporter *menuImporter = new MenuImporter(this);
    menuImporter->connectToBus();

    m_appMenuModel = new AppMenuModel(this);

    connect(m_appMenuModel, &AppMenuModel::modelNeedsUpdate, this, &AppMenuWidget::updateMenu);
}

void AppMenuWidget::updateMenu()
{
    if (!m_appMenuModel->menuAvailable()) {
        m_menuBar->clear();
        m_menuBar->setVisible(false);
        return;
    }

    QMenu *menu = m_appMenuModel->menu();
    if (menu) {
        for (QAction *a : menu->actions()) {
            m_menuBar->addAction(a);
        }
        m_menuBar->setVisible(true);
    }
}
