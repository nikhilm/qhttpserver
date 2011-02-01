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
    m_connection->flush();
    // TODO: end connection and delete ourselves
}
