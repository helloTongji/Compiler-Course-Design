//
//  syn.cpp
//  exc0compiler
//
//  Created by sciencefans on 14/11/18.
//  Copyright (c) 2014年 sciencefans. All rights reserved.
//



#include "glob.h"
#include "err.h"



int wd = -1;
symtable maintable;
int  value;     //保存常量的值；对于子程序，0表示void，1表示int
int  address;   //变量的地址；函数返回值的地址
int  paranum;   //全局变量，用来处理函数定义，因为函数的参数个数只能在参数表子程序执行结束之后才知道，所以只能在后面才插入符号表信息
int  kind;      //正在处理的标志符的类型
char name[MAXIDENLEN]; //标志符的名字，包括常量变量和函数
int hsymid; //上一个单词类型
char nowitem[200];
int returnnum = 0; //记录是否有返回值
char procname[100];
int hiscnt = 1;
int sislob = 1;
int factclass = 0;
#define INT 1
#define CHAR 2
#define subfuncindex(n) maintable.findextable[(n)]

//fill in symbol table
//name:     symbol name
//kind:     the kind of name(see syn.h)
//value:    the value of const
//address:  the address of the var
//para:     the formal parameter of function
int arrnum(char *name) {
	int n;
	int m = maintable.findextable[1];
	n = maintable.findextable[maintable.ftotal];
	//查找符号表中是否有
	while (n < maintable.index) {
		if (strcmp(maintable.element[n].name, name) == 0)
			return maintable.element[n].paranum;
		n++;
	}

	//如果分程序符号表中无定义，查找最外层符号表
	if (n == maintable.index) {
		n = 0;
		while (n < m) {
			if (strcmp(maintable.element[n].name, name) == 0)
				return maintable.element[n].paranum;
			n++;
		}
		//符号表中没有定义
		if (n == m) {
			printf("\"%s\" ", name);
			//error(VARNOTDEFINE);
			return -1;
		}
	}
}

void pushtable(char* name, int kind, int value, int address, int paranum, int isVec = 0) {

	//whether table is full
	if (maintable.index >= MAXSYMTABLENUM) {
		error(OUTOFTABLE);
		exit(0);
	}

	//whether conflict
	//function
	if (kind == FUNCTION) {
		int iter = 1;
		for (iter = 1; iter <= maintable.ftotal; iter++) {
			if (!strcmp(maintable.element[maintable.findextable[iter]].name, name)) {
				error(SYMBOLCONFLICT);
				return;
			}
		}
		maintable.findextable[++maintable.ftotal] = maintable.index;
	}
	else {
		//others
		int iter = maintable.findextable[maintable.ftotal];
		for (; iter < maintable.index; iter++) {
			if (!strcmp(maintable.element[iter].name, name)) {
				error(SYMBOLCONFLICT);
				return;
			}
		}
		//if not para and non-glob-var, compare with global var.
		if (kind != PARA && sislob) {
			iter = 0;
			while (iter < maintable.findextable[1]) {
				if (!strcmp(maintable.element[iter].name, name)) {
					error(SYMBOLCONFLICT);
					return;
				}
				iter++;
			}
		}
	}

	strcpy(maintable.element[maintable.index].name, name);
	maintable.element[maintable.index].kind = kind;
	maintable.element[maintable.index].value = value;
	maintable.element[maintable.index].address = maintable.index;
	maintable.element[maintable.index].paranum = isVec ? isVec : paranum;
	if (maintable.index <= hiscnt) {
		symtablehis << "----------------------------------------------------------------" << endl;
	}
	hiscnt = maintable.index;
	symtablehis << maintable.index << "\t" << name << "\t" << kind << "\t" << value << "\t" << address << "\t" << paranum << "\t" << isVec << endl;
	maintable.index++;
}

void insertproc(int paranum)
{
	int x = maintable.findextable[maintable.ftotal];   //array[i]，指向的是当前正在被分析的函数在符号表中的索引
	maintable.element[x].paranum = paranum;//插入函数参数个数信息
	return;
}

//look up symbol table
//flag=1时，说明要在分程序索引里查找，flag=0时，在当前所在分程序符号表中查找
//flag2=1时，只在当前函数作用域里找
int searchst(char* name, int flag, int flag2 = 0) {
	int n;
	int m = maintable.findextable[1];
	if (flag == 1) {
		n = 1;
		while (n <= maintable.ftotal) {
			if (strcmp(maintable.element[maintable.findextable[n]].name, name) == 0)
				break;
			n++;
		}

		//如果函数标志符没有定义
		if (n > maintable.ftotal) {
			printf("\"%s\" ", name);
			//error(FUNCTIONNOTFOUND);
			return -1 * FUNCTIONNOTFOUND;
		}

		//如果函数的实参个数与形参个数不匹配
		if (maintable.element[maintable.findextable[n]].paranum != paranum) {
			printf("\"%s\" ", name);
			//error(FORMALPARANUMUNMATCH);
			return -1 * FORMALPARANUMUNMATCH;
		}

		//如果函数标识符无内容
		if (maintable.element[maintable.findextable[n]].value == 0)
			return -1 * FUNCTIONRETURNNULL;

		return maintable.element[maintable.findextable[n]].address;
	}
	else {
		n = maintable.findextable[maintable.ftotal];
		//查找符号表中是否有
		while (n < maintable.index) {
			if (strcmp(maintable.element[n].name, name) == 0)
				break;
			n++;
		}

		//如果分程序符号表中无定义，查找最外层符号表
		if (n == maintable.index) {
			n = 0;
			while (n < m) {
				if (strcmp(maintable.element[n].name, name) == 0)
					break;
				n++;
			}
			//符号表中没有定义
			if (n == m) {
				printf("\"%s\" ", name);
				//error(VARNOTDEFINE);
				return -1;
			}
		}
		if (maintable.element[n].kind == INTTK || maintable.element[n].kind == CHARTK)
			return maintable.element[n].address;
		if (maintable.element[n].kind == CINTCON || maintable.element[n].kind == CCHARCON)
			return -2;    //const 返回负两倍地址
		if (maintable.element[n].kind == PARA)
			return maintable.element[n].address;    //参数标志
		return -1;
	}
}



