#include "qhttpserverfwd.h"

#include <QObject>

/// BodyData

class BodyData : public QObject
{
    Q_OBJECT

public:
    BodyData();

private slots:
    void handleRequest(QHttpRequest *req, QHttpResponse *resp);
};

/// Responder

class Responder : public QObject
{
    Q_OBJECT

public:
    Responder(QHttpRequest *req, QHttpResponse *resp);
    ~Responder();

signals:
    void done();

private slots:
    void accumulate(const QByteArray &data);
    void reply();

private:
    QHttpRequest *m_req;
    QHttpResponse *m_resp;
};
