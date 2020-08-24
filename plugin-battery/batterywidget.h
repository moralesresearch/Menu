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

#ifndef BATTERYWIDGET_H
#define BATTERYWIDGET_H

#include <QWidget>
#include <QLabel>
#include <Solid/Battery>
#include <Solid/Device>

class BatteryWidget : public QWidget
{
    Q_OBJECT

public:
    BatteryWidget(QWidget *parent = nullptr);
    ~BatteryWidget();

    QString popupText();

private:
    QString stateToString(Solid::Battery::ChargeState state);
    void updateIcon();

private:
    // QLabel *m_percentLabel;
    QLabel *m_iconLabel;
    QPixmap m_iconPixmap;
    Solid::Battery *m_primaryBattery;
};

#endif
