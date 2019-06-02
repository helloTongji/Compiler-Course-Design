#define _CRT_SECURE_NO_WARNINGS
#include "STree.h"
#include <iostream>
#include<string.h>
using namespace std;
STree::STree()
{
    for (int i = 0; i < MAX_CHILD_NUM; i++)
    {
        child[i] = NULL;
        QLchild[i] = NULL;
    }
    QLparent = new Quadruples_List;
    QLparent->head = NULL;
    QLparent->tail = NULL;
    begin = NULL;
    next = NULL;
    True = NULL;
    False = NULL;
    child_num = 0;
    ProNo = -1;
}
STree::STree(const char* str)
{
    data=str;
    for (int i = 0; i < MAX_CHILD_NUM; i++)
    {
        child[i] = NULL;
        QLchild[i] = NULL;
    }
    QLparent = new Quadruples_List;
    QLparent->head = NULL;
    QLparent->tail = NULL;
    begin = NULL;
    next = NULL;
    True = NULL;
    False = NULL;
    child_num = 0;
    ProNo = -1;
}
void STree::setData(const char *str)
{
    data=str;
}
void STree::setName(const char *str)
{
    strcpy(name, str);
}
const char* STree::getData()
{
    return data;
}
char* STree::getName()
{
    return name;
}
int STree::getchild_num()
{
    return child_num;
}
STree* STree::getChild(int position)
{
    return child[position];
}
void STree::Add_Child(STree T, int position)
{
    child[position] = new STree;
    child[position]->data = T.data;
    for (int i = 0; i < MAX_CHILD_NUM; i++)
        child[position]->child[i] = NULL;
    child_num++;
}
void STree::QuadOut(STree* T)
{
    Quadruples* Temp;
    cout << setiosflags(ios::left) << setw(16) << "label" << setw(16) << "op" << setw(16) << "arg1" << setw(16)
        << "arg2" << setw(16) << "result" << endl;
    Temp = T->QLparent->head;
    while (Temp != NULL)
    {
        if(Temp->label)
        {
            cout << setiosflags(ios::left) << setw(16)<< Temp->label;
        }
        else
        {
            cout<< setiosflags(ios::left) << setw(16)<< " ";
        }
        cout << setiosflags(ios::left) << setw(16) << Temp->op << setw(16) << Temp->arg1 << setw(16)
            << Temp->arg2 << setw(16) << Temp->result << endl;
        Temp = Temp->next;
    }
}
