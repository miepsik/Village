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
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPlainTextEdit>
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
    QGroupBox *resourcesBox;
    QPushButton *foodUpgrade;
    QPushButton *woodUpgrade;
    QTextEdit *woodCost;
    QTextEdit *foodCost;
    QTextEdit *woodSpeed;
    QTextEdit *woodAmmount;
    QTextEdit *foodAmmount;
    QTextEdit *foodSpeed;
    QLabel *foodIcon;
    QLabel *woodIcon;
    QGroupBox *armyBox;
    QTextEdit *aRecruiting;
    QLabel *archerLabel;
    QPushButton *archerRec;
    QLabel *archersNumber;
    QLabel *spearmanLabel;
    QTextEdit *sRecruiting;
    QPushButton *spearRec;
    QLabel *spearmenNumber;
    QGroupBox *groupBox;
    QPushButton *attackButton;
    QLabel *targetsLabel;
    QLabel *armyLabel;
    QComboBox *availableTarget;
    QPlainTextEdit *archerAttack;
    QPlainTextEdit *spearAttack;
    QLabel *label_3;
    QLabel *label_4;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(535, 433);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        resourcesBox = new QGroupBox(centralWidget);
        resourcesBox->setObjectName(QStringLiteral("resourcesBox"));
        resourcesBox->setGeometry(QRect(10, 0, 491, 191));
        resourcesBox->setFlat(true);
        foodUpgrade = new QPushButton(resourcesBox);
        foodUpgrade->setObjectName(QStringLiteral("foodUpgrade"));
        foodUpgrade->setGeometry(QRect(300, 120, 80, 31));
        woodUpgrade = new QPushButton(resourcesBox);
        woodUpgrade->setObjectName(QStringLiteral("woodUpgrade"));
        woodUpgrade->setGeometry(QRect(300, 40, 80, 31));
        woodCost = new QTextEdit(resourcesBox);
        woodCost->setObjectName(QStringLiteral("woodCost"));
        woodCost->setGeometry(QRect(390, 40, 61, 31));
        woodCost->setReadOnly(true);
        foodCost = new QTextEdit(resourcesBox);
        foodCost->setObjectName(QStringLiteral("foodCost"));
        foodCost->setGeometry(QRect(390, 120, 61, 31));
        foodCost->setReadOnly(true);
        woodSpeed = new QTextEdit(resourcesBox);
        woodSpeed->setObjectName(QStringLiteral("woodSpeed"));
        woodSpeed->setGeometry(QRect(120, 40, 61, 31));
        woodSpeed->setReadOnly(true);
        woodAmmount = new QTextEdit(resourcesBox);
        woodAmmount->setObjectName(QStringLiteral("woodAmmount"));
        woodAmmount->setGeometry(QRect(190, 40, 81, 31));
        woodAmmount->setReadOnly(true);
        foodAmmount = new QTextEdit(resourcesBox);
        foodAmmount->setObjectName(QStringLiteral("foodAmmount"));
        foodAmmount->setGeometry(QRect(190, 120, 81, 31));
        foodAmmount->setReadOnly(true);
        foodSpeed = new QTextEdit(resourcesBox);
        foodSpeed->setObjectName(QStringLiteral("foodSpeed"));
        foodSpeed->setGeometry(QRect(120, 120, 61, 31));
        foodSpeed->setReadOnly(true);
        foodIcon = new QLabel(resourcesBox);
        foodIcon->setObjectName(QStringLiteral("foodIcon"));
        foodIcon->setGeometry(QRect(20, 110, 61, 61));
        woodIcon = new QLabel(resourcesBox);
        woodIcon->setObjectName(QStringLiteral("woodIcon"));
        woodIcon->setGeometry(QRect(20, 30, 61, 61));
        armyBox = new QGroupBox(centralWidget);
        armyBox->setObjectName(QStringLiteral("armyBox"));
        armyBox->setGeometry(QRect(20, 190, 171, 171));
        aRecruiting = new QTextEdit(armyBox);
        aRecruiting->setObjectName(QStringLiteral("aRecruiting"));
        aRecruiting->setGeometry(QRect(20, 60, 51, 31));
        archerLabel = new QLabel(armyBox);
        archerLabel->setObjectName(QStringLiteral("archerLabel"));
        archerLabel->setGeometry(QRect(10, 30, 58, 16));
        archerRec = new QPushButton(armyBox);
        archerRec->setObjectName(QStringLiteral("archerRec"));
        archerRec->setGeometry(QRect(80, 60, 71, 31));
        archersNumber = new QLabel(armyBox);
        archersNumber->setObjectName(QStringLiteral("archersNumber"));
        archersNumber->setGeometry(QRect(80, 30, 58, 16));
        spearmanLabel = new QLabel(armyBox);
        spearmanLabel->setObjectName(QStringLiteral("spearmanLabel"));
        spearmanLabel->setGeometry(QRect(10, 100, 71, 16));
        sRecruiting = new QTextEdit(armyBox);
        sRecruiting->setObjectName(QStringLiteral("sRecruiting"));
        sRecruiting->setGeometry(QRect(20, 130, 51, 31));
        spearRec = new QPushButton(armyBox);
        spearRec->setObjectName(QStringLiteral("spearRec"));
        spearRec->setGeometry(QRect(80, 130, 71, 31));
        spearmenNumber = new QLabel(armyBox);
        spearmenNumber->setObjectName(QStringLiteral("spearmenNumber"));
        spearmenNumber->setGeometry(QRect(80, 100, 58, 16));
        groupBox = new QGroupBox(centralWidget);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setGeometry(QRect(230, 190, 281, 171));
        attackButton = new QPushButton(groupBox);
        attackButton->setObjectName(QStringLiteral("attackButton"));
        attackButton->setGeometry(QRect(180, 110, 81, 31));
        attackButton->setAutoDefault(false);
        targetsLabel = new QLabel(groupBox);
        targetsLabel->setObjectName(QStringLiteral("targetsLabel"));
        targetsLabel->setGeometry(QRect(10, 30, 111, 16));
        armyLabel = new QLabel(groupBox);
        armyLabel->setObjectName(QStringLiteral("armyLabel"));
        armyLabel->setGeometry(QRect(10, 60, 101, 16));
        availableTarget = new QComboBox(groupBox);
        availableTarget->setObjectName(QStringLiteral("availableTarget"));
        availableTarget->setGeometry(QRect(120, 30, 141, 26));
        availableTarget->setEditable(false);
        archerAttack = new QPlainTextEdit(groupBox);
        archerAttack->setObjectName(QStringLiteral("archerAttack"));
        archerAttack->setGeometry(QRect(30, 90, 61, 31));
        spearAttack = new QPlainTextEdit(groupBox);
        spearAttack->setObjectName(QStringLiteral("spearAttack"));
        spearAttack->setGeometry(QRect(30, 130, 61, 31));
        label_3 = new QLabel(groupBox);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(100, 100, 58, 16));
        label_4 = new QLabel(groupBox);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(100, 140, 58, 16));
        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 535, 24));
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
        resourcesBox->setTitle(QApplication::translate("MainWindow", "Resources", Q_NULLPTR));
        foodUpgrade->setText(QApplication::translate("MainWindow", "Upgrade", Q_NULLPTR));
        woodUpgrade->setText(QApplication::translate("MainWindow", "Upgrade", Q_NULLPTR));
        foodIcon->setText(QApplication::translate("MainWindow", "TextLabel", Q_NULLPTR));
        woodIcon->setText(QApplication::translate("MainWindow", "TextLabel", Q_NULLPTR));
        armyBox->setTitle(QApplication::translate("MainWindow", "Army", Q_NULLPTR));
        aRecruiting->setPlaceholderText(QString());
        archerLabel->setText(QApplication::translate("MainWindow", "Archers:", Q_NULLPTR));
        archerRec->setText(QApplication::translate("MainWindow", "Recruit", Q_NULLPTR));
        archersNumber->setText(QApplication::translate("MainWindow", "0", Q_NULLPTR));
        spearmanLabel->setText(QApplication::translate("MainWindow", "Spearmen:", Q_NULLPTR));
        spearRec->setText(QApplication::translate("MainWindow", "Recruit", Q_NULLPTR));
        spearmenNumber->setText(QApplication::translate("MainWindow", "0", Q_NULLPTR));
        groupBox->setTitle(QApplication::translate("MainWindow", "Attack Panel", Q_NULLPTR));
        attackButton->setText(QApplication::translate("MainWindow", "Attack!", Q_NULLPTR));
        targetsLabel->setText(QApplication::translate("MainWindow", "Available targets:", Q_NULLPTR));
        armyLabel->setText(QApplication::translate("MainWindow", "Attacking army:", Q_NULLPTR));
        label_3->setText(QApplication::translate("MainWindow", "A", Q_NULLPTR));
        label_4->setText(QApplication::translate("MainWindow", "S", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
