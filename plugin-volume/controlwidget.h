#ifndef CONTROLWIDGET_H
#define CONTROLWIDGET_H

#include <QWidget>
#include "volumewidget.h"

class ControlWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ControlWidget(QWidget *parent = nullptr);

    QWidget *popupWindow() { return m_widget; }

private:
    VolumeWidget *m_widget;
};

#endif // DATETIMEWIDGET_H
