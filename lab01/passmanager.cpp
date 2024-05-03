#include "passmanager.h"
#include "ui_passmanager.h"
#include "form.h"
#include "encrypt.h"

#include <openssl/evp.h>

#include <iostream>
#include <QFile>
#include <QJsonParseError>
#include <QJsonObject>
#include <QDir>
#include <QCryptographicHash>
#include <QBuffer>

PassManager::PassManager(QWidget *parent, QString pincode)
    : QDialog(parent)
    , ui(new Ui::PassManager)
{
    ui->setupUi(this);
    QByteArray hash = QCryptographicHash::hash(pincode.toUtf8(), QCryptographicHash::Sha256);
    unsigned char hash_key[32] = {0};
    memcpy(hash_key, hash.data(), 32);

    qDebug() << hash.toHex();

    readJson("../../creds_enc.json", hash_key);

    for (qsizetype i = 0; i < this->jsonArr.size(); i++) {
        QJsonObject jsonItem = this->jsonArr[i].toObject();
        Form *formWidget = new Form(this, jsonItem["site"].toString(), jsonItem["login"].toString(), jsonItem["password"].toString());
        QListWidgetItem *newItem = new QListWidgetItem();

        ui->listWidget_creds->addItem(newItem);
        ui->listWidget_creds->setItemWidget(newItem, formWidget);

        newItem->setSizeHint(formWidget->sizeHint());
        //ui->listWidget_creds->addItem(credForm);
    }
}

void PassManager::readJson(const char* filename, unsigned char *key) {
    QFile inFile(filename);
    inFile.open(QIODevice::ReadOnly);
    QByteArray hexEncryptedBytes = inFile.readAll();
    inFile.close();

    QByteArray encryptedBytes = QByteArray::fromHex(hexEncryptedBytes);
    QByteArray decryptedBytes;

    decryptQByteArray(encryptedBytes, decryptedBytes, key);

    QJsonParseError errorPtr;
    QJsonDocument doc = QJsonDocument::fromJson(decryptedBytes, &errorPtr);

    qDebug() << "fromJson err: " << errorPtr.errorString();

    QJsonObject jsonObj = doc.object();

    this->jsonArr = jsonObj.value("cridentials").toArray();
}

PassManager::~PassManager()
{
    delete ui;
}

void PassManager::on_pushButton_searchSite_clicked()
{
    //readJson("../../creds.json");

    QString searchTerm = ui->lineEdit_searchSite->text().toLower();
    bool isNeedFilter = false;
    if (searchTerm.length() != 0) {
        isNeedFilter = true;
    }
    ui->listWidget_creds->clear();
    for (qsizetype i = 0; i < this->jsonArr.size(); i++) {
        QJsonObject jsonItem = this->jsonArr[i].toObject();
        if (!isNeedFilter || jsonItem["site"].toString().toLower().contains(searchTerm)) {
            Form *formWidget = new Form(this, jsonItem["site"].toString(), jsonItem["login"].toString(), jsonItem["password"].toString());
            QListWidgetItem *newItem = new QListWidgetItem();

            ui->listWidget_creds->addItem(newItem);
            ui->listWidget_creds->setItemWidget(newItem, formWidget);

            newItem->setSizeHint(formWidget->sizeHint());
        }
    }
}