//clear symbol table
void flushst() {
	int x = maintable.index - 1;
	int y;
	while ((maintable.element[x].kind == INTTK || maintable.element[x].kind == CHARTK || maintable.element[x].kind == CINTCON
		|| maintable.element[x].kind == CCHARCON || maintable.element[x].kind == PARA) && strcmp(maintable.element[x].name, procname) != 0) {
		maintable.element[x].kind = 0;
		maintable.element[x].address = 0;
		maintable.element[x].paranum = 0;
		maintable.element[x].value = 0;
		y = 0;
		while (y<30)
			maintable.element[x].name[y++] = '\0';
		x--;
	}
	maintable.index = x + 1;
	return;
}

//9.＜程序＞    ::= ［＜常量说明＞］［＜变量说明＞］{＜有返回值函数定义＞|＜无返回值函数定义＞}＜主函数＞
void program() {
	getsym();

	if (symid == CONSTTK) {
		//调用常量说明子程序
		conststate();
		//如果是分号,则是正常的
	}

	//变量定义和有返回值的函数是相同的声明头部。因此预读3个单词用于判断
	while (symid == INTTK || symid == CHARTK) {
		int tempid = symid;//恢复现场用
		char tempch = ch;//恢复现场用
		int tempcnum = cnum;//恢复现场用
		kind = symid;
		getsym();
		strcpy(name, id.c_str());//拷贝名字

								 //如果标识符合法
		if (symid != IDEN) {
			error(IDENTIFIERLACK);//notdone
			continue;
		}
		getsym();
		//如果是逗号或者分号，则是变量定义
		if (symid == COMMA || symid == LBRACK) {
			symid = tempid;
			cnum = tempcnum;
			ch = tempch;
			varstate();
			continue;
		}
		else if (symid == SEMICN) {
			value = 0;
			address = 0;
			paranum = 0;
			pushtable(name, kind, value, address, paranum);
			if (kind == INTTK) insmidcode("int", "  ", "  ", name);
			else if (kind == CHARTK) insmidcode("char", "  ", "  ", name);
			getsym();
			continue;
		}
		else {
			symid = tempid;
			cnum = tempcnum;
			ch = tempch;
			break;
		}
	}
	if (symid == CONSTTK) {
		errnum++;
		printf("Error %d: Line %d, 常量定义位置错误！\n", errnum, lnum);
		conststate();
		//如果是分号,则是正常的
	}
	sislob = 0;
	//进入函数部分
	while (symid == INTTK || symid == CHARTK || symid == VOIDTK) {
		int temp = symid;

		//19.＜有返回值函数定义＞  ::=  ＜声明头部＞‘(’＜参数＞‘)’  ‘{’＜复合语句＞‘}’
		if (symid == INTTK || symid == CHARTK) {
			defhead();
			if (symid != LPARENT) {
				error(LEFTPARENTLACK);
				continue;
			}
			varcnt = 0;     //临时变量重新开始计数
			returnnum = 0;//将是否存在返回值的标记初始化，0表示无返回值，即未出现return
			kind = FUNCTION;//表示当前的函数是子程序，即非主函数
			value = 1;//1对于函数来说，表示返回值为Int 0返回值为void
			address = 0;
			paranum = 0;
			pushtable(name, kind, value, address, paranum);
			strcpy(procname, name);//函数名拷贝
			if (temp == INTTK) insmidcode("func", "int", "  ", procname);
			if (temp == CHARTK) insmidcode("func", "char", "  ", procname);
			getsym();
			parametertable();
			//缺'('＜参数＞')'的右小括号
			if (symid != RPARENT) {
				error(RIGHTPARENTLACK);
				continue;
			}
			getsym();
			//缺'{'＜复合语句＞'}'的左大括号
			if (symid != LBRACE) {
				error(LEFTBRACELACK);
				continue;
			}
			getsym();
			complexsentence();
			//缺'{'＜复合语句＞'}'的右大括号
			if (symid != RBRACE) {
				error(RIGHTBRACELACK);
				continue;
			}

			//函数缺少返回值
			if (returnnum == 0) {
				error(NONERETURN);
				continue;
			}
			getsym();
			insmidcode("end", "  ", "  ", procname);//函数部分四元式结束
			flushst();//清除符号表，这里清空的是此子函数的符号表,不清除子函数名
		}

		if (symid == VOIDTK) {
			getsym();
			//如果下一个单词是main，则是＜主函数＞    ::= void main '(' ')'  '{'＜复合语句＞'}'
			if (symid == MAINTK) {
				varcnt = 0;        //临时变量重新开始计数
				strcpy(name, "main");
				kind = FUNCTION;
				value = 0;
				address = 0;
				paranum = 0;
				pushtable(name, kind, value, address, paranum);
				strcpy(procname, name);
				insmidcode("func", "  ", "  ", procname);
				getsym();
				if (symid != LPARENT) {
					error(LEFTPARENTLACK);
					continue;
				}
				getsym();
				if (symid != RPARENT) {
					error(RIGHTPARENTLACK);
					continue;
				}
				getsym();
				if (symid != LBRACE) {
					error(LEFTBRACELACK);
					continue;
				}
				getsym();
				complexsentence();
				mf++;    //主函数的个数+1
				insmidcode("end", "  ", "  ", procname);
				if (symid != RBRACE)
				{
					error(RIGHTBRACELACK);
					continue;
				}
				return;    //源程序结束，跳出
			}

			//20.＜无返回值函数定义＞  ::= void＜标识符＞‘(’＜参数＞‘)’  ‘{’＜复合语句＞‘}’
			if (symid != IDEN) {
				error(IDENTIFIERLACK);
				continue;
			}
			varcnt = 0;          //临时变量重新开始计数
			strcpy(name, id.c_str());

			kind = FUNCTION;
			value = 0;
			address = 0;
			paranum = 0;
			pushtable(name, kind, value, address, paranum);
			strcpy(procname, name);
			insmidcode("func", "void", "  ", procname);
			getsym();
			if (symid != LPARENT) {
				error(LEFTPARENTLACK);
				continue;
			}
			getsym();
			parametertable();
			if (symid != RPARENT) {
				error(RIGHTPARENTLACK);
				continue;
			}
			getsym();
			if (symid != LBRACE) {
				error(LEFTBRACELACK);
				continue;
			}
			getsym();
			complexsentence();
			if (symid != RBRACE) {
				error(RIGHTBRACELACK);
				continue;
			}
			getsym();
			insmidcode("end", "  ", "  ", procname);
			flushst();
		}
	}
}


