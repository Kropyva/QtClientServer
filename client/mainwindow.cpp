#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QTcpSocket>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , validNumber(new QRegularExpressionValidator(QRegularExpression("^(-?\\d+(.\\d{2}))$"), this))
    , validAddress(new QRegularExpressionValidator(QRegularExpression("^(\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}\\.\\d{1,3})$"), this))
    , validPort(new QRegularExpressionValidator(QRegularExpression("^(\\d{4})$"), this))
{
    ui->setupUi(this);

    ui->lineNumber->setValidator(validNumber);
    ui->lineAddress->setValidator(validAddress);
    ui->linePort->setValidator(validPort);

    ui->lineNumber->setPlaceholderText("Number");
    ui->lineAddress->setPlaceholderText("IPv4");
    ui->linePort->setPlaceholderText("Port");

    ui->textVector->setPlaceholderText("Values");
    ui->plainTextResult->setPlaceholderText("Result");

    QObject::connect(ui->buttonCalculate, &QPushButton::clicked, this, [this](){
        QTcpSocket socket {};

        socket.connectToHost("localhost", 8888);

        if (!socket.waitForConnected()) {
            qDebug() << "Failed ot connect to server: " << socket.errorString();
            return;
        }

        QString data { ui->textVector->toPlainText() };
        socket.write(data.toUtf8());
        socket.flush();

        if (!socket.waitForReadyRead(10000)) {
            qDebug() << "Cannot receive a response from server";
            return;
        }

        QByteArray response = socket.readAll();
        ui->plainTextResult->setPlainText(QString::fromUtf8(response));

        socket.close();
    });
}

MainWindow::~MainWindow()
{
    delete validNumber;
    delete validAddress;
    delete validPort;
    delete ui;
}

