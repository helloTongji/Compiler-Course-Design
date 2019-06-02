#include "syntax.h"
#include "STree.h"
#include<QDebug>
bool equal(char* s1, const char* s2)
{
    int i;
    int len1 = strlen(s1);
    int len2 = strlen(s2);
    if (len1 != len2)
        return false;
    else
    {
        for (i = 0; i < len1; i++)
        {
            if (s1[i] != s2[i])
                return false;
        }
        return true;
    }
}
Grammar::Grammar()
{
    int i, j;
    //置初值
    for (i = 0; i <= Nonterminal_Sign_num; i++)
    {
        nonterminal_sign[i] = NULL;
    }
    for (i = 0; i < Production_num; i++)
    {
        proc[i].left = 0;
        for (j = 0; j < Production_maxlength; j++)
        {
            proc[i].right[j] = 0;
        }
    }
    for (i = 0; i <= Nonterminal_Sign_num; i++)
    {
        for (j = 0; j <= Terminal_Sign_num; j++)
        {
            first_set[i][j] = 0;
            follow_set[i][j] = 0;
        }
    }

    //定义终结符
    terminal_sign[0] = NULL;
    terminal_sign[1] = "int";
    terminal_sign[2] = "void";
    terminal_sign[3] = "if";
    terminal_sign[4] = "else";
    terminal_sign[5] = "while";
    terminal_sign[6] = "return";
    //关键字
    terminal_sign[7] = "<ID>";
    //标识符
    terminal_sign[8] = "num";
    //数值
    terminal_sign[9] = "+";
    terminal_sign[10] = "-";
    terminal_sign[11] = "*";
    terminal_sign[12] = "/";
    terminal_sign[13] = "=";
    terminal_sign[14] = "<";
    terminal_sign[15] = "<=";
    terminal_sign[16] = ">";
    terminal_sign[17] = ">=";
    terminal_sign[18] = "==";
    terminal_sign[19] = "!=";
    terminal_sign[20] = "(";
    terminal_sign[21] = ")";
    terminal_sign[22] = "{";
    terminal_sign[23] = "}";
    terminal_sign[24] = ",";
    terminal_sign[25] = ";";
    terminal_sign[26] = "NONE";
    terminal_sign[27] = "#";
    //算符
    tsign_num = 27;
}
void Grammar::Init_Grammar()
{
    char ch;
    char temp[100];			//存放从文件读入的字符串
    int len = 0;			//记录读入字符串的长度
    int line = 1;			//记录当前是第几条产生式（第几行）
    int count = 0;			//记录当前读入单词是该行的第几个单词
    bool terminal_existflag = 0;
    bool nonterminal_existflag = 0;
    bool word_flag=0;
    int i;

    ntsign_num = 0;
    proc_num = 0;

    ifstream infile;
//    infile.open("D:\\visual studio projects\\Compiler_Win\\grammar-en.txt", ios::in);
    infile.open("C:\\Users\\admin\\Desktop\\1652262\\grammar-en.txt", ios::in);
    if (!infile.is_open())
    {
        qDebug() << "打开文件失败" << endl;
        return;
    }

    while (!infile.eof())
    {
        ch = infile.get();
        //一个单词读入完成
        if (ch == ' ' || ch == '\n')
        {
            if (word_flag == 1)
            {
                temp[len] = '\0';
                if (!(strlen(temp) == 3 && temp[0] == ':'&&temp[1] == ':'&&temp[2] == '='))
                {
                    count++;
                    //判断终结符表中是否已经存有当前读入的符号
                    for (i = 1; i <= tsign_num; i++)
                    {
                        if (equal(temp, terminal_sign[i]))
                        {
                            terminal_existflag = 1;
                            break;
                        }
                    }
                    //已存有，直接用编号建立对应关系
                    if (terminal_existflag == 1)
                    {
                        if (count == 1)
                        {
                            proc[line].left = i;
                            proc[line].left_terflag = 1;
                        }
                        else
                        {
                            proc[line].right[count - 2] = i;
                            proc[line].right_terflag[count - 2] = 1;
                        }
                    }
                    //终结符表中没有该符号，则继续判断非终结符表中是否已经存有当前读入的符号
                    else
                    {

                        for (i = 1; i <= ntsign_num; i++)
                        {
                            if (equal(temp, nonterminal_sign[i]))
                            {
                                nonterminal_existflag = 1;
                                break;
                            }
                        }
                        //已存有，直接用编号建立对应关系
                        if (nonterminal_existflag == 1)
                        {
                            if (count == 1)
                            {
                                proc[line].left = i;
                                proc[line].left_terflag = 0;
                            }
                            else
                            {
                                proc[line].right[count - 2] = i;
                                proc[line].right_terflag[count - 2] = 0;
                            }
                        }
                        //未存有，动态生成新的非终结符，并建立对应关系
                        else
                        {
                            ntsign_num++;
                            nonterminal_sign[ntsign_num] = new char[len + 1];
                            strcpy_s(nonterminal_sign[ntsign_num], len + 1, temp);
                            if (count == 1)
                            {
                                proc[line].left = ntsign_num;
                                proc[line].left_terflag = 0;
                            }
                            else
                            {
                                proc[line].right[count - 2] = ntsign_num;
                                proc[line].right_terflag[count - 2] = 0;
                            }

                        }
                    }
                    terminal_existflag = 0;
                    nonterminal_existflag = 0;
                }
            }
            //一行结束，一条产生式处理完毕
            if (ch == '\n')
            {
                line++;
                count = 0;
            }
            //一个单词处理完毕，相关变量置0
            word_flag = 0;
            len = 0;
        }
        //未读入一个完整的单词
        else
        {
            if (word_flag == 0)
            {
                word_flag = 1;
                temp[len] = ch;
                len++;
            }
            else
            {
                temp[len] = ch;
                len++;
            }
        }
    }
    proc_num = line - 1;
    infile.close();
}
void Grammar::Show_Grammar()
{
    int i, j;
    for (i = 0; i < proc_num; i++)
    {
        if (proc[i].left_terflag == 1)
            cout << terminal_sign[proc[i].left] << ' ';
        else
            cout << nonterminal_sign[proc[i].left] << ' ';
        cout << "::= ";
        for (j = 0; proc[i].right[j] != 0 && j < Production_maxlength; j++)
        {
            if (proc[i].right_terflag[j] == 1)
                cout << terminal_sign[proc[i].right[j]] << ' ';
            else
                cout << nonterminal_sign[proc[i].right[j]] << ' ';
        }
        cout << endl;
    }
}
bool Grammar::Exist(int A[Terminal_Sign_num + 1], int e)
{
    int i;
    bool flag = 0;
    for (i = 0; i < tsign_num && A[i] != 0; i++)
    {
        if (A[i] == e)
        {
            flag = 1;
            break;
        }
    }
    return flag;
}
bool Grammar::Add_etoB(int e, int B[Terminal_Sign_num + 1])
{
    bool flag = 0;
    bool exist_flag = 0;
    int d;
    for (d = 0; d < tsign_num && B[d] != 0; d++)
    {
        if (e == B[d])
        {
            exist_flag = 1;
            break;
        }
    }
    if (exist_flag == 0 && d < tsign_num)
    {
        B[d] = e;
        flag = 1;
    }
    return flag;
}
bool Grammar::Add_AtoB(int A[Terminal_Sign_num + 1], int B[Terminal_Sign_num + 1], bool cancel_empty)
{
    bool flag = 0;
    bool exist_flag = 0;
    int s, d;
    for (s = 0; s < tsign_num && A[s] != 0; s++)
    {
        exist_flag = 0;
        for (d = 0; d < tsign_num && B[d] != 0; d++)
        {
            if (A[s] == B[d])
            {
                exist_flag = 1;
                break;
            }
        }
        if (exist_flag == 0 && d < tsign_num && !(A[s] == 26 && cancel_empty == true))
        {
            B[d] = A[s];
            flag = 1;
        }
    }
    return flag;
}
void Grammar::First()
{
    bool flag = 1;
    int i, j;
    //构造First集算法第（2）步
    for (i = 0; i < proc_num; i++)
    {
        //某个产生式右端第一个单词是终结符
        if (proc[i].right_terflag[0] == 1)
        {
            //判断First集中是否已存有该终结符，如果没有，加进去
            Add_etoB(proc[i].right[0], first_set[proc[i].left]);
        }
    }
    //构造First集算法第（3）步
    while (flag == 1)
    {
        flag = 0;
        //Show_First();
        for (i = 0; i < proc_num; i++)
        {
            //某个产生式右端第一个单词是非终结符
            if (proc[i].right_terflag[0] == 0)
            {
                //把右端第一个非终结符的First集加到左端的非终结符的First集里
                flag = flag || Add_AtoB(first_set[proc[i].right[0]], first_set[proc[i].left], true);
                //如果产生式右端的某个非终结符的First集里含有元素"空"，把下一个符号的First集加到左端的非终结符的First集里
                for (j = 0; j < Production_maxlength - 1 && proc[i].right[j + 1] != 0 && proc[i].right_terflag[j] == 0; j++)
                {
                    if (Exist(first_set[proc[i].right[j]], 26))
                    {
                        if (proc[i].right_terflag[j + 1] == 1)
                        {
                            flag = flag || Add_etoB(proc[i].right[j + 1], first_set[proc[i].left]);
                        }
                        else
                            flag = flag || Add_AtoB(first_set[proc[i].right[j + 1]], first_set[proc[i].left], true);
                    }
                    else
                        break;
                }
                //如果产生式右端所有非终结符的First集里含有元素"空"，把"空"加到左端的非终结符的First集里
                if (j == Production_maxlength - 1 || proc[i].right[j + 1] == 0)
                {
                    if (Exist(first_set[proc[i].right[j]], 26))
                    {
                        flag = flag || Add_etoB(26, first_set[proc[i].left]);
                    }
                }
            }
        }
    }
}
void Grammar::Follow()
{
    bool flag = 1;
    int i, j;
    //构造Follow集算法第（1）步
    follow_set[1][0] = 27;
    while (flag == 1)
    {
        flag = 0;
        for (i = 0; i < proc_num; i++)
        {
            //构造Follow集算法第（2）步
            for (j = 0; j < Production_maxlength - 1 && proc[i].right[j + 1] != 0; j++)
            {
                if (proc[i].right_terflag[j] == 0)
                {
                    //该非终结符后面跟着终结符，把终结符加到非终结符的Follow集中
                    if (proc[i].right_terflag[j + 1] == 1)
                    {
                        flag = flag || Add_etoB(proc[i].right[j + 1], follow_set[proc[i].right[j]]);
                    }
                    //该非终结符后面跟着非终结符，把后面的非终结符的First集加到非终结符的Follow集中
                    else
                    {
                        flag = flag || Add_AtoB(first_set[proc[i].right[j + 1]], follow_set[proc[i].right[j]], true);
                    }
                }
            }
            //构造Follow集算法第（3）步
            for (j = 0; j < Production_maxlength && proc[i].right[j] != 0; j++);
            j--;
            if (proc[i].right_terflag[j] == 0)
            {
                flag = flag || Add_AtoB(follow_set[proc[i].left], follow_set[proc[i].right[j]], false);
                for (; j >= 1; j--)
                {
                    if (proc[i].right_terflag[j - 1] == 0 && Exist(first_set[proc[i].right[j]], 26))
                        flag = flag || Add_AtoB(follow_set[proc[i].left], follow_set[proc[i].right[j - 1]], false);
                    else
                        break;
                }
            }
        }
    }
}
void Grammar::Show_First()
{
    int i, j;
    cout << "First集:" << endl;
    for (i = 1; i <= ntsign_num; i++)
    {
        cout << nonterminal_sign[i] << " : ";
        for (j = 0; j < tsign_num && first_set[i][j] != 0; j++)
        {
            cout << terminal_sign[first_set[i][j]] << " ";
        }
        cout << endl;
    }
    cout << endl;
}
void Grammar::Show_Follow()
{
    cout << "Follow集:" << endl;
    int i, j;
    for (i = 1; i <= ntsign_num; i++)
    {
        cout << nonterminal_sign[i] << " : ";
        for (j = 0; j < tsign_num && follow_set[i][j] != 0; j++)
        {
            if (follow_set[i][j] == 100)
                cout << '#' << " ";
            else
                cout << terminal_sign[follow_set[i][j]] << " ";
        }
        cout << endl;
    }
    cout << endl;
}
void Grammar::fill_proc(int nter, int ter, int i)
{
    table[nter][ter].pro.left = proc[i].left;
    table[nter][ter].pro.left_terflag = proc[i].left_terflag;
    for (int j = 0; j < Production_maxlength; j++)
    {
        table[nter][ter].pro.right[j] = proc[i].right[j];
        table[nter][ter].pro.right_terflag[j] = proc[i].right_terflag[j];
    }
    table[nter][ter].ProNo = i;
}
void Grammar::MTable()
{
    int i, j, k;
    //置空值
    for (i = 0; i <= Nonterminal_Sign_num; i++)
    {
        for (j = 0; j <= Terminal_Sign_num; j++)
        {
            table[i][j].pro.left = 0;
            for (k = 0; k < Production_maxlength; k++)
            {
                table[i][j].pro.right[k] = 0;
            }
            table[i][j].ProNo = -1;
        }
    }
    for (i = 1; i <= proc_num; i++)
    {
        //构造分析表算法第（2）步
        if (proc[i].right_terflag[0] == 1)
        {
            if (proc[i].right[0] == 26)
            {
                for (k = 0; k < tsign_num && follow_set[proc[i].left][k] != 0; k++)
                    fill_proc(proc[i].left, follow_set[proc[i].left][k], i);
            }
            else
                fill_proc(proc[i].left, proc[i].right[0], i);
        }
        else
        {
            for (j = 0; j < tsign_num && first_set[proc[i].right[0]][j] != 0; j++)
            {
                if (first_set[proc[i].right[0]][j] == 26)
                {
                    for (k = 0; k < tsign_num && follow_set[proc[i].left][k] != 0; k++)
                        fill_proc(proc[i].left, follow_set[proc[i].left][k], i);
                }
                else
                    fill_proc(proc[i].left, first_set[proc[i].right[0]][j], i);
            }
        }

    }

}
bool Analyze(STree* &root,FILE* fp)
{
    Grammar G;
    G.Init_Grammar();
    G.First();
    G.Follow();
    G.MTable();

    Lexical L;
   int i;
    int ChildNo;		//在用某条产生式生成子节点的时候记录子节点个数
    Word next_word;
    //语法分析要用到的栈
    stack <Elem> S;
    stack <STree*> ST;
    //语法分析树根节点
    STree GTree;
    root = &GTree;
    STree* node;
    STree temp;

    Elem e;
    Elem left;
    //把#号和起始符号压入栈
    e.code = 27;
    e.ter_flag = 1;
    S.push(e);
    e.code = 1;
    e.ter_flag = 0;
    S.push(e);
    GTree.setData(G.nonterminal_sign[1]);
    ST.push(&GTree);

    next_word = L.Result(fp);

    while (!S.empty())
    {
        //栈顶是终结符
        if (S.top().ter_flag == 1)
        {
            if (next_word.code == S.top().code)
            {
                S.pop();
                if (S.empty())
                    break;
                node = ST.top();
                ST.pop();
                //如果节点是<ID>（标识符）或者num（数值），记录下它的值
                if (strcmp(node->getData(), G.terminal_sign[7]) == 0 || strcmp(node->getData(), G.terminal_sign[8]) == 0)
                    node->setName(next_word.value);

                next_word = L.Result(fp);
            }
            else
                return false;

        }
        //栈顶是非终结符
        else
        {
            if (G.table[S.top().code][next_word.code].pro.left != 0)
            {
                left = S.top();
                node = ST.top();
                node->ProNo = G.table[S.top().code][next_word.code].ProNo;
                S.pop();
                ST.pop();
                for (ChildNo = 0; ChildNo < Production_maxlength && G.table[left.code][next_word.code].pro.right[ChildNo] != 0; ChildNo++);
                ChildNo--;
                for (i = ChildNo; i >= 0; i--)
                {
                    e.code = G.table[left.code][next_word.code].pro.right[i];
                    e.ter_flag= G.table[left.code][next_word.code].pro.right_terflag[i];
                    if (!(e.code == 26 && e.ter_flag == 1))
                        S.push(e);

                    if (G.table[left.code][next_word.code].pro.right_terflag[i] == 1)
                        temp.setData(G.terminal_sign[G.table[left.code][next_word.code].pro.right[i]]);
                    else
                        temp.setData(G.nonterminal_sign[G.table[left.code][next_word.code].pro.right[i]]);

                    node->Add_Child(temp, i);
                    if (strcmp(temp.getData() ,"NONE")!=0)
                        ST.push(node->getChild(i));
                }
            }
            else
                return false;
        }

    }
    fclose(fp);
    return true;
}
