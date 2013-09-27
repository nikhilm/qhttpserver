#include "helloworld.h"

#include <QCoreApplication>

#include <qhttpserver.h>
#include <qhttprequest.h>
#include <qhttpresponse.h>

Hello::Hello()
{
    QHttpServer *server = new QHttpServer;
    server->listen(QHostAddress::Any, 5000);
    connect(server, SIGNAL(newRequest(QHttpRequest*, QHttpResponse*)),
            this, SLOT(handle(QHttpRequest*, QHttpResponse*)));
}

void Hello::handle(QHttpRequest *req, QHttpResponse *resp)
{
    Q_UNUSED(req);
    resp->setHeader("Content-Length", "11");
    resp->writeHead(200);
    resp->write(QByteArray("Hello World"));
    resp->end();
}

int main(int argc, char **argv)
{
    QCoreApplication app(argc, argv);
    Hello hello;
    app.exec();
}
