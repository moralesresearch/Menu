#ifndef POPUPWINDOW_H
#define POPUPWINDOW_H

#include <QMenu>

class QVBoxLayout;
class PopupWindow : public QMenu
{
    Q_OBJECT

public:
    explicit PopupWindow(QWidget *parent = nullptr);

    void setWidget(QWidget *w);

private:
    QVBoxLayout *m_mainLayout;
};

#endif // POPUPWINDOW_H
