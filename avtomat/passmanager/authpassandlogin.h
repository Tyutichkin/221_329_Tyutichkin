#ifndef AUTHPASSANDLOGIN_H
#define AUTHPASSANDLOGIN_H

#include <QWidget>
#include <QCloseEvent>

namespace Ui {
class authPassAndLogin;
}

class authPassAndLogin : public QWidget
{
    Q_OBJECT

public:
    explicit authPassAndLogin(QWidget *parent = nullptr);
    ~authPassAndLogin();
    bool isAuth = false;
    QString pin;
protected:
    void closeEvent(QCloseEvent *event);
private slots:
    void on_pushButton_login_clicked();
signals:
    void loginSuccessful();




private:
    Ui::authPassAndLogin *ui;
    void hideEvent(QHideEvent *bar);
};

#endif // AUTHPASSANDLOGIN_H
