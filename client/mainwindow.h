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

    ~MainWindow();

private:
    Ui::MainWindow *ui;


public slots:
    void readTcpData();

private slots:
    void on_pushButton_clicked();
};

#endif // MAINWINDOW_H
