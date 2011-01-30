/*
Copyright 2009,2010 Nikhil Marathe <nsm.nikhil@gmail.com>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to
deal in the Software without restriction, including without limitation the
rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
sell copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
IN THE SOFTWARE. 
*/
#include "qhttpserver.h"

#include <QTcpServer>
#include <QTcpSocket>
#include <QVariant>
#include <QDebug>

#include <http_parser.h>

QHttpServer::QHttpServer(QObject *parent)
    : QObject(parent)
    , m_tcpServer(0)
{
}

QHttpServer::~QHttpServer()
{
}

void QHttpServer::parseRequest()
{
    QTcpSocket *socket = qobject_cast<QTcpSocket*>(QObject::sender());

    QVariant x = socket->property("http_parser");
    http_parser *parser = (http_parser*)qvariant_cast<void*>(x);
    Q_ASSERT(parser);

    while(socket->bytesAvailable())
    {
        QByteArray arr = socket->read(80*1024);

        if( arr.size() < 0 ) {
            // TODO
            qDebug() << "Handle closed connection";
        }
        else {
            qDebug() << "X";
            int nparsed = http_parser_execute(parser, (const http_parser_settings*)parser->data, arr.data(), arr.size());
            qDebug() << "Y";
            if( nparsed != arr.size() ) {
                qDebug() << "ERROR PARSING!";
            }
        }
    }
}

void QHttpServer::newConnection()
{
    Q_ASSERT(m_tcpServer);
    while(m_tcpServer->hasPendingConnections()) {
        QTcpSocket *socket = m_tcpServer->nextPendingConnection();
        qDebug() << "Got new connection" << socket->peerAddress() << socket->peerPort();

        http_parser_init(parser, HTTP_REQUEST);

        socket->setProperty("http_parser", QVariant::fromValue((void*)parser));

        connect(socket, SIGNAL(readyRead()), this, SLOT(parseRequest()));
    }
}

bool QHttpServer::listen(const QHostAddress &address, quint16 port)
{
    m_tcpServer = new QTcpServer;

    connect(m_tcpServer, SIGNAL(newConnection()), this, SLOT(newConnection()));
    return m_tcpServer->listen(address, port);
}
