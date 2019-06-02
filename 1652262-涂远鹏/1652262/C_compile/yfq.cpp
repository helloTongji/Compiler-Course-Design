#include "yfq.h"
#include <fstream>
#include <string>
#include <cstring>
#include <iostream>
#include "structure.h"
using namespace std;


yfq::yfq()
{
	end_flag = 0;
	read_main = 0;      //读到main函数置1（在get_kind中置1）
	available_L = 0;    //当前可用使用的Li的i值
	available_T = 0;    //当前可用使用的Ti的i值
	is_hs_dy = 0;
	len_kh = 0;
	fin.open("inputfile.txt");
	fout1.open("mid_code.txt");
	fout2.open("mips_code.txt");
}
yfq::~yfq()
{
	fin.close();
	fout1.close();
	fout2.close();
}
void yfq::error_rep()
{
	yfq::end_flag = 1;
	cout << "程序有误！";
	return;
}

bool yfq::zs_handle()
{
	int flag;
	c = fin.get();
	if (c == '/')
		flag = 1;
	else if (c == '*')
		flag = 2;
	else
		return false;
	if (flag == 1)               //  形为//的注释
	{
		while (c != '\n')
		{
			c = fin.get();
		}
		return true;
	}
	else
	{

		while (1)
		{

			c = fin.get();
			while (c != '*')
			{

				c = fin.get();
			}

			c = fin.get();

			if (c == '/')
			{
				while (c != '\n')
					c = fin.get();
				return true;
			}
		}
		return true;
	}
}
int yfq::get_kind(char c[], int len_c)
// 输入格式：c[len_c] 为'\0', len_c为有效字符的长度
//返回：is_bl - 已定义变量 ；is_num - 数字 ；is_hs - 已定义函数 ； is_main - main函数
//其他：bl_name - 记录变量（函数）名 ；temp_num - 数字
{
	if (c[0] >= 'a' && c[0] <= 'z' && len_c <= 3)
	{
		strcpy_s(bl_name,20, c);               //bl_name记录变量名
		return is_bl;
	}
	else if (c[0] >= 'a' && c[0] <= 'z' && len_c > 3)
	{
		char main[7] = "main()";
		if (strcmp(c, main) == 0)
		{
			read_main = 1;
			kh[len_kh++] = is_main;
			return is_main;
		}
		else
		{
			for (int i = 0; i < 4; i++)
			{
				hs_name[i] = c[i];
			}
			hs_name[4] = '\0';
			if (is_hs_dy == 0)         //函数调用
			{
				var_name[0][0] = c[5];
				var_name[0][1] = '\0';
				var_name[1][0] = c[7];
				var_name[1][1] = '\0';
			}
			return is_hs;
		}
	}
	else if (c[0] >= '0' && c[0] <= '9')
	{
		strcpy_s(bl_name,20,c);
		return is_num;
	}
	else
		return error;

}
int yfq::fh_rank(char fh)
{
	if (fh == '<' || fh == '>' || fh == '=')
		return 1;
	else if (fh == '+' || fh == '-')
		return 2;
	else if (fh == '*' || fh == '/')
		return 3;
	else
		return error;
}
bool yfq::r_handle(char c[], int len_c,int mode)   
// 有效字符为 0 - len_c-1 ; c[len_c]无字符
//mode：0 -> = r;  1 -> if (r)  2 -> while (r)
{
	int i;
	int fh_num = 0;                         //0：无运算符 1：一个运算符 2：两个运算符
	for (i = 0; i < len_c; i++)
	{
		if (c[i] == '+' || c[i] == '-' || c[i] == '*' || c[i] == '/' || c[i] == '>' || c[i] == '<' || c[i] == '=')
			fh_num++;
	}
	if (mode > 2)
		return false;
	else
	{
		if (mode == 0 && fh_num == 0)       
		{
			c[len_c] = '\0';
			int r00 = get_kind(c,len_c);
			if (r00 == is_bl || r00 == is_num)
			{
				fout1 << "( = , " << bl_name << " , " << " _ " << headwords << " )" << endl<<endl;
				return true;
			}
			else if (r00 == is_hs)
			{
				fout1 << "par " << var_name[0] << endl;
				fout1 << "par " << var_name[1] << endl;
				fout1 << "call " << hs_name << endl;
				//cout << "is_hs_dy =" << is_hs_dy << endl << "is_hs_head =" << is_hs_head << endl;
				if(is_hs_dy == 0 && is_hs_head == 0)          //函数在式中调用
				    fout1 << "( = , " << return_words << " _ ," << headwords << " )" << endl;
				fout1 << "return" << endl << endl;
				return true;
			}
			else
				return error;
		}
		else if (mode == 0 && fh_num == 1)
		{
			int fh_pos;
			char temp_name1[20];
			char temp_name2[20];
			int t1;
			int t2 = 0;
			int t;
			for (fh_pos = 0; fh_pos < len_c; fh_pos++)
			{
				if (c[fh_pos] == '+' || c[fh_pos] == '-' || c[fh_pos] == '*' || c[fh_pos] == '/' || c[fh_pos] == '>' || c[fh_pos] == '<' || c[fh_pos] == '=')
					break;
			}
			for (t1 = 0; t1 < fh_pos; t1++)
				temp_name1[t1] = c[t1];
			temp_name1[t1] = '\0';
			for (t = fh_pos+1; t < len_c; t++,t2++)
				temp_name2[t2] = c[t];
			temp_name2[t2] = '\0';
			fout1 << "( " <<c[fh_pos]<<" , "<< temp_name1 << " , " << temp_name2 << " , " <<headwords<<")"<< endl<<endl;
		}
		else if (mode == 0 && fh_num == 2)
		{
			int fh_pos1 = -1;
			int fh_pos2 = -1;
			char temp_name1[20];
			char temp_name2[20];
			char temp_name3[20];
			int t;
			int k;
			for (t = 0; t < len_c; t++)
			{
				if (c[t] == '+' || c[t] == '-' || c[t] == '*' || c[t] == '/' || c[t] == '>' || c[t] == '<' || c[t] == '=')
				{
					if (fh_pos1 == -1)
						fh_pos1 = t;
					else
						fh_pos2 = t;
				}
			}
			for (t = 0; t < fh_pos1; t++)
				temp_name1[t] = c[t];
			temp_name1[t] = '\0';
			for (k=0,t = fh_pos1 + 1; t < fh_pos2; t++, k++)
				temp_name2[k] = c[t];
			temp_name2[k] = '\0';
			for (k = 0, t = fh_pos2 + 1; t < len_c; t++, k++)
				temp_name3[k] = c[t];
			temp_name3[k] = '\0';
			int rank1 = fh_rank(c[fh_pos1]);
			int rank2 = fh_rank(c[fh_pos2]);
			if (rank1 < rank2)
			{
				fout1 << "( " << c[fh_pos2] << " , " << temp_name2 << " , " << temp_name3 << " , " << "T" << available_T << " )" << endl;
				available_T++;
				fout1 << "( " << c[fh_pos1] << " , " << temp_name1 << " , " << "T" << available_T-1 <<" , "<<headwords<<  " )" << endl;
				return true;
			}
			else
			{
				fout1 << "( " << c[fh_pos1] << " , " << temp_name1 << " , " << temp_name2 << " , " << "T" << available_T << " )" << endl;
				available_T++;
				fout1 << "( " << c[fh_pos2] << " , " << temp_name3 << " , " << "T" << available_T -1<< " , " << headwords << " )" << endl;
				return true;
			}
		}
		else if (mode == 1) // if (r) ，c只为r
		{
			char name1[3];
			char name2[3];
			name1[0] = c[0];
			name1[1] = '\0';
			name2[0] = c[2];
			name2[1] = '\0';
			fout1 << "L" << available_L++ << ":" << endl;
			fout1 << "( j" << c[1] << " , " << name1 << " , " << name2 << " , " << "L" << available_L + 1 << " )" << endl;
			fout1 << "L" << available_L++ << ":" << endl; 
			fout1 << "( j " << " , _" <<  " , _" <<" , " << "L" << available_L + 1 << " )" << endl;
			fout1 << "L" << available_L++ << ":" << endl;
		}
		else if (mode == 2) // while (r) ，c只为r
		{
			char name1[3];
			char name2[3];
			name1[0] = c[0];
			name1[1] = '\0';
			name2[0] = c[2];
			name2[1] = '\0';
			fout1 << "L" << available_L++ << ":" << endl;
			fout1 << "( j" << c[1] << " , " << name1 << " , " << name2 << " , " << "L" << available_L + 1 << " )" << endl;
			fout1 << "L" << available_L++ << ":" << endl;
			fout1 << "( j " << " , _" << " , _" << " , " << "L" << available_L + 2 << " )" << endl;
			fout1 << "L" << available_L++ << ":" << endl;
		}
		return true;
	}
}
bool yfq::bl_handle()
{
	char tail[30];
	int len_tail = 0;
	while (c == ' ')
		c = fin.get();
	if (c != '=')
		return false;
	c = fin.get();
	while (c == ' ')
		c = fin.get();
	while (c != ';')
	{
		if (c == '\n')
			return false;
		else if (c != ' ')
			tail[len_tail++] = c;
		c = fin.get();
	}
	if (len_tail == 0)       //  = ;
		return false;
	else
		return r_handle(tail, len_tail,0);    //不以'\0'结尾
}
bool yfq::void_handle()
{
	c = fin.get();
	char temp[20];
	char n_hs[6];
	char cs1[4];
	char cs2[4];
	int len_temp = 0;
	while (c != ')')
	{
		temp[len_temp++] = c;
		c = fin.get();
	}
	temp[len_temp++] = ')';
	int i = 0;
	for (; i < 4; i++)
		n_hs[i] = temp[i];
	n_hs[4] = '\0';
	cs1[0] = temp[9];
	cs1[1] = '\0';
	cs2[0] = temp[15];
	cs2[1] = '\0';
	fout1 << n_hs << ":" << endl;
	return true;
}
bool yfq::int_handle()
{
	while (c == ' ')
	{
		c = fin.get();
	}
	char temp_name[20];
	int len_temp = 0;
	while (c != ')' && c != ';')
	{
		temp_name[len_temp++] = c;
		c = fin.get();
	}
	if(c == ')')
		temp_name[len_temp++] = ')';
	temp_name[len_temp] = '\0';
	cout << "temp_name = "<<temp_name << endl;
	int i_res = get_kind(temp_name,len_temp);
	cout << "i_res =" <<i_res<< endl;
	if (i_res == is_hs)
	{
		//cout << "int_handle with hs" << endl;
		fout1 << hs_name << ":" << endl;
	}
	else if (i_res == is_main)
	{
		cout << "int_handle with main" << endl;
		fout1 << endl << "main :" << endl;
	}
	return true;
}
int yfq::readhead()
{

	c = fin.get();
	while (c == ' ' || c == '\n' || c == '\r' || c == '{')       //处理结尾/开头的空格、\r、\n、{
	{
		c = fin.get();
	}
	char head[100];
	int len_head = 0;
	if (c == '}')
	{
		head[len_head++] = '}';
		cout << "read }" << endl;
	}
	if (c == '/')      //注释处理
	{
		if (zs_handle() == false)
			return error;
		else          //处理后得到'\n'
			c = fin.get();
	}
	if (len_head == 0)
	{
		while (c != ' ' && c != '(')
		{
			head[len_head++] = c;
			c = fin.get();
		}
	}
	head[len_head] = '\0';
	cout << "head = " << head << endl;
	if (strcmp(head, "void") == 0)
		return _void;
	else if (strcmp(head, "int") == 0)
	{
		//cout << "int" << endl;
		return _int;
	}
	else if (strcmp(head, "if") == 0)
	{
		//cout << "if" << endl;
		return _if;
	}
	else if (strcmp(head, "while") == 0)
	{
		//cout << "while" << endl;
		return _while;
	}
	else if (strcmp(head, "return") == 0)
	{
		//cout << "return" << endl;
		return _return;
	}
	else if (head[0] == '}')
	{
		//cout << "ydkh";
		return _ydkh;
	}
	else
	{
		if (c == '(')
		{
			while (c != ')')
			{
				head[len_head++] = c;
				c = fin.get();
			}
			head[len_head++] = ')'; 
			head[len_head] = '\0';
			len_hs = len_head;
			strcpy_s(hs_name,20,head);
			int res = get_kind(head, len_head);
			if (res != is_hs)
				return error;
			else
				return _hs;
		}
		else
		{
			int res = get_kind(head, len_head);
			if (res != is_bl)
				return error;
			else
				return _bl;
		}
	}

}
void yfq::analyze()
{
	while (end_flag == 0)             //程序停止标志
	{
		int headword = readhead();    //句首字符
		if (headword == _bl)          //句首为合法变量
		{
			
			strcpy_s(headwords,20,bl_name);  //将句首变量名记录到headname中
			is_hs_head = 0;
			
			if (bl_handle() == true)
				continue;
		}
		else if (headword == _hs)
		{
			is_hs_head = 1;
			
			r_handle(hs_name,len_hs,0); 
		}
		else if (headword == _void)
		{
			is_hs_head = 0;
			is_hs_dy = 1;
			if (void_handle() == true)
			{
				is_hs_dy = 0;
				continue;
			}
		}
		else if (headword == _int)
		{
			is_hs_head = 0;
			is_hs_dy = 1;
			if (int_handle() == true)
			{
				is_hs_dy = 0;
				continue;
			}

		}
		else if (headword == _if)
		{
			is_hs_head = 0;
			kh[len_kh++] = _if;
			char temp[20];
			int len_temp = 0;
			c = fin.get();        // '('
			c = fin.get();
			while (c != ')')
			{
				temp[len_temp++] = c;
				c = fin.get();
			}
			if (r_handle(temp, len_temp, 1) == true)
				continue;
		}
		else if (headword == _while)
		{
			is_hs_head = 0;
			kh[len_kh++] = _while;
			char temp[20];
			int len_temp = 0;
			c = fin.get();        // '('
			c = fin.get();
			while (c != ')')
			{
				temp[len_temp++] = c;
				c = fin.get();
			}
			if (r_handle(temp, len_temp, 2) == true)
				continue;
		}
		else if (headword == _return)   //在函数定义的时候，将return 的值写入 return_words
		{
			cout << "c0 =" << c<<endl;
			is_hs_head = 0;
			int count = 1;
			if (read_main == 0)         
			{
				//if (is_hs_dy == 1)
				//{
					char temp[30];
					int len_temp = 0;
					//c = fin.get();
					//cout << "c" << count++ << " =" << c << endl;

					while (c == ' ')
					{
						c = fin.get();
						//cout << "c" << count++ << " =" << c << endl;
					}
					if (c != ';')             //有返回值
					{
						while (c != ';')
						{
							temp[len_temp++] = c;
							c = fin.get();
							//cout << "c" << count++ << " =" << c << endl;
						}
							temp[len_temp] = '\0';
					}
					strcpy_s(return_words,25,temp);
					//cout <<"return_words ="<< return_words;
				//}
			}
			else
			{
				end_flag = 1;
			}
		}
		else if (headword == _ydkh)
		{
			is_hs_head = 0;
			//cout << kh[0];
			//cout << len_kh;
			len_kh--;
			if (kh[len_kh] == is_main)   //在get_kind中入栈
			{
				end_flag = 1;
			}
			else if (kh[len_kh] == _if)
			{
				fout1 << "L" << available_L++ << ":"<<endl;
			}
			else if (kh[len_kh] == _while)
			{
				fout1 << endl << "L" << available_L++ << endl;
				fout1 << "( j " << " , _" << " , _" <<  " , " << "L" << available_L - 4 << " )" << endl;
				fout1 << endl << "L" << available_L++ << endl;
			}
		
		}
		else        //出错
		{
			error_rep();
		}
	}
}
