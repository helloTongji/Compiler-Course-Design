#include <fstream>
using namespace std;

class yfq
{
private:
	int end_flag;       //程序结束标志
	int read_main;      //是否已经读过main函数
	int return_flag;
	char return_words[100];
	char c;             //c = fin.get()，临时读入变量
	char bl_name[100];        //临时记录变量名(以'\0'结束)
	char hs_name[100];        //临时记录函数名（以'\0'结束）
	int len_hs;              //从函数名至')'的长度
	int available_T;         //Ti,i=available_T，为当前可用的i
	int available_L;         //Li,i=available_L
	char var_name[10][100];    //记录参数名
	int kh[100];             //记录读到的右括号
	int len_kh;              //
	char headwords[100];     //句首字符
	int is_hs_head;        //是否函数居于句首标志（判断是否需要return）
	int is_hs_dy;          //是否函数定义
	ifstream fin;
	ofstream fout1;         //输出中间代码
	ofstream fout2;         //输出汇编代码
public:
	yfq();
	~yfq();
	bool zs_handle();                       //判断是否为有效注释 && 去除注释
											//bool u_handle(char u[], int len_u,int mode);     //因子处理函数
											//bool s_handle(char c[], int len_c);     //加法表达式处理函数
	int fh_rank(char fh);                   //计算符号等级
	bool r_handle(char c[], int len_c, int mode);      //表达式处理函数，mode: 0-> = r;  1-> if( r )  2-> while( r )
	bool bl_handle();                       //句首为变量的处理函数
	bool void_handle();                     //句首为void的处理函数
	bool int_handle();                      //句首为int的处理函数
	int get_kind(char c[], int len_c);      //合法变量名，合法数字，
	void error_rep();                       //报错及相关处理函数
	int readhead();                         //读句首
	void analyze();
};
