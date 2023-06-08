#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "operations.h"

#include <QTcpSocket>
#include <QMessageBox>
#include <memory>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , server(new QTcpServer())
    , validPort(new QRegularExpressionValidator(QRegularExpression("^(\\d{4})$"), this))
{
    ui->setupUi(this);
    ui->linePort->setValidator(validPort);

    QObject::connect(server, &QTcpServer::newConnection, this, [this](){
        QTcpSocket *socket = std::move(server->nextPendingConnection());

        if (!socket->waitForReadyRead(5000)) {
            QMessageBox::warning(nullptr, "Warning", socket->errorString());
            return;
        }

        appendResult(">> Client connected");

        QByteArray data { socket->readAll() };
        appendResult(">> Data received:\n" + data);

        QVector<double> numbers { dataToNumberVector(data) };
        double average { arithmeticMean(numbers) };
        double deviation { squareDeviation(numbers) };

        QString message {};
        message += "The arithmetic mean is: " + QString::number(average, 'f', 2);
        message += "\nThe square deviation is: " + QString::number(deviation, 'f', 2);

        socket->write(message.toUtf8());
        socket->flush();

        appendResult(">> Data sent:\n" + message);

        socket->deleteLater();
        socket->disconnectFromHost();
        socket->close();
        delete socket;

        appendResult(">> Client disconnected");
    });

    QObject::connect(ui->buttonServer, &QPushButton::clicked, this, [this](){
        QString port { ui->linePort->text() };

        if (ui->buttonServer->text() == "Start") {
            if (!server->listen(QHostAddress::AnyIPv4, port.toInt())) {
                ui->plainTextResult->setPlainText(">> Server could not start!");
            } else {
                ui->plainTextResult->setPlainText(">> Server started and listening on port " + port);
                ui->buttonServer->setText("Stop");
            }
        } else {
            ui->buttonServer->setText("Start");
            appendResult(">> Server stopped!");
            server->close();
        }
    });
}

MainWindow::~MainWindow()
{
    delete server;
    delete validPort;
    delete ui;
}

void MainWindow::appendResult(QString str) {
    ui->plainTextResult->appendPlainText(str);
}
