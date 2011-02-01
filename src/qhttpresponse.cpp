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

#include "qhttpresponse.h"

#include "qhttpserver.h"
#include "qhttpconnection.h"

QHttpResponse::QHttpResponse(QHttpConnection *connection)
    // TODO: parent child relation
    : QObject(0)
    , m_connection(connection)
    , m_headerWritten(false)
{
}

QHttpResponse::~QHttpResponse()
{
}

void QHttpResponse::setHeader(const QString &field, const QString &value)
{
    m_headers[field] = value;
}

void QHttpResponse::writeHead(int status)
{
    if( m_headerWritten ) return;

    m_connection->write(QString("HTTP/1.1 %1 %2\r\n").arg(status).arg(STATUS_CODES[status]).toAscii());
    
    foreach(QString name, m_headers.keys())
    {
        m_connection->write(name.toAscii());
        m_connection->write(": ");
        m_connection->write(m_headers[name].toAscii());
        m_connection->write("\r\n");
    }

    m_connection->write("\r\n");
    m_headerWritten = true;
}

void QHttpResponse::write(const QByteArray &data)
{
    if( !m_headerWritten )
        writeHead(200);

    m_connection->write(data);
}

void QHttpResponse::write(const QString &data)
{
    m_connection->write(data.toAscii());
}

void QHttpResponse::end(const QByteArray &data)
{
    write(data);

    deleteLater();
    // TODO: end connection and delete ourselves
}
