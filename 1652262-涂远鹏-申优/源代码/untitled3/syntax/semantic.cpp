#define _CRT_SECURE_NO_WARNINGS
#include "semantic.h"
#include <iostream>
#include <string.h>
#include<QDebug>
using namespace std;
Semantic::Semantic()
{
    TempNo = 0;
    LabelNo = 0;
}

LabelTable::LabelTable()
{
    LT_Node* head = NULL;
    LT_Node* tail= NULL;
}
void LabelTable::AddLabel(char label[LABEL_LENGTH])
{
    if (head == NULL)
    {
        head = new LT_Node;
        strcpy(head->label, label);
        head->line = -1;
        head->next = NULL;
        tail = head;
    }
    else
    {
        tail->next = new LT_Node;
        strcpy(tail->next->label, label);
        tail->next->line = -1;
        tail->next->next = NULL;

        tail = tail->next;
    }
}

SymbolTableS::SymbolTableS()
{
    Sroot=new SymbolTable;
    Sroot->head = NULL;
    Sroot->tail = NULL;
    Sroot->OFFSET = 0;
    Sroot->parent_table=NULL;
    Scurrent = Sroot;
}
void SymbolTableS::AddSymbol(ST_Node* S)
{
    if (Scurrent->head == NULL)
    {
        S->next = NULL;
        Scurrent->head = S;
        Scurrent->tail = S;
    }
    else
    {
        S->next = NULL;
        Scurrent->tail->next = S;
        Scurrent->tail = Scurrent->tail->next;
    }
}
ErrorTable::ErrorTable()
{
    head = NULL;
    tail = NULL;
}
void ErrorTable::AddError(ERR_Node* E)
{
    if (head == NULL)
    {
        head = E;
        tail = E;
        tail->next = NULL;
    }
    else
    {
        tail->next = E;
        tail = tail->next;
        tail->next = NULL;
    }
}
void Semantic::Num_to_Str(int num, char str[10])
{
    int len = 0;
    int q, r;		//商和余数
    r = num;
    q = r / 10;
    r = r % 10;
    while (q > 0 && len < 10)
    {
        str[len] = q + 48;
        len++;
        q = r / 10;
        r = r % 10;
    }
    str[len] = r + 48;
    len++;
    str[len] = '\0';
}
void Semantic::NewTemp(char Temp[NAME_LENGTH])
{
    char str[10];
    TempNo++;
    Num_to_Str(TempNo, str);
    Temp[0] = 'T';
    Temp[1] = '\0';
    int i, j;
    for (i = 1, j = 0; str[j] != '\0'; i++, j++)
    {
        Temp[i] = str[j];
    }
    Temp[i] = '\0';
}
void Semantic::NewLabel(char Label[LABEL_LENGTH])
{
    char str[10];
    LabelNo++;
    Num_to_Str(LabelNo, str);
    Label[0] = 'L';
    Label[1] = '\0';
    int i, j;
    for (i = 1, j = 0; str[j] != '\0'; i++, j++)
    {
        Label[i] = str[j];
    }
    Label[i] = '\0';
}

//生成一条四元式
Quadruples* Semantic::Generate(const char* op, const char* arg1, const char* arg2, const char* result)
{
    Quadruples* p = new Quadruples;
    strcpy_s(p->op, op);
    strcpy_s(p->arg1, arg1);
    strcpy_s(p->arg2, arg2);
    strcpy_s(p->result, result);
    p->label = NULL;
    p->next = NULL;
    return p;
}
//用于拼接四元式链
void Semantic::QList_Connect(int num, int No[MAX_CHILD_NUM], Quadruples_List* QLparent, Quadruples_List* QLchild[MAX_CHILD_NUM])
{
    int i, j;
    //找到第一条非空的四元式链
    for (i = 0; i < num; i++)
    {
        if (QLchild[No[i]]->head != NULL)
            break;
    }
    if (i < num)
    {
        QLparent->head = QLchild[No[i]]->head;
        //往后搜索，如果遇到非空四元式链，进行拼接
        for (j = i + 1; j < num; j++)
        {
            if (QLchild[No[j]]->head != NULL)
            {
                QLchild[No[i]]->tail->next = QLchild[No[j]]->head;
                i = j;
            }
        }
        QLparent->tail = QLchild[No[i]]->tail;
    }

}
//将Qlist L2接入到L1的尾部
void Semantic::QList_Append(Quadruples_List* L1, Quadruples_List* L2)
{
    if (L1->head != NULL && L2->head != NULL)
    {
        L1->tail->next = L2->head;
        L1->tail = L2->tail;
    }
    else if (L1->head == NULL && L2->head != NULL)
    {
        L1->head = L2->head;
        L1->tail = L2->tail;
    }
}

void Semantic::PostOrderTraverse(STree* T)
{
    int i;
    T->flag = 0;
    SemanticFun_Entrance(T);
    for (i = 0; i < T->getchild_num(); i++)
    {
        PostOrderTraverse(T->getChild(i));
    }
    T->flag = 1;
    SemanticFun_Entrance(T);
}
void Semantic::Semantic_Analyze(STree* T)
{
    //给Semantic里的成员初始化

    PostOrderTraverse(T);
}

void Semantic::SemanticFun_Entrance(STree* T)
{
    switch (T->ProNo)
    {
    case 5: {
        SemanticFun_5(T);
        break;
    }
    case 6: {
        SemanticFun_6(T);
        break;
    }
    case 17:
    case 23: {
        SemanticFun_17(T);
        break;
    }
    case 18: {
        SemanticFun_18(T);
        break;
    }
    case 24: {
        SemanticFun_24(T);
        break;
    }
    case 25: {
        SemanticFun_25(T);
        break;
    }
    case 26: {
        SemanticFun_26(T);
        break;
    }
    case 27:
    case 28:
    case 29:
    case 30: {
        SemanticFun_27(T);
        break;
    }
    case 31: {
        SemanticFun_31(T);
        break;
    }
    case 32: {
        SemanticFun_32(T);
        break;
    }
    case 34: {
        SemanticFun_34(T);
        break;
    }
    case 35: {
        SemanticFun_35(T);
        break;
    }
    case 36: {
        SemanticFun_36(T);
        break;
    }
    case 39: {
        SemanticFun_39(T);
        break;
    }
    case 41:
    case 42:
    case 43:
    case 44:
    case 45:
    case 46: {
        SemanticFun_41(T);
        break;
    }
    case 38:
    case 47:
    case 51: {
        SemanticFun_47(T);
        break;
    }
    case 48:
    case 49:
    case 52:
    case 53: {
        SemanticFun_48(T);
        break;
    }
    case 55: {
        SemanticFun_55(T);
        break;
    }
    case 56: {
        SemanticFun_56(T);
        break;
    }
    case 57: {
        SemanticFun_57(T);
        break;
    }
    case 63: {
        SemanticFun_63(T);
        break;
    }
    case 64: {
        SemanticFun_64(T);
        break;
    }
    default: {
        SemanticFun_Other(T);
        break;
    }
    }
}


//5:  <声明> ::= int  <ID>  <声明类型>
void Semantic::SemanticFun_5(STree* T)
{
    if (T->flag == 0)
    {
        for (int i = 0; i < MAX_CHILD_NUM; i++)
        {
            if (T->child[i] != NULL)
                T->QLchild[i] = T->child[i]->QLparent;
        }
        if(T->child[2]->ProNo==8)
        {
            Define_Check(T->child[1]->name);

            ST_Node* NewSymbol = new ST_Node;

            strcpy_s(NewSymbol->name, T->child[1]->name);
            strcpy_s(NewSymbol->type, "int");
            NewSymbol->offset = Stable.Scurrent->OFFSET;
            NewSymbol->child_table = new SymbolTable;
            NewSymbol->child_table->head = NULL;
            NewSymbol->child_table->tail = NULL;
            NewSymbol->child_table->OFFSET = 0;
            NewSymbol->child_table->parent_table = Stable.Scurrent;
            NewSymbol->next = NULL;

            Stable.AddSymbol(NewSymbol);
            Stable.Scurrent = NewSymbol->child_table;
        }
    }
    else
    {
        if(T->child[2]->ProNo==7)
        {
            if (Define_Check(T->child[1]->name) == 0)
            {
                ST_Node* NewSymbol = new ST_Node;
                strcpy(NewSymbol->name, T->child[1]->name);
                strcpy(NewSymbol->type, "int");
                NewSymbol->offset = Stable.Scurrent->OFFSET;
                NewSymbol->child_table = NULL;
                NewSymbol->next = NULL;

                Stable.Scurrent->OFFSET += 4;
                Stable.AddSymbol(NewSymbol);

            }
        }
        else if(T->child[2]->ProNo==8)
        {                       
            Stable.Scurrent = Stable.Scurrent->parent_table;       //一个函数的定义结束，当前符号表指针指向最外层（整个程序的）符号表
            Stable.Scurrent->OFFSET += Stable.Scurrent->tail->child_table->OFFSET;
        }

        char *FuncLabel = new char[LABEL_LENGTH];
        strcpy(FuncLabel,T->child[1]->name);
        if(T->child[2]->ProNo == 8)
        {
            Quadruples* NewQuad = Generate("nop", "-", "-", "-");
            NewQuad->label = FuncLabel;
            Quadruples_List* TempList=new Quadruples_List;
            TempList->head = NewQuad;
            TempList->tail = NewQuad;
            QList_Append(T->QLparent, TempList);
        }

        QList_Append(T->QLparent, T->QLchild[2]);

    }
}
//6:  <声明> ::= void  <ID>  <函数声明>
void Semantic::SemanticFun_6(STree* T)
{
    if (T->flag == 0)
    {
        for (int i = 0; i < MAX_CHILD_NUM; i++)
        {
            if (T->child[i] != NULL)
                T->QLchild[i] = T->child[i]->QLparent;
        }


        Define_Check(T->child[1]->name);

        ST_Node* NewSymbol = new ST_Node;

        strcpy_s(NewSymbol->name, T->child[1]->name);
        strcpy_s(NewSymbol->type, "void");
        NewSymbol->offset = Stable.Scurrent->OFFSET;
        NewSymbol->child_table = new SymbolTable;
        NewSymbol->child_table->head = NULL;
        NewSymbol->child_table->tail = NULL;
        NewSymbol->child_table->OFFSET = 0;
        NewSymbol->child_table->parent_table = Stable.Scurrent;
        NewSymbol->next = NULL;

        Stable.AddSymbol(NewSymbol);
        Stable.Scurrent = NewSymbol->child_table;
    }
    else
    {
        Stable.Scurrent = Stable.Scurrent->parent_table;       //一个函数的定义结束，当前符号表指针指向最外层（整个程序的）符号表
        Stable.Scurrent->OFFSET += Stable.Scurrent->tail->child_table->OFFSET;

        char *FuncLabel = new char[LABEL_LENGTH];
        strcpy(FuncLabel,T->child[1]->name);

        Quadruples* NewQuad = Generate("nop", "-", "-", "-");
        NewQuad->label = FuncLabel;
        Quadruples_List* TempList=new Quadruples_List;
        TempList->head = NewQuad;
        TempList->tail = NewQuad;
        QList_Append(T->QLparent, TempList);

        QList_Append(T->QLparent, T->QLchild[2]);
    }
}
//17:  <参数> ::= int  <ID>
//23:  <内部变量声明> ::= int  <ID> ;
void Semantic::SemanticFun_17(STree* T)
{
    if(T->flag==0)
    {
        for (int i = 0; i < MAX_CHILD_NUM; i++)
        {
            if (T->child[i] != NULL)
                T->QLchild[i] = T->child[i]->QLparent;
        }
    }
    else
    {
        if (Define_Check(T->child[1]->name) == 0)
        {
            ST_Node* NewSymbol = new ST_Node;
            strcpy(NewSymbol->name, T->child[1]->name);
            strcpy(NewSymbol->type, "int");
            NewSymbol->offset = Stable.Scurrent->OFFSET;
            NewSymbol->child_table = NULL;
            NewSymbol->next = NULL;
            Stable.Scurrent->OFFSET += 4;
            Stable.AddSymbol(NewSymbol);
        }

        int num = 1;
        int No[MAX_CHILD_NUM] = { 1,-1,-1,-1,-1,-1 };
        QList_Connect(num, No, T->QLparent, T->QLchild);
    }
}
//18:	<语句块> ::= { <内部声明>  <语句串> }
void Semantic::SemanticFun_18(STree* T)
{
    if (T->flag == 0)
    {
        for (int i = 0; i < MAX_CHILD_NUM; i++)
        {
            if (T->child[i] != NULL)
                T->QLchild[i] = T->child[i]->QLparent;
        }

        T->child[2]->begin = new char[LABEL_LENGTH];
        T->child[2]->begin[0] = '\0';
    }
    else
    {
        /*孩子结点的code进行连接*/
        int num = 2;
        int No[MAX_CHILD_NUM] = { 1,2,-1,-1,-1,-1 };
        QList_Connect(num, No, T->QLparent, T->QLchild);
    }
}
//24:  <语句串> ::= <语句> <可选语句串>
void Semantic::SemanticFun_24(STree* T)
{
    if (T->flag == 0)
    {
        for (int i = 0; i < MAX_CHILD_NUM; i++)
        {
            if (T->child[i] != NULL)
                T->QLchild[i] = T->child[i]->QLparent;
        }
        T->child[1]->begin = new char[LABEL_LENGTH];
        T->child[1]->begin[0] = '\0';

        T->child[0]->next = T->child[1]->begin;
        T->child[0]->begin = T->begin;
        T->child[1]->next = T->next;
    }
    else
    {
        int No[MAX_CHILD_NUM] = { 0,1,-1,-1,-1,-1 };
        QList_Connect(2, No, T->QLparent, T->QLchild);
    }
}
//25:  <可选语句串> ::= <语句> <可选语句串>
void Semantic::SemanticFun_25(STree* T)
{
    if (T->flag == 0)
    {
        for (int i = 0; i < MAX_CHILD_NUM; i++)
        {
            if (T->child[i] != NULL)
                T->QLchild[i] = T->child[i]->QLparent;
        }
        T->child[1]->begin = new char[LABEL_LENGTH];
        T->child[1]->begin[0] = '\0';
        T->child[0]->next = T->child[1]->begin;
        T->child[0]->begin = T->begin;
    }
    else
    {
        int No[MAX_CHILD_NUM] = { 0,1,-1,-1,-1,-1 };
        QList_Connect(2, No, T->QLparent, T->QLchild);
        //label和四元式关联
    }
}
//26:  <可选语句串> :: = 空
void Semantic::SemanticFun_26(STree* T)
{
    if (T->flag == 0)
    {
        for (int i = 0; i < MAX_CHILD_NUM; i++)
        {
            if (T->child[i] != NULL)
                T->QLchild[i] = T->child[i]->QLparent;
        }

    }
    else
    {
        if(T->begin !=NULL && T->begin[0] !='\0' )
        {
            T->QLparent->head = Generate("nop", "-", "-", "-");
            T->QLparent->head->label = T->begin;
            T->QLparent->tail = T->QLparent->head;
        }
    }
}
//27:	<语句> ::= <if语句>
//28:	<语句> ::= <while语句>
//29:	<语句> ::= <return语句>
//30:	<语句> ::= <赋值语句>
void Semantic::SemanticFun_27(STree* T)
{
    if (T->flag == 0)
    {
        for (int i = 0; i < MAX_CHILD_NUM; i++)
        {
            if (T->child[i] != NULL)
                T->QLchild[i] = T->child[i]->QLparent;
        }
        //strcpy(T->child[0]->next, T->next);
        //strcpy_s(T->child[0]->begin, T->begin);
        T->child[0]->next = T->next;
        T->child[0]->begin = T->begin;
    }
    else
    {

        /*孩子结点的code进行连接*/
        int num = 1;
        int No[MAX_CHILD_NUM] = { 0,-1,-1,-1,-1,-1 };
        QList_Connect(num, No, T->QLparent, T->QLchild);
    }
}
//31:	<赋值语句> ::= <ID> = <表达式>
void Semantic::SemanticFun_31(STree* T)
{
    if (T->flag == 0)
    {
        for (int i = 0; i < MAX_CHILD_NUM; i++)
        {
            if (T->child[i] != NULL)
                T->QLchild[i] = T->child[i]->QLparent;
        }
        if (T->child[2]->child[1]->ProNo == 39)
        {
            //NewLabel(T->child[0]->begin);
            T->child[0]->begin = new char[LABEL_LENGTH];
            T->child[0]->begin[0] = '\0';
        }
        T->child[2]->begin = T->begin;
        //strcpy(T->child[2]->next, T->child[0]->begin);

        T->child[2]->next = new char[LABEL_LENGTH];
        T->child[2]->next[0] = '\0';
    }
    else
    {
        /*<ID>.name 查找符号表*/
        Use_Check(T->child[0]->name);

        /*孩子结点的code进行连接*/
        int No[MAX_CHILD_NUM] = { 2,-1,-1,-1,-1,-1 };
        QList_Connect(1, No, T->QLparent, T->QLchild);

        if (T->QLparent->head != NULL)
        {
            T->QLparent->head->label = T->begin;
        }
        Quadruples* NewQuad = Generate("assign", T->child[2]->name, "-", T->child[0]->name);
        if (T->child[2]->child[1]->ProNo == 39)
        {
            //strcpy_s(NewQuad->label, T->child[0]->begin);
            NewQuad->label = T->child[2]->next;
        }
        else
        {
            //strcpy_s(NewQuad->label, "");
        }
        Quadruples_List* TempList = new Quadruples_List();
        TempList->head = NewQuad;
        TempList->tail = NewQuad;

        QList_Append(T->QLparent, TempList);
        delete TempList;
    }
}
//32:	<return语句> ::= return <表达式>
void Semantic::SemanticFun_32(STree* T)
{
    if (T->flag == 0)
    {
        /*T结点的QLchild的code初始化为孩子结点的QLparent*/
        for (int i = 0; i < MAX_CHILD_NUM; i++)
        {
            if (T->child[i] != NULL)
                T->QLchild[i] = T->child[i]->QLparent;
        }
    }
    else
    {
        /*孩子结点的code进行连接*/
        int No[MAX_CHILD_NUM] = { 1,-1,-1,-1,-1,-1 };
        QList_Connect(1, No, T->QLparent, T->QLchild);

        Quadruples* NewQuad;
        if(T->child[1]->ProNo != 33)
            NewQuad = Generate("ret", T->child[1]->name, "-", "-");
        else
            NewQuad = Generate("ret", "-", "-", "-");
        NewQuad->label = T->begin;

        NewQuad = Generate("ret", T->child[1]->name, "-", "-");
        Quadruples_List* TempList = new Quadruples_List();
        TempList->head = NewQuad;
        TempList->tail = NewQuad;

        QList_Append(T->QLparent, TempList);
        delete TempList;
    }
}
//34:  <while语句> :: = while ( <表达式> ) <语句块>
void Semantic::SemanticFun_34(STree* T)
{
    if (T->flag == 0)
    {
        for (int i = 0; i < MAX_CHILD_NUM; i++)
        {
            if (T->child[i] != NULL)
                T->QLchild[i] = T->child[i]->QLparent;
        }
        T->child[2]->True = new char[LABEL_LENGTH];
        T->child[2]->True[0] = '\0';

        T->child[2]->next = T->child[2]->True;
        T->child[2]->False = T->next;
        T->child[4]->next = T->begin;
    }
    else
    {
        //拼接四元式链
        if (T->next[0] == '\0')
        {
            NewLabel(T->next);
        }
        Quadruples* NewQuad1 = Generate("jz", T->child[2]->name, "-", T->next);
        NewQuad1->label = T->child[2]->next;
        if (T->begin[0] == '\0')
        {
            NewLabel(T->begin);
        }
        Quadruples* NewQuad2 = Generate("j", "-", "-", T->begin);

        if (T->child[2]->QLparent->head != NULL)
        {
            T->child[2]->QLparent->head->label = T->begin;
        }

        Quadruples_List* TempList=new Quadruples_List;
        TempList->head = NewQuad1;
        TempList->tail = NewQuad1;

        QList_Append(T->QLparent, T->QLchild[2]);
        QList_Append(T->QLparent, TempList);
        QList_Append(T->QLparent, T->QLchild[4]);

        TempList->head = NewQuad2;
        TempList->tail = NewQuad2;
        QList_Append(T->QLparent, TempList);

        //label和四元式关联

        delete TempList;
    }
}
//35:  <if语句> ::= if ( <表达式> ) <语句块> <可选else语句块>
void Semantic::SemanticFun_35(STree* T)
{
    if (T->flag == 0)
    {
        /*T结点的QLchild的code初始化为孩子结点的QLparent*/
        for (int i = 0; i < MAX_CHILD_NUM; i++)
        {
            if (T->child[i] != NULL)
                T->QLchild[i] = T->child[i]->QLparent;
        }

        T->child[2]->True = new char[LABEL_LENGTH];
        T->child[2]->True[0] = '\0';
        T->child[2]->next = T->child[2]->True;
        T->child[2]->False = new char[LABEL_LENGTH];
        T->child[2]->False[0] = '\0';
        T->child[4]->next = T->next;
        T->child[5]->next = T->next;
        T->child[2]->begin= T->begin;
    }
    else
    {
        char* TempJ;
        if (T->child[5]->QLparent->head != NULL)
        {
            TempJ = T->child[2]->False;
        }
        else
        {
            TempJ = T->next;
        }
        if (TempJ[0] == '\0')
        {
            NewLabel(TempJ);
        }
        Quadruples* NewQuad1 = Generate("jz", T->child[2]->name, "-", TempJ);				// gent(j, -, -, <if语句>.next)

        NewQuad1->label = T->child[2]->next;
        if (T->next[0] == '\0')
        {
            NewLabel(T->next);
        }

        Quadruples* NewQuad2 = Generate("j", "-", "-", T->next);				// gent(j, -, -, <if语句>.next)
        if (T->child[5]->QLparent->head != NULL)
        {
            T->child[5]->QLparent->head->label = T->child[2]->False;
        }

                                                                                /*孩子结点的code进行连接*/
        QList_Append(T->QLparent, T->QLchild[2]);

        Quadruples_List* TempList1 = new Quadruples_List();
        TempList1->head = NewQuad1;
        TempList1->tail = NewQuad1;
        QList_Append(T->QLparent, TempList1);
        delete TempList1;

        QList_Append(T->QLparent, T->QLchild[4]);

        Quadruples_List* TempList2 = new Quadruples_List();
        TempList2->head = NewQuad2;
        TempList2->tail = NewQuad2;

        if(T->child[5]->ProNo == 36)
            QList_Append(T->QLparent, TempList2);
        QList_Append(T->QLparent, T->QLchild[5]);

        delete TempList2;

    }
}