//18.＜类型标识符＞   ::=    int | float | char
//22.＜参数＞         ::=    ＜参数表＞
//23.＜参数表＞    ::=  ＜类型标识符＞＜标识符＞{,＜类型标识符＞＜标识符＞}| ＜空＞
void parametertable()
{
	int i = 0;     //记录参数个数
	int temp;
	if (symid == INTTK || symid == CHARTK) {
		do {
			if (symid == COMMA)
				getsym();
			temp = symid;
			defhead();    //函数参数和变量定义时完全相同的，只是最后不是以分号结尾
			kind = PARA;     //4表示为函数参数
			value = 0;
			address = i;  //地址为i，即参数的位置，地址全部为相对地址？
			paranum = 0;
			pushtable(name, kind, value, address, paranum);  //将行数参数插入符号表
			if (temp == INTTK) insmidcode("para", "int", "  ", name);
			else if (temp == CHARTK) insmidcode("para", "char", "  ", name);
			i++;//参数个数加一
		} while (symid == COMMA);//如果是逗号，则还有其他的参数
	}
	paranum = i;//当前的参数个数
	insertproc(paranum);//插入函数的参数个数
	return;
}

//10.＜常量说明＞ ::=  const＜常量定义＞;{ const＜常量定义＞;}
//ATTENTION:在这里会直接读出类型
void conststate() {
	while (symid == CONSTTK) {
		getsym();
		int tclass;
		if (symid == INTTK || symid == CHARTK) {
			kind = (symid == INTTK) ? CINTCON : CCHARCON;
			address = 0;
			paranum = 0;
			tclass = symid;
			getsym();
			constdef(tclass);
			while (symid == COMMA) {
				getsym();
				constdef(tclass);
			}
			//定义结束，下一个字符一定是分号
			if (symid != SEMICN) {
				error(SEMICOLONLACK);
				return;
			}
			hsymid = symid;
			getsym();
		}
		else {
			error(KEYWORDERROR);
			return;
		}
	}
	return;
}

