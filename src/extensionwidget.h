#ifndef EXTENSIONWIDGET_H
#define EXTENSIONWIDGET_H

#include <QWidget>
#include <QTimer>
#include "blurwindow.h"
#include "../interfaces/pluginsiterface.h"

class ExtensionWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ExtensionWidget(StatusBarExtension *extension = nullptr, QWidget *parent = nullptr);

protected:
    void enterEvent(QEvent *e) override;
    void leaveEvent(QEvent *e) override;

private:
    void showPopupText();
    void hidePopupText();

private:
    StatusBarExtension *m_extension;
    QTimer *m_popupTextDelayTimer;
    BlurWindow *m_popupText;
};

#endif // EXTENSIONWIDGET_H
