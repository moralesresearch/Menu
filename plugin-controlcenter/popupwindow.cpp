#include "popupwindow.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QPainter>

PopupWindow::PopupWindow(QWidget *parent)
  : QMenu(parent),
    m_volumeWidget(new VolumeWidget)
{
    QVBoxLayout *layout = new QVBoxLayout;
    layout->setContentsMargins(10, 10, 10, 10);
    layout->setSpacing(0);
    setLayout(layout);

    layout->addWidget(m_volumeWidget, 0, Qt::AlignTop);

    setFixedSize(200, 200);
    //setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::WindowDoesNotAcceptFocus | Qt::Popup);
    setVisible(false);
}

// void PopupWindow::popup(QPoint pos)
// {
//     setVisible(true);
//     move(pos);
// }

void PopupWindow::paintEvent(QPaintEvent *e)
{
    QWidget::paintEvent(e);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setPen(QColor(0, 0, 0, 20));
    painter.setBrush(QColor(241, 241, 241));
    const qreal radius = 5.0; //std::min(rect().height(), rect().width()) / 2 - 2;
    painter.drawRoundedRect(rect(), radius, radius);
}