//11.＜常量定义＞   ::=   int＜标识符＞＝＜整数＞{,＜标识符＞＝＜整数＞}    | char＜标识符＞＝＜字符＞{,＜标识符＞＝＜字符＞}
//ATTENTION:这里不包含int和char, int, 这两者将从tclass传入
void constdef(int tclass) {
	//without int or char
	char temp[200];
	//结构：IDEN = XXXX
	if (symid == IDEN) {
		strcpy(name, id.c_str());
		getsym();
		//if is '='
		if (symid == ASSIGN) {
			getsym();
			// if is '+' or '-'
			if (symid == PLUS || symid == MINU) {
				int symb = symid;
				getsym();
				// if is integer
				if (tclass == INTTK && symid == INTCON) {
					if (symb == PLUS) {
						value = num;
					}
					else {
						value = 0 - num;
					}
					pushtable(name, kind, value, address, paranum);
					sprintf(temp, "%d", value);
					insmidcode("const", "int", temp, name);
				}

				// if is char
			}
			else if (symid == CHARCON) {
				value = id.c_str()[0];
				pushtable(name, kind, value, address, paranum);
				sprintf(temp, "%d", value);
				insmidcode("const", "char", temp, name);
			}
			else if (symid == INTCON) {
				value = num;
				pushtable(name, kind, value, address, paranum);
				sprintf(temp, "%d", value);
				insmidcode("const", "int", temp, name);
			}
			else {
				error(UNKNOWRIGHTSYM);
				return;
			}
		}
		else {
			error(CONSTNOTINIT);
			return;
		}
	}
	else {
		error(CSTDEFINEFAIL);
	}
	getsym();
	return;
}

//15.＜声明头部＞   ::=  int＜标识符＞ | char＜标识符＞
//ATTENTION: 读到的是int/char和标识符以及其后面的一个单词！！
void defhead()
{
	if (symid == INTTK || symid == CHARTK) {
		kind = symid;
		getsym();

		if (symid != IDEN) {
			error(IDENTIFIERLACK, 1);
			return;
		}
		else {
			strcpy(name, id.c_str());//拷贝函数的名字
		}
	}
	else {
		error(KEYWORDERROR, 1);
		return;
	}
	getsym();
	return;
}

//16.＜变量说明＞  ::= ＜变量定义＞;{＜变量定义＞;}
void varstate()
{
	vardef();
	//缺少';'
	if (symid != SEMICN) {
		error(SEMICOLONLACK, 1);
	}
	getsym();
	return;
}


//17.＜变量定义＞  ::= ＜类型标识符＞(＜标识符＞|＜标识符＞‘[’＜无符号整数＞‘]’){,(＜标识符＞|＜标识符＞‘[’＜无符号整数＞‘]’ )}
void vardef()
{
	if (symid == INTTK || symid == CHARTK) {
		kind = symid;
		getsym();

		if (symid != IDEN) {
			error(IDENTIFIERLACK, 2);
			return;
		}
		else {
			strcpy(name, id.c_str());//拷贝函数的名字
		}
	}
	else {
		error(KEYWORDERROR, 2);
		return;
	}
	getsym();
	int isVec = (symid == LBRACK);
	value = 0;
	address = 0;
	paranum = 0;
	if (isVec) {    //如果是数组
		getsym();
		if (symid != INTCON) {
			error(IMMLACK);
			return;
		}
		else {
			pushtable(name, kind, value, address, paranum, num);
			if (kind == INTTK)
				insmidcode("inta", 0, num, name);
			else if (kind == CHARTK)
				insmidcode("chara", 0, num, name);
			getsym();
			if (symid != RBRACK) {
				error(RIGHTBRACKLACK, 1);
				return;
			}
			else {
				getsym();
			}
		}
	}
	else {
		pushtable(name, kind, value, address, paranum, isVec);
		if (kind == INTTK)
			insmidcode("int", "  ", "  ", name);
		else if (kind == CHARTK)
			insmidcode("char", "  ", "  ", name);
	}

	//如果后面是逗号，那么变量定义未结束
	while (symid == COMMA) {
		getsym();
		//出现不合法的标志符
		if (symid != IDEN) {
			error(IDENTIFIERLACK, 2);
			return;
		}
		strcpy(name, id.c_str());
		getsym();
		isVec = (symid == LBRACK);
		if (isVec) {    //如果是数组
			getsym();
			if (symid != INTCON) {
				error(IMMLACK);
				return;
			}
			else {
				pushtable(name, kind, value, address, paranum, num);
				if (kind == INTTK)
					insmidcode("int", 0, num, name);
				else if (kind == CHARTK)
					insmidcode("char", 0, num, name);
				getsym();
				if (symid != RBRACK) {
					error(RIGHTBRACKLACK);
					return;
				}
				else {
					getsym();
				}
			}
		}
		else {
			pushtable(name, kind, value, address, paranum, isVec);
			if (kind == INTTK)
				insmidcode("int", "  ", "  ", name);
			else if (kind == CHARTK)
				insmidcode("char", "  ", "  ", name);
		}
	}
	return;
}

//21.＜复合语句＞   ::=  ［＜常量说明＞］［＜变量说明＞］＜语句列＞
void complexsentence()
{
	if (symid == CONSTTK) {
		//调用常量说明子程序
		conststate();
		//如果是分号,则是正常的
		while (hsymid == SEMICN) {
			if (symid == CONSTTK)
				conststate();
			else break;
		}
	}

	//判断是否为变量说明
	//设置部分变量用于恢复现场
	while (symid == INTTK || symid == CHARTK) {
		int tempid = symid;//恢复现场用
		char tempch = ch;//恢复现场用
		int tempcnum = cnum;//恢复现场用
		kind = symid;
		getsym();
		strcpy(name, id.c_str());//拷贝名字

								 //如果标识符合法
		if (symid != IDEN) {
			error(IDENTIFIERLACK, 2);
			return;
		}
		getsym();
		//如果是逗号或者分号，则是变量定义
		if (symid == COMMA || symid == LBRACK) {
			symid = tempid;
			cnum = tempcnum;
			ch = tempch;
			varstate();
			continue;
		}
		else if (symid == SEMICN) {
			value = 0;
			address = 0;
			paranum = 0;
			pushtable(name, kind, value, address, paranum);
			if (kind == INTTK) insmidcode("int", "  ", "  ", name);
			else if (kind == CHARTK) insmidcode("char", "  ", "  ", name);
			getsym();
			continue;
		}
		else {
			error(SEMICOLONLACK, 1);
			return;
		}
	}

	//语句列
	sentencelist();
	return;
}

