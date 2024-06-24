#include "websocketclient.h"
#include <QDebug>
#include <QTimer>

WebSocketClient::WebSocketClient(const QUrl &url, QObject *parent)
    : QObject(parent), url(url)
{
    webSocket = new QWebSocket;
    connect(webSocket, &QWebSocket::connected, this, &WebSocketClient::onConnected);
    connect(webSocket, &QWebSocket::textMessageReceived, this, &WebSocketClient::onTextMessageReceived);
    connect(webSocket, QOverload<QAbstractSocket::SocketError>::of(&QWebSocket::error), this, &WebSocketClient::onError);
    this->url = url;
    // webSocket->open(url);
}

WebSocketClient::~WebSocketClient()
{
    webSocket->deleteLater();
}

void WebSocketClient::connectToServer()
{
    face_key = "";
    webSocket->open(url);
}

bool WebSocketClient::isConnected() const
{
    return webSocket->state() == QAbstractSocket::ConnectedState;
}

void WebSocketClient::disconnectFromServer()
{
    if (webSocket->state() == QAbstractSocket::ConnectedState) {
        webSocket->close(QWebSocketProtocol::CloseCodeNormal, "Client requested disconnect");
    }
}

void WebSocketClient::sendMessage(const QString &message)
{
    if (webSocket->isValid()) {
        webSocket->sendTextMessage(message);
    } else {
        qDebug() << "WebSocket is not connected.";
    }
}

void WebSocketClient::onConnected()
{
    qDebug() << "WebSocket connected";
}

void WebSocketClient::onTextMessageReceived(const QString &message)
{
    face_key = message;
}

void WebSocketClient::onError(QAbstractSocket::SocketError error)
{
    qDebug() << "WebSocket error: " << webSocket->errorString();
    // if (webSocket->errorString() == "The remote host closed the connection")
    // QTimer::singleShot(3000, this, &WebSocketClient::connectToServer); // Retry after 3 seconds
}
QString WebSocketClient::getFaceKey() const
{
    return face_key;
}
