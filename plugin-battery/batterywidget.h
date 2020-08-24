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
