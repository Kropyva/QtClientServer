#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>
#include <QTcpSocket>
#include <QFile>
#include <QTextStream>
#include <QDebug>

static void setFile(const QString& text) {
    QString path("./numbers.txt");
    QFile file(path);

    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream stream(&file);
        stream << text;
        file.close();
    }
}
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , validNumber(new QRegularExpressionValidator(QRegularExpression("-?\\d+(.\\d{2})"), this))
    , validAddress(new QRegularExpressionValidator(QRegularExpression("^(\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}\\.\\d{1,3})$"), this))
    , validPort(new QRegularExpressionValidator(QRegularExpression("^(\\d{4})$"), this))
{
    ui->setupUi(this);

    ui->lineNumber->setValidator(validNumber);
    ui->lineAddress->setValidator(validAddress);
    ui->linePort->setValidator(validPort);

    QString path("./numbers.txt");
    QFile file(path);

    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream stream(&file);
        ui->textVector->setText(stream.readAll());

        file.close();
    }

    QObject::connect(ui->buttonCalculate, &QPushButton::clicked, this, [this](){
        QTcpSocket socket {};

        socket.connectToHost(ui->lineAddress->text(), ui->linePort->text().toInt());

        if (!socket.waitForConnected(5000)) {
            QMessageBox::warning(nullptr, "Warning", socket.errorString());
            return;
        }

        QString data { ui->textVector->toPlainText() };
        socket.write(data.toUtf8());
        socket.flush();

        if (!socket.waitForReadyRead(5000)) {
            QMessageBox::warning(nullptr, "Warning", "Cannot receive a response from the server");
            return;
        }

        QByteArray response { socket.readAll() };
        ui->plainTextResult->setPlainText(QString::fromUtf8(response));

        socket.close();
    });

    QObject::connect(ui->buttonPush, &QPushButton::clicked, this, [this](){
        if (ui->lineNumber->text().size() > 0) {
            QString number(ui->lineNumber->text());

            ui->textVector->append(number);
            setFile(ui->textVector->toPlainText());
            ui->lineNumber->clear();
        }
    });

    QObject::connect(ui->buttonPop, &QPushButton::clicked, this, [this](){
        QString text(ui->textVector->toPlainText());
        int index(text.lastIndexOf("\n"));

        text.chop(text.length() - index);
        ui->textVector->setPlainText(text);
        setFile(text);
    });
}

MainWindow::~MainWindow()
{
    delete validNumber;
    delete validAddress;
    delete validPort;
    delete ui;
}
