#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <QTcpSocket>
#include <arpa/inet.h>



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QPixmap wood("/media/miebs/Dane1/Uczelnia/semestr5/sk/village/Village/client/pics/Wood_Icon.png");
    QPixmap food("/media/miebs/Dane1/Uczelnia/semestr5/sk/village/Village/client/pics/food.png");
    ui->woodIcon->setPixmap(wood);
    ui->foodIcon->setPixmap(food);
    playerID = "-1";
    connectTcp();
}

void MainWindow::connectTcp()
{
    QByteArray data; // <-- fill with data

    pSocket = new QTcpSocket( this );
    connect( pSocket, SIGNAL(readyRead()), SLOT(readTcpData()) );

    pSocket->connectToHost("127.0.0.1", 1234);
    if( pSocket->waitForConnected() ) {
        pSocket->write( data );
    }
}

void MainWindow::readTcpData()
{
    QByteArray data = pSocket->readAll();
    if (playerID == "-1") {
        playerID = data;
        ui->woodCost->append(playerID + "\n");
    } else {
        ui->woodAmmount->setText(data);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    pSocket->write(" wood");
}
