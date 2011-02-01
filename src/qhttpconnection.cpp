/*
 * Copyright 2011 Nikhil Marathe <nsm.nikhil@gmail.com>
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE. 
 */

#include "qhttpconnection.h"

#include <QTcpSocket>
#include <QHostAddress>
#include <QDebug>

#include "qhttprequest.h"
#include "qhttpresponse.h"

QHttpConnection::QHttpConnection(QTcpSocket *socket, QObject *parent)
    : QObject(parent)
    , m_socket(socket)
    , m_parser(0)
{
    qDebug() << "Got new connection" << socket->peerAddress() << socket->peerPort();

    m_parser = (http_parser*)malloc(sizeof(http_parser));
    http_parser_init(m_parser, HTTP_REQUEST);

    m_parserSettings.on_message_begin = MessageBegin;
    m_parserSettings.on_path = Path;
    m_parserSettings.on_query_string = 0;
    //m_parserSettings.on_query_string = QueryString;
    m_parserSettings.on_url = Url;
    m_parserSettings.on_fragment = Fragment;
    m_parserSettings.on_header_field = HeaderField;
    m_parserSettings.on_header_value = HeaderValue;
    m_parserSettings.on_headers_complete = HeadersComplete;
    m_parserSettings.on_body = Body;
    m_parserSettings.on_message_complete = MessageComplete;

    m_parser->data = this;

    connect(socket, SIGNAL(readyRead()), this, SLOT(parseRequest()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(deleteLater()));
}

QHttpConnection::~QHttpConnection()
{
    qDebug() << "Disconnected. Deleting this connection object";
    delete m_socket;
    m_socket = 0;

    if( m_request )
        delete m_request;

    free(m_parser);
    m_parser = 0;
}

void QHttpConnection::parseRequest()
{
    Q_ASSERT(m_parser);

    while(m_socket->bytesAvailable())
    {
        QByteArray arr = m_socket->read(80*1024);

        if( arr.size() < 0 ) {
            // TODO
            qDebug() << "Handle closed connection";
        }
        else {
            int nparsed = http_parser_execute(m_parser, &m_parserSettings, arr.data(), arr.size());
            if( nparsed != arr.size() ) {
                qDebug() << "ERROR PARSING!";
            }
        }
    }
}

void QHttpConnection::write(const QByteArray &data)
{
    m_socket->write(data);
}

void QHttpConnection::flush()
{
    m_socket->flush();
}

/********************
 * Static Callbacks *
 *******************/
int QHttpConnection::MessageBegin(http_parser *parser)
{
    QHttpConnection *theConnection = (QHttpConnection *)parser->data;
    theConnection->m_request = new QHttpRequest(theConnection);
    return 0;
}

int QHttpConnection::HeadersComplete(http_parser *parser)
{
    qDebug() << "header complete";
    qDebug() << parser->method << parser->http_major << parser->http_minor;
    QHttpConnection *theConnection = (QHttpConnection *)parser->data;
    Q_ASSERT(theConnection->m_request);

    /** set method **/
    QString method;
    switch(parser->method)
    {
        case HTTP_DELETE:     method = "DELETE"; break;
        case HTTP_GET:        method = "GET"; break;
        case HTTP_HEAD:       method = "HEAD"; break;
        case HTTP_POST:       method = "POST"; break;
        case HTTP_PUT:        method = "PUT"; break;
        case HTTP_CONNECT:    method = "CONNECT"; break;
        case HTTP_OPTIONS:    method = "OPTIONS"; break;
        case HTTP_TRACE:      method = "TRACE"; break;
    }
    theConnection->m_request->setMethod(method);

    /** set version **/
    theConnection->m_request->setVersion(QString("%1.%2").arg(parser->http_major).arg(parser->http_minor));

    // TODO don't create a response object just like that
    emit theConnection->newRequest(theConnection->m_request, new QHttpResponse(theConnection));
    return 0;
}

int QHttpConnection::MessageComplete(http_parser *parser)
{
    qDebug() << "Message complete";
    return 0;
}

int QHttpConnection::Path(http_parser *parser, const char *at, size_t length)
{
    QHttpConnection *theConnection = (QHttpConnection *)parser->data;
    Q_ASSERT(theConnection->m_request);
    QString path = QString::fromAscii(at, length);

    QUrl url = theConnection->m_request->url();
    url.setPath(path);
    theConnection->m_request->setUrl(url);
    qDebug() << "GOT Path" << theConnection->m_request->url();
    return 0;
}

int QHttpConnection::QueryString(http_parser *parser, const char *at, size_t length)
{
    QHttpConnection *theConnection = (QHttpConnection *)parser->data;
    Q_ASSERT(theConnection->m_request);

    qDebug() << "GOT query string" << QString::fromAscii(at, length) << "FAILING DUE TO NON-IMPLEMENTATION";
    Q_ASSERT(false);
    return 0;
}

int QHttpConnection::Url(http_parser *parser, const char *at, size_t length)
{
    QHttpConnection *theConnection = (QHttpConnection *)parser->data;
    theConnection->m_request->setUrl(QString::fromAscii(at, length));

    qDebug() << "GOT URL" << theConnection->m_request->url();
    return 0;
}

int QHttpConnection::Fragment(http_parser *parser, const char *at, size_t length)
{
    return 0;
}

int QHttpConnection::HeaderField(http_parser *parser, const char *at, size_t length)
{
    return 0;
}

int QHttpConnection::HeaderValue(http_parser *parser, const char *at, size_t length)
{
    return 0;
}

int QHttpConnection::Body(http_parser *parser, const char *at, size_t length)
{
    return 0;
}
