#ifndef ENCRYPT_H
#define ENCRYPT_H

#include <QByteArray>
#include <openssl/evp.h>

#include <iostream>
#include <QFile>
#include <QJsonParseError>
#include <QJsonObject>
#include <QDir>
#include <QCryptographicHash>
#include <QBuffer>



QByteArray decryptString(QString str, unsigned char *key);
int decryptQByteArray(const QByteArray& encryptedBytes, QByteArray& decryptedBytes, unsigned char *key);

#endif // ENCRYPT_H
