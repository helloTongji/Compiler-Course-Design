#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include"syntax/lexical.h"
#include "syntax/syntax.h"
#include <QMainWindow>
#include <QTableWidget>
#include<QTextStream>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    QTextStream *TextStr;
    QString PATH;




    Lexical *Lex;
    STree *STREE;


    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void Compile();
    void openFile();
    void saveFile();
    void LexicalDisplay();
    void SynatxDisplay();
    void MiddleDisplay();
    void OptMiddleDisplay();
    void SymbolTableDisplay();
    void FinalDisplay();
    void initialGlobal();

    FILE* FP(QString path);
private:
    Ui::MainWindow *ui;
};



#endif // MAINWINDOW_H
