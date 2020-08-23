#ifndef VOLUMEWIDGET_H
#define VOLUMEWIDGET_H

#include <QWidget>
#include <QToolButton>
#include <QSlider>

class VolumeWidget : public QWidget
{
    Q_OBJECT

public:
    explicit VolumeWidget(QWidget *parent = nullptr);

    void initUI();
    void initSignal();
    void toggleMute();
    void handleSliderValueChanged(int value);

signals:
    void requestUpdateIcon(QString iconName);

private slots:
    void handleVolumeChanged(int volume);
    void handleMuteChanged(bool mute);

private:
    QToolButton *m_iconButton;
    QSlider *m_slider;
    bool m_firstLoad;
};

#endif // DATETIMEWIDGET_H
