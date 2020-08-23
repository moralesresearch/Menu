#include "popupwindow.h"
#include <QVBoxLayout>

PopupWindow::PopupWindow(QWidget *parent)
  : QMenu(parent),
    m_mainLayout(new QVBoxLayout)
{
    m_mainLayout->setContentsMargins(10, 10, 10, 10);
    m_mainLayout->setSpacing(0);
    setLayout(m_mainLayout);
    setVisible(false);
}

void PopupWindow::setWidget(QWidget *w)
{
    if (!w)
        return;
    
    w->setParent(this);
    m_mainLayout->addWidget(w);
    adjustSize();
}