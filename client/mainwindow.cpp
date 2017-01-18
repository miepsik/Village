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
    QPixmap wood("/home/cranberry/Desktop/Village/client/pics/Wood_Icon.png");
    QPixmap food("/home/cranberry/Desktop/Village/client/pics/food.png");
    ui->woodIcon->setPixmap(wood);
    ui->foodIcon->setPixmap(food);
    playerID = "-1";
    // ui->availableTarget->addItem("23"); <-- mock for checking attack sending
    connectTcp();
}

void MainWindow::connectTcp()
{
    QByteArray data;
    data = "HELLOe";
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
    pSocket->close();
    delete ui;
}

void MainWindow::on_attackButton_clicked()
{
    if(ui->availableTarget->count()==0)
        pSocket->write("ae");
    else
    {
        QByteArray data = "s " + playerID + " " + ui->availableTarget->currentText().toLatin1() + " " + ui->archerAttack->toPlainText().toLatin1() + " " + ui->spearAttack->toPlainText().toLatin1() + "e";
        pSocket->write(data);
    }
}

void MainWindow::on_woodUpgrade_clicked()
{
    pSocket->write("uwe");
}

void MainWindow::on_foodUpgrade_clicked()
{
    pSocket->write("ufe");
}

void MainWindow::on_archerRec_clicked()
{
    QByteArray data = "ra" + ui->aRecruiting->toPlainText().toLatin1() + "e";
    pSocket->write( data );
    ui->aRecruiting->clear();
}

void MainWindow::on_spearRec_clicked()
{
    QByteArray data = "rs" + ui->sRecruiting->toPlainText().toLatin1() + "e";
    pSocket->write( data );
    ui->sRecruiting->clear();
}
