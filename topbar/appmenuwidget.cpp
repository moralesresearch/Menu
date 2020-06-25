#include "appmenuwidget.h"
#include "appmenu/menuimporteradaptor.h"
#include <QHBoxLayout>
#include <QDebug>
#include <QMenu>
#include <QX11Info>

#include <QDBusConnection>
#include <QDBusConnectionInterface>
#include <QDBusServiceWatcher>

#include <KF5/KWindowSystem/KWindowSystem>
#include <KF5/KWindowSystem/KWindowInfo>
#include <KF5/KWindowSystem/NETWM>

AppMenuWidget::AppMenuWidget(QWidget *parent)
    : QWidget(parent),
      m_minButton(new QToolButton),
      m_restoreButton(new QToolButton),
      m_closeButton(new QToolButton),
      m_buttonsAnimation(new QPropertyAnimation)
{
    QHBoxLayout *layout = new QHBoxLayout;
    setLayout(layout);

    m_buttonsWidget = new QWidget(this);
    QHBoxLayout *buttonsLayout = new QHBoxLayout(m_buttonsWidget);
    buttonsLayout->setContentsMargins(0, 0, 0, 0);
    buttonsLayout->setSpacing(3);
    buttonsLayout->addWidget(m_closeButton);
    buttonsLayout->addWidget(m_restoreButton);
    buttonsLayout->addWidget(m_minButton);

    QSize iconSize(32, 32);
    m_minButton->setIcon(QIcon(":/resources/min.svg"));
    m_restoreButton->setIcon(QIcon(":/resources/restore.svg"));
    m_closeButton->setIcon(QIcon(":/resources/close.svg"));
    m_minButton->setIconSize(iconSize);
    m_restoreButton->setIconSize(iconSize);
    m_closeButton->setIconSize(iconSize);

    m_menuBar = new QMenuBar(this);
    m_menuBar->setAttribute(Qt::WA_TranslucentBackground);
    m_menuBar->setStyleSheet("background: transparent");
    layout->addWidget(m_buttonsWidget, 0, Qt::AlignVCenter);
    layout->addWidget(m_menuBar, 0, Qt::AlignVCenter);
    layout->setContentsMargins(0, 0, 0, 0);

    m_buttonsAnimation->setTargetObject(m_buttonsWidget);
    m_buttonsAnimation->setPropertyName("maximumWidth");
    m_buttonsAnimation->setDuration(300);
    m_buttonsWidth = m_buttonsWidget->width();

    MenuImporter *menuImporter = new MenuImporter(this);
    menuImporter->connectToBus();

    m_appMenuModel = new AppMenuModel(this);
    connect(m_appMenuModel, &AppMenuModel::modelNeedsUpdate, this, &AppMenuWidget::updateMenu);
    connect(KWindowSystem::self(), &KWindowSystem::activeWindowChanged, this, &AppMenuWidget::delayUpdateActiveWindow);
    connect(KWindowSystem::self(), static_cast<void (KWindowSystem::*)(WId, NET::Properties, NET::Properties2)>(&KWindowSystem::windowChanged),
            this, &AppMenuWidget::onWindowChanged);

    connect(m_minButton, &QToolButton::clicked, this, &AppMenuWidget::minimizeWindow);
    connect(m_closeButton, &QToolButton::clicked, this, &AppMenuWidget::clsoeWindow);
    connect(m_restoreButton, &QToolButton::clicked, this, &AppMenuWidget::restoreWindow);

    delayUpdateActiveWindow();

    setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
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

void AppMenuWidget::delayUpdateActiveWindow()
{
    if (m_windowID == KWindowSystem::activeWindow())
        return;

    m_windowID = KWindowSystem::activeWindow();
    onActiveWindowChanged();
}

void AppMenuWidget::onActiveWindowChanged()
{
    KWindowInfo info(m_windowID, NET::WMState | NET::WMWindowType | NET::WMGeometry, NET::WM2TransientFor);
    bool isMax = info.hasState(NET::Max);
    bool isWindow = !info.hasState(NET::SkipTaskbar) ||
            info.windowType(NET::UtilityMask) != NET::Utility ||
            info.windowType(NET::DesktopMask) != NET::Desktop;

    m_buttonsAnimation->stop();

    if (isWindow && isMax) {
        m_buttonsAnimation->setStartValue(m_buttonsWidget->width());
        m_buttonsAnimation->setEndValue(m_buttonsWidth);
        m_buttonsAnimation->setEasingCurve(QEasingCurve::InOutCubic);
        m_buttonsAnimation->start();
    } else {
        m_buttonsAnimation->setStartValue(m_buttonsWidget->width());
        m_buttonsAnimation->setEndValue(0);
        m_buttonsAnimation->setEasingCurve(QEasingCurve::InOutCubic);
        m_buttonsAnimation->start();
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
