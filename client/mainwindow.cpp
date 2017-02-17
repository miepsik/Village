#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qmessagebox.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <fstream>
#include <iostream>
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
    QPixmap wood("../client/pics/Wood_Icon.png");
    QPixmap food("../client/pics/food.png");
    QPixmap wall("../client/pics/wall.png");
    ui->woodIcon->setPixmap(wood);
    ui->foodIcon->setPixmap(food);
    ui->wallIcon->setPixmap(wall);
    playerID = "-1";
    connectTcp();
}

void MainWindow::connectTcp()
{
    int port;
    std::string line,Add;
    std::ifstream input("../config");
    while( std::getline(input,line))
    {
        if(line.length()==4)
            port = std::atoi(line.c_str());
        else
            Add = line;
    }
    QByteArray data;
    data = "HELLOe";
    pSocket = new QTcpSocket( this );
    connect( pSocket, SIGNAL(readyRead()), SLOT(readTcpData()) );
    pSocket->connectToHost(Add.c_str(), port);
    if( pSocket->waitForConnected() ) {
        pSocket->write( data );
    }
}

void MainWindow::readTcpData()
{
    QByteArray data = pSocket->readAll();
    QByteArray temp = "",res = " Game finished! You ", t="";;
    QMessageBox msgBox;
    int xx,i;

    if (playerID == "-1"){
        for(int i=1;i<data.size()-2;i++)
            temp=temp+data[i];
        QList<QByteArray> x = temp.split(' ');
        playerID = x[0];
        ui->woodSpeed->setText(x[1]);
        ui->foodSpeed->setText(x[2]);
        ui->archer_wcost->setText(x[3]);
        ui->archer_fcost->setText(x[4]);
        ui->spear_wcost->setText(x[5]);
        ui->spear_fcost->setText(x[6]);
        ui->wood_wcost->setText(x[7]);
        ui->wood_fcost->setText(x[8]);
        ui->food_wcost->setText(x[9]);
        ui->food_fcost->setText(x[10]);
        ui->wall_wcost->setText(x[11]);
        ui->wall_fcost->setText(x[12]);
        ui->wallLvl->setText("1");
        temp="";
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
        if(data.contains("-1"))
        {
            msgBox.setWindowTitle("Error!");
            msgBox.setText("Action could not be completed.");
            msgBox.exec();
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
                    for(i=1;i<data.size()-1;i++) {
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
                    for(int i=2;i<data.size()-1;i++)
                        temp=temp+data[i];
                    QList<QByteArray> pieces = temp.split(' ');
                    if(data[1]=='w')
                    {
                        ui->woodSpeed->setText(pieces[0]);
                        ui->wood_wcost->setText(pieces[1]);
                        ui->wood_fcost->setText(pieces[2]);
                    }
                    if(data[1]=='f')
                    {
                        ui->foodSpeed->setText(pieces[0]);
                        ui->food_wcost->setText(pieces[1]);
                        ui->food_fcost->setText(pieces[2]);
                    }
                    if(data[1]=='d')
                    {
                        ui->wallLvl->setText(pieces[0]);
                        ui->wall_wcost->setText(pieces[1]);
                        ui->wall_fcost->setText(pieces[2]);
                    }
                };break;
                case 'r' :
                {
                    for(int i=2;i<data.size()-1;i++)
                        temp=temp+data[i];
                    if(data[1]=='a') ui->archersNumber->setText(temp);
                        else ui->spearmenNumber->setText(temp);
                };break;
                case 'a' :
                {
                    for(int i=1;i<data.size()-1;i++)
                        temp=temp+data[i];
                    QList<QByteArray> pieces = temp.split(' ');
                    foreach( const QByteArray &piece, pieces)
                    {
                        ui->availableTarget->addItem(piece);
                    }
                };break;
                case 'b' :
                {
                    for(int i=1;i<data.size()-2;i++)
                        temp=temp+data[i];
                    QList<QByteArray> pieces = temp.split(' ');
                    ui->woodAmmount->setText(pieces[1]);
                    ui->foodAmmount->setText(pieces[2]);
                    ui->archersNumber->setText(pieces[3]);
                    ui->spearmenNumber->setText(pieces[4]);
                    t=pieces[0];
                    temp = "You were attacked by player " + t + ". Your resources have been updated.";
                    msgBox.setWindowTitle("Attacked!");
                    msgBox.setText(temp);
                    msgBox.exec();
                };break;
                case 'h' :
                {
                    for(int i=1;i<data.size()-1;i++)
                        temp=temp+data[i];
                    QList<QByteArray> pieces = temp.split(' ');
                    ui->archersNumber->setText(pieces[0]);
                    ui->spearmenNumber->setText(pieces[1]);
                    ui->woodAmmount->setText(pieces[3]);
                    ui->foodAmmount->setText(pieces[4]);
                    msgBox.setWindowTitle("Attack successful!");
                    res = "Attack successful! You now have " + pieces[2] + " points.";
                    msgBox.setText(res);
                    msgBox.exec();
                };break;
                case 's' :
                {
                    for(int i=1;i<data.size()-1;i++)
                        temp=temp+data[i];
                    QList<QByteArray> pieces = temp.split(' ');
                    ui->archersNumber->setText(pieces[0]);
                    ui->spearmenNumber->setText(pieces[1]);
                };break;
            }
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

void MainWindow::on_wallUpgrade_clicked()
{
    pSocket->write("ude");
}

void MainWindow::on_archerRec_clicked()
{
    if(ui->aRecruiting->toPlainText().length()!=0)
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
