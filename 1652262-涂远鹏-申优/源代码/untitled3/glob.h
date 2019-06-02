#ifndef exc0compiler_glob_h
#define exc0compiler_glob_h
#define _CRT_SECURE_NO_WARNINGS
#include"syntax/STree.h"
#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <map>
#include <ctype.h>
#include <QTableWidget>

using namespace std;

extern const int kwdnum;
extern char *word[];

extern char *wsym[];



#define BEGINTK     1
#define CALLTK      2
#define CASETK      3
#define CHARTK      4
#define CONSTTK     5
#define DEFAULTTK   6
#define DOTK        7
#define ELSETK      8
#define ENDTK       9
#define FLOATTK     10
#define FORTK       11
#define IFTK        12
#define INTTK       13
#define MAINTK      14
#define ODDTK       15
#define PRINTFTK    16
#define PROCETK     17
#define READTK      18
#define RETURNTK    19
#define RPTTK       20
#define SCANFTK     21
#define SWITCHTK    22
#define THENTK      23
#define UTLTK       24
#define VARTK       25
#define VOIDTK      26
#define WHILETK     27
#define WRITETK     28
#define IDEN        29
#define INTCON      30
#define CHARCON     31
#define STRCON      32
#define PLUS        33
#define MINU        34
#define MULT        35
#define PERIOD      36
#define DIV         37
#define COLON       38
#define LSS         39
#define QMARK       40
#define LEQ         41
#define DQMARK      42
#define GRE         43
#define LPARENT     44
#define GEQ         45
#define RPARENT     46
#define EQL         47
#define LBRACK      48
#define NEQ         49
#define RBRACK      50
#define ASSIGN      51
#define LBRACE      52
#define SEMICN      53
#define RBRACE      54
#define COMMA       55

#define CINTCON     56
#define CCHARCON    57

extern ofstream laxrst, midcoderst, asmrst, symtablehis;
extern int ch;//the

extern int line[500], backupline[500];
extern int lnum, cnum, llen, prllen;
extern FILE* src;

//define id, num and sym
extern string id, prid;
extern int num;
extern string sym;
extern double dnum;
extern int symid;

//declare functioint getsym();
void getch();
int getsym();
bool stringcmp(char* a, char* b);


void program();
void defhead();
void varstate();
void vardef();
void constdef(int tclass);
void conststate();
void sentencelist();
void complexsentence();
void sentence();
void condition();
void loopsentence();
void valueofpara();
void scanfsentence();
void printfsentence();
void parametertable();
void returnsentence();
void expression();
void item();
void factor();
void lower(char* s);
extern int wd;

#define MAXIDENLEN      30
#define MAXSYMTABLENUM  200

typedef struct {
    char name[MAXIDENLEN];
    int kind;
    int value;
    int address;
    int paranum;
}symb;
#define VARIABLE    1
#define FUNCTION    2
#define PARA        3
typedef struct {
	symb element[MAXSYMTABLENUM];
	int index;
    int ftotal;
    int findextable[MAXSYMTABLENUM];
}symtable;
extern symtable maintable;
extern int  value;
extern int  address;
extern int  paranum;
extern int  kind;
extern char name[MAXIDENLEN];
extern int hsymid;
extern char nowitem[200];
extern int returnnum;
extern char procname[100];
extern int mf;
void insmidcode(char* op, char* t1, char* t2, char* t3);
void insmidcode(char* op, int t1, int t2, char* t3);
void insmidcode(char* op, char t1, int t2, char* t3);
char* nextlab();
char* nextvar();
#define MAXMIDCODE 1000
typedef struct {
	char op[10];
	char var1[200];
	char var2[30];
	char var3[30];
}fourvarcode;
extern fourvarcode midcode[MAXMIDCODE];
extern int midcodeiter;
extern int labelcnt;
extern int varcnt;

extern FILE* optcodefile;
void printOptimize();
int isconst(char name[]);
void delsetlab();
void delpublic();
void combine();
void scan();


extern int mi;
extern int sp;
//extern int x;
void midcode2asm();
int findvartable(char *name);
void midcode2asm();
void insertaddress(int kind, int addr = -1, int nmi = -1);
void pushstack(char* item = "0", int lenth = 1);
void funcasm();
int varaddr(char *name);
void dataseg();
void jmpasm();
void printint();
void callasm();
void setlabasm();
void addasm();
void subasm();
void mulasm();
void divasm();
void greasm();
void geqasm();
void lssasm();
void leqasm();
void eqlasm();
void neqasm();
void assasm();
void aassasm();
void assaasm();
void scfasm();
void prtasm();
void fupaasm();
void retasm();
void paraasm();
void jneasm();
void intcharasm();
void constdefasm();
void intcharaasm();
void savesreg();
void loadsreg();
void cnt(char * name);
void cntopt();

class Global_Variable{

    int wd;
    int mi;
    int sp;
    int x;
    FILE* optcodefile;
    fourvarcode midcode[MAXMIDCODE];
    int midcodeiter;
    int labelcnt;
    int varcnt;
    symtable maintable;
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
    int symid;

};


extern QTableWidget *Dock_MiddleTableWidget;
extern QTableWidget *Dock_OptMiddleTableWidget;
extern QTableWidget *Dock_SymbolTableWidget;
extern QTableWidget *Dock_LexicalTableWidget;
extern int SymbolRow;
extern int MiddleRow;
extern QString WrongLineMessage;
#endif

