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
QByteArray processing="";

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QPixmap wood("/home/cranberry/Desktop/Village/client/pics/Wood_Icon.png");
    QPixmap food("/home/cranberry/Desktop/Village/client/pics/food.png");
    QPixmap wall("/home/cranberry/Desktop/Village/client/pics/wall.png");
    ui->woodIcon->setPixmap(wood);
    ui->foodIcon->setPixmap(food);
    ui->wallIcon->setPixmap(wall);
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
    QByteArray data = ""; // = pSocket->readAll();
    QByteArray temp = "",res = " Game finished! You ", t="";;
    QMessageBox msgBox;
    int xx,i,ind;

    /*
    if (playerID == "-1"){
        QList<QByteArray> x = data.remove(0, 1).split(' ');
        playerID = x[0];
        //playerID.remove(playerID.length()-1); //deleting the newline - unnecessary if not sent
    }
    */

    processing+=pSocket->readAll();
    if(processing.contains("\n"))
    {
        ind=processing.indexOf("\n");
        temp=processing.left(ind);
        temp.append(processing.mid(ind+1));
        processing=temp;
    }
    temp="";
    if(processing.contains("e"))
    {
        ind=processing.indexOf("e");
        data=processing.left(ind+1);
        temp.append(processing.mid(ind+1));
        processing=temp;
    }
    if ((playerID == "-1") && (data[0]=='h')){
        i=0;
        while(data[i]!= 'e') i++;
        xx = i;
        for(i=1;i<xx;i++)
            temp=temp+data[i];
        QList<QByteArray> pieces = temp.split(' ');
        playerID = pieces[0];
        ui->woodSpeed->setText(pieces[1]);
        ui->foodSpeed->setText(pieces[2]);
        ui->archer_wcost->setText(pieces[3]);
        ui->archer_fcost->setText(pieces[4]);
        ui->spear_wcost->setText(pieces[5]);
        ui->spear_fcost->setText(pieces[6]);
        ui->wood_wcost->setText(pieces[7]);
        ui->wood_fcost->setText(pieces[8]);
        ui->food_wcost->setText(pieces[9]);
        ui->food_fcost->setText(pieces[10]);
        ui->wall_wcost->setText(pieces[11]);
        ui->wall_fcost->setText(pieces[12]);
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
