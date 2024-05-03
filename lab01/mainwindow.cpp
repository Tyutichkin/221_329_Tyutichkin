#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::on_pushButton_login_clicked()
{
    //pushButton_login
    const QString pin = QString("1234");
    // key 03ac674216f3e15c761ee1a5e255f067953623c8b388b4459e13f978d7c846f4
    QString inputPin = ui->lineEdit_Pin->text();
    if (inputPin != pin) {
        return;
    }

    hide();
    passManager = new PassManager(this, inputPin);
    passManager->show();

    qDebug() << "pass: " << inputPin << "\n";
}

