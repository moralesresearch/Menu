#include "extensionwidget.h"
#include <QHBoxLayout>

ExtensionWidget::ExtensionWidget(StatusBarExtension *extension, QWidget *parent)
  : QWidget(parent),
    m_extension(extension)
{
    QHBoxLayout *layout = new QHBoxLayout;
    layout->setMargin(0);
    layout->setSpacing(0);
    setLayout(layout);

    if (extension->itemWidget())
        layout->addWidget(extension->itemWidget());
}