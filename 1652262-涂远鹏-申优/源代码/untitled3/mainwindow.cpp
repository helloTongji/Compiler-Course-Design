#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "glob.h"
#include "err.h"
#include "synatxtreewin.h"
#include"syntax/lexical.h"
#include<QDebug>
#include<QMessageBox>
#include<QFile>
#include<QFileDialog>
#include<QTableWidget>
#include <QTreeWidgetItem>
#include <QScrollArea>
#include <syntax/STree.h>
#include <syntax/syntax.h>
int prllen;
const int kwdnum = 28;
char *word[] = {
    "begin", "$call", "case", "char", "const",//0~4
    "default", "do", "else", "$end", "float",  //5~9
    "for", "if", "int", "main", "$odd",//10~14
    "printf", "$procedure", "$read", "return", "$repeat",  //15~19
    "scanf", "switch", "$then", "until", "$var",//20~24
    "void", "while", "$write"//25~27
};
char *wsym[] = {
    "BEGINTK", "CALLTK", "CASETK", "CHARTK", "CONSTTK",
    "DEFAULTTK", "DOTK", "ELSETK", "ENDTK", "FLOATTK",
    "FORTK", "IFTK", "INTTK", "MAINTK", "ODDTK",
    "PRINTFTK", "PROCETK", "READTK", "RETURNTK", "RPTTK",
    "SCANFTK", "SWITCHTK", "THENTK", "UTLTK", "VARTK",
    "VOIDTK", "WHILETK", "WRITETK"
};

int ch = ' ';

int line[500], backupline[500];
int lnum = 0, cnum = 0, llen = 0;
FILE* src = NULL;
string id = "";
int num;
string sym;
double dnum;
int symid;
string prid;


int getsym() {
    prid = id;
    id = "";
    while (isspace(ch) && ch != EOF) {
        getch();
    }
    if (ch == EOF) {
        symid = -1;
        return -1;
    }
    if (isalpha(ch) || ch == '_') {
        string tempstr;
        while (isalpha(ch) || isdigit(ch) || ch == '_') {
            ch = tolower(ch);
            tempstr += ch;
            getch();
        }
        id = tempstr;
        //seatch reserved word table
        int iter = 0;
        for (iter = 0; iter < kwdnum; iter++) {
            if (stringcmp(word[iter], (char*)tempstr.c_str())) {
                sym = wsym[iter];
                symid = iter + 1;
                break;
            }
        }
        if (iter == kwdnum) {
            sym = "IDEN";
            symid = IDEN;
        }
    }
    else if (isdigit(ch)) {

        sym = "INTCON";
        symid = INTCON;
        num = 0;
        while (isdigit(ch)) {
            id += ch;
            num = num * 10 + (int)(ch - '0');
            getch();
        }
        if (ch == '.') {
            id += ch;
            sym = "FLOATCON";
            symid = -1;
            double t = 0.1;
            dnum = num;
            getch();
            while (isdigit(ch)) {
                id += ch;
                dnum = dnum + (int)(ch - '0') * t;
                t /= 10;
                getch();
            }
        }
    }
    else if (ch == '=') {
        id += ch;
        sym = "ASSIGN";
        symid = ASSIGN;
        getch();
        if (ch == '=') {
            id += ch;
            sym = "EQL";
            symid = EQL;
            getch();
        }
    }
    else if (ch == '<') {
        id += ch;
        getch();
        if (ch == '=') {
            id += ch;
            sym = "LEQ";
            symid = LEQ;
            getch();
        }
        else {
            sym = "LSS";
            symid = LSS;
        }
    }
    else if (ch == '>') {
        id += ch;
        getch();
        if (ch == '=') {
            id += ch;
            sym = "GEQ";
            symid = GEQ;
            getch();
        }
        else {
            sym = "GRE";
            symid = GRE;
        }
    }
    else if (ch == '!') {
        id += ch;
        getch();
        if (ch == '=') {
            id += ch;
            sym = "NEQ";
            symid = NEQ;
            getch();
        }
    }
    else if (ch == '\"') {
        string tempstr;
        getch();
        while (ch == 32 || ch == 33 || (ch <= 126 && ch >= 35)) {
            tempstr += ch;
            getch();
        }
        if (ch == '\"') {
            getch();
            sym = "STRCON";
            symid = STRCON;
            id = tempstr;
        }
        else {
            error(DOUBLEQUOTESLACK);
            return -1;
        }
    }
    else if (ch == '\'') {
        getch();
        if (ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '_' || isalnum(ch)) {
            id += ch;
            getch();
        }
        else {
            error(UNACCEPTATLECHAR);
            return -1;
        }
        if (ch == '\'') {
            sym = "CHARCON";
            symid = CHARCON;
            getch();
        }
        else {
            error(SINGLEQUOTESLACK);
            return -1;
        }
    }
    else {
        if (ch == '+') {
            sym = "PLUS";
            symid = PLUS;
        }
        else if (ch == '-') {
            sym = "MINU";
            symid = MINU;
        }
        else if (ch == '*') {
            sym = "MULT";
            symid = MULT;
        }
        else if (ch == '/') {
            sym = "DIV";
            symid = DIV;
        }
        else if (ch == ',') {
            sym = "COMMA";
            symid = COMMA;
        }
        else if (ch == ':') {
            sym = "COLON";
            symid = COLON;
        }
        else if (ch == ';') {
            sym = "SEMICN";
            symid = SEMICN;
        }
        else if (ch == '{') {
            sym = "LBRACE";
            symid = LBRACE;
        }
        else if (ch == '[') {
            sym = "LBRACK";
            symid = LBRACK;
        }
        else if (ch == '(') {
            sym = "LPARENT";
            symid = LPARENT;
        }
        else if (ch == '}') {
            sym = "RBRACE";
            symid = RBRACE;
        }
        else if (ch == ']') {
            sym = "RBRACK";
            symid = RBRACK;
        }
        else if (ch == ')') {
            sym = "RPARENT";
            symid = RPARENT;
        }
        id += ch;
        getch();
    }
    if (!strcmp(sym.c_str(), "INTCON")) {
        laxrst << sym << ' ' << num << endl;
    }
    else if (!strcmp(sym.c_str(), "FLOATCON")) {
        laxrst << sym << ' ' << dnum << endl;
    }
    else {
        laxrst << sym << ' ' << id << endl;
    }

    return 0;
}

void getch() {
    int i = 0;
    prllen = llen;
    if (cnum == llen && ~line[llen]) {

        ch = fgetc(src);
        while (ch == ' ' || ch == '\n' || ch == '\t') {
            if (ch == '\n') {
                lnum++;
            }
            backupline[i++] = ch;  //保存源文件的行，用于发生错误时输出
            ch = fgetc(src);
        }
        for (llen = 0; ch != '\n' && ch != EOF; llen++) {
            line[llen] = ch;
            backupline[i++] = ch;   //保存源文件的行，用于发生错误时输出
            ch = fgetc(src);
        }
        if (ch == EOF)
            line[llen] = EOF;
        else
            line[llen] = '\n';
        backupline[i] = '\0';

        cnum = 0;
        ch = '\n';
        lnum++;
    }
    else {
        ch = line[cnum++];
    }
}
bool stringcmp(char *a, char *b) {
    int i = 0;
    while (true) {
        if ((a[i] == 0) && (b[i] == 0)) {
            return true;
        }
        else if (a[i] != b[i]) {
            return false;
        }
        else {
            i++;
        }

    }
}
int mf;
ofstream laxrst, midcoderst, asmrst, symtablehis;
FILE* optcodefile;



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QMenu *menuOpen=new QMenu();
    QMenu *menuCompile=new QMenu();
    QMenu *menuLook=new QMenu();

    ui->menuBar->addMenu(menuOpen);
    ui->menuBar->addMenu(menuCompile);
    ui->menuBar->addMenu(menuLook);
    QAction *actionOpen=new QAction();
    QAction *actionSave=new QAction();
    QAction *actionCompile=new QAction();
    QAction *actionLexical=new QAction();
    QAction *actionSynatx=new QAction();
    QAction *actionMiddle=new QAction();
    QAction *actionOptMiddle=new QAction();
    QAction *actionSymbolTable=new QAction();
    QAction *actionFinal=new QAction();
    menuOpen->setTitle("文件");
    menuCompile->setTitle("编译");
    menuLook->setTitle("查看");

    actionOpen->setText("打开");
    actionSave->setText("保存");
    actionCompile->setText("运行");
    actionLexical->setText("词法分析结果");
    actionSynatx->setText("语法分析结果");
    actionMiddle->setText("中间代码查看");
    actionOptMiddle->setText("优化中间代码");
    actionSymbolTable->setText("符号表");
    actionFinal->setText("汇编代码");

    actionOpen->setIcon(QIcon(":/Resource/1.ico"));
    actionSave->setIcon(QIcon(":/Resource/2.ico"));
    actionCompile->setIcon(QIcon(":/Resource/1.ico"));
    actionLexical->setIcon(QIcon(":/Resource/1.ico"));
    actionSynatx->setIcon(QIcon(":/Resource/2.ico"));
    actionMiddle->setIcon(QIcon(":/Resource/3.ico"));
    actionOptMiddle->setIcon(QIcon(":/Resource/4.ico"));
    actionSymbolTable->setIcon(QIcon(":/Resource/5.ico"));
    actionFinal->setIcon(QIcon(":/Resource/6.PNG"));
    menuOpen->addAction(actionOpen);
    menuOpen->addAction(actionSave);

    menuCompile->addAction(actionCompile);

    menuLook->addAction(actionLexical);
    menuLook->addAction(actionSynatx);
    menuLook->addAction(actionMiddle);
    menuLook->addAction(actionOptMiddle);
    menuLook->addAction(actionSymbolTable);
    menuLook->addAction(actionFinal);


    ui->SyntaxAnalysisWin->setWindowTitle(tr("语法分析树"));
    ui->dockWidget_2->setWindowTitle(tr("中间代码(优化后四元式)"));
    ui->dockWidget_3->setWindowTitle(tr("符号表"));
    ui->dockWidget_4->setWindowTitle(tr("中间代码(优化前四元式)"));




    connect(actionOpen, &QAction::triggered, this, &MainWindow::openFile);
    connect(actionSave, &QAction::triggered, this, &MainWindow::saveFile);
    connect(actionCompile,&QAction::triggered,this,&MainWindow::Compile);
    connect(actionLexical,&QAction::triggered,this,&MainWindow::LexicalDisplay);
    connect(actionSynatx,&QAction::triggered,this,&MainWindow::SynatxDisplay);
    connect(actionMiddle,&QAction::triggered,this,&MainWindow::MiddleDisplay);
    connect(actionOptMiddle,&QAction::triggered,this,&MainWindow::OptMiddleDisplay);
    connect(actionSymbolTable,&QAction::triggered,this,&MainWindow::SymbolTableDisplay);
    connect(actionFinal,&QAction::triggered,this,&MainWindow::FinalDisplay);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::initialGlobal(){
    wd=-1;
    memset(&mi,0,sizeof(mi));
    sp=0x7fffeffc;
//    memset(void(&x),0,4);
    memset(optcodefile,0,sizeof(optcodefile));
    memset(midcode,0,sizeof(midcode));
    memset(&midcodeiter,0,sizeof(midcodeiter));
    memset(&labelcnt,0,sizeof(labelcnt));
    memset(&varcnt,0,sizeof(varcnt));
    memset(&maintable,0,sizeof(maintable));
    memset(&value,0,sizeof(value));
    memset(&maintable,0,sizeof(maintable));
    memset(&address,0,sizeof(address));
    memset(&paranum,0,sizeof(paranum));
    memset(&kind,0,sizeof(kind));
    memset(name,0,sizeof(name));
    memset(&hsymid,0,sizeof(hsymid));
    memset(nowitem,0,sizeof(nowitem));
    memset(&returnnum,0,sizeof(returnnum));
    memset(&procname,0,sizeof(procname));
    memset(&mf,0,sizeof(mf));
    memset(procname,0,sizeof(procname));
    memset(word,0,sizeof(word));
    memset(wsym,0,sizeof(wsym));
    memset(&laxrst,0,sizeof(laxrst));
    memset(&midcoderst,0,sizeof(midcoderst));
    memset(&asmrst,0,sizeof(asmrst));
    memset(&symtablehis,0,sizeof(symtablehis));
    memset(&ch,0,sizeof(ch));
    memset(line,0,sizeof(line));
    memset(backupline,0,sizeof(backupline));
    memset(&lnum,0,sizeof(lnum));
    memset(&cnum,0,sizeof(cnum));
    memset(&llen,0,sizeof(llen));
    memset(&prllen,0,sizeof(prllen));
    memset(&id,0,sizeof(id));
    memset(&prid,0,sizeof(prid));
    memset(&num,0,sizeof(num));
    memset(&sym,0,sizeof(sym));
    memset(&dnum,0,sizeof(dnum));
    memset(&symid,0,sizeof(symid));
/*
    int  value;
    int  address;
    int  paranum;
    int  kind;
    char name[MAXIDENLEN];
    int hsymid;
    char nowitem[200];
    int returnnum;
    char procname[100];
    int mf;
    const int kwdnum;
    char *word[];

    char *wsym[];
    ofstream laxrst, midcoderst, asmrst, symtablehis;
    int ch;//the

    int line[500], backupline[500];
    int lnum, cnum, llen, prllen;
    FILE* src;

    //define id, num and sym
    string id, prid;
    int num;
    string sym;
    double dnum;
    int symid;*/


}

