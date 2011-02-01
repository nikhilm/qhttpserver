#include "qhttprequest.h"

#include "qhttpconnection.h"

QHttpRequest::QHttpRequest(QHttpConnection *connection, QObject *parent)
    : QObject(parent)
    , m_connection(connection)
{
}

QHttpRequest::~QHttpRequest()
{
}

