#include "helloworld.h"

#include <QCoreApplication>
#include <QDebug>

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
    qDebug() << "Handling request";
    qDebug() << "Method" << req->method();
    qDebug() << "HTTP Version" << req->httpVersion();
    qDebug() << "URL" << req->url();
    resp->setHeader("Content-Type", "text/html");
    resp->writeHead(200);
    resp->write(QString("Hello World"));
    resp->end();
}

int main(int argc, char **argv)
{
    QCoreApplication app(argc, argv);

    Hello hello;
    
    app.exec();
}
