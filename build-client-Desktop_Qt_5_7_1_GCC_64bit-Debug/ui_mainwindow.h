/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.7.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QTextEdit *woodSpeed;
    QPushButton *pushButton;
    QLabel *woodIcon;
    QLabel *foodIcon;
    QTextEdit *foodSpeed;
    QTextEdit *foodAmmount;
    QTextEdit *woodAmmount;
    QPushButton *woodUpgrade;
    QPushButton *foodUpgrade;
    QTextEdit *foodCost;
    QTextEdit *woodCost;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(487, 357);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        woodSpeed = new QTextEdit(centralWidget);
        woodSpeed->setObjectName(QStringLiteral("woodSpeed"));
        woodSpeed->setGeometry(QRect(90, 20, 61, 31));
        woodSpeed->setReadOnly(true);
        pushButton = new QPushButton(centralWidget);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(150, 200, 80, 22));
        woodIcon = new QLabel(centralWidget);
        woodIcon->setObjectName(QStringLiteral("woodIcon"));
        woodIcon->setGeometry(QRect(10, 10, 61, 61));
        foodIcon = new QLabel(centralWidget);
        foodIcon->setObjectName(QStringLiteral("foodIcon"));
        foodIcon->setGeometry(QRect(10, 90, 61, 61));
        foodSpeed = new QTextEdit(centralWidget);
        foodSpeed->setObjectName(QStringLiteral("foodSpeed"));
        foodSpeed->setGeometry(QRect(90, 100, 61, 31));
        foodSpeed->setReadOnly(true);
        foodAmmount = new QTextEdit(centralWidget);
        foodAmmount->setObjectName(QStringLiteral("foodAmmount"));
        foodAmmount->setGeometry(QRect(160, 100, 81, 31));
        foodAmmount->setReadOnly(true);
        woodAmmount = new QTextEdit(centralWidget);
        woodAmmount->setObjectName(QStringLiteral("woodAmmount"));
        woodAmmount->setGeometry(QRect(160, 20, 81, 31));
        woodAmmount->setReadOnly(true);
        woodUpgrade = new QPushButton(centralWidget);
        woodUpgrade->setObjectName(QStringLiteral("woodUpgrade"));
        woodUpgrade->setGeometry(QRect(260, 20, 80, 31));
        foodUpgrade = new QPushButton(centralWidget);
        foodUpgrade->setObjectName(QStringLiteral("foodUpgrade"));
        foodUpgrade->setGeometry(QRect(260, 100, 80, 31));
        foodCost = new QTextEdit(centralWidget);
        foodCost->setObjectName(QStringLiteral("foodCost"));
        foodCost->setGeometry(QRect(350, 100, 61, 31));
        foodCost->setReadOnly(true);
        woodCost = new QTextEdit(centralWidget);
        woodCost->setObjectName(QStringLiteral("woodCost"));
        woodCost->setGeometry(QRect(350, 20, 61, 31));
        woodCost->setReadOnly(true);
        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 487, 19));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", Q_NULLPTR));
        pushButton->setText(QApplication::translate("MainWindow", "PushButton", Q_NULLPTR));
        woodIcon->setText(QApplication::translate("MainWindow", "TextLabel", Q_NULLPTR));
        foodIcon->setText(QApplication::translate("MainWindow", "TextLabel", Q_NULLPTR));
        woodUpgrade->setText(QApplication::translate("MainWindow", "Upgrade", Q_NULLPTR));
        foodUpgrade->setText(QApplication::translate("MainWindow", "Upgrade", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
