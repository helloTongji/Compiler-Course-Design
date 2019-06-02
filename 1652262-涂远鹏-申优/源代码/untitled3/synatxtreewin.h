#ifndef SYNATXTREEWIN_H
#define SYNATXTREEWIN_H

#include <QTreeWidget>
#include <QWidget>

namespace Ui {
class SynatxTreeWin;
}

class SynatxTreeWin : public QWidget
{
    Q_OBJECT

public:
    explicit SynatxTreeWin(QWidget *parent = 0);
    ~SynatxTreeWin();
    QList<QTreeWidgetItem *> rootList;
private:
    Ui::SynatxTreeWin *ui;
};

#endif // SYNATXTREEWIN_H