void MainWindow::Compile(){

//    initialGlobal();
    SymbolRow=0;
    MiddleRow=0;
    WrongLineMessage="";
    Dock_SymbolTableWidget=new QTableWidget();
    Dock_LexicalTableWidget=new QTableWidget();
    Dock_OptMiddleTableWidget=new QTableWidget();
    Dock_MiddleTableWidget=new QTableWidget();



    QStringList headerLabels;
    headerLabels << "index" << "name"<<"kind"<<"value"<<"address"<<"paranum"<<"isVec";
    Dock_SymbolTableWidget->setColumnCount(7);
    Dock_SymbolTableWidget->setRowCount(100);
    Dock_SymbolTableWidget->setHorizontalHeaderLabels(headerLabels);
    Dock_SymbolTableWidget->setSelectionBehavior(QAbstractItemView::SelectItems);
    Dock_SymbolTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);


    QStringList headerLabels2;
    headerLabels2 << "Operation" << "Var1"<<"Var2"<<"Var3";
    Dock_MiddleTableWidget->setColumnCount(4);
    Dock_MiddleTableWidget->setRowCount(100);
    Dock_MiddleTableWidget->setHorizontalHeaderLabels(headerLabels2);
    Dock_MiddleTableWidget->setSelectionBehavior(QAbstractItemView::SelectItems);
    Dock_MiddleTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);




    char filename[500] = "in.c";
    char *tempsrc=new char[500];
    tempsrc=PATH.toLatin1().data();

    if (strlen(tempsrc) > 2)
        if (tempsrc[0] == '\"') {
            strcpy(filename, tempsrc + 1);
            filename[strlen(filename) - 1] = 0;
        }
        else
            strcpy(filename, tempsrc);
    src = fopen(filename, "r");
    if (src == NULL)
    {
        error(NOSUCHFILE);
        return;
    }
    laxrst.open("laxrst.txt");
    midcoderst.open("midcode.txt");
    asmrst.open("asmrst.asm");
    symtablehis.open("symbolTable.txt");
    symtablehis << "index" << "\t" << "name" << "\t" << "kind" << "\t" << "value" << "\t" << "address" << "\t" << "paranum" << "\t" << "isVec" << endl;
    getch();
    program();

    if (mf < 1) error(MAINLACK);                         //没有定义主函数
    do  getsym(); while (ch == '\n' || ch == ' ' || ch == '\t');
    if (symid != -1)  error(MOREWORD);                //主函数后有多余的代码

    if (errnum == 0) {
        qDebug()<<"\n\n编译成功，没有语法语义错误!\n\n";
        printOptimize();     //打印优化后的四元式
        qDebug()<<("四元式 生成完成...\n");
        midcode2asm();   //生成汇编码
        qDebug()<<("汇编指令 生成完成...\n");
        scan();          //扫描四元式结构数组，完成优化
        printOptimize();
        qDebug()<<("优化后的四元式 生成完成...\n");
    }

    laxrst.close();
    asmrst.close();
    midcoderst.close();
    symtablehis.close();
    fclose(src);


    QString WrongMessage="";
    if (errnum == 0)
    {
        WrongMessage="Compile Success.";
        ui->AnalysisResult->setText(WrongMessage);
        if(1){
            QFile asmPath("asmrst.asm");
            if (!asmPath.open(QIODevice::ReadOnly | QIODevice::Text)) {
                QMessageBox::warning(this, tr("Read File"),
                                     tr("Cannot open file:\n%1").arg("asmrst.asm"));
                return;
            }

            QTextStream in(&asmPath);
            TextStr = new QTextStream(&asmPath);
            //ui->SynatxTree->setVisible(false);
            ui->tableWidget->setVisible(false);
            ui->CodeDisplay->setVisible(true);
            ui->CodeDisplay->setPlainText(in.readAll());
            asmPath.close();
        }
        ui->groupBox_3->setTitle("Mips汇编代码");
        return;
    }

    WrongMessage=WrongMessage+"Errors:\t";
    WrongMessage=WrongMessage+QString::number(errnum);
    WrongMessage=WrongMessage+"\tTotal Line: ";
    WrongMessage=WrongMessage+QString::number(lnum);
    WrongMessage=WrongMessage+"\n";

    ui->AnalysisResult->setText(WrongMessage+WrongLineMessage);
    qDebug()<<("Errors:\t%d\tTotal Line: %d\n", errnum, lnum);



}