//37.＜语句列＞   ::= ｛＜语句＞｝
void sentencelist()
{
	sentence();
	//语句，如果读到的是如下的东西，那么还是语句
	while (symid == IFTK || symid == WHILETK || symid == FORTK || symid == LBRACE
		|| symid == IDEN || symid == RETURNTK || symid == SCANFTK || symid == PRINTFTK) {
		sentence();
	}
}

//30.＜条件语句＞  ::=  if ‘(’＜条件＞‘)’＜语句＞［else＜语句＞］
void ifsentence()
{
	char label1[10], label2[10], conditionvalue[30];
	strcpy(label1, nextlab());  //如果不满足if，则跳过label1，label1是if的结束部分
	strcpy(label2, nextlab());  //如果还有else，则else的结束位置是label2
	getsym();
	//少“(”
	if (symid != LPARENT) {
		error(LEFTPARENTLACK, 1);
		return;
	}
	getsym();
	//＜条件＞
	condition();
	strcpy(conditionvalue, nowitem);  //条件计算的值在nowitem里面,此处应该是个临时变量$_x
	insmidcode("jne", "  ", "  ", label1);  //比较，为假的时候跳转
	if (symid != RPARENT) {
		error(RIGHTPARENTLACK, 1);
		return;
	}
	getsym();
	sentence();
	insmidcode("jmp", "  ", "  ", label2);//不执行else的部分
	insmidcode("lab:", "  ", "  ", label1);
	if (symid == ELSETK) {
		getsym();
		sentence();
	}
	insmidcode("lab:", "  ", "  ", label2);
	return;
}

//31.＜条件＞    ::=  ＜表达式＞＜关系运算符＞＜表达式＞｜＜表达式＞
void condition()
{
	char place1[30], place2[30];
	expression();
	strcpy(place1, nowitem);  //条件至少有一个表达式
							  //关系运算符
	if (symid == LSS || symid == LEQ || symid == GRE || symid == GEQ || symid == NEQ || symid == EQL) {
		if (symid == LSS) {
			getsym();
			expression();
			strcpy(place2, nowitem);
			insmidcode("<", place1, place2, "  ");
		}
		else if (symid == LEQ) {
			getsym();
			expression();
			strcpy(place2, nowitem);
			insmidcode("<=", place1, place2, "  ");
		}
		else if (symid == GRE) {
			getsym();
			expression();
			strcpy(place2, nowitem);
			insmidcode(">", place1, place2, "  ");
		}
		else if (symid == GEQ) {
			getsym();
			expression();
			strcpy(place2, nowitem);
			insmidcode(">=", place1, place2, "  ");
		}
		else if (symid == NEQ) {
			getsym();
			expression();
			strcpy(place2, nowitem);
			insmidcode("!=", place1, place2, "  ");
		}
		else if (symid == EQL) {
			getsym();
			expression();
			strcpy(place2, nowitem);
			insmidcode("==", place1, place2, "  ");
		}
		return;
	}
	strcpy(place2, "0");
	insmidcode("!=", place1, place2, "  ");
	return;
}

