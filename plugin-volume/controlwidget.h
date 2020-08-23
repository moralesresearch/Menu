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

protected:
    void paintEvent(QPaintEvent *e) override;

private:
    VolumeWidget *m_widget;
    QPixmap m_iconPixmap;
};

#endif // DATETIMEWIDGET_H
