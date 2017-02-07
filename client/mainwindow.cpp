#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qmessagebox.h"
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
    QByteArray temp = "",res=" Game finished! You ";
    QMessageBox msgBox;
    int xx,i;
    if (playerID == "-1"){
        QList<QByteArray> x = data.remove(0, 1).split(' ');
        playerID = x[0];
        //playerID.remove(playerID.length()-1); //deleting the newline - unnecessary if not sent
    }
    else if(data.contains("WINNER"))
    {
        for(int i=6;i<data.size()-2;i++)
            temp=temp+data[i];
        if(temp == playerID)
            res = res+"win!";
        else
            res = res+"lose.\nThe winner is player " + temp;
        msgBox.setWindowTitle("Game finished!");
        msgBox.setText(res);
        msgBox.exec();
        if(msgBox.Accepted==true)
            qApp->exit();
    }
    else
    {
        switch(data[0])
        {
            case 'x' :
            {
                i=0;
                while(data[i]!= ' ') i++;
                xx = i;
                for(i=1;i<data.size()-2;i++) {
                    if (i==xx) {
                        ui->woodAmmount->setText(temp);
                        temp = "";
                    }
                    else
                        temp=temp+data[i];
                }
                ui->foodAmmount->setText(temp);
            };break;
            case 'u' :
            {
                for(int i=2;i<data.size()-2;i++)        //when sending with endline - if not, change to -2 to -1
                    temp=temp+data[i];
                if(data[1]=='w')
                    ui->woodSpeed->setText(temp);
                else
                    ui->foodSpeed->setText(temp);
            };break;
            case 'r' :
            {
                for(int i=2;i<data.size()-2;i++)
                    temp=temp+data[i];
                if(data[1]=='a') ui->archersNumber->setText(temp);
                    else ui->spearmenNumber->setText(temp);
            };break;
            case 'a' :
            {
                for(int i=1;i<data.size()-2;i++)
                    temp=temp+data[i];
                QList<QByteArray> pieces = temp.split(' ');
                foreach( const QByteArray &piece, pieces)
                {
                    ui->availableTarget->addItem(piece);
                }
            };break;
            case 'h' :
            {
                for(int i=1;i<data.size()-2;i++)
                    temp=temp+data[i];
                QList<QByteArray> pieces = temp.split(' ');
                ui->archersNumber->setText(pieces[0]);
                ui->spearmenNumber->setText(pieces[1]);
                ui->woodAmmount->setText(pieces[3]);
                ui->foodAmmount->setText(pieces[4]);
            };break;
        }
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
    else if((ui->archerAttack->toPlainText().length()>0)&&(ui->spearAttack->toPlainText().length()>0))
    {
        QByteArray data = "s " + playerID + " " + ui->availableTarget->currentText().toLatin1() + " " + ui->archerAttack->toPlainText().toLatin1() + " " + ui->spearAttack->toPlainText().toLatin1() + "e";
        pSocket->write(data);
		ui->availableTarget->clear();
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
    if(ui->aRecruiting->toPlainText().length()!=0) //doesn't send when field is empty
    {
        QByteArray data = "ra" + ui->aRecruiting->toPlainText().toLatin1() + "e";
        pSocket->write( data );
        ui->aRecruiting->clear();
    }
}

void MainWindow::on_spearRec_clicked()
{
    if(ui->sRecruiting->toPlainText().length()!=0)
    {
        QByteArray data = "rs" + ui->sRecruiting->toPlainText().toLatin1() + "e";
        pSocket->write( data );
        ui->sRecruiting->clear();
    }
}
