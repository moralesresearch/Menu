#ifndef POPUPWINDOW_H
#define POPUPWINDOW_H

#include <QMenu>
#include "volumewidget.h"

class PopupWindow : public QMenu
{
    Q_OBJECT

public:
    explicit PopupWindow(QWidget *parent = nullptr);

    // void popup(QPoint pos);

protected:
    void paintEvent(QPaintEvent *e);

private:
    VolumeWidget *m_volumeWidget;
};

#endif // DATETIMEWIDGET_H
