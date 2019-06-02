#ifndef GRAMMAR_H
#define GRAMMAR_H
#include<iostream>
#include<string.h>
#include<fstream>
#include<cstdio>
#include<stack>
#include "STree.h"
#include "lexical.h"
#define Production_maxlength 6
#define Terminal_Sign_num 27
#define Nonterminal_Sign_num 40
#define Production_num 80
#define STACK_INIT_SIZE 50
#define STACK_INCREMENT 10
#define _CRT_SECURE_NO_WARNINGS
using namespace std;

struct Elem {
    int code;
    bool ter_flag;
};
struct Production {
    int left;
    bool left_terflag;
    int right[Production_maxlength];
    bool right_terflag[Production_maxlength];
};

struct TableElem {
    Production pro;
    int ProNo;
};

bool Analyze(STree* &root,FILE* fp);
class Grammar {
public:
    int proc_num;
    int tsign_num;
    int ntsign_num;
    Production proc[Production_num];
    int first_set[Nonterminal_Sign_num + 1][Terminal_Sign_num + 1];
    int follow_set[Nonterminal_Sign_num + 1][Terminal_Sign_num + 1];
    const char* terminal_sign[Terminal_Sign_num + 1];
    char* nonterminal_sign[Nonterminal_Sign_num + 1];
    TableElem table[Nonterminal_Sign_num + 1][Terminal_Sign_num + 1];
    Grammar();
    void Init_Grammar();
    void Show_Grammar();
    void First();
    void Follow();
    void Show_First();
    void Show_Follow();
    void MTable();
    bool Exist(int A[Terminal_Sign_num + 1], int e);							//判断集合A中是否含有元素e
    bool Add_etoB(int e, int B[Terminal_Sign_num + 1]);							//把元素e加到集合B
    bool Add_AtoB(int A[Terminal_Sign_num + 1], int B[Terminal_Sign_num + 1],bool cancel_empty);	//把集合A加到集合B
    void fill_proc(int nter, int ter, int i);
    friend bool Analyze(STree* &root,FILE* fp);
};

#endif // COMPILER_H