//36:	<可选else语句块> ::= else <语句块>
void Semantic::SemanticFun_36(STree* T)
{
    if (T->flag == 0)
    {
        /*T结点的QLchild的code初始化为孩子结点的QLparent*/
        for (int i = 0; i < MAX_CHILD_NUM; i++)
        {
            if (T->child[i] != NULL)
                T->QLchild[i] = T->child[i]->QLparent;
        }
    }
    else
    {
        int No[MAX_CHILD_NUM] = { 1,-1,-1,-1,-1,-1 };
        QList_Connect(1, No, T->QLparent, T->QLchild);
    }
}


//<可选表达式> :: = <relop> <加法表达式> <可选表达式>
void Semantic::SemanticFun_39(STree* T)
{
    if (T->flag == 0)
    {
        T->QLchild[0] = T->getChild(0)->QLparent;
        T->QLchild[1] = T->getChild(1)->QLparent;
        T->QLchild[2] = T->getChild(2)->QLparent;
    }
    else
    {
        //传递四元式串
        int No[MAX_CHILD_NUM] = { 0,1,2,-1,-1,-1 };
        QList_Connect(3, No, T->QLparent, T->QLchild);

        //本节点新生成的四元式

        NewTemp(T->name);
        char* L1;
        L1 = new char[LABEL_LENGTH];
        L1[0] = '\0';

        char Temp_op[50];
        strcpy(Temp_op, "j");
        strcat(Temp_op, T->getChild(0)->getName());

        char TempArg1[NAME_LENGTH];
        strcpy_s(TempArg1, Stemp.top());
        Stemp.pop();
        char TempArg2[NAME_LENGTH];
        strcpy_s(TempArg2, Stemp.top());
        Stemp.pop();

        NewLabel(L1);
        Quadruples* p = Generate(Temp_op, TempArg2, TempArg1, L1);
        p->label = T->begin;

        Quadruples* q = Generate("assign", "0", "-", T->getName());
        p->next = q;

        if (T->next[0] == '\0')
        {
            NewLabel(T->next);
        }
        q = Generate("j", "-", "-", T->next);
        p->next->next = q;


        q = Generate("assign", "1", "-", T->getName());
        q->label = L1;

        p->next->next->next = q;
        q->next = NULL;

        Stemp.push(T->name);

        if (T->QLparent->head != NULL && T->QLparent->tail != NULL)
        {
            T->QLparent->tail->next = p;
            T->QLparent->tail = q;
        }
        else
        {
            T->QLparent->head = p;
            T->QLparent->tail = q;
        }

    }
}

