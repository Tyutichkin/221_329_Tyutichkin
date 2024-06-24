#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "passmanager.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    bool readJSON(unsigned char *key);

private slots:
    void on_pushButton_login_clicked();

private:
    Ui::MainWindow *ui;
    PassManager *passManager;
};
#endif // MAINWINDOW_H
