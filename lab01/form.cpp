#include "form.h"
#include "ui_form.h"
#include "encrypt.h"



Form::Form(QWidget *parent, QString site, QString login_encrypted, QString password_encrypted)
    : QWidget(parent)
    , ui(new Ui::Form)
{
    ui->setupUi(this);

    this->pass_encr = password_encrypted;
    this->log_encr = login_encrypted;

    this->isShow = false;

    ui->label_siteName->setText(site);
    ui->label_login->setText("******");
    ui->label_password->setText("******");
}

Form::~Form()
{
    delete ui;
}

void Form::on_pushButton_copyCreds_clicked()
{

    authPassAndLogin *auth = new authPassAndLogin();
    QEventLoop loop;
    connect(auth, &authPassAndLogin::loginSuccessful,
            &loop, &QEventLoop::quit);
    auth->show();
    loop.exec();

    if (auth->isAuth) {
        QByteArray hash = QCryptographicHash::hash(auth->pin.toUtf8(), QCryptographicHash::Sha256);
        unsigned char hash_key[32] = {0};
        memcpy(hash_key, hash.data(), 32);
        qDebug() << "login " << this->log_encr << " password " << this->pass_encr;

        QClipboard* clipBoard = QApplication::clipboard();
        QString credsClipBoard = QString("login: %1 password: %2").arg(decryptString(this->log_encr, hash_key), decryptString(this->pass_encr, hash_key));
        clipBoard->setText(credsClipBoard);
    }
}


void Form::on_pushButton_ShowHide_clicked()
{
    authPassAndLogin *auth = new authPassAndLogin();
    QEventLoop loop;
    connect(auth, &authPassAndLogin::loginSuccessful,
            &loop, &QEventLoop::quit);
    if (!this->isShow) {
        auth->show();
        loop.exec();
        qDebug() << "is auth: " << auth->isAuth;
        // auth->hide();
        //this->setEnabled(true);
    }

    if (this->isShow || auth->isAuth == true) {
        this->isShow = !this->isShow;
        if (this->isShow) {

            QByteArray hash = QCryptographicHash::hash(auth->pin.toUtf8(), QCryptographicHash::Sha256);
            unsigned char hash_key[32] = {0};
            memcpy(hash_key, hash.data(), 32);

            ui->label_login->setText(decryptString(this->log_encr, hash_key));
            ui->label_password->setText(decryptString(this->pass_encr, hash_key));
            ui->pushButton_ShowHide->setText("Hide");
        } else {
            ui->label_login->setText("******");
            ui->label_password->setText("******");
            ui->pushButton_ShowHide->setText("Show");
        }
    }
    delete auth;
}

