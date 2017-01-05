#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    void connectTcp();
    QTcpSocket * pSocket;
    QByteArray playerID;
    Ui::MainWindow *ui;

    ~MainWindow();


public slots:
    void readTcpData();
    void on_pushButton_clicked();
};

#endif // MAINWINDOW_H
