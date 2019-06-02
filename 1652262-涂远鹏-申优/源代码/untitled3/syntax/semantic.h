#ifndef SEMANTIC_H
#define SEMANTIC_H
#pragma once
#include<stack>
#include "STree.h"
using namespace std;

class Semantic;

struct LT_Node {
    char label[LABEL_LENGTH];
    int line;
    LT_Node* next;
};
class LabelTable{
private:
    LT_Node* head;
    LT_Node* tail;
public:
    LabelTable();
    void AddLabel(char label[LABEL_LENGTH]);
};


struct SymbolTable;
struct ST_Node {
    char name[NAME_LENGTH];
    char type[20];
    int offset;
    SymbolTable* child_table;
    ST_Node* next;
};
struct SymbolTable {
    ST_Node* head;
    ST_Node* tail;
    int OFFSET;
    SymbolTable* parent_table;
};
class SymbolTableS {
private:
    SymbolTable* Sroot;
    SymbolTable* Scurrent;

public:
    SymbolTableS();
    void AddSymbol(ST_Node *S);
    friend Semantic;
};


struct ERR_Node {
    char ErrorMessage[100];
    ERR_Node *next;
};
class ErrorTable {
private:
    ERR_Node *head;
    ERR_Node *tail;
public:
    ErrorTable();
    void AddError(ERR_Node* E);
    friend Semantic;
    friend MainWindow;
};


class Semantic {
private:
    int line;
    int LabelNo;
    int TempNo;


    LabelTable Ltable;
    SymbolTableS Stable;
    ErrorTable Etable;
    stack <char*> Stemp;

public:

    Semantic();
    friend MainWindow;

    void Num_to_Str(int num, char str[10]);
    void NewTemp(char Temp[NAME_LENGTH]);
    void NewLabel(char Label[LABEL_LENGTH]);

    Quadruples* Generate(const char* op, const char* arg1, const char* arg2, const char* result);			//生成一条四元式
    void QList_Connect(int num, int No[MAX_CHILD_NUM], Quadruples_List* QLparent, Quadruples_List* QLchild[MAX_CHILD_NUM]);		//用于拼接四元式链
    void QList_Append(Quadruples_List* L1, Quadruples_List* L2);											// 将Qlist L2接入到L1的尾部

    void PostOrderTraverse(STree* T);
    void Semantic_Analyze(STree* T);


    void SemanticFun_Entrance(STree* T);

    void SemanticFun_5(STree* T);      //5:  <声明> ::= int  <ID>  <声明类型>
    void SemanticFun_6(STree* T);      //6:  <声明> ::= void  <ID>  <函数声明>

    void SemanticFun_17(STree* T);		//17:  <参数> ::= int  <ID>
                                        //23:  <内部变量声明> ::= int  <ID> ;
    void SemanticFun_18(STree* T);		//18:	<语句块> ::= { <内部声明>  <语句串> }


    void SemanticFun_24(STree* T);		//24:  <语句串> ::= <语句> <可选语句串>
    void SemanticFun_25(STree* T);		//25:  <可选语句串> ::= <语句> <可选语句串>
    void SemanticFun_26(STree* T);		//26:  <可选语句串> ::= 空
    void SemanticFun_27(STree* T);		//27:	<语句> ::= <if语句>
                                        //28:	<语句> ::= <while语句>
                                        //29:	<语句> ::= <return语句>
                                        //30:	 <语句> ::= <赋值语句>
    void SemanticFun_31(STree* T);		//31:	<赋值语句> ::= <ID> = <表达式>
    void SemanticFun_32(STree* T);		//32:	<return语句> ::= return <表达式>
    void SemanticFun_34(STree* T);		//34:  <while语句> ::= while ( <表达式> ) <语句块>
    void SemanticFun_35(STree* T);      //35:  <if语句> ::= if ( <表达式> ) <语句块> <可选else语句块>
    void SemanticFun_36(STree* T);		//36:  <可选else语句块> ::= else <语句块>

    void SemanticFun_39(STree* T);		//39:  <可选表达式> :: = <relop> <加法表达式> <可选表达式>
    void SemanticFun_41(STree* T);		//41:	<relop> ::= <
                                        //42:	<relop> ::= <=
                                        //43:	<relop> ::= >
                                        //44:	<relop> ::= >=
                                        //45:	<relop> ::= ==
                                        //46:	<relop> ::= !=
    void SemanticFun_47(STree* T);		//47:	<加法表达式> ::= <项> <可选加法表达式>
                                        //51:	<项> ::= <因子> <可选项>
    void SemanticFun_48(STree* T);		//48:  <可选加法表达式> ::= + <项> <可选加法表达式>
                                        //49:  <可选加法表达式> ::= - <项> <可选加法表达式>
                                        //52 : <可选项> :: = * <因子> <可选项>
                                        //53 : <可选项> :: = / <因子> <可选项>

    void SemanticFun_55(STree* T);		//<因子> ::= num
    void SemanticFun_56(STree* T);		//<因子> ::= ( <表达式> )
    void SemanticFun_57(STree* T);		//<因子> ::= <ID>  FTYPE
    void SemanticFun_63(STree* T);		//<实参列表> ::= <表达式> <可选实参列表>
    void SemanticFun_64(STree* T);		//<可选实参列表> ::= , <表达式> <可选实参列表>

    void SemanticFun_Other(STree* T);	//只传递四元式链

    int Define_Check(char IDname[NAME_LENGTH]);
    int Use_Check(char IDname[NAME_LENGTH]);

    void StableOut();
};
#endif // SEMANTIC_H
