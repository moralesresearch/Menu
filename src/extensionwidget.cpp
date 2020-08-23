#include "extensionwidget.h"
#include <QEvent>
#include <QHBoxLayout>
#include <QApplication>
#include <QScreen>
#include <QDebug>

ExtensionWidget::ExtensionWidget(StatusBarExtension *extension, QWidget *parent)
  : QWidget(parent),
    m_extension(extension),
    m_popupTextDelayTimer(new QTimer(this)),
    m_popupText(new BlurWindow)
{
    m_popupTextDelayTimer->setInterval(300);
    m_popupTextDelayTimer->setSingleShot(true);

    QHBoxLayout *layout = new QHBoxLayout;
    layout->setMargin(0);
    layout->setSpacing(0);
    setLayout(layout);

    if (extension->itemWidget())
        layout->addWidget(extension->itemWidget());

    connect(m_popupTextDelayTimer, &QTimer::timeout, this, &ExtensionWidget::showPopupText);
}

void ExtensionWidget::enterEvent(QEvent *e)
{
    m_popupTextDelayTimer->start();

    QWidget::enterEvent(e);
}

void ExtensionWidget::leaveEvent(QEvent *e)
{
    m_popupTextDelayTimer->stop();
    hidePopupText();

    QWidget::leaveEvent(e);
}

void ExtensionWidget::showPopupText()
{
    if (m_extension->popupText().isEmpty())
        return;

    const int offset = 10;

    m_popupText->setText(m_extension->popupText());
    m_popupText->setVisible(true);

    QPoint p(mapToGlobal(QPoint(pos().x(), height()) - pos()));
    p += QPoint(0, offset);

    // rekols: Screen edge processing.
    QRect screenGeometry = qApp->primaryScreen()->geometry();
    if (p.x() + m_popupText->geometry().x() >= screenGeometry.x()) {
        p.setX(screenGeometry.width() - m_popupText->width() - offset);
    }

    m_popupText->move(p);
    m_popupText->update();
}

void ExtensionWidget::hidePopupText()
{
    m_popupText->hide();
}