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

#include "mainwindow.h"
#include <QApplication>
#include <QHBoxLayout>
#include <QScreen>
#include <QPainter>

#include <KF5/KWindowSystem/KWindowSystem>
#include <KF5/KWindowSystem/KWindowEffects>

#define TOPBAR_HEIGHT 21

MainWindow::MainWindow(QWidget *parent)
    : QFrame(parent),
      m_fakeWidget(new QWidget(nullptr)),
      m_mainPanel(new MainPanel)
{
    QHBoxLayout *layout = new QHBoxLayout;
    // layout->addSpacing(0);
    layout->addWidget(m_mainPanel);
    // layout->addSpacing(10);
    layout->setMargin(0);
    layout->setSpacing(0);
    setLayout(layout);

    m_fakeWidget->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowDoesNotAcceptFocus | Qt::SplashScreen);
    m_fakeWidget->setAttribute(Qt::WA_TranslucentBackground);

    setAttribute(Qt::WA_NoSystemBackground, false);
    // setAttribute(Qt::WA_TranslucentBackground);
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowDoesNotAcceptFocus);

    // KWindowEffects::enableBlurBehind(winId(), true);
    KWindowSystem::setOnDesktop(effectiveWinId(), NET::OnAllDesktops);
    KWindowSystem::setType(winId(), NET::Dock);

    // Appear with an animation
    QPropertyAnimation *animation = new QPropertyAnimation(this, "pos");
    animation->setDuration(1500);
    animation->setStartValue(QPoint(0, -2 * this->height()));
    animation->setEndValue(QPoint(0,0));
    animation->setEasingCurve(QEasingCurve::OutCubic);
    animation->start(QPropertyAnimation::DeleteWhenStopped);

    initSize();

    connect(qApp->primaryScreen(), &QScreen::geometryChanged, this, &MainWindow::initSize);
}

MainWindow::~MainWindow()
{

}

void MainWindow::paintEvent(QPaintEvent *e)
{
    QWidget::paintEvent(e);
    // QPainter painter(this);
    // painter.setRenderHint(QPainter::Antialiasing);
    // const QPalette &palette = this->palette();
    // QColor backgroundColor = palette.color(QPalette::Window);
    // backgroundColor.setAlpha(100);
    // painter.setPen(Qt::NoPen);
    // painter.setBrush(backgroundColor);
    // painter.fillRect(rect(), backgroundColor);
    // painter.fillRect(QRect(0, rect().height() - 1, rect().width(), 1), QColor(0, 0, 0, 50));
}

void MainWindow::initSize()
{
    QRect primaryRect = qApp->primaryScreen()->geometry();
    qreal scale = qApp->primaryScreen()->devicePixelRatio();
    setFixedWidth(primaryRect.width());
    setFixedHeight(TOPBAR_HEIGHT);
    move(0, 0);

    setStrutPartial();

//    KWindowSystem::setState(winId(), NET::SkipTaskbar);
//    KWindowSystem::setState(winId(), NET::SkipSwitcher);
}

void MainWindow::setStrutPartial()
{
    // 不清真的作法，kwin设置blur后设置程序支撑导致模糊无效
    QRect r(geometry());
    r.setHeight(1);
    r.setWidth(1);
    m_fakeWidget->setGeometry(r);
    m_fakeWidget->setVisible(true);

    const QRect windowRect = this->rect();
    NETExtendedStrut strut;

    strut.top_width = height();
    strut.top_start = x();
    strut.top_end = x() + width();

    KWindowSystem::setExtendedStrut(m_fakeWidget->winId(),
                                     strut.left_width,
                                     strut.left_start,
                                     strut.left_end,
                                     strut.right_width,
                                     strut.right_start,
                                     strut.right_end,
                                     strut.top_width,
                                     strut.top_start,
                                     strut.top_end,
                                     strut.bottom_width,
                                     strut.bottom_start,
                                     strut.bottom_end);

//    KWindowSystem::setState(winId(), NET::SkipTaskbar);
//    KWindowSystem::setState(winId(), NET::SkipSwitcher);
//    KWindowSystem::setState(m_fakeWidget->winId(), NET::SkipTaskbar);
//    KWindowSystem::setState(m_fakeWidget->winId(), NET::SkipSwitcher);
}
