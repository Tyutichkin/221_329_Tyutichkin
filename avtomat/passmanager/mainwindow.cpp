#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QDebug>
#include <QThread>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , webSocketClient(new WebSocketClient(QUrl(QStringLiteral("wss://de79-185-204-1-212.ngrok-free.app")), this))
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_login_clicked()
{
    const QString pin = QString("1234");
    // key 03ac674216f3e15c761ee1a5e255f067953623c8b388b4459e13f978d7c846f4
    QString inputPin = ui->lineEdit_Pin->text();
    if (inputPin != pin) {
        return;
    }

    openPassManager(inputPin);

    qDebug() << "pass: " << inputPin << "\n";
}


void MainWindow::on_pushButton_face_auth_clicked()
{
    webSocketClient->connectToServer();
    QString myFaceKey = "461ff58b4ec5d6fe7c1cecf6329acc2268ef76d95854512c8f3fbc1f7b47c2e2";

    // 60000 = 60sec
    int timeout = 60000;
    int elapsedTime = 0;
    const int interval = 1000;
    // while (!webSocketClient->isConnected()) {
        // webSocketClient->connectToServer();
        // qDebug() << "try to connect to websocket";
        // QCoreApplication::processEvents();
        // QThread::sleep(1); // 1 second delay
    // }
    // qDebug() << "successfull connected to websocket";
    while (elapsedTime < timeout && webSocketClient->getFaceKey() != myFaceKey) {
        if (!webSocketClient->isConnected())
            webSocketClient->connectToServer();
        QCoreApplication::processEvents();
        QThread::msleep(interval);
        elapsedTime += interval;
        qDebug() << "face key: " << webSocketClient->getFaceKey();
    }
    webSocketClient->disconnectFromServer();
    if (!webSocketClient->getFaceKey().isEmpty()) {
        qDebug() << "Ключ лица: " << webSocketClient->getFaceKey();
        if (webSocketClient->getFaceKey() == myFaceKey) {
            const QString pin = QString("1234");
            openPassManager(pin);
        }

    } else {
        qDebug() << "Пустой ключ";
    }
}

void MainWindow::openPassManager(QString inputPin)
{
    hide();
    passManager = new PassManager(this, inputPin);
    passManager->show();
}
