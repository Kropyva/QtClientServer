#include "mainwindow.h"
#include "ui_mainwindow.h"

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
}

MainWindow::~MainWindow()
{
    delete regExp;
    delete ui;
}

