#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QRegularExpressionValidator>
#include <QTcpServer>
#include <QTcpSocket>
#include <memory>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    void appendResult(QString str);

private:
    Ui::MainWindow *ui;
    QTcpServer *server;
    QTcpSocket *socket;
    QRegularExpressionValidator *validPort;
};

#endif // MAINWINDOW_H
