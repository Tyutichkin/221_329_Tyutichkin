#ifndef PASSMANAGER_H
#define PASSMANAGER_H

#include <QDialog>
#include <QJsonArray>
#include <QBoxLayout>

namespace Ui {
class PassManager;
}

class PassManager : public QDialog
{
    Q_OBJECT

public:
    explicit PassManager(QWidget *parent = nullptr, QString pincode = "");
    ~PassManager();
    QJsonArray jsonArr;
    void readJson(const char* filename, unsigned char *key);

private slots:
    void on_pushButton_searchSite_clicked();

private:
    Ui::PassManager *ui;
};

#endif // PASSMANAGER_H
