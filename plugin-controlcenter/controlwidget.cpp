#include "controlwidget.h"
#include <QHBoxLayout>
#include <QToolButton>
#include <QLabel>

ControlWidget::ControlWidget(QWidget *parent)
  : QWidget(parent),
    m_window(new PopupWindow(this))
{
    QHBoxLayout *layout = new QHBoxLayout;
    layout->setMargin(0);
    layout->setSpacing(0);
    setLayout(layout);

    QToolButton *iconButton = new QToolButton;
    iconButton->setIcon(QIcon::fromTheme("configure"));
    iconButton->setFixedSize(QSize(30, 30));
    iconButton->setIconSize(QSize(30, 30));
    iconButton->setAutoRaise(true);
    layout->addWidget(iconButton);

    connect(iconButton, &QToolButton::clicked, this, [=] {
        const int offset = 0;
        QPoint p(this->geometry().topLeft());
        p += QPoint(rect().width() / 2 - m_window->width() / 2,
                    0 - m_window->height() - offset);
        m_window->popup(p);
    });
}
