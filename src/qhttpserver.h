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
#ifndef Q_HTTP_SERVER
#define Q_HTTP_SERVER

#include <QObject>
#include <QHostAddress>

struct http_parser;

class QTcpServer;

class QHttpServer : public QObject
{
    Q_OBJECT

public:
    QHttpServer(QObject *parent = 0);
    virtual ~QHttpServer();

    bool listen(const QHostAddress &address = QHostAddress::Any, quint16 port = 0);

private slots:
    void newConnection();
    void parseRequest();

private:
    static int MessageBegin(http_parser *parser);
    static int Path(http_parser *parser, const char *at, size_t length);
    static int QueryString(http_parser *parser, const char *at, size_t length);
    static int Url(http_parser *parser, const char *at, size_t length);
    static int Fragment(http_parser *parser, const char *at, size_t length);
    static int HeaderField(http_parser *parser, const char *at, size_t length);
    static int HeaderValue(http_parser *parser, const char *at, size_t length);
    static int HeadersComplete(http_parser *parser);
    static int Body(http_parser *parser, const char *at, size_t length);
    static int MessageComplete(http_parser *parser);

private:
    QTcpServer *m_tcpServer;
};

#endif
