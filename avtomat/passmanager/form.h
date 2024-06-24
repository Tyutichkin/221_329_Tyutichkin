#ifndef FORM_H
#define FORM_H

#include "authpassandlogin.h"

#include <QWidget>
#include <QClipboard>

namespace Ui {
class Form;
}

class Form : public QWidget
{
    Q_OBJECT

public:
    explicit Form(QWidget *parent = nullptr, QString site = "", QString login_encrypted = "", QString password_encrypted = "");
    ~Form();

private slots:
    void on_pushButton_copyCreds_clicked();

    void on_pushButton_ShowHide_clicked();

private:
    bool isShow;
    Ui::Form *ui;
    QString pass_encr;
    QString log_encr;
};

#endif // FORM_H
