#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QProcess>
#include <QStyle>
#include <QDesktopWidget>
#include <QTime>
#include <QPropertyAnimation>
#include <QIcon>
#include <QtQml/QQmlEngine>
#include <QtQml/QQmlComponent>
#include <QTimer>
#include <QUrl>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setGeometry(
                QStyle::alignedRect(
                    Qt::LeftToRight,
                    Qt::AlignCenter,
                    this->size(),
                    qApp->desktop()->availableGeometry()
                    )
                );
    ui->iconLabel->setPixmap(QIcon::fromTheme("exit").pixmap(48));
    // TODO: Find a way to make this grayscale rather than color
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_logoutButton_clicked()
{
    QTimer::singleShot(2500, []() { QProcess::execute("killall", QStringList() << "sh"); } );
    // this->clearScreen(); // TODO: Another animation, e.g., fade to just the wallpaper, then size down to black background (similar to Welcome Assistant)
}

void MainWindow::on_restartButton_clicked()
{
    QTimer::singleShot(2500, []() { QProcess::execute("sudo", QStringList() << "shutdown" << "-r" << "now"); } );
    this->clearScreen();
}

void MainWindow::on_shutdownButton_clicked()
{
    QTimer::singleShot(2500, []() { QProcess::execute("sudo", QStringList() << "shutdown" << "-p" << "now"); } );
    this->clearScreen();
}

// When Xorg gets killed, window decorations disappear first, the whole thing is unsighty
// hence we fill the screen before we exit
void MainWindow::clearScreen()
{
    QQmlEngine engine;
    // qDebug() << QCoreApplication::applicationDirPath() + "/shutdown.qml";
    // QQmlComponent component(&engine, QUrl::fromLocalFile(QCoreApplication::applicationDirPath() + "/shutdown.qml"));
    QQmlComponent component(&engine, QUrl("qrc:/shutdown.qml"));
    component.create();
}