//41:	<relop> ::= <
//42:	<relop> ::= <=
//43:	<relop> ::= >
//44:	<relop> ::= >=
//45:	<relop> ::= ==
//46:	<relop> ::= !=
void Semantic::SemanticFun_41(STree* T)
{
    if (T->flag == 1)
    {
        strcpy_s(T->name, T->child[0]->data);
    }
}
//47:	<加法表达式> ::= <项> <可选加法表达式>
//51:	<项> ::= <因子> <可选项>
void Semantic::SemanticFun_47(STree *T)
{
    if (T->flag == 0)
    {
        /*T结点的QLchild的code初始化为孩子结点的QLparent*/
        for (int i = 0; i < MAX_CHILD_NUM; i++)
        {
            if (T->child[i] != NULL)
                T->QLchild[i] = T->child[i]->QLparent;
        }
        T->child[1]->next = T->next;
        T->child[0]->begin = T->begin;
        T->child[1]->begin = T->begin;
    }
    else
    {
        /*孩子结点的code进行连接*/
        int No[MAX_CHILD_NUM] = { 0, 1, -1, -1, -1, -1 };
        QList_Connect(2, No, T->QLparent, T->QLchild);

        if (T->child[1]->ProNo == 39)
        {
            T->child[1]->begin = T->begin;
        }

        /*T结点的name和next赋值*/
        strcpy_s(T->name, Stemp.top());

    }
}
//48:  <可选加法表达式> ::= + <项> <可选加法表达式>
//49:  <可选加法表达式> ::= - <项> <可选加法表达式>
//52 : <可选项> :: = * <因子> <可选项>
//53 : <可选项> :: = / <因子> <可选项>
void Semantic::SemanticFun_48(STree* T)
{
    if (T->flag == 0)
    {
        /*T结点的QLchild的code初始化为孩子结点的QLparent*/
        for (int i = 0; i < MAX_CHILD_NUM; i++)
        {
            if (T->child[i] != NULL)
                T->QLchild[i] = T->child[i]->QLparent;
        }
        T->child[2]->next = T->next;
    }
    else
    {
        /*孩子结点的code进行连接*/
        int No[6] = { 1, 2, -1, -1, -1, -1 };
        QList_Connect(2, No, T->QLparent, T->QLchild);

        /*产生新的四元式（+）和对T结点的name,next赋值*/
        NewTemp(T->name);
        char Temp_op[20];
        strcpy_s(Temp_op, T->child[0]->data);						//op域赋值

        char TempArg1[NAME_LENGTH];
        strcpy_s(TempArg1, Stemp.top());
        Stemp.pop();

        char TempArg2[NAME_LENGTH];
        strcpy_s(TempArg2, Stemp.top());
        Stemp.pop();

        Quadruples* NewQuad = Generate(Temp_op, TempArg2, TempArg1, T->name);
        Stemp.push(T->name);

        NewQuad->next = NULL;
        if (T->QLparent->head != NULL && T->QLparent->tail != NULL)
        {
            T->QLparent->tail->next = NewQuad;								//新的四元式加入到链的尾部
            T->QLparent->tail = NewQuad;
        }
        else
        {
            T->QLparent->head = NewQuad;
            T->QLparent->tail = NewQuad;
        }

        /*对T的True和False赋值(当前产生式不需要)*/
    }
}

