/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDockWidget>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QGroupBox *groupBox;
    QPlainTextEdit *SourceCode;
    QGroupBox *groupBox_2;
    QTextBrowser *AnalysisResult;
    QGroupBox *groupBox_3;
    QTextBrowser *CodeDisplay;
    QTableWidget *tableWidget;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;
    QDockWidget *SyntaxAnalysisWin;
    QWidget *dockWidgetContents_8;
    QDockWidget *dockWidget_4;
    QWidget *dockWidgetContents_9;
    QDockWidget *dockWidget_2;
    QWidget *dockWidgetContents_11;
    QDockWidget *dockWidget_3;
    QWidget *dockWidgetContents;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(964, 888);
        QFont font;
        font.setFamily(QString::fromUtf8("\345\271\274\345\234\206"));
        MainWindow->setFont(font);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        groupBox = new QGroupBox(centralWidget);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setGeometry(QRect(10, 20, 391, 611));
        groupBox->setFont(font);
        SourceCode = new QPlainTextEdit(groupBox);
        SourceCode->setObjectName(QStringLiteral("SourceCode"));
        SourceCode->setGeometry(QRect(10, 20, 371, 581));
        groupBox_2 = new QGroupBox(centralWidget);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        groupBox_2->setGeometry(QRect(10, 630, 391, 151));
        AnalysisResult = new QTextBrowser(groupBox_2);
        AnalysisResult->setObjectName(QStringLiteral("AnalysisResult"));
        AnalysisResult->setGeometry(QRect(10, 20, 371, 121));
        groupBox_3 = new QGroupBox(centralWidget);
        groupBox_3->setObjectName(QStringLiteral("groupBox_3"));
        groupBox_3->setGeometry(QRect(410, 20, 531, 761));
        CodeDisplay = new QTextBrowser(groupBox_3);
        CodeDisplay->setObjectName(QStringLiteral("CodeDisplay"));
        CodeDisplay->setGeometry(QRect(10, 20, 511, 721));
        CodeDisplay->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        CodeDisplay->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        CodeDisplay->setReadOnly(false);
        tableWidget = new QTableWidget(groupBox_3);
        tableWidget->setObjectName(QStringLiteral("tableWidget"));
        tableWidget->setGeometry(QRect(10, 20, 511, 721));
        tableWidget->raise();
        CodeDisplay->raise();
        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 964, 26));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);
        SyntaxAnalysisWin = new QDockWidget(MainWindow);
        SyntaxAnalysisWin->setObjectName(QStringLiteral("SyntaxAnalysisWin"));
        dockWidgetContents_8 = new QWidget();
        dockWidgetContents_8->setObjectName(QStringLiteral("dockWidgetContents_8"));
        SyntaxAnalysisWin->setWidget(dockWidgetContents_8);
        MainWindow->addDockWidget(static_cast<Qt::DockWidgetArea>(4), SyntaxAnalysisWin);
        dockWidget_4 = new QDockWidget(MainWindow);
        dockWidget_4->setObjectName(QStringLiteral("dockWidget_4"));
        dockWidgetContents_9 = new QWidget();
        dockWidgetContents_9->setObjectName(QStringLiteral("dockWidgetContents_9"));
        dockWidget_4->setWidget(dockWidgetContents_9);
        MainWindow->addDockWidget(static_cast<Qt::DockWidgetArea>(4), dockWidget_4);
        dockWidget_2 = new QDockWidget(MainWindow);
        dockWidget_2->setObjectName(QStringLiteral("dockWidget_2"));
        dockWidgetContents_11 = new QWidget();
        dockWidgetContents_11->setObjectName(QStringLiteral("dockWidgetContents_11"));
        dockWidget_2->setWidget(dockWidgetContents_11);
        MainWindow->addDockWidget(static_cast<Qt::DockWidgetArea>(4), dockWidget_2);
        dockWidget_3 = new QDockWidget(MainWindow);
        dockWidget_3->setObjectName(QStringLiteral("dockWidget_3"));
        dockWidgetContents = new QWidget();
        dockWidgetContents->setObjectName(QStringLiteral("dockWidgetContents"));
        dockWidget_3->setWidget(dockWidgetContents);
        MainWindow->addDockWidget(static_cast<Qt::DockWidgetArea>(4), dockWidget_3);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "\347\261\273C\347\250\213\345\272\217\347\274\226\350\257\221\345\231\250- 1652262 -\346\266\202\350\277\234\351\271\217", Q_NULLPTR));
        groupBox->setTitle(QApplication::translate("MainWindow", "\346\272\220\347\250\213\345\272\217", Q_NULLPTR));
        groupBox_2->setTitle(QApplication::translate("MainWindow", "\345\210\206\346\236\220\347\273\223\346\236\234", Q_NULLPTR));
        groupBox_3->setTitle(QApplication::translate("MainWindow", "\347\273\223\346\236\234", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
