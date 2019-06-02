#ifndef STREE_H
#define STREE_H
#define MAX_CHILD_NUM 6
#define NAME_LENGTH 50
#define LABEL_LENGTH 50
#include <QTableWidget>
#include <iomanip>




struct Quadruples {
        char op[20];
        char arg1[NAME_LENGTH];
        char arg2[NAME_LENGTH];
        char result[NAME_LENGTH];
        struct Quadruples* next;
        char* label;		//该四元式所在行对应的标号
};
struct Quadruples_List {
        Quadruples* head;
        Quadruples* tail;
};
class Semantic;
class Grammar;
class MainWindow;
class STree{
private:
        const char* data;
        int child_num;
        STree* child[MAX_CHILD_NUM];


        //变量名
        char name[NAME_LENGTH];
        //各种Label
        char* True;
        char* False;
        char* next;
        char* begin;

        bool flag;		//访问标志，区分语法树的遍历是进去还是回来的时候
        Quadruples_List* QLparent;
        Quadruples_List* QLchild[MAX_CHILD_NUM];

public:
        int ProNo;
        STree();
        STree(const char* str);

        void setData(const char *str);
        void setName(const char *str);
        const char* getData();
        char* getName();

        int getchild_num();
        STree* getChild(int position);
        void Add_Child(STree T,int position);
        friend Semantic;
        friend MainWindow;
        friend bool Analyze(STree* &root);

        void QuadOut(STree* T);
};
#endif // STREE_H
