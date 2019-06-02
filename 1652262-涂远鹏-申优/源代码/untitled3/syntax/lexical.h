#ifndef LEXICAL_H
#define LEXICAL_H
#include <vector>
#include <QTextStream>
#include <QFile>
#include <stdio.h>
using namespace std;
#define ERRORMAX 100
struct Word {
    int code;
    char value[50];
};
class Lexical
{
public:
    Lexical();
    void clearmid();
    int IsNum(char c);
    int IsChar(char c);
    int IsOperator(char c);
    int IsLimiter(char c);
    int IsKeyWord(char *mid);
    void printf(int a);
    Word Result(FILE* fp);
    vector <pair <QString,const char *>> getRESULT();
private:
    vector<pair<int,const char *> > keyword;
    vector<pair<int,const char *> > opera;
    char mid[50];
    char errormid[ERRORMAX];
    vector <pair< QString,const char *>>RESULT;
    int WordType;
};
#endif // LEXICAL_H
