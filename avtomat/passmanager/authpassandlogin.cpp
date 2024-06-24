#include "authpassandlogin.h"
#include "ui_authpassandlogin.h"

authPassAndLogin::authPassAndLogin(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::authPassAndLogin)
{
    ui->setupUi(this);
}

authPassAndLogin::~authPassAndLogin()
{
    delete ui;
}

void authPassAndLogin::closeEvent(QCloseEvent *event)
{
    hide();
    event->ignore();
}

void authPassAndLogin::on_pushButton_login_clicked()
{
    const QString pin = QString("1234");
    // key 03ac674216f3e15c761ee1a5e255f067953623c8b388b4459e13f978d7c846f4
    QString inputPin = ui->lineEdit_Pin->text();
    if (inputPin != pin) {
        return;
    }
    this->pin = inputPin;
    ui->lineEdit_Pin->clear();
    this->isAuth = true;
    emit loginSuccessful();
    this->hide();
    //this->close();
}

void authPassAndLogin::hideEvent(QHideEvent *bar){
    emit hide();
    bar->accept();
    this->hide();
}
