#ifndef EXTENSIONWIDGET_H
#define EXTENSIONWIDGET_H

#include <QWidget>
#include <QTimer>
#include "blurwindow.h"
#include "popupwindow.h"
#include "../interfaces/pluginsiterface.h"

class ExtensionWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ExtensionWidget(StatusBarExtension *extension = nullptr, QWidget *parent = nullptr);

    QSize sizeHint() const override;

protected:
    void enterEvent(QEvent *e) override;
    void leaveEvent(QEvent *e) override;
    void mousePressEvent(QMouseEvent *e) override;
    void mouseReleaseEvent(QMouseEvent *e) override;
    bool eventFilter(QObject *watched, QEvent *event) override;

private:
    QPoint popupPoint(int contentWidth);
    void showPopupText();
    void hidePopupText();
    void showPopupWindow();

private:
    StatusBarExtension *m_extension;
    QTimer *m_popupTextDelayTimer;
    BlurWindow *m_popupText;
    PopupWindow *m_popupWindow;
    QWidget *m_contentWidget;
};

#endif // EXTENSIONWIDGET_H
