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

#include <QAbstractItemView>
#include <QDBusConnection>
#include <QDBusConnectionInterface>
#include <QDBusServiceWatcher>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QPushButton>
#include <QStyle>
#include <QDesktopWidget>
#include <QScreen>
#include <QObject>
#include <QStandardPaths>

#include <KF5/KWindowSystem/KWindowSystem>
#include <KF5/KWindowSystem/KWindowInfo>
#include <KF5/KWindowSystem/NETWM>

#include "actionsearch/actionsearch.h"

void AppMenuWidget::findAppsInside(QStringList locationsContainingApps, QMenu *m_systemMenu)
// probono: Check locationsContainingApps for applications and add them to the m_systemMenu.
// TODO: Nested submenus rather than flat ones with '→'
// This code is similar to the code in the 'launch' command
{
    QStringList nameFilter({"*.app", "*.AppDir", "*.desktop"});
    foreach (QString directory, locationsContainingApps) {
        // Shall we process this directory? Only if it contains at least one application, to optimize for speed
        // by not descending into directory trees that do not contain any applications at all. Can make
        // a big difference.

        QDir dir(directory);
        int numberOfAppsInDirectory = dir.entryList(nameFilter).length();
        QMenu *submenu;

        if(directory.endsWith(".app") == false && directory.endsWith(".AppDir") == false && numberOfAppsInDirectory > 0) {
            qDebug() << "# Descending into" << directory;
            QStringList locationsToBeChecked = {directory};
            QFileInfo fi(directory);

            QString base = fi.completeBaseName(); // baseName() gets it wrong e.g., when there are dots in version numbers
            // submenu = m_systemMenu->addMenu(base); // TODO: Use this once we have nested submenus rather than flat ones with '→'
            submenu = m_systemMenu->addMenu(directory.remove(0, 1).replace("/", " → "));
            // TODO: Make it possible to open the directory that contains the app by clicking on the submenu itself, not one of its items,
            // https://stackoverflow.com/a/3799197 (seems to require subclassing QMenu; I tend to avoid subclassing Qt)
            submenu->setToolTip(directory);
            submenu->setToolTipsVisible(true); // Seems to be needed here, too, so that the submenu items show their correct tooltips?
            connect(submenu, SIGNAL(triggered(QAction*)), SLOT(actionLaunch(QAction*)));
        } else {
            continue;
        }

        // Use QDir::entryList() insted of QDirIterator because it supports sorting
        QStringList candidates = dir.entryList();
        QString candidate;
        foreach(candidate, candidates ) {
            candidate = dir.path() + "/" + candidate;
            // qDebug() << "probono: Processing" << candidate;
            QString nameWithoutSuffix = QFileInfo(candidate).completeBaseName(); // baseName() gets it wrong e.g., when there are dots in version numbers
            QFileInfo file(candidate);
            if (file.fileName().endsWith(".app")){
                QString AppCand = candidate + "/" + nameWithoutSuffix;
                // qDebug() << "################### Checking" << AppCand;
                if(QFileInfo(AppCand).exists() == true) {
                    qDebug() << "# Found" << AppCand;
                    QFileInfo fi(file.fileName());
                    QString base = fi.baseName();  // The name of the .app directory without suffix
                    QAction *action = submenu->addAction(base);
                    action->setToolTip(file.absoluteFilePath()); // Abusing this to store the full path; FIXME (how)?
                }
            }
            else if (file.fileName().endsWith(".AppDir")) {
                QString AppCand = candidate + "/" + "AppRun";
                // qDebug() << "################### Checking" << AppCand;
                if(QFileInfo(AppCand).exists() == true){
                    qDebug() << "# Found" << AppCand;
                    QFileInfo fi(file.fileName());
                    QString base = fi.completeBaseName(); // baseName() gets it wrong e.g., when there are dots in version numbers
                    QStringList executableAndArgs = {AppCand};
                    QAction *action = submenu->addAction(base);
                    action->setToolTip(file.absoluteFilePath()); // Abusing this to store the full path; FIXME (how)?
                }
            }
            else if (file.fileName().endsWith(".desktop")) {
                // .desktop file
                qDebug() << "# Found" << file.fileName() << "TODO: Parse it for Exec=";
                QFileInfo fi(file.fileName());
                QString base = fi.completeBaseName(); // baseName() gets it wrong e.g., when there are dots in version numbers
                QStringList executableAndArgs = {fi.absoluteFilePath()};
                QAction *action = submenu->addAction(base);
                // action->setToolTip(file.absoluteFilePath()); // Abusing this to store the full path; FIXME (how)?
                action->setToolTip("TODO: Convert " + file.absoluteFilePath() + " to an .app bundle"); // Abusing this to store the full path; FIXME (how)?
                action->setDisabled(true); // As a reminder that we consider those legacy and encourage people to swtich
            }
            else if (locationsContainingApps.contains(candidate) == false && file.isDir() && candidate.endsWith("/..") == false && candidate.endsWith("/.") == false && candidate.endsWith(".app") == false && candidate.endsWith(".AppDir") == false) {
                qDebug() << "# Found" << file.fileName() << ", a directory that is not an .app bundle nor an .AppDir";
                QStringList locationsToBeChecked({candidate});
                findAppsInside(locationsToBeChecked, m_systemMenu);
            }
        }
    }
}



AppMenuWidget::AppMenuWidget(QWidget *parent)
    : QWidget(parent)
{
    // QProcess *process = new QProcess(this);
    // process->start("/usr/bin/gmenudbusmenuproxy", QStringList());

    QHBoxLayout *layout = new QHBoxLayout;
    setLayout(layout);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    m_systemMenu = new QMenu("System");
    m_systemMenu->setToolTipsVisible(true); // Works; shows the full path

    QAction *aboutAction = m_systemMenu->addAction("About This Computer");
    connect(aboutAction, SIGNAL(triggered()), this, SLOT(actionAbout()));

    // Add submenus with applications to the System menu
    QStringList locationsContainingApps = {};
    locationsContainingApps.append(QDir::homePath());
    locationsContainingApps.append(QStandardPaths::writableLocation(QStandardPaths::DownloadLocation));
    locationsContainingApps.append(QDir::homePath() + "/Applications");
    locationsContainingApps.append(QDir::homePath() + "/bin");
    locationsContainingApps.append(QDir::homePath() + "/.bin");
    locationsContainingApps.append("/Applications");
    locationsContainingApps.removeDuplicates(); // Make unique
    findAppsInside(locationsContainingApps, m_systemMenu);

    QMenu *submenuPrefs = m_systemMenu->addMenu("Preferences (deprecated)");

    QAction *displaysAction = submenuPrefs->addAction("Displays");
    connect(displaysAction, SIGNAL(triggered()), this, SLOT(actionDisplays()));

    QAction *shortcutsAction = submenuPrefs->addAction("Shortcuts");
    connect(shortcutsAction, SIGNAL(triggered()), this, SLOT(actionShortcuts()));

    QAction *soundAction = submenuPrefs->addAction("Sound");
    connect(soundAction, SIGNAL(triggered()), this, SLOT(actionSound()));

    QAction *logoutAction = m_systemMenu->addAction("Log Out");
    connect(logoutAction, SIGNAL(triggered()), this, SLOT(actionLogout()));

    m_menuBar = new QMenuBar(this);
    m_menuBar->setAttribute(Qt::WA_TranslucentBackground);
    m_menuBar->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_menuBar->setStyleSheet("background: transparent");
    m_menuBar->setFont(qApp->font());
    // layout->addWidget(m_buttonsWidget, 0, Qt::AlignVCenter);

    // Add System Menu
    integrateSystemMenu(m_menuBar);

    // Add search box to menu
    searchLineEdit = new QLineEdit(this);
    searchLineEdit->setMinimumWidth(300);
    searchLineEdit->setStyleSheet("border-radius: 9px"); // FIXME: Does not seem to work here, why?
    searchLineEdit->setWindowFlag(Qt::WindowDoesNotAcceptFocus, false);
    searchLineEdit->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    searchLineEdit->setPlaceholderText("Search in Menu");
    searchLineEdit->setStyleSheet("background: white");

    layout->addWidget(m_menuBar, 0, Qt::AlignLeft);

    layout->addSpacing(10);

    searchLineWidget = new QWidget(this);
    searchLineWidget->setWindowFlag(Qt::WindowDoesNotAcceptFocus, false);
    auto searchLineLayout = new QHBoxLayout(searchLineWidget);
    searchLineLayout->setContentsMargins(0, 0, 0, 0);
    // searchLineLayout->setSpacing(3);
    searchLineLayout->addWidget(searchLineEdit);
    searchLineWidget->setLayout(searchLineLayout);
    searchLineWidget->setObjectName("SearchLineWidget");

    layout->addWidget(searchLineWidget, 0, Qt::AlignRight);
    searchLineWidget->show();

    layout->setContentsMargins(0, 0, 0, 0);

    MenuImporter *menuImporter = new MenuImporter(this);
    menuImporter->connectToBus();

    m_appMenuModel = new AppMenuModel(this);
    connect(m_appMenuModel, &AppMenuModel::modelNeedsUpdate, this, &AppMenuWidget::updateMenu);

    connect(KWindowSystem::self(), &KWindowSystem::activeWindowChanged, this, &AppMenuWidget::delayUpdateActiveWindow);
    connect(KWindowSystem::self(), static_cast<void (KWindowSystem::*)(WId, NET::Properties, NET::Properties2)>(&KWindowSystem::windowChanged),
            this, &AppMenuWidget::onWindowChanged);

    // Load action search
    actionSearch = nullptr;
    actionCompleter = nullptr;
    updateActionSearch(m_menuBar);
}

AppMenuWidget::~AppMenuWidget() {
    if(actionSearch) {
        delete actionSearch;
    }
}

void AppMenuWidget::integrateSystemMenu(QMenuBar *menuBar) {
    if(!menuBar || !m_systemMenu)
        return;

    menuBar->addMenu(m_systemMenu);
}

void AppMenuWidget::handleActivated(const QString &name) {
    searchLineEdit->selectAll();
    searchLineEdit->clearFocus();
    searchLineEdit->clear();
    actionSearch->execute(name);
}

void AppMenuWidget::updateActionSearch(QMenuBar *menuBar) {
    if(!menuBar){
        return;
    }

    if(!actionSearch) {
        actionSearch = new ActionSearch;
    }

    /// Update the action search.
    actionSearch->clear();
    actionSearch->update(menuBar);

    /// Update completer
    if(actionCompleter) {
        actionCompleter->disconnect();
        actionCompleter->deleteLater();
    }

    actionCompleter = new QCompleter(actionSearch->getActionNames(), this);

    // Make the completer match search terms in the middle rather than just those at the beginning of the menu
    actionCompleter->setCaseSensitivity(Qt::CaseInsensitive);
    actionCompleter->setFilterMode(Qt::MatchContains);

    // Style the completer; https://stackoverflow.com/a/38084484
    QAbstractItemView *popup = actionCompleter->popup();
    // popup->setStyleSheet("QListView { padding: 10px; margin: 10px; }"); // FIXME: This is just an example so far. QAbstractItemView describes the whole QListView widget, not the lines inside it
    /* Do the individual items get created only after this, and hence not get the styling? */

    // Set first result active; https://stackoverflow.com/q/17782277. FIXME: This does not work yet. Why?
    QItemSelectionModel* sm = new QItemSelectionModel(actionCompleter->completionModel());
    actionCompleter->popup()->setSelectionModel(sm);
    sm->select(actionCompleter->completionModel()->index(0,0), QItemSelectionModel::Select);

    popup->setAlternatingRowColors(true);
    searchLineEdit->setCompleter(actionCompleter);

    connect(actionCompleter,
            QOverload<const QString &>::of(&QCompleter::activated),
            this,
            &AppMenuWidget::handleActivated);
}

