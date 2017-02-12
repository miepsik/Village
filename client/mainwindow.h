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
    void on_attackButton_clicked();
    void on_woodUpgrade_clicked();
    void on_foodUpgrade_clicked();
    void on_archerRec_clicked();
    void on_spearRec_clicked();
    void on_wallUpgrade_clicked();
};

#endif // MAINWINDOW_H
