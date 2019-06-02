/********************************************************************************
** Form generated from reading UI file 'synatxtreewin.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SYNATXTREEWIN_H
#define UI_SYNATXTREEWIN_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SynatxTreeWin
{
public:
    QTreeWidget *treeWidget;

    void setupUi(QWidget *SynatxTreeWin)
    {
        if (SynatxTreeWin->objectName().isEmpty())
            SynatxTreeWin->setObjectName(QStringLiteral("SynatxTreeWin"));
        SynatxTreeWin->resize(615, 530);
        treeWidget = new QTreeWidget(SynatxTreeWin);
        QTreeWidgetItem *__qtreewidgetitem = new QTreeWidgetItem();
        __qtreewidgetitem->setText(0, QStringLiteral("1"));
        treeWidget->setHeaderItem(__qtreewidgetitem);
        treeWidget->setObjectName(QStringLiteral("treeWidget"));
        treeWidget->setGeometry(QRect(5, 1, 601, 521));

        retranslateUi(SynatxTreeWin);

        QMetaObject::connectSlotsByName(SynatxTreeWin);
    } // setupUi

    void retranslateUi(QWidget *SynatxTreeWin)
    {
        SynatxTreeWin->setWindowTitle(QApplication::translate("SynatxTreeWin", "Form", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class SynatxTreeWin: public Ui_SynatxTreeWin {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SYNATXTREEWIN_H
