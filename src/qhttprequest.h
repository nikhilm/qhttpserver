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
#ifndef Q_HTTP_REQUEST
#define Q_HTTP_REQUEST

#include <QObject>
#include <QHash>
#include <QUrl>

class QTcpSocket;

class QHttpConnection;

class QHttpRequest : public QObject
{
    Q_OBJECT

public:
    virtual ~QHttpRequest();

    const QString method() const { return m_method; };
    QUrl url() const { return m_url; };
    QString path() const;
    QString httpVersion() const { return m_version; };
    QString queryString() const;

signals:
    void data(const QByteArray &);
    void end();

private:
    QHttpRequest(QHttpConnection *connection, QObject *parent = 0);

    void setMethod(const QString &method) { m_method = method; }
    void setVersion(const QString &version) { m_version = version; }
    void setUrl(const QUrl &url) { m_url = url; }

    QHttpConnection *m_connection;
    QHash<QString, QString> m_headers;
    QString m_method;
    QUrl m_url;
    QString m_version;

    friend class QHttpConnection;
};

#endif
