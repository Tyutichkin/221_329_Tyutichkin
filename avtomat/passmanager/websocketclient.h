#ifndef WEBSOCKETCLIENT_H
#define WEBSOCKETCLIENT_H

#include <QObject>
#include <QWebSocket>
#include <QNetworkReply>

class WebSocketClient : public QObject
{
    Q_OBJECT

public:
    explicit WebSocketClient(const QUrl &url, QObject *parent = nullptr);
    ~WebSocketClient();
    void sendMessage(const QString &message);
    void connectToServer();
    void disconnectFromServer();
    bool isConnected() const;
    QString getFaceKey() const;

private slots:
    void onConnected();
    void onTextMessageReceived(const QString &message);
    void onError(QAbstractSocket::SocketError error);

private:
    QWebSocket *webSocket;
    QUrl url;
    QString face_key;
};

#endif // WEBSOCKETCLIENT_H