//32.＜循环语句＞   ::=   while '('＜条件＞')'＜语句＞
//                   | for'('＜标识符＞＝＜表达式＞;＜条件＞;＜标识符＞＝＜标识符＞(+|-)＜步长＞')'＜语句＞
void loopsentence()
{
	int s = 0;
	char names[30], names1[30], names2[30], place2[30], place3[30];
	char label1[10], label2[10], op_flag[5];
	strcpy(label1, nextlab());
	strcpy(label2, nextlab());
	if (symid == WHILETK) {
		getsym();
		if (symid != LPARENT) {
			error(LEFTPARENTLACK, 1);
			return;
		}
		insmidcode("lab:", "  ", "  ", label1);
		getsym();
		condition();
		if (symid != RPARENT) {
			error(RIGHTPARENTLACK, 1);
			return;
		}
		insmidcode("jne", "  ", "  ", label2);
		getsym();
		sentence();
		insmidcode("jmp", "  ", "  ", label1);
		insmidcode("lab:", "  ", "  ", label2);
		return;
	}

	if (symid == FORTK) {    //for|(...)
		getsym();
		if (symid != LPARENT) {//for(|...)
			error(LEFTPARENTLACK, 1);
			return;
		}
		getsym();

		if (symid != IDEN) {    //for(i|=x;...;...)
			error(UNACCEPTABLESENTENCE);//不合法的句子
			return;
		}
		strcpy(names, id.c_str());
		getsym();
		if (symid != ASSIGN) {    //for(i=|..;...;...)
			error(UNACCEPTABLESENTENCE);
			return;
		}
		s = searchst(names, 0);
		if (s < 0) {     //如果是常量
			if (s == -1) error(VARNOTDEFINE, 1);  //"="左边是不合法的标识符
			else    error(ASSIGNCONST);
			return;
		}
		getsym();
		expression();    //for(i=a+b|;...;...)
		insmidcode("=", nowitem, "  ", names);
		if (symid != SEMICN) {
			error(SEMICOLONLACK, 2);
			return;
		}
		getsym();//for(i=a+b;|...;...)
		insmidcode("lab:", "  ", "  ", label1);
		condition();//for(i=a+b;...;|...)
		insmidcode("jne", "  ", "  ", label2);
		if (symid != SEMICN) {
			error(SEMICOLONLACK, 2);
			return;
		}
		getsym();        //for(i=a+b;...;|...)
						 //＜标识符＞＝＜标识符＞(+|-)＜步长＞!!!!!!!!!!!
		if (symid != IDEN) {
			error(IDENTIFIERLACK, 3);
			return;
		}
		strcpy(names1, id.c_str());
		s = searchst(names1, 0);
		if (s < 0) {     //如果是常量
			if (s == -1) error(VARNOTDEFINE, 1);  //"="左边是不合法的标识符
			else    error(ASSIGNCONST);
			return;
		}
		getsym();
		if (symid != ASSIGN) {
			error(UNACCEPTABLESENTENCE);
			return;
		}
		getsym();
		if (symid != IDEN) {
			error(IDENTIFIERLACK, 3);
			return;
		}
		strcpy(names2, id.c_str());
		s = searchst(names2, 0);
		if (s < 0) {     //如果是常量
			if (s == -1) error(VARNOTDEFINE, 1);  //"="左边是不合法的标识符
			else    error(ASSIGNCONST);
			return;
		}

		getsym();
		if (symid != PLUS && symid != MINU) {
			error(PLUSMINULACK);
			return;
		}
		if (symid == PLUS) strcpy(op_flag, "+ ");
		else if (symid == MINU) strcpy(op_flag, "- ");

		getsym();
		if (symid != INTCON || num == 0) {
			error(UNKNOWRIGHTSYM, 1);     //等号右边不是合法的整数
			return;
		}
		strcpy(place2, id.c_str());

		getsym();
		if (symid != RPARENT) {
			error(RIGHTPARENTLACK, 1);
			return;
		}

		getsym();
		sentence();
		strcpy(place3, nextvar());
		insmidcode(op_flag, names2, place2, place3);
		insmidcode("=", place3, "  ", names1);

		insmidcode("jmp", "  ", "  ", label1);
		insmidcode("lab:", "  ", "  ", label2);
		return;
	}
}

//28.＜语句＞    ::= ＜条件语句＞｜＜循环语句＞｜‘{’＜语句列＞‘}’｜＜有返回值函数调用语句＞;
//|＜无返回值函数调用语句＞;｜＜赋值语句＞;｜＜读语句＞;｜＜写语句＞;｜＜空＞;｜＜返回语句＞;
void sentence() {
	int s;

	//条件语句
	if (symid == IFTK) {
		ifsentence();
		return;
	}

	//循环语句
	if (symid == WHILETK || symid == FORTK) {
		loopsentence();
		return;
	}

	//{‘语句列’}
	if (symid == LBRACE) {
		getsym();
		sentencelist();
		//缺“}”
		if (symid != RBRACE) {
			error(RIGHTBRACELACK, 1);
			return;
		}
		//不缺“}”
		getsym();
		return;
	}

	//函数调用语句｜＜赋值语句＞;

	if (symid == IDEN) {
		int isVec = 0;
		char names[MAXIDENLEN];
		strcpy(names, id.c_str());
		getsym();
		//＜赋值语句＞;
		//29.＜赋值语句＞   ::=  ＜标识符＞＝＜表达式＞|＜标识符＞'['<表达式>']'＝＜表达式＞
		char place2[100];
		if (symid == LBRACK) { //如果是数组
			s = searchst(names, 0);
			if (s == -1)
			{
				error(VARNOTDEFINE, 1);
				return;
			}
			getsym();
			expression();
			strcpy(place2, nowitem);
			if (isdigit(nowitem[0])) {
				int arrnums = 0;
				arrnums = arrnum(names);
				if (atoi(nowitem) >= arrnums) {
					printf("Warning: Line:%d index of array out of boundary, which should be less than %d.\n", lnum, atoi(nowitem));
				}
			}
			isVec = 1;
			if (symid != RBRACK) {
				error(RIGHTBRACKLACK, 1);
				return;
			}
			getsym();
		}
		if (symid == ASSIGN) {
			s = searchst(names, 0);
			if (s < -1) {     //如果是常量
				error(ASSIGNCONST);            //"="左边是不合法的标识符
				return;
			}
			getsym();
			expression();
			if (isVec) {
				insmidcode("[]=", names, place2, nowitem);
			}
			else {
				insmidcode("=", nowitem, "  ", names);
			}

			if (symid != SEMICN) {
				error(SEMICOLONLACK, 2);
				return;
			}
			getsym();
			return;
		}

		//函数调用语句
		if (symid == LPARENT) {
			getsym();
			valueofpara();  //计算并记录参数的值以及个数
			if (symid != RPARENT) {
				error(RIGHTPARENTLACK, 1);
				return;
			}
			s = searchst(names, 1);
			if (s == -1 * FUNCTIONNOTFOUND) {
				error(FUNCTIONNOTFOUND);
				return;
			}
			else if (s == -1 * FORMALPARANUMUNMATCH) {
				error(FORMALPARANUMUNMATCH, 1);
				return;
			}
			insmidcode("call", names, "  ", "  ");
			getsym();
			if (symid != SEMICN) {
				error(SEMICOLONLACK, 2);
				return;
			}
			getsym();
		}

		else {
			error(UNACCEPTABLESENTENCE);    //不合法的句子
			return;
		}
		return;
	}

	//读语句
	if (symid == SCANFTK) {
		scanfsentence();
		if (symid != SEMICN) {
			error(SEMICOLONLACK, 2);
			return;
		}
		getsym();
		return;
	}

	//写语句
	if (symid == PRINTFTK) {
		printfsentence();
		if (symid != SEMICN) {
			error(SEMICOLONLACK, 2);
			return;
		}
		getsym();
		return;
	}

	//返回语句
	if (symid == RETURNTK) {
		returnsentence();
		//返回语句后缺“；”
		if (symid != SEMICN) {
			error(SEMICOLONLACK, 2);
			return;
		}
		getsym();
		return;
	}
	if (symid == SEMICN)
	{
		getsym();
		return;
	}

	//不合法的句子
	error(UNACCEPTABLESENTENCE);
	return;
}

