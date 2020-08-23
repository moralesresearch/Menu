#ifndef EXTENSIONWIDGET_H
#define EXTENSIONWIDGET_H

#include <QWidget>
#include "../interfaces/pluginsiterface.h"

class ExtensionWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ExtensionWidget(StatusBarExtension *extension = nullptr, QWidget *parent = nullptr);

private:
    StatusBarExtension *m_extension;
};

#endif // EXTENSIONWIDGET_H
