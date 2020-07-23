#ifndef CONTROLWIDGET_H
#define CONTROLWIDGET_H

#include <QWidget>
#include "popupwindow.h"

class ControlWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ControlWidget(QWidget *parent = nullptr);

private:
    PopupWindow *m_window;
};

#endif // DATETIMEWIDGET_H