//36.＜值参数表＞   ::=   ＜表达式＞{,＜表达式＞}｜＜空＞
void valueofpara()
{
	int j = 0;
	vector<string> paraqueue;
	do {
		if (symid == COMMA)
			getsym();
		if (symid == PLUS || symid == MINU || symid == IDEN || symid == LPARENT || symid == CHARCON || symid == INTCON) {
			expression();
			paraqueue.push_back(nowitem);
			j++;
		}
	} while (symid == COMMA);
	char tc[20];
	for (int i = 0; i < paraqueue.size(); i++) {
		strcpy(tc, paraqueue[i].c_str());
		insmidcode("fupa", "  ", "  ", tc);
	}
	paranum = j;
	paraqueue.~vector();
	return;
}

//38.＜读语句＞    ::=  scanf ‘(’＜标识符＞{,＜标识符＞}‘)’
void scanfsentence()
{
	char names[30];
	int s;
	getsym();
	if (symid != LPARENT) {
		error(LEFTPARENTLACK, 1);
		return;
	}

	do {
		getsym();
		if (symid != IDEN) {
			error(IDENTIFIERLACK, 3);         //不合法的标志符
			return;
		}
		strcpy(names, id.c_str());
		s = searchst(names, 0);
		if (s < 0) {
			if (s == -1)
				error(VARNOTDEFINE, 1);            //未定义标志符
			else
				error(ASSIGNCONST, 1);
			return;
		}
		insmidcode("scf", "  ", "  ", names);
		getsym();
	} while (symid == COMMA);

	if (symid != RPARENT) {
		error(RIGHTPARENTLACK, 1);
		return;
	}
	getsym();
	return;
}

//39.＜写语句＞    ::=   printf ‘(’＜字符串＞,＜表达式＞‘)’|
//                     printf ‘(’＜字符串＞‘)’| printf ‘(’＜表达式＞‘)’
void printfsentence()
{
	char place1[200] = "", place2[30] = ""; //place1是字符串，place2是表达式
	getsym();
	if (symid != LPARENT) {
		error(LEFTPARENTLACK, 1);
		return;
	}
	getsym();
	if (symid == STRCON) {
		strcpy(place1, id.c_str());
		getsym();
		if (symid == COMMA) {
			getsym();
			expression();
			strcpy(place2, nowitem);
		}
	}
	else {
		expression();
		strcpy(place2, nowitem);
	}
	if (symid != RPARENT) {
		error(RIGHTPARENTLACK, 1);
		return;
	}
	insmidcode("prt", place1, place2, (factclass == CHAR) ? "char" : "int");
	getsym();
	return;
}

//40.＜返回语句＞   ::=  return[‘(’＜表达式＞‘)’]
void returnsentence()
{
	char place3[30];
	getsym();
	if (symid == LPARENT) {
		getsym();
		expression();//表达式子程序
		strcpy(place3, nowitem);
		if (symid != RPARENT) {
			error(RIGHTPARENTLACK, 1);
			return;
		}
		insmidcode("ret", "  ", "  ", place3);
		returnnum++;
		getsym();
		return;
	}
	insmidcode("ret", "  ", "  ", "  ");
	return;
}

