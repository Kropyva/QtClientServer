#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QTcpSocket>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , regExp(new QRegularExpression("^((\\d+(\\s?))+)$"))
{
    ui->setupUi(this);

    QObject::connect(ui->textVector, &QTextEdit::textChanged, this, [this](){
        QString text { ui->textVector->toPlainText() };
        QRegularExpressionMatch match { regExp->match(text)};

        if (!match.hasMatch()) {
            QTextCursor cursor = ui->textVector->textCursor();

            cursor.movePosition(QTextCursor::End);
            cursor.movePosition(QTextCursor::Left, QTextCursor::KeepAnchor);
            cursor.removeSelectedText();
        }
    });

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

        if (!socket.waitForReadyRead()) {
            qDebug() << "Cannot receive a response from server";
            return;
        }

        QByteArray response = socket.readAll();
        qDebug() << "Response from server:" << response;

        socket.close();
    });
}

MainWindow::~MainWindow()
{
    delete regExp;
    delete ui;
}