//<因子> ::= num
void Semantic::SemanticFun_55(STree* T)
{
    if (T->flag == 1)
    {
        T->setName(T->getChild(0)->getName());
        Stemp.push(T->name);
    }
}
//<因子> ::= ( <表达式> )
void Semantic::SemanticFun_56(STree* T)
{
    if (T->flag == 0)
    {
        T->QLchild[1] = T->getChild(1)->QLparent;
    }
    else
    {
        T->setName(T->getChild(1)->getName());

        //传递四元式串
        int No[MAX_CHILD_NUM] = { 1,-1,-1,-1,-1,-1 };
        QList_Connect(1, No, T->QLparent, T->QLchild);
    }

}
//<因子> ::= <ID>  FTYPE
void Semantic::SemanticFun_57(STree* T)
{
    if (T->flag == 0)
    {
        T->QLchild[0] = T->getChild(0)->QLparent;
        T->QLchild[1] = T->getChild(1)->QLparent;
    }
    else
    {
        T->setName(T->getChild(0)->getName());
        Stemp.push(T->name);

        Use_Check(T->name);
        //传递四元式串
        int No[MAX_CHILD_NUM] = { 0,1,-1,-1,-1,-1 };
        QList_Connect(2, No, T->QLparent, T->QLchild);

        //本节点新生成的四元式
        if (T->getChild(1)->ProNo == 58)
        {
            Quadruples* NewQuad = Generate("call", Stemp.top(), "-", "-");
            NewQuad->next = NULL;
            if (T->QLparent->head != NULL && T->QLparent->tail != NULL)
            {
                T->QLparent->tail->next = NewQuad;
                T->QLparent->tail = NewQuad;
            }
            else
            {
                T->QLparent->head = NewQuad;
                T->QLparent->tail = NewQuad;
            }
        }
    }
}
//<实参列表> ::= <表达式> <可选实参列表>
void Semantic::SemanticFun_63(STree* T)
{
    if (T->flag == 0)
    {
        T->QLchild[0] = T->getChild(0)->QLparent;
        T->QLchild[1] = T->getChild(1)->QLparent;
    }
    else
    {
        //传递四元式串
        int No[MAX_CHILD_NUM] = { 0,1,-1,-1,-1,-1 };
        QList_Connect(2, No, T->QLparent, T->QLchild);
        //本节点新生成的四元式

        Quadruples* NewQuad = Generate("param", T->getChild(0)->getName(), "-", "-");
        NewQuad->next = NULL;

        if (T->QLparent->head != NULL && T->QLparent->tail != NULL)
        {
            T->QLparent->tail->next = NewQuad;
            T->QLparent->tail = NewQuad;
        }
        else
        {
            T->QLparent->head = NewQuad;
            T->QLparent->tail = NewQuad;
        }

    }
}
//<可选实参列表> ::= , <表达式> <可选实参列表>
void Semantic::SemanticFun_64(STree* T)
{
    if (T->flag == 0)
    {
        T->QLchild[0] = T->getChild(0)->QLparent;
        T->QLchild[1] = T->getChild(1)->QLparent;
        T->QLchild[2] = T->getChild(2)->QLparent;
    }
    else
    {
        //传递四元式串
        int No[MAX_CHILD_NUM] = { 1,2,-1,-1,-1,-1 };
        QList_Connect(2, No, T->QLparent, T->QLchild);
        //本节点新生成的四元式
        Quadruples* NewQuad = Generate("param", T->getChild(0)->getName(), "-", "-");
        NewQuad->next = NULL;

        if (T->QLparent->head != NULL && T->QLparent->tail != NULL)
        {
            T->QLparent->tail->next = NewQuad;
            T->QLparent->tail = NewQuad;
        }
        else
        {
            T->QLparent->head = NewQuad;
            T->QLparent->tail = NewQuad;
        }
    }
}
//只传递四元式链
void Semantic::SemanticFun_Other(STree* T)
{
    if (T->flag == 0)
    {
        for (int i = 0; i < MAX_CHILD_NUM; i++)
        {
            if (T->child[i] != NULL)
                T->QLchild[i] = T->child[i]->QLparent;
        }
    }
    else
    {

        /*孩子结点的code进行连接*/

        int num = 0;
        int No[MAX_CHILD_NUM] = { -1,-1,-1,-1,-1,-1 };

        for (int i = 0; i < T->child_num; i++)
        {
            if (T->QLchild[i]->head != NULL)
            {
                No[num] = i;
                num++;
            }
        }
        QList_Connect(num, No, T->QLparent, T->QLchild);
    }
}