void MainWindow::openFile()
{
    QString path = QFileDialog::getOpenFileName(this,
                                                tr("Open File"),
                                                ".",
                                                tr("Text Files(*.txt);;Cpp Files(*.cpp);;C Files(*.c);;Header Files(*.h)"));
    if(!path.isEmpty()) {
        QFile file(path);
        PATH=path;
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QMessageBox::warning(this, tr("Read File"),
                                 tr("Cannot open file:\n%1").arg(path));
            return;
        }

        QTextStream in(&file);
        TextStr = new QTextStream(&file);
        ui->SourceCode->setPlainText(in.readAll());
        file.close();
    } else {
        QMessageBox::warning(this, tr("Path"),
                             tr("You did not select any file."));
    }
}
void MainWindow::saveFile()
{

    QString path=PATH;
    if(!path.isEmpty()) {
        QFile file(path);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QMessageBox::warning(this, tr("Write File"),
                                       tr("Cannot open file:\n%1").arg(path));
            return;
        }
        QTextStream out(&file);

        out << ui->SourceCode->toPlainText();
        file.close();
    } else {
        QMessageBox::warning(this, tr("Path"),
                             tr("You did not select any file."));
    }
}

FILE* MainWindow::FP(QString path){
    QString tempPath;
    for(int i=0;i<path.size();i++)
    {
        if(path[i]=='/')
        {
            tempPath.append('\\');
            tempPath.append('\\');
        }
        else
        {
            tempPath.append(path[i]);
        }
    }
    if(!path.isEmpty()) {

        char* PATH;
        QByteArray ba = tempPath.toLatin1(); // must
        PATH=ba.data();
        FILE *fp=fopen(PATH,"r");
        if (!fp) {
            return NULL;
        }
        else
            return fp;
    }
    return NULL;
}
void MainWindow::LexicalDisplay(){
    Lex=new Lexical();

    FILE *fp=FP(PATH);


    while (fp&&!feof(fp)) {

        Lex->Result(fp);
    }
    fclose(fp);
    vector <pair<QString,const char *>>RESULT=Lex->getRESULT();
    ui->tableWidget->setRowCount(RESULT.size());
    ui->tableWidget->setColumnCount(2);
    QStringList headerLabels;
    headerLabels << "单词值" << "单词类型";
    ui->tableWidget->setHorizontalHeaderLabels(headerLabels);
    ui->tableWidget->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectItems);
    int row=0;
    for(vector<pair<QString,const char *>>::const_iterator iter = RESULT.cbegin(); iter != RESULT.cend(); iter++,row++)
    {
        QTableWidgetItem *item0, *item1;
        item0 = new QTableWidgetItem;
        item1 = new QTableWidgetItem;

        QString txt = QString((*iter).first);
        item0->setText(txt);
        ui->tableWidget->setItem(row, 0, item0);
        txt = QString((*iter).second);
        item1->setText(txt);
        ui->tableWidget->setItem(row, 1, item1);
     }
     ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
     ui->tableWidget->setVisible(true);
     ui->CodeDisplay->setVisible(false);
     ui->groupBox_3->setTitle("词法分析表");
}
void MainWindow::SynatxDisplay(){
    FILE *fp=FP(PATH);
    STREE = new STree();
    if(!Analyze(STREE,fp))
        QMessageBox::about(NULL, "错误", "语法分析错误。");

    QTreeWidget *STT=new QTreeWidget(ui->SyntaxAnalysisWin);
    QList<QTreeWidgetItem *> rootList;
    stack<STree*> ST;
    stack<QTreeWidgetItem *>QItem;
    STree *current;

    ST.push(STREE);
    QTreeWidgetItem *imageItem1 = new QTreeWidgetItem;
    imageItem1->setText(0,QString(STREE->getData()) );
    QItem.push(imageItem1);
    rootList.append(imageItem1);
    while (!ST.empty())
    {

        current = ST.top();
        ST.pop();
        QTreeWidgetItem * tempItem=QItem.top();
        QItem.pop();
        for (int j=0; j < current->getchild_num(); j++)
        {
            ST.push(current->getChild(j));
            QTreeWidgetItem *newItem = new QTreeWidgetItem;
            newItem->setText(0,tr(current->getChild(j)->getData()));
            tempItem->addChild(newItem);
            QItem.push(newItem);
        }
    }

    QScrollArea *scrollArea = new QScrollArea;
    scrollArea->setBackgroundRole(QPalette::Dark);
    scrollArea->setWidget(STT);
    scrollArea->setAlignment(Qt::AlignCenter);  // 居中对齐
    scrollArea->setWidgetResizable(true);  // 自动调整大小
    STT->setHeaderLabel(tr("语法分析树"));
    STT->insertTopLevelItems(0,rootList);  //将结点插入部件中
    STT->expandAll(); //全部展开
    ui->SyntaxAnalysisWin->setWindowTitle("语法分析树窗体");
    ui->SyntaxAnalysisWin->setWidget(scrollArea);
    ui->SyntaxAnalysisWin->setFloating(true);

    ui->SyntaxAnalysisWin->show();


}
void MainWindow::MiddleDisplay(){
    QScrollArea *scrollArea = new QScrollArea;
    scrollArea->setBackgroundRole(QPalette::Dark);
    scrollArea->setWidget(Dock_MiddleTableWidget);
    scrollArea->setAlignment(Qt::AlignCenter);  // 居中对齐
    scrollArea->setWidgetResizable(true);  // 自动调整大小
    ui->dockWidget_4->setWindowTitle("优化后中间代码窗体");
    ui->dockWidget_4->setWidget(scrollArea);
    ui->dockWidget_4->setFloating(true);

    ui->dockWidget_4->show();
}
void MainWindow::OptMiddleDisplay(){
    QScrollArea *scrollArea = new QScrollArea;
    scrollArea->setBackgroundRole(QPalette::Dark);
    scrollArea->setWidget(Dock_OptMiddleTableWidget);
    scrollArea->setAlignment(Qt::AlignCenter);  // 居中对齐
    scrollArea->setWidgetResizable(true);  // 自动调整大小
    ui->dockWidget_2->setWindowTitle("优化后中间代码窗体");
    ui->dockWidget_2->setWidget(scrollArea);
    ui->dockWidget_2->setFloating(true);

    ui->dockWidget_2->show();
}
void MainWindow::SymbolTableDisplay(){
    QScrollArea *scrollArea = new QScrollArea;
    scrollArea->setBackgroundRole(QPalette::Dark);
    scrollArea->setWidget(Dock_SymbolTableWidget);
    scrollArea->setAlignment(Qt::AlignCenter);  // 居中对齐
    scrollArea->setWidgetResizable(true);  // 自动调整大小
    ui->dockWidget_3->setWindowTitle("符号表窗体");
    ui->dockWidget_3->setWidget(scrollArea);
    ui->dockWidget_3->setFloating(true);

    ui->dockWidget_3->show();
}
void MainWindow::FinalDisplay(){
    if(1){
        QFile asmPath("asmrst.asm");
        if (!asmPath.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QMessageBox::warning(this, tr("Read File"),
                                 tr("Cannot open file:\n%1").arg("asmrst.asm"));
            return;
        }

        QTextStream in(&asmPath);
        TextStr = new QTextStream(&asmPath);
        ui->tableWidget->setVisible(false);
        ui->CodeDisplay->setVisible(true);
        ui->CodeDisplay->setPlainText(in.readAll());
        ui->groupBox_3->setTitle("Mips汇编代码");
        asmPath.close();
    }
}
