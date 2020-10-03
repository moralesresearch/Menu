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

#include "mainpanel.h"
#include "extensionwidget.h"

#include "actionsearch/actionsearch.h"
#include "actionsearch/ui/dialog.h"

#include <QMouseEvent>
#include <QHBoxLayout>
#include <QLabel>
#include <QDebug>
#include <QStyle>
#include <QApplication>
#include <QProcess>
#include <QFileInfo>
#include <QPainter>
#include <QMessageBox>
#include <QDialog>

MainPanel::MainPanel(QWidget *parent)
    : QWidget(parent),
      m_globalMenuLayout(new QHBoxLayout),
      m_statusnotifierLayout(new QHBoxLayout),
      m_controlCenterLayout(new QHBoxLayout),
      m_dateTimeLayout(new QHBoxLayout),
      m_appMenuWidget(new AppMenuWidget),
      m_pluginManager(new PluginManager(this))
{
    m_pluginManager->start();

    QWidget *statusnotifierWidget = new QWidget;
    statusnotifierWidget->setLayout(m_statusnotifierLayout);
    statusnotifierWidget->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
    m_statusnotifierLayout->setMargin(0);

    QWidget *dateTimeWidget = new QWidget;
    dateTimeWidget->setLayout(m_dateTimeLayout);
    dateTimeWidget->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
    m_dateTimeLayout->setMargin(0);

    m_controlCenterLayout->setSpacing(10);

    // Second QMenuBar for the leftmost menu
    // so that it does not interfere with the main menu
    QMenuBar *leftmostMenuBar = new QMenuBar(nullptr);
    leftmostMenuBar->setMaximumWidth(70);
    QMenu *leftmostMenu = leftmostMenuBar->addMenu("System");
    // leftmostMenu->setIcon(this->style()->standardIcon(QStyle::SP_ComputerIcon));
    QAction *aboutAction = leftmostMenu->addAction("About This Computer");
    connect(aboutAction, SIGNAL(triggered()), this, SLOT(actionAbout()));
    QAction *logoutAction = leftmostMenu->addAction("Log Out");
    connect(logoutAction, SIGNAL(triggered()), this, SLOT(actionLogout()));

    QHBoxLayout *layout = new QHBoxLayout;
    layout->setSpacing(0);
    layout->addSpacing(10);
    layout->addWidget(leftmostMenuBar);
    layout->addWidget(m_appMenuWidget);
    layout->addStretch();
    layout->addWidget(statusnotifierWidget);
    layout->addSpacing(10);
    layout->addLayout(m_controlCenterLayout);
    layout->addSpacing(10);
    layout->addWidget(dateTimeWidget);
    layout->addSpacing(10);
    layout->setContentsMargins(0, 0, 0, 0);
    setLayout(layout);

    loadModules();

    // Load action search
    ActionSearch actionSearch{leftmostMenuBar};
    actionSearch.update();
    auto dialog = new Dialog{NULL, actionSearch.getActionNames()};
    connect(dialog, &Dialog::accepted, [&actionSearch, &dialog]() {
        actionSearch.execute(dialog->getActionName());
    });
    // dialog->setModal(true);
    dialog->setModal(false);
    dialog->show();
}

void MainPanel::loadModules()
{
    loadModule("datetime", m_dateTimeLayout);
    loadModule("statusnotifier", m_statusnotifierLayout);
    loadModule("volume", m_controlCenterLayout);
    loadModule("battery", m_controlCenterLayout);
}

void MainPanel::loadModule(const QString &pluginName, QHBoxLayout *layout)
{
    ExtensionWidget *extensionWidget = m_pluginManager->plugin(pluginName);
    if (extensionWidget) {
        // extensionWidget->setFixedHeight(rect().height());
        layout->addWidget(extensionWidget);
    }
}

void MainPanel::mouseDoubleClickEvent(QMouseEvent *e)
{
    QWidget::mouseDoubleClickEvent(e);

    // if (e->button() == Qt::LeftButton)
    //     m_appMenuWidget->toggleMaximizeWindow();
}

void MainPanel::actionAbout()
{
    qDebug() << "actionAbout() called";

    QString translatedTextAboutQtCaption;
    translatedTextAboutQtCaption = "<h3>About This Computer</h3>";
    QString translatedTextAboutQtText;
    translatedTextAboutQtText = "<p>A friendly, welcoming desktop. Less, but better.</p>";
    QMessageBox *msgBox = new QMessageBox(QMessageBox::NoIcon, "Title", "Text");
    msgBox->setAttribute(Qt::WA_DeleteOnClose);
    msgBox->setWindowTitle("About This Computer");
    msgBox->setText(translatedTextAboutQtCaption);
    msgBox->setInformativeText(translatedTextAboutQtText);

    QPixmap pm(QLatin1String(":/qt-project.org/qmessagebox/images/qtlogo-64.png"));
    if (!pm.isNull())
        msgBox->setIconPixmap(pm);
    msgBox->exec();
}

void MainPanel::actionLogout()
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
