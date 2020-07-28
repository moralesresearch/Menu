#include "volumewidget.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QTimer>
#include <QDebug>

#include <QDBusConnection>
#include <QDBusInterface>
#include <QDBusMessage>
#include <QDBusReply>
#include <QDBusServiceWatcher>
#include <QDBusPendingCall>

static const QString Service = "org.panda.settings";
static const QString ObjectPath = "/Audio";
static const QString Interface = "org.panda.Audio";

VolumeWidget::VolumeWidget(QWidget *parent)
  : QWidget(parent),
    m_iconButton(new QToolButton),
    m_slider(new QSlider(Qt::Horizontal)),
    m_firstLoad(true)
{
    QHBoxLayout *layout = new QHBoxLayout;
    layout->setMargin(0);
    layout->setSpacing(0);
    setLayout(layout);

    layout->addWidget(m_iconButton);
    layout->addSpacing(10);
    layout->addWidget(m_slider);

    m_iconButton->setIcon(QIcon::fromTheme("audio-volume-medium"));
    m_slider->setMinimum(0);
    m_slider->setMaximum(100);

    connect(m_slider, &QSlider::valueChanged, this, &VolumeWidget::handleSliderValueChanged);
    connect(m_iconButton, &QToolButton::clicked, this, &VolumeWidget::toggleMute);

    initSignal();
    initUI();

    QDBusServiceWatcher *serviceWatcher = new QDBusServiceWatcher(Service, QDBusConnection::sessionBus(),
                                                                  QDBusServiceWatcher::WatchForRegistration);
    connect(serviceWatcher, &QDBusServiceWatcher::serviceRegistered, this, [=] {
        initSignal();
        initUI();
    });
}

void VolumeWidget::initUI()
{
    QDBusInterface iface(Service, ObjectPath, Interface, QDBusConnection::sessionBus(), this);
    if (iface.isValid()) {
        QDBusMessage result = iface.call("volume");
        if (!result.errorName().isEmpty())
            return;

        int volume = result.arguments().first().toInt();
        bool mute = false;

        result = iface.call("mute");
        mute = result.arguments().first().toBool();

        if (m_firstLoad) {
            m_slider->setValue(volume);
            m_firstLoad = false;
        }

        if (mute) {
            m_iconButton->setIcon(QIcon::fromTheme("audio-volume-muted"));
        } else if (volume > 0 && volume <= 30) {
            m_iconButton->setIcon(QIcon::fromTheme("audio-volume-low"));
        } else if (volume > 30 && volume <= 60) {
            m_iconButton->setIcon(QIcon::fromTheme("audio-volume-medium"));
        } else if (volume > 60 && volume <= 100) {
            m_iconButton->setIcon(QIcon::fromTheme("audio-volume-high"));
        }
    }
}

void VolumeWidget::initSignal()
{
    QDBusInterface iface(Service, ObjectPath, Interface, QDBusConnection::sessionBus(), this);

    if (!iface.isValid())
        return;

    QDBusConnection::sessionBus().connect(Service, ObjectPath, Interface, "volumeChanged",
                                          this, SLOT(handleVolumeChanged(int)));
    QDBusConnection::sessionBus().connect(Service, ObjectPath, Interface, "muteChanged",
                                          this, SLOT(handleMuteChanged(bool)));
}

void VolumeWidget::toggleMute()
{
    QDBusInterface iface(Service, ObjectPath, Interface, QDBusConnection::sessionBus(), this);
    if (iface.isValid()) {
        iface.call("toggleMute");
        initUI();
    }
}

void VolumeWidget::handleSliderValueChanged(int value)
{
    QDBusInterface iface(Service, ObjectPath, Interface, QDBusConnection::sessionBus(), this);
    if (iface.isValid()) {
        iface.call("setVolume", QVariant::fromValue(value));
        initUI();
    }
}

void VolumeWidget::handleVolumeChanged(int volume)
{
    m_slider->blockSignals(true);
    m_slider->setValue(volume);
    m_slider->blockSignals(false);
}

void VolumeWidget::handleMuteChanged(bool mute)
{
    Q_UNUSED(mute)

    initUI();
}
