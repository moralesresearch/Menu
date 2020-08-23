#include "controlwidget.h"
#include <QHBoxLayout>
#include <QToolButton>

ControlWidget::ControlWidget(QWidget *parent)
  : QWidget(parent),
    m_widget(new VolumeWidget)
{
    QHBoxLayout *layout = new QHBoxLayout;
    layout->setMargin(0);
    layout->setSpacing(0);
    setLayout(layout);

    QToolButton *iconButton = new QToolButton;
    iconButton->setFixedSize(QSize(30, 30));
    iconButton->setIconSize(QSize(30, 30));
    iconButton->setAutoRaise(true);
    layout->addWidget(iconButton);

    connect(m_widget, &VolumeWidget::requestUpdateIcon, this, [=] (QString iconName) {
        iconButton->setIcon(QIcon::fromTheme(iconName));
    });
}
