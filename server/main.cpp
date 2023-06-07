#include "operations.h"

#include <QCoreApplication>
#include <QTcpServer>
#include <QTcpSocket>
#include <QDebug>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QTcpServer server {};

    QObject::connect(&server, &QTcpServer::newConnection, &server, [&server](){
        QTcpSocket *socket { server.nextPendingConnection() };
        qDebug() << "New connection";

        QObject::connect(socket, &QTcpSocket::readyRead, [socket]() {
            QByteArray data = socket->readAll();
            qDebug() << "Data received:" << data;

            QVector<double> numbers { dataToNumberVector(data) };

            double average { arithmeticMean(numbers) };
            double deviation { squareDeviation(numbers) };

            QString message {};
            message += "The arithmetic mean is: " + QString::number(average, 'f', 2);
            message += "\nThe square deviation is: " + QString::number(deviation, 'f', 2);

            socket->write(message.toUtf8());
            socket->flush();
        });

        QObject::connect(socket, &QTcpSocket::disconnected, [socket]() {
            qDebug() << "Client disconnected";
            socket->deleteLater();
        });
    });

    if (!server.listen(QHostAddress::LocalHost, 8888)) {
        qDebug() << "Server could not start!";
        return -1;
    } else {
        qDebug() << "Server started and listening on port 8888";
    }

    return a.exec();
}