void AppMenuWidget::updateMenu()
{
    // qDebug() << "AppMenuWidget::updateMenu() called";
    m_menuBar->clear();
    integrateSystemMenu(m_menuBar); // Insert the 'System' menu first

    if (!m_appMenuModel->menuAvailable()) {
        updateActionSearch(m_menuBar);
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

    updateActionSearch(m_menuBar);
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
    if (m_windowID == KWindowSystem::activeWindow())
        return;

    m_windowID = KWindowSystem::activeWindow();

    onActiveWindowChanged();
}

void AppMenuWidget::onActiveWindowChanged()
{
    KWindowInfo info(m_windowID, NET::WMState | NET::WMWindowType | NET::WMGeometry, NET::WM2TransientFor);
    // bool isMax = info.hasState(NET::Max);
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

void AppMenuWidget::actionAbout()
{
    qDebug() << "actionAbout() called";

    QString translatedTextAboutQtCaption;
    translatedTextAboutQtCaption = "<h3>About This Computer</h3>";
    QMessageBox *msgBox = new QMessageBox(this);
    msgBox->setAttribute(Qt::WA_DeleteOnClose);
    msgBox->setWindowTitle("About This Computer");

    // On FreeBSD, get information about the machine
    if(which("kenv")){
        QProcess p;
        QString program = "kenv";
        QStringList arguments;
        arguments << "-q" << "smbios.system.maker";
        p.start(program, arguments);
        p.waitForFinished();
        QString vendorname(p.readAllStandardOutput());
        vendorname.replace("\n", "");
        vendorname = vendorname.trimmed();
        qDebug() << "vendorname:" << vendorname;

        QStringList arguments2;
        arguments2 << "-q" << "smbios.system.product";
        p.start(program, arguments2);
        p.waitForFinished();
        QString productname(p.readAllStandardOutput());
        productname.replace("\n", "");
        productname = productname.trimmed();
        qDebug() << "systemname:" << productname;
        msgBox->setText("<b>" + vendorname + " " + productname + "</b>");

        QString program2 = "uname";
        QStringList arguments3;
        arguments3 << "-v";
        p.start(program2, arguments3);
        p.waitForFinished();
        QString operatingsystem(p.readAllStandardOutput());
        operatingsystem.replace("\n", "");
        operatingsystem = operatingsystem.trimmed();

        QStringList arguments4;
        arguments4 << "-K";
        p.start(program2, arguments4);
        p.waitForFinished();
        QString kernelversion(p.readAllStandardOutput());
        kernelversion.replace("\n", "");
        kernelversion = kernelversion.trimmed();
        qDebug() << "kernelversion:" << kernelversion;

        QString program3 = "sysctl";
        QStringList arguments5;
        arguments5 << "-n" << "hw.model";
        p.start(program3, arguments5);
        p.waitForFinished();
        QString cpu(p.readAllStandardOutput());
        cpu = cpu.trimmed();
        cpu = cpu.replace("(R)", "®");
        cpu = cpu.replace("(TM)", "™");
        qDebug() << "cpu:" << cpu;

        QStringList arguments6;
        arguments6 << "-n" << "hw.realmem";
        p.start(program3, arguments6);
        p.waitForFinished();
        QString memory(p.readAllStandardOutput());
        memory = memory.trimmed();
        qDebug() << "memory:" << memory;
        double m = memory.toDouble();
        m = m/1024/1024/1024;
        qDebug() << "m:" << m;

        QStringList arguments7;
        arguments7 << "-n" << "kern.disks";
        p.start(program3, arguments7);
        p.waitForFinished();
        QString disks(p.readAllStandardOutput());
        disks = disks.replace("\n", "");
        QString disk = disks.split(" ")[0];
        qDebug() << "disk:" << disk;

        QString program4 = "diskinfo";
        QStringList arguments8;
        arguments8 << "-v" << disk;
        p.start(program4, arguments8);
        p.waitForFinished();
        QString diskinfo(p.readAllStandardOutput());
        QStringList di;
        di = diskinfo.split("\n");
        double d = 0.0;
        foreach (QString ds, di) {
            if(ds.contains("mediasize in bytes")) {
                QString disksize = ds.split("\t")[1].trimmed();
                qDebug() << "disksize:" << disksize ;
                d = disksize.toDouble();
                d = d/1024/1024/1024;
                qDebug() << "d:" << d ;
            }
        }

        QString icon = "/usr/local/share/icons/elementary-xfce/devices/128/computer-hello.png";

        // If we found a way to read dmi without needing to be root, we could show a notebook icon for notebooks...
        // icon = "/usr/local/share/icons/elementary-xfce/devices/128/computer-laptop.png";

        // See https://github.com/openwebos/qt/blob/92fde5feca3d792dfd775348ca59127204ab4ac0/tools/qdbus/qdbusviewer/qdbusviewer.cpp#L477 for loading icon from resources
        msgBox->setText("<center><img src=\"file://" + icon + "\"><h3>" + vendorname + " " + productname  + "</h3>" + \
                                   "<p>" + operatingsystem +"</p><small>" + \
                                   "<p>Kernel version: " + kernelversion +"</p>" + \
                                   "<p>Processor: " + cpu +"<br>" + \
                                   "Memory: " + QString::number(m) +" GiB<br>" + \
                                   "Startup Disk: " + QString::number(d) +" GiB</p>" + \
                                   "<p><a href='file:///COPYRIGHT'>FreeBSD copyright information</a><br>" + \
                                   "Other components are subject to<br>their respective license terms</p>" + \
                                   "</small></center>");
    }

    // Center window on screen
    msgBox->setFixedWidth(350); // FIXME: Remove hardcoding; but need to be able to center on screen
    msgBox->setFixedHeight(500); // FIXME: Same
    QRect screenGeometry = QGuiApplication::screens()[0]->geometry();
    int x = (screenGeometry.width()-msgBox->geometry().width()) / 2;
    int y = (screenGeometry.height()-msgBox->geometry().height()) / 2;
    msgBox->move(x, y);

    msgBox->setStyleSheet("QWidget { padding-right: 20px }"); // FIXME: Desperate attempt to get the text really centered

    msgBox->exec();

}

void AppMenuWidget::actionLaunch(QAction *action)
{
    qDebug() << "actionLaunch(QAction *action) called";
    QStringList pathToBeLaunched = {action->toolTip()};  // Abusing this to store the full path; FIXME (how)?
    QProcess::startDetached("launch", pathToBeLaunched);
}

void AppMenuWidget::actionDisplays()
{
    qDebug() << "actionDisplays() called";
    // TODO: Find working Qt based tool
    if(which("arandr")) {
        QProcess::startDetached("arandr"); // sudo pkg install arandr // Gtk
    } else if (which("lxrandr")) {
        QProcess::startDetached("lxrandr"); // sudo pkg install lxrandr // Gtk
    } else {
        qDebug() << "arandr, lxrandr not found";
    }
}

void AppMenuWidget::actionShortcuts()
{
    qDebug() << "actionShortcuts() called";
    QProcess::startDetached("lxqt-config-globalkeyshortcuts");
}

void AppMenuWidget::actionSound()
{
    qDebug() << "actionSound() called";
    QProcess::startDetached("dsbmixer");
}

void AppMenuWidget::actionLogout()
{
    qDebug() << "actionLogout() called";
    // Check if we have the Shutdown binary at hand
    if(QFileInfo(QCoreApplication::applicationDirPath() + QString("/Shutdown")).isExecutable()) {
        QProcess::execute(QCoreApplication::applicationDirPath() + QString("/Shutdown"));
    } else {
        qDebug() << "Shutdown executable not available next to Menubar executable, exiting";
        QApplication::exit(); // In case we are lacking the Shutdown executable
    }
}

bool AppMenuWidget::which(QString command)
{
    QProcess findProcess;
    QStringList arguments;
    arguments << command;
    findProcess.start("which", arguments);
    findProcess.setReadChannel(QProcess::ProcessChannel::StandardOutput);

    if(!findProcess.waitForFinished())
        return false; // Not found or which does not work

    QString retStr(findProcess.readAll());

    retStr = retStr.trimmed();

    QFile file(retStr);
    QFileInfo check_file(file);
    if (check_file.exists() && check_file.isFile())
        return true; // Found!
    else
        return false; // Not found!
}
