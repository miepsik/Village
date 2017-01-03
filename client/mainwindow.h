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

    ~MainWindow();

private:
    Ui::MainWindow *ui;


public slots:
    void readTcpData();

};

#endif // MAINWINDOW_H
