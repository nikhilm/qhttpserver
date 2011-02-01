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

#ifndef Q_HTTP_REQUEST
#define Q_HTTP_REQUEST

#include <QObject>
#include <QHash>
#include <QUrl>

class QTcpSocket;

class QHttpConnection;

/*! \class QHttpRequest
 *
 * The QHttpRequest class represents the header and data
 * sent by the client.
 *
 * Header data is available immediately.
 *
 * Body data is streamed as it comes in via the data(const QByteArray&) signal.
 * As a consequence the application's request callback should ensure that it
 * connects to the data() signal before control returns back to the event loop.
 * Otherwise there is a risk of some data never being received by the
 * application.
 *
 * The class is <strong>read-only</strong> by users of %QHttpServer.
 */
class QHttpRequest : public QObject
{
    Q_OBJECT

public:
    virtual ~QHttpRequest();

    /*!
     * The method used for the request.
     *
     * \returns One of the following strings:
     * <ul>
     * <li>DELETE</li>
     * <li>GET</li>
     * <li>HEAD</li>
     * <li>POST</li>
     * <li>PUT</li>
     * <li>CONNECT</li>
     * <li>OPTIONS</li>
     * <li>TRACE</li>
     * </ul>
     */
    const QString method() const { return m_method; };

    /*!
     * The complete URL for the request. This
     * includes the path and query string.
     *
     */
    QUrl url() const { return m_url; };

    /*!
     * The path portion of the query URL.
     *
     * \sa url()
     */
    QString path() const { return m_url.path(); };

    /*!
     * The HTTP version used by the client as a 
     * 'x.x' string.
     */
    QString httpVersion() const { return m_version; };

    /*!
     * Any query string included as part of a request.
     * Usually used to send data in a GET request.
     */
    QString queryString() const;

signals:
    /*!
     * This signal is emitted whenever body data is encountered
     * in a message.
     * This may be emitted zero or more times.
     */
    void data(const QByteArray &);

    /*!
     * Emitted at the end of the HTTP request.
     * No data() signals will be emitted after this.
     */
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
