#include "lexical.h"
#include<iostream>
#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include<fstream>
#include<vector>
#include<QString>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QObject>
#include <QDebug>
#include <QTextCodec>
using namespace std;



void Lexical::clearmid()
{
    int i;
    for (i = 0; i<20; i++)
        mid[i] = '\0';
}

int Lexical::IsNum(char c)
{
    if (c >= '0'&&c <= '9')
        return 1;
    else
        return 0;
}

int Lexical::IsChar(char c)
{
    if ((c >= 'a'&&c <= 'z') || (c >= 'A'&&c <= 'Z') || c == '_')
        return 1;
    else
        return 0;
}

int Lexical::IsKeyWord(char *MID)
{
    unsigned int i;
    for (i = 0; i < keyword.size(); i++)
    {
        if (strcmp(MID, keyword[i].second) == 0)
            return keyword[i].first;
    }
    return -1;
}

int Lexical::IsOperator(char c)
{
    if (c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>' || c == '!')
        return 1;
    else
        return 0;
}

int Lexical::IsLimiter(char c)
{
    if (c == '(' || c == ')' || c == '{' || c == '}' || c == '[' || c == ']' || c == ',' || c == ';' || c == '\'' || c == '\"')
        return 1;
    else
        return 0;
}

void Lexical::printf(int a) {

    switch (a) {

    case 1:
    {
        RESULT.push_back(make_pair(QString::fromLocal8Bit(mid), "主函数"));
        break;
    }
    case 2:
    {
        RESULT.push_back(make_pair(QString::fromLocal8Bit(mid), "返回"));
        break;
    }
    case 3:
    {
        RESULT.push_back(make_pair(QString::fromLocal8Bit(mid), "关键字"));
        break;
    }
    case 4:
    {
        RESULT.push_back(make_pair(QString::fromLocal8Bit(mid), "标识符"));
        break;
    }
    case 5:
    {
        RESULT.push_back(make_pair(QString::fromLocal8Bit(mid), "数值"));
        break;
    }
    case 6:
    {
        RESULT.push_back(make_pair(QString::fromLocal8Bit(mid), "运算符"));
        break;
    }
    case 7:
    {
        RESULT.push_back(make_pair(QString::fromLocal8Bit(mid), "界符"));
        break;
    }
    case 8:
    {
        RESULT.push_back(make_pair(QString::fromLocal8Bit(mid), "字符串"));
        break;
    }
    case 9:
    {
        RESULT.push_back(make_pair(QString::fromLocal8Bit(mid), "字符串"));
        break;
    }
    default:
        break;

    }
}

Word Lexical::Result(FILE* fp)
{
    Word w;
    char ch;
    int i;
    int code;

    clearmid();
    i = 0;

    if (feof(fp))
    {
        w.code = 27;
        w.value[0] = '#';
        w.value[1] = '\0';
        return w;
    }
    ch = fgetc(fp);
    //读入的是空格，回车，或者tab，则忽略，继续读入下一个字符
    while (!feof(fp) && (ch == ' ' || ch == '\n' || ch == '\t'))
    {
        ch = fgetc(fp);
    }

    if (feof(fp) && (ch == ' ' || ch == '\n' || ch == '\t'))//已经读到文件末尾,而且最后读入的字符是空格，回车，或者tab
    {
        w.code = 27;
        w.value[0] = '#';
        w.value[1] = '\0';
        return w;
    }


    if (IsChar(ch)) //首字符是字母
    {
        mid[i] = ch;
        i++;
        while (!feof(fp))
        {
            ch = fgetc(fp);
            if (IsNum(ch) || IsChar(ch))
            {
                mid[i] = ch;
                i++;
            }
            else
            {
                fseek(fp, -1, SEEK_CUR);       //把多读入的那个字符退回去（文件指针回退1位）
                break;
            }
        }
        mid[i] = '\0';

        code = IsKeyWord(mid);
        if (code >= 1 && code <= 6)
            w.code = code;
        else
            w.code = 7;

        if (code != -1)
        {
            if (code == 0)
                printf(1);      //main
            else if (code == 6)
                printf(2);      //return
            else
                printf(3);      //其他关键字
        }
        else
        {
            printf(4);      //标识符
        }
    }

    else if (IsNum(ch))//首字符是数字
    {
        mid[i] = ch;
        i++;
        while (!feof(fp))
        {
            ch = fgetc(fp);
            if (IsNum(ch))
            {
                mid[i] = ch;
                i++;
            }
            else
            {
                fseek(fp, -1, SEEK_CUR);       //把多读入的那个字符退回去（文件指针回退1位）
                break;
            }
        }
        if (IsChar(ch))
        {
            printf(0);      //出错
            w.code = 0;
            return w;
        }
        mid[i] = '\0';

        w.code = 8;
        printf(5);      //数值
    }

    else if (IsOperator(ch))//首字符是运算符
    {
        mid[i] = ch;
        i++;
        if (!feof(fp) && (ch == '<' || ch == '>' || ch == '=' || ch == '!'))
        {
            ch = fgetc(fp);
            if (ch == '=')
            {
                mid[i] = ch;
                i++;
            }
            else
            {
                fseek(fp, -1, SEEK_CUR);
            }
        }
        mid[i] = '\0';

        unsigned int j = 0;
        for (j = 0; j < opera.size(); j++)
        {
            if (strcmp(mid, opera[j].second) == 0)
            {
                w.code = opera[j].first;
                printf(6);
                break;
            }
        }
    }

    else if (IsLimiter(ch))//首字符是界符
    {
        mid[i] = ch;
        i++;
        if (ch == '\'')
        {
            char tempch;
            bool flag = false;
            int len = 0;
            if (!feof(fp))
            {
                tempch = fgetc(fp);
                len++;
                if (tempch == '\\')    //转义字符
                    flag = true;
                while (!feof(fp))
                {
                    tempch = fgetc(fp);
                    len++;
                    if ((flag = false && len == 2) || (flag == true && len == 3))
                        break;
                }

                if (tempch == '\'' && (flag = false && len == 2 || flag == true && len == 3))
                {
                    fseek(fp, -len, SEEK_CUR);
                    for (int k = 1; k <= len; k++)
                    {
                        ch = fgetc(fp);
                        mid[i] = ch;
                        i++;
                    }
                    mid[i] = '\0';
                    printf(8);
                }
                else
                {
                    printf(0);
                }

            }
            else
            {
                printf(0);
            }
            w.code = 0;
        }
        if (ch == '\"')    //读入的界符是双引号，可能是字符串
        {
            char tempch;
            int len = 0;
            while (!feof(fp))
            {
                tempch = fgetc(fp);
                len++;
                if (tempch == '\"' || tempch == '\n')
                {
                    break;
                }
            }
            if (tempch == '\"')//找到与左双引号匹配右双引号
            {
                fseek(fp, -len, SEEK_CUR);
                ch = fgetc(fp);
                mid[i] = ch;
                i++;
                while (ch != '\"')
                {
                    ch = fgetc(fp);
                    mid[i] = ch;
                    i++;
                }
                mid[i] = '\0';

                printf(9);
            }
            else
            {
                printf(0);
            }
            w.code = 0;     //字符串  语法分析器无法识别
        }
        else
        {
            mid[i] = '\0';
            unsigned int j = 0;
            for (j = 0; j < opera.size(); j++)
            {
                if (strcmp(mid, opera[j].second) == 0)
                {
                    code = opera[j].first;
                    printf(7);
                    break;
                }
            }
            if (code>25)
                w.code = 0;//'['和']'这两个界符 语法分析器无法识别
            else
                w.code = code;
        }
    }

    else//无法识别的字符
    {
        if (feof(fp))
        {
            w.code = 27;
            w.value[0] = '#';
            w.value[1] = '\0';
            return w;
        }
        w.code = 0;
        printf(0);
    }
    if (w.code > 0)
    {
        strcpy(w.value, mid);
    }
    return w;
}

Lexical::Lexical()
{

    keyword.clear();
    keyword.push_back(make_pair(0,"main"));
    keyword.push_back(make_pair(1,"int"));
    keyword.push_back(make_pair(2,"void"));
    keyword.push_back(make_pair(3,"if"));
    keyword.push_back(make_pair(4,"else"));
    keyword.push_back(make_pair(5,"while"));
    keyword.push_back(make_pair(6,"return"));

    keyword.push_back(make_pair(30,"for"));
    keyword.push_back(make_pair(31,"double"));
    keyword.push_back(make_pair(32,"float"));
    keyword.push_back(make_pair(33,"short"));
    keyword.push_back(make_pair(34,"long"));

    keyword.push_back(make_pair(35,"char"));
    keyword.push_back(make_pair(36,"string"));
    keyword.push_back(make_pair(37,"break"));
    keyword.push_back(make_pair(38,"case"));
    keyword.push_back(make_pair(39,"default"));

    keyword.push_back(make_pair(40,"printf"));
    keyword.push_back(make_pair(41,"static"));
    keyword.push_back(make_pair(42,"const"));

    opera.clear();
    opera.push_back(make_pair(9,"+"));
    opera.push_back(make_pair(10,"-"));
    opera.push_back(make_pair(11,"*"));
    opera.push_back(make_pair(12,"/"));
    opera.push_back(make_pair(13,"="));

    opera.push_back(make_pair(14,"<"));
    opera.push_back(make_pair(15,"<="));
    opera.push_back(make_pair(16,">"));
    opera.push_back(make_pair(17,">="));
    opera.push_back(make_pair(18,"=="));
    opera.push_back(make_pair(19,"!="));

    opera.push_back(make_pair(20,"("));
    opera.push_back(make_pair(21,")"));
    opera.push_back(make_pair(22,"{"));
    opera.push_back(make_pair(23,"}"));
    opera.push_back(make_pair(24,","));
    opera.push_back(make_pair(25,";"));

    opera.push_back(make_pair(28,"["));
    opera.push_back(make_pair(29,"]"));
}

vector <pair<QString,const char *>>Lexical::getRESULT(){
    return RESULT;
}


