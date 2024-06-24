#ifndef PROCESS_HANDLER_H
#define PROCESS_HANDLER_H

#include <QCoreApplication>
#include <QProcess>
#include <QDebug>

class ProcessHandler : public QObject
{
    Q_OBJECT

public:
    ProcessHandler(QObject *parent = nullptr)
        : QObject(parent), process(new QProcess(this))
    {
        connect(process, &QProcess::readyReadStandardOutput, this, &ProcessHandler::handleReadyRead);
        connect(process, QOverload<QProcess::ProcessError>::of(&QProcess::errorOccurred), this, &ProcessHandler::handleError);

        QString program = "path/to/your/executable.exe";
        QStringList arguments;
        // Добавьте аргументы, если они нужны
        // arguments << "arg1" << "arg2";

        process->start(program, arguments);
    }

private slots:
    void handleReadyRead()
    {
        QString output = process->readAllStandardOutput().trimmed();
        if (output.contains("1")) {
            qDebug() << "yes";
            process->terminate();
        }
    }

    void handleError(QProcess::ProcessError error)
    {
        qDebug() << "Process error:" << error;
    }

private:
    QProcess *process;
};

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    ProcessHandler handler;

    return a.exec();
}

//#include "main.moc"

#endif // PROCESS_HANDLER_H
