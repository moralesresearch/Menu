#include "controlwidget.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QDebug>

ControlWidget::ControlWidget(QWidget *parent)
  : QWidget(parent),
    m_widget(new VolumeWidget)
{
    QHBoxLayout *layout = new QHBoxLayout;
    layout->setMargin(0);
    layout->setSpacing(0);
    setLayout(layout);

    QLabel *iconLabel = new QLabel;
    iconLabel->setFixedSize(QSize(24, 24));
    layout->addWidget(iconLabel);

    connect(m_widget, &VolumeWidget::requestUpdateIcon, this, [=] (QString iconName) {
        iconLabel->setPixmap(QIcon::fromTheme(iconName).pixmap(QSize(iconLabel->width(), iconLabel->height())));
    });
}
