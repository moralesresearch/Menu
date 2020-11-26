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

#include <qtsingleapplication/qtsingleapplication.h>

// probono: See
// https://github.com/qtproject/qt-solutions/blob/master/qtsingleapplication/examples/trivial/main.cpp

int main(int argc, char **argv)
{
    QtSingleApplication instance(argc, argv);
    if (instance.sendMessage("Wake up!"))
    return 0;

    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    instance.setActivationWindow(&w);

    QObject::connect(&instance, SIGNAL(messageReceived(const QString&)),
             &w, SLOT(append(const QString&)));

    return instance.exec();
}