int Semantic::Define_Check(char IDname[NAME_LENGTH])
{
    ST_Node* p = Stable.Scurrent->head;
    while (p)
    {
        if (strcmp(p->name, IDname) == 0)
        {
            ERR_Node* E = new ERR_Node;
            strcpy_s(E->ErrorMessage, IDname);
            strcat(E->ErrorMessage, " redefinition.");
            Etable.AddError(E);
            return -1;
        }
        p = p->next;
    }
    return 0;
}
int Semantic::Use_Check(char IDname[NAME_LENGTH])
{
    SymbolTable* tp= Stable.Scurrent;
    ST_Node *np;
    while (tp)
    {
        np = tp->head;
        while (np)
        {
            if (strcmp(np->name, IDname) == 0)
            {
                return 0;
            }
            np = np->next;
        }
        tp = tp->parent_table;
    }
    ERR_Node* E = new ERR_Node;

    strcpy_s(E->ErrorMessage, IDname);
    strcat(E->ErrorMessage, " is undefined.");
    Etable.AddError(E);

    return -1;
}

/*print Stable in console*/
void Semantic::StableOut()
{
    SymbolTable* T = Stable.Sroot;

    stack <SymbolTable*> ST;
    stack <int> DT;
    SymbolTable* current;

    ST.push(T);
    DT.push(0);

    int depth = 0;

    while (!ST.empty())
    {
        current = ST.top();
        ST.pop();
        depth = DT.top();
        DT.pop();
        for (int i = 0; i < depth; i++)
        {
            qDebug() << "  ";
        }

        depth++;
        for (ST_Node* temp = current->head; temp != NULL ; temp = temp->next)
        {
            if(temp->child_table != NULL)
            {
                ST.push(temp->child_table);
                DT.push(depth);
            }

        }
    }
}
