/*
 * Copyright (C) 2020 PandaOS Team.
 *
 * Author:     rekols <revenmartin@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "appmenuwidget.h"
#include "appmenu/menuimporteradaptor.h"
#include <QProcess>
#include <QHBoxLayout>
#include <QDebug>
#include <QMenu>
#include <QX11Info>
#include <QApplication>

#include <QDBusConnection>
#include <QDBusConnectionInterface>
#include <QDBusServiceWatcher>

#include <KF5/KWindowSystem/KWindowSystem>
#include <KF5/KWindowSystem/KWindowInfo>
#include <KF5/KWindowSystem/NETWM>

#include "actionsearch/actionsearch.h"
#include "actionsearch/ui/dialog.h"

AppMenuWidget::AppMenuWidget(QWidget *parent)
    : QWidget(parent)
      // m_minButton(new QToolButton),
      // m_restoreButton(new QToolButton),
      // m_closeButton(new QToolButton)
      //m_buttonsAnimation(new QPropertyAnimation)
{
    // QProcess *process = new QProcess(this);
    // process->start("/usr/bin/gmenudbusmenuproxy", QStringList());

    QHBoxLayout *layout = new QHBoxLayout;
    setLayout(layout);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    // m_buttonsWidget = new QWidget(this);
    // QHBoxLayout *buttonsLayout = new QHBoxLayout(m_buttonsWidget);
    // buttonsLayout->setContentsMargins(0, 0, 0, 0);
    // buttonsLayout->setSpacing(3);
    // buttonsLayout->addWidget(m_closeButton);
    // buttonsLayout->addWidget(m_restoreButton);
    // buttonsLayout->addWidget(m_minButton);

    // QSize iconSize(32, 32);
    // m_minButton->setIcon(QIcon(":/resources/min.svg"));
    // m_restoreButton->setIcon(QIcon(":/resources/restore.svg"));
    // m_closeButton->setIcon(QIcon(":/resources/close.svg"));
    // m_minButton->setIconSize(iconSize);
    // m_restoreButton->setIconSize(iconSize);
    // m_closeButton->setIconSize(iconSize);

    m_menuBar = new QMenuBar(this);
    m_menuBar->setAttribute(Qt::WA_TranslucentBackground);
    m_menuBar->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_menuBar->setStyleSheet("background: transparent");
    m_menuBar->setFont(qApp->font());
    // layout->addWidget(m_buttonsWidget, 0, Qt::AlignVCenter);
    layout->addWidget(m_menuBar, 0, Qt::AlignVCenter);
    layout->setContentsMargins(0, 0, 0, 0);

//    m_buttonsAnimation->setTargetObject(m_buttonsWidget);
//    m_buttonsAnimation->setPropertyName("maximumWidth");
//    m_buttonsAnimation->setDuration(300);
//    m_buttonsWidth = m_buttonsWidget->width();

    MenuImporter *menuImporter = new MenuImporter(this);
    menuImporter->connectToBus();

    m_appMenuModel = new AppMenuModel(this);
    connect(m_appMenuModel, &AppMenuModel::modelNeedsUpdate, this, &AppMenuWidget::updateMenu);
    // connect(KWindowSystem::self(), &KWindowSystem::activeWindowChanged, this, &AppMenuWidget::delayUpdateActiveWindow);
    // connect(KWindowSystem::self(), static_cast<void (KWindowSystem::*)(WId, NET::Properties, NET::Properties2)>(&KWindowSystem::windowChanged),
    //         this, &AppMenuWidget::onWindowChanged);

    // connect(m_minButton, &QToolButton::clicked, this, &AppMenuWidget::minimizeWindow);
    // connect(m_closeButton, &QToolButton::clicked, this, &AppMenuWidget::clsoeWindow);
    // connect(m_restoreButton, &QToolButton::clicked, this, &AppMenuWidget::restoreWindow);

    delayUpdateActiveWindow();

    // Load action search
    ActionSearch actionSearch{m_menuBar};
    actionSearch.update();
    auto dialog = new Dialog{this, actionSearch.getActionNames()};
    connect(dialog, &Dialog::accepted, [&actionSearch, &dialog]() {
        actionSearch.execute(dialog->getActionName());
    });
    // dialog->setModal(true);
    dialog->setModal(false);
    dialog->show();

}

void AppMenuWidget::updateMenu()
{
    m_menuBar->clear();

    if (!m_appMenuModel->menuAvailable()) {
        return;
    }

    QMenu *menu = m_appMenuModel->menu();
    if (menu) {
        for (QAction *a : menu->actions()) {
            if (!a->isEnabled())
                continue;

            m_menuBar->addAction(a);
        }
    }

}

void AppMenuWidget::toggleMaximizeWindow()
{
    KWindowInfo info(KWindowSystem::activeWindow(), NET::WMState);
    bool isMax = info.hasState(NET::Max);
    bool isWindow = !info.hasState(NET::SkipTaskbar) ||
            info.windowType(NET::UtilityMask) != NET::Utility ||
            info.windowType(NET::DesktopMask) != NET::Desktop;

    if (!isWindow)
        return;

    if (isMax) {
        restoreWindow();
    } else {
        maxmizeWindow();
    }
}

bool AppMenuWidget::event(QEvent *e)
{
    if (e->type() == QEvent::ApplicationFontChange) {
        QMenu *menu = m_appMenuModel->menu();
        if (menu) {
            for (QAction *a : menu->actions()) {
                a->setFont(qApp->font());
            }
        }
        qDebug() << "gengxinle  !!!" << qApp->font().toString();
        m_menuBar->setFont(qApp->font());
        m_menuBar->update();
    }

    return QWidget::event(e);
}

bool AppMenuWidget::isAcceptWindow(WId id)
{
    QFlags<NET::WindowTypeMask> ignoreList;
    ignoreList |= NET::DesktopMask;
    ignoreList |= NET::DockMask;
    ignoreList |= NET::SplashMask;
    ignoreList |= NET::ToolbarMask;
    ignoreList |= NET::MenuMask;
    ignoreList |= NET::PopupMenuMask;
    ignoreList |= NET::NotificationMask;

    KWindowInfo info(id, NET::WMWindowType | NET::WMState, NET::WM2TransientFor | NET::WM2WindowClass);

    if (!info.valid())
           return false;

   if (NET::typeMatchesMask(info.windowType(NET::AllTypesMask), ignoreList))
       return false;

   if (info.state() & NET::SkipTaskbar)
       return false;

   // WM_TRANSIENT_FOR hint not set - normal window
   WId transFor = info.transientFor();
   if (transFor == 0 || transFor == id || transFor == (WId) QX11Info::appRootWindow())
       return true;

   info = KWindowInfo(transFor, NET::WMWindowType);

   QFlags<NET::WindowTypeMask> normalFlag;
   normalFlag |= NET::NormalMask;
   normalFlag |= NET::DialogMask;
   normalFlag |= NET::UtilityMask;

   return !NET::typeMatchesMask(info.windowType(NET::AllTypesMask), normalFlag);
}

void AppMenuWidget::delayUpdateActiveWindow()
{
    // if (m_windowID == KWindowSystem::activeWindow())
    //     return;

    // m_windowID = KWindowSystem::activeWindow();

    // onActiveWindowChanged();
}

void AppMenuWidget::onActiveWindowChanged()
{
    KWindowInfo info(m_windowID, NET::WMState | NET::WMWindowType | NET::WMGeometry, NET::WM2TransientFor);
    bool isMax = info.hasState(NET::Max);

//    m_buttonsAnimation->stop();

    if (isAcceptWindow(m_windowID) && isMax) {
//        m_buttonsAnimation->setStartValue(m_buttonsWidget->width());
//        m_buttonsAnimation->setEndValue(m_buttonsWidth);
//        m_buttonsAnimation->setEasingCurve(QEasingCurve::InOutCubic);
//        m_buttonsAnimation->start();
        m_buttonsWidget->setVisible(true);
    } else {
//        m_buttonsAnimation->setStartValue(m_buttonsWidget->width());
//        m_buttonsAnimation->setEndValue(0);
//        m_buttonsAnimation->setEasingCurve(QEasingCurve::InOutCubic);
//        m_buttonsAnimation->start();
        m_buttonsWidget->setVisible(false);
    }
}

void AppMenuWidget::onWindowChanged(WId /*id*/, NET::Properties /*properties*/, NET::Properties2 /*properties2*/)
{
    if (m_windowID == KWindowSystem::activeWindow())
        onActiveWindowChanged();
}

void AppMenuWidget::minimizeWindow()
{
    KWindowSystem::minimizeWindow(KWindowSystem::activeWindow());
}

void AppMenuWidget::clsoeWindow()
{
    NETRootInfo(QX11Info::connection(), NET::CloseWindow).closeWindowRequest(KWindowSystem::activeWindow());
}

void AppMenuWidget::maxmizeWindow()
{
    KWindowSystem::setState(KWindowSystem::activeWindow(), NET::Max);
}

void AppMenuWidget::restoreWindow()
{
    KWindowSystem::clearState(KWindowSystem::activeWindow(), NET::Max);
}