//25.＜表达式＞    ::= ［＋｜－］＜项＞{＜加法运算符＞＜项＞}
void expression()
{
	factclass = 0;
	char place1[30], place2[30], place3[30];
	//有＋｜－的情况
	if (symid == PLUS || symid == MINU) {
		factclass = INT;
		if (symid == PLUS) {
			getsym();
			item();//项，结束后，项的结果会放入全局变量nowitem
			strcpy(place3, nowitem);//把项的结果放入临时变量的位置
		}
		if (symid == MINU) {
			getsym();
			item();//项
			strcpy(place1, nowitem);
			strcpy(place3, nextvar());
			insmidcode("-", "0 ", place1, place3); //place3 = 0 - (place1)
		}
	}
	else {
		item();
		strcpy(place3, nowitem);
	}
	while (symid == PLUS || symid == MINU) {
		factclass = INT;
		strcpy(place1, place3);
		if (symid == PLUS) {
			getsym();
			item();
			strcpy(place2, nowitem);
			strcpy(place3, nextvar());
			insmidcode("+", place1, place2, place3);
			continue;
		}
		else {
			getsym();
			item();
			strcpy(place2, nowitem);
			strcpy(place3, nextvar());
			insmidcode("-", place1, place2, place3);
			continue;
		}
	}
	strcpy(nowitem, place3);//把表达式的最终值存放在p之中
	return;
}

//26.＜项＞     ::= ＜因子＞{＜乘法运算符＞＜因子＞}
void item()
{
	char place1[200], place2[200], place3[200];
	factor();
	strcpy(place3, nowitem);//这种操作是为了对付只有赋值的情况
	while (symid == MULT || symid == DIV) {
		factclass = INT;
		strcpy(place1, place3);
		if (symid == MULT) {
			getsym();
			factor();
			strcpy(place2, nowitem);
			strcpy(place3, nextvar());//申请临时变量
			insmidcode("*", place1, place2, place3);
			continue;
		}
		if (symid == DIV) {
			getsym();
			factor();
			strcpy(place2, nowitem);
			strcpy(place3, nextvar());
			insmidcode("/", place1, place2, place3);
			continue;
		}
	}
	strcpy(nowitem, place3);  //每一个项，计算后的值都在变量nowitem里面
	return;
}


//27.＜因子＞    ::= ＜标识符＞｜＜标识符＞‘[’＜表达式＞‘]’｜＜整数＞|＜字符＞｜＜有返回值函数调用语句＞|‘(’＜表达式＞‘)’
void factor()
{
	int t = -1;
	char names[30], place3[30];
	//＜标识符＞｜＜有返回值函数调用语句＞
	if (symid == IDEN) {
		strcpy(names, id.c_str());
		getsym();
		//如果有左括号，则是＜有返回值函数调用语句＞
		if (symid == LPARENT) {
			getsym();
			valueofpara();
			if (symid != RPARENT) {
				error(LEFTPARENTLACK, 2);
				return;
			}
			t = searchst(names, 1);
			if (t < 0) {
				if (t == -1 * FUNCTIONRETURNNULL)
					error(FUNCTIONRETURNNULL);   //函数无返回值，不能出现在表达式中
				else if (t == -1 * FUNCTIONNOTFOUND)
					error(FUNCTIONNOTFOUND, 1);
				return;
			}
			strcpy(place3, nextvar());//生成临时变量
			insmidcode("call", names, "  ", place3);//将调用的返回值存放在临时变量里面
			strcpy(nowitem, place3);
			getsym();
			return;
		}

		//如果是变量、数组、
		else if (symid != LBRACK) {
			t = searchst(names, 0);      //查表，查找到标识符对应的地址
			if (t == -1) {
				error(IDENTIFIERLACK, 4);
				return;
			}
			if (t < -1) {
				t = -t / 2;
			}
			strcpy(nowitem, names);
		}
		else if (symid == LBRACK) {      //如果是数组
			getsym();
			int tclass = factclass;
			expression();
			factclass = tclass;
			char ttt[30];
			strcpy(ttt, nowitem);

			if (isdigit(nowitem[0])) {
				int arrnums = 0;
				arrnums = arrnum(names);
				if (atoi(ttt) >= arrnums) {
					printf("Warning: Line:%d index of array out of boundary, which should be less than %d.\n", lnum, atoi(ttt));
				}
			}
			if (symid != RBRACK) {
				error(RIGHTBRACKLACK);
				return;
			}
			else {
				strcpy(nowitem, nextvar());
				insmidcode("geta", names, ttt, nowitem);
				getsym();
			}
		}
		return;
	}

	//'('＜表达式＞')'
	if (symid == LPARENT) {
		getsym();
		expression();
		if (symid != RPARENT) {
			error(RIGHTPARENTLACK, 2);
			return;
		}
		getsym();
		return;
	}

	// ＜整数＞｜＜字符＞
	if (symid == PLUS || symid == MINU || symid == INTCON || symid == CHARCON) {
		int temp = 1;
		if (symid == PLUS)
			getsym();
		else if (symid == MINU) {
			temp = -1;
			getsym();
		}
		if (symid != INTCON) {
			if (symid == CHARCON) {
				if (factclass != INT) {
					factclass = CHAR;
				}
				sprintf(nowitem, "%d", id.c_str()[0]);
				getsym();
				return;
			}
			error(IMMLACK);
			return;
		}
		sprintf(nowitem, "%d", temp * num);
		factclass = INT;
		getsym();
		return;
	}

	error(EXPRESSIONERROR);                  //表达式缺失或错误
	return;
}