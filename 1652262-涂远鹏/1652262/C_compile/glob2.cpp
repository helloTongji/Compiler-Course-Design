#include "glob.h"
#define rsT asmrst
#define VOID 0
#define INT 1
#define CHAR 2
#define WINT 3
#define WCHAR 4
#define INITSTACK 0x7fffeffc
#define INITDATA 0x10010000
#define OPTFLAG 0    //优化开关

int mi = 0;    //四元式处理的行数
int sp = INITSTACK;    //栈指针,此地址为即将分配的！！相对fp！！地址
int fp = 0;//帧指针
int ap;//地址表计数器
int paran = 0;
int constedge = 0;
int tmi;
int ismain = 0;
int tlabelnum = 0;
int isglob;
int funckin;
int funcnum = 0;

typedef struct {
	char name[100];
	int kind;
}funcclass;
vector<funcclass> fc;//函数表

typedef struct {
	char name[100];
	int address;
	int kind;
	int cnt;
}tempvaraddress;//变量表
tempvaraddress addrtable[1000];//临时变量在栈中的地址表

typedef struct {
	int symbnum;
	int cnt;
}cntstruct;
cntstruct cnttable[200];//计数器
int cntindex = 0;

int varreg[200];

bool cmpcnt(cntstruct a, cntstruct b) {
	return a.cnt > b.cnt;
}

int funckind(char *fname) {
	for (int i = 0; i < fc.size(); i++) {
		if (strcmp(fc[i].name, fname) == 0) {
			return fc[i].kind;
		}
	}
}

int findvartable(char *name) {
	int t = ap - 1;
	if (name[0] == '+' || name[0] == '-' || name[0] >= '0'&&name[0] <= '9')
		return -1;
	while (t >= 0) {
		if (strcmp(addrtable[t].name, name) == 0)
			return t;
		t--;
	}
	return -1;
}

int varkind(char *name) {
	int t = ap - 1;
	if (name[0] == '+' || name[0] == '-' || name[0] >= '0'&&name[0] <= '9')
		return -1;
	while (t >= 0) {
		if (strcmp(addrtable[t].name, name) == 0)
			return addrtable[t].kind;
		t--;
	}
	return -1;
}

void midcode2asm() {
	memset(varreg, 0xff, sizeof(int) * 200);
	rsT << "\t.text" << endl;
	rsT << "\t\tori\t$fp\t$sp\t0" << endl;
	rsT << "\t\tli\t$t9\t0x7fffeffc\t#global stack bottom" << endl;
	rsT << "\t\tli\t$t8\t0x10010000\t#save word" << endl;
	sp = 0;
	mi = ap = 0;

	while (mi < midcodeiter) {
		//全局常量定义
		while (strcmp(midcode[mi].op, "const") == 0) {
			pushstack(midcode[mi].var2);
			if (strcmp(midcode[mi].var1, "int") == 0) {
				insertaddress(WINT);
			}
			else {
				insertaddress(WCHAR);
			}
			mi++;
		}
		//全局变量定义
		while (strcmp(midcode[mi].op, "int") == 0 || strcmp(midcode[mi].op, "char") == 0) {
			pushstack("0");
			if (strcmp(midcode[mi].op, "int") == 0) {
				insertaddress(WINT);
			}
			else {
				insertaddress(WCHAR);
			}
			mi++;
		}
		//全局数组定义
		while (strcmp(midcode[mi].op, "inta") == 0 || strcmp(midcode[mi].op, "chara") == 0) {
			pushstack("0", atoi(midcode[mi].var2));
			if (strcmp(midcode[mi].op, "inta") == 0) {
				insertaddress(WINT);
			}
			else {
				insertaddress(WCHAR);
			}
			mi++;
		}
		rsT << "\t\tj\t__main" << endl;
		constedge = ap;
		//函数定义
		funcclass tfc;
		while (strcmp(midcode[mi].op, "func") == 0) {
			funcnum++;
			if (strcmp(midcode[mi].var1, "char") == 0) {
				tfc.kind = CHAR;
				strcpy(tfc.name, midcode[mi].var3);
				fc.push_back(tfc);
			}
			else if (strcmp(midcode[mi].var1, "int") == 0) {
				tfc.kind = INT;
				strcpy(tfc.name, midcode[mi].var3);
				fc.push_back(tfc);
			}
			else {
				tfc.kind = VOID;
				strcpy(tfc.name, midcode[mi].var3);
				fc.push_back(tfc);
			}
			if (strcmp(midcode[mi].var3, "main") == 0) {
				ismain = 1;
				rsT << "__main:" << endl;
			}
			else {
				ismain = 0;
				rsT << midcode[mi].var3 << ":" << endl;
			}
			mi++;
			funcasm();
		}
	}
}

//给地址表插入相对地址，sp不变
void insertaddress(int kind, int addr, int nmi) {
	if (nmi == -1) {
		strcpy(addrtable[ap].name, midcode[mi].var3);
	}
	else {
		strcpy(addrtable[ap].name, midcode[nmi].var3);
	}
	if (addr == -1) {
		addrtable[ap].address = sp + 4;
	}
	else {
		addrtable[ap].address = addr;
	}
	addrtable[ap].kind = kind;
	addrtable[ap].cnt = 0;
	ap++;
}

void pushstack(char* item, int lenth) {
	if (lenth == 1) {
		rsT << "\t\tli\t$t0\t" << item << "\t#" << midcode[tmi].var3 << endl; //li    $t0 item
		rsT << "\t\tsw\t$t0\t($sp)" << endl;      //sw    $t0 $sp
	}
	sp -= (4 * lenth);
	rsT << "\t\tsubi\t$sp\t$sp\t" << 4 * lenth << endl; //subi  $sp $sp 4  
	return;
}

//处理函数内容，不处理最后的end和开始的func
void funcasm() {
	memset(varreg, 0xff, sizeof(int) * 200);
	sp = 0;//相对偏移为0
		   //保存现场
	rsT << "\t\t#Save Register" << endl;//
	savesreg();
	rsT << "\t\tsw\t$fp\t($sp)" << endl;//保存上一个函数的$fp
	rsT << "\t\tadd\t$fp\t$sp\t$0" << endl;//设置本函数$fp：$fp=$sp
	sp -= 4;
	rsT << "\t\tsubi\t$sp\t$sp\t4" << endl;//$sp-=4
	rsT << "\t\tsw\t$ra\t($sp)" << endl;//保存$ra
	sp -= 4;
	rsT << "\t\tsubi\t$sp\t$sp\t4" << endl;//$sp-=4
	rsT << "\t\t#Save Register Done!" << endl;//

											  //while ( strcmp(midcode[ mi ].op, "int") == 0 || strcmp(midcode[ mi ].op, "char") == 0
											  //    || strcmp(midcode[ mi ].op, "inta") == 0 || strcmp(midcode[ mi ].op, "chara") == 0 ) {
											  //    ////变量定义
											  //    //while ( strcmp(midcode[ mi ].op, "int") == 0 || strcmp(midcode[ mi ].op, "char") == 0 ) {
											  //    //    pushstack("0");
											  //    //    if ( strcmp(midcode[ mi ].var1, "int") == 0 ) {
											  //    //        insertaddress(INT);
											  //    //    } else {
											  //    //        insertaddress(CHAR);
											  //    //    }
											  //    //    mi++;
											  //    //}
											  //    ////数组定义
											  //    //while ( strcmp(midcode[ mi ].op, "inta") == 0 || strcmp(midcode[ mi ].op, "chara") == 0 ) {
											  //    //    pushstack("0", atoi(midcode[ mi ].var2));
											  //    //    if ( strcmp(midcode[ mi ].var1, "inta") == 0 ) {
											  //    //        insertaddress(INT);
											  //    //    } else {
											  //    //        insertaddress(CHAR);
											  //    //    }
											  //    //    mi++;
											  //    //}
											  //}
											  //临时变量定义
	tmi = mi;
	while (strcmp(midcode[tmi].op, "end") != 0) {
		char v1[100], v2[100], v3[100];
		strcpy(v1, midcode[tmi].var1);
		strcpy(v2, midcode[tmi].var2);
		strcpy(v3, midcode[tmi].var3);


		if (v1[0] == '$' && varaddr(v1) == -1) {
			pushstack("0");
			insertaddress(INT, -1, tmi);
		}
		if (v2[0] == '$' && varaddr(v2) == -1) {
			pushstack("0");
			insertaddress(INT, -1, tmi);
		}
		if (v3[0] == '$' && varaddr(v3) == -1) {
			pushstack("0");
			if (strcmp(midcode[tmi].op, "call") == 0) {
				insertaddress(funckind(midcode[tmi].var1), -1, tmi);
			}
			else if (strcmp(midcode[tmi].op, "geta") == 0) {
				insertaddress(varkind(v1), -1, tmi);
			}
			else {
				insertaddress(INT, -1, tmi);
			}
		}
		tmi++;
	}

	while (strcmp(midcode[mi].op, "end") != 0) {
		paran = 0;
		for (int i = 0; i < strlen(midcode[mi].op); i++) {
			if (midcode[mi].op[i] == ' ') {
				midcode[mi].op[i] = '\0';
				break;
			}
		}
		if (strcmp(midcode[mi].op, "+") == 0) addasm();
		if (strcmp(midcode[mi].op, "-") == 0) subasm();
		if (strcmp(midcode[mi].op, "*") == 0) mulasm();
		if (strcmp(midcode[mi].op, "/") == 0) divasm();
		if (strcmp(midcode[mi].op, ">") == 0) greasm();
		if (strcmp(midcode[mi].op, ">=") == 0) geqasm();
		if (strcmp(midcode[mi].op, "<") == 0) lssasm();
		if (strcmp(midcode[mi].op, "<=") == 0) leqasm();
		if (strcmp(midcode[mi].op, "!=") == 0) neqasm();
		if (strcmp(midcode[mi].op, "==") == 0) eqlasm();
		if (strcmp(midcode[mi].op, "=") == 0) assasm();
		if (strcmp(midcode[mi].op, "[]=") == 0) aassasm();
		if (strcmp(midcode[mi].op, "geta") == 0) assaasm();
		if (strcmp(midcode[mi].op, "lab:") == 0) setlabasm();
		if (strcmp(midcode[mi].op, "scf") == 0) {
			scfasm();
		}
		if (strcmp(midcode[mi].op, "prt") == 0) {
			prtasm();
		}
		if (strcmp(midcode[mi].op, "jne") == 0) jneasm();
		if (strcmp(midcode[mi].op, "jmp") == 0) jmpasm();
		if (strcmp(midcode[mi].op, "fupa") == 0) fupaasm();
		if (strcmp(midcode[mi].op, "call") == 0) callasm();
		if (strcmp(midcode[mi].op, "ret") == 0) retasm();
		if (strcmp(midcode[mi].op, "para") == 0) paraasm();
		if (OPTFLAG) {
			int flag = 0;
			while (strcmp(midcode[mi].op, "int") == 0
				|| strcmp(midcode[mi].op, "char") == 0) {
				flag = 1;
				intcharasm();
				mi++;
			}
			if (flag)    mi--;
			//引用计数优化
			cntopt();
		}
		else {
			if (strcmp(midcode[mi].op, "int") == 0
				|| strcmp(midcode[mi].op, "char") == 0) intcharasm();
		}

		if (strcmp(midcode[mi].op, "const") == 0) constdefasm();
		if (strcmp(midcode[mi].op, "inta") == 0 || strcmp(midcode[mi].op, "chara") == 0) intcharaasm();
		mi++;
	}
	ap = constedge;
	//恢复现场
	rsT << "__FEND_LAB_" << funcnum << ":" << endl;//结束开始
	rsT << "\t\tlw\t$ra\t-4($fp)" << endl;//恢复$ra
	rsT << "\t\tadd\t$sp\t$fp\t$0" << endl;//退栈,恢复$sp
	rsT << "\t\tlw\t$fp\t($fp)" << endl;//恢复上一个函数的fp
	loadsreg();//恢复$s0-$s7
	if (ismain) {
		rsT << "\t\tli\t$v0\t10" << endl;
		rsT << "\t\tsyscall" << endl;//终止程序
	}
	else {
		rsT << "\t\tjr\t$ra\t" << endl;//返回
	}
	mi = mi + 1;
	return;
}

//查找汇编变量地址
int varaddr(char *name) {
	int t = ap - 1;
	isglob = 0;
	if (name[0] == '+' || name[0] == '-' || name[0] >= '0'&&name[0] <= '9')
		return -1;
	while (t >= 0) {
		if (strcmp(addrtable[t].name, name) == 0) {
			if (t < constedge) {
				isglob = 1;
			}
			return addrtable[t].address;
		}
		t--;
	}
	return -1;
}

//void dataseg() {
//    rsT << "\t.data" << endl;
//    while ( strcmp(midcode[mi].op, "const") == 0 ) {
//        if ( strcmp(midcode[mi].var1, "int") == 0 || 
//            strcmp(midcode[mi].var1, "char") == 0 ) {
//            rsT << ".word" << endl;
//        }
//    }
//}

//    jmp ,  ,  ,
void jmpasm() {
	rsT << "\t\tj\t" << midcode[mi].var3 << endl;
}

//    jne
void jneasm() {
	rsT << "\t\tbne\t$t0\t1\t" << midcode[mi].var3 << endl;
}

//    call, f ,   , a 
void callasm() {
	rsT << "\t\tjal\t" << midcode[mi].var1 << endl;
	rsT << "\t\tnop\n";
	if (midcode[mi].var3[0] != ' ' && midcode[mi].var3[0] != '\0') {
		int addr2;
		addr2 = varaddr(midcode[mi].var3);
		if (isglob)
			rsT << "\t\tsw\t$v0\t" << addr2 << "($t9)" << endl;
		else
			rsT << "\t\tsw\t$v0\t" << addr2 << "($fp)" << endl;
	}
}

//    lab,  ,  ,
void setlabasm() {
	rsT << midcode[mi].var3 << ":\n";
}

//    add, a, b, c
void addasm() {
	int addr1, addr2, addr3;
	if (isdigit(midcode[mi].var1[0]) || midcode[mi].var1[0] == '-' || midcode[mi].var1[0] == '+') {
		rsT << "\t\tli\t$t0\t" << midcode[mi].var1 << endl;
	}
	else {
		addr1 = varaddr(midcode[mi].var1);
		if (isglob)
			rsT << "\t\tlw\t$t0\t" << addr1 << "($t9)" << endl;
		else
			rsT << "\t\tlw\t$t0\t" << addr1 << "($fp)" << endl;
	}
	if (isdigit(midcode[mi].var2[0]) || midcode[mi].var2[0] == '-' || midcode[mi].var2[0] == '+') {
		rsT << "\t\tli\t$t1\t" << midcode[mi].var2 << endl;
	}
	else {
		addr2 = varaddr(midcode[mi].var2);
		if (isglob)
			rsT << "\t\tlw\t$t1\t" << addr2 << "($t9)" << endl;
		else
			rsT << "\t\tlw\t$t1\t" << addr2 << "($fp)" << endl;
	}
	addr3 = varaddr(midcode[mi].var3);
	rsT << "\t\tadd\t$t0\t$t0\t$t1" << endl;
	if (isglob)
		rsT << "\t\tsw\t$t0\t" << addr1 << "($t9)" << endl;
	else
		rsT << "\t\tsw\t$t0\t" << addr3 << "($fp)" << endl;
}

//    sub, a, b, c
void subasm() {
	int addr1, addr2, addr3;
	if (isdigit(midcode[mi].var1[0]) || midcode[mi].var1[0] == '-' || midcode[mi].var1[0] == '+') {
		rsT << "\t\tli\t$t0\t" << midcode[mi].var1 << endl;
	}
	else {
		addr1 = varaddr(midcode[mi].var1);
		if (isglob)
			rsT << "\t\tlw\t$t0\t" << addr1 << "($t9)" << endl;
		else
			rsT << "\t\tlw\t$t0\t" << addr1 << "($fp)" << endl;
	}
	if (isdigit(midcode[mi].var2[0]) || midcode[mi].var2[0] == '-' || midcode[mi].var2[0] == '+') {
		rsT << "\t\tli\t$t1\t" << midcode[mi].var2 << endl;
	}
	else {
		addr2 = varaddr(midcode[mi].var2);
		if (isglob)
			rsT << "\t\tlw\t$t1\t" << addr2 << "($t9)" << endl;
		else
			rsT << "\t\tlw\t$t1\t" << addr2 << "($fp)" << endl;
	}
	addr3 = varaddr(midcode[mi].var3);
	rsT << "\t\tsub\t$t0\t$t0\t$t1" << endl;
	if (isglob)
		rsT << "\t\tsw\t$t0\t" << addr3 << "($t9)" << endl;
	else
		rsT << "\t\tsw\t$t0\t" << addr3 << "($fp)" << endl;
}

//    mul, a, b, c
void mulasm() {
	int addr1, addr2, addr3;
	if (isdigit(midcode[mi].var1[0]) || midcode[mi].var1[0] == '-' || midcode[mi].var1[0] == '+') {
		rsT << "\t\tli\t$t0\t" << midcode[mi].var1 << endl;
	}
	else {
		addr1 = varaddr(midcode[mi].var1);
		if (isglob)
			rsT << "\t\tlw\t$t0\t" << addr1 << "($t9)" << endl;
		else
			rsT << "\t\tlw\t$t0\t" << addr1 << "($fp)" << endl;
	}
	if (isdigit(midcode[mi].var2[0]) || midcode[mi].var2[0] == '-' || midcode[mi].var2[0] == '+') {
		rsT << "\t\tli\t$t1\t" << midcode[mi].var2 << endl;
	}
	else {
		addr2 = varaddr(midcode[mi].var2);
		if (isglob)
			rsT << "\t\tlw\t$t1\t" << addr2 << "($t9)" << endl;
		else
			rsT << "\t\tlw\t$t1\t" << addr2 << "($fp)" << endl;
	}
	addr3 = varaddr(midcode[mi].var3);
	rsT << "\t\tmul\t$t0\t$t0\t$t1" << endl;
	if (isglob)
		rsT << "\t\tsw\t$t0\t" << addr3 << "($t9)" << endl;
	else
		rsT << "\t\tsw\t$t0\t" << addr3 << "($fp)" << endl;
}

//    div, a, b, c
void divasm() {
	int addr1, addr2, addr3;
	if (isdigit(midcode[mi].var1[0]) || midcode[mi].var1[0] == '-' || midcode[mi].var1[0] == '+') {
		rsT << "\t\tli\t$t0\t" << midcode[mi].var1 << endl;
	}
	else {
		addr1 = varaddr(midcode[mi].var1);
		if (isglob)
			rsT << "\t\tlw\t$t0\t" << addr1 << "($t9)" << endl;
		else
			rsT << "\t\tlw\t$t0\t" << addr1 << "($fp)" << endl;
	}
	if (isdigit(midcode[mi].var2[0]) || midcode[mi].var2[0] == '-' || midcode[mi].var2[0] == '+') {
		rsT << "\t\tli\t$t1\t" << midcode[mi].var2 << endl;
	}
	else {
		addr2 = varaddr(midcode[mi].var2);
		if (isglob)
			rsT << "\t\tlw\t$t1\t" << addr2 << "($t9)" << endl;
		else
			rsT << "\t\tlw\t$t1\t" << addr2 << "($fp)" << endl;
	}
	addr3 = varaddr(midcode[mi].var3);
	rsT << "\t\tdiv\t$t0\t$t0\t$t1" << endl;
	if (isglob)
		rsT << "\t\tsw\t$t0\t" << addr3 << "($t9)" << endl;
	else
		rsT << "\t\tsw\t$t0\t" << addr3 << "($fp)" << endl;
}

//    > , a, b, c
void greasm() {
	int addr1, addr2;
	if (isdigit(midcode[mi].var1[0]) || midcode[mi].var1[0] == '-' || midcode[mi].var1[0] == '+') {
		rsT << "\t\tli\t$t0\t" << midcode[mi].var1 << endl;
	}
	else {
		addr1 = varaddr(midcode[mi].var1);
		if (isglob)
			rsT << "\t\tlw\t$t0\t" << addr1 << "($t9)" << endl;
		else
			rsT << "\t\tlw\t$t0\t" << addr1 << "($fp)" << endl;
	}
	if (isdigit(midcode[mi].var2[0]) || midcode[mi].var2[0] == '-' || midcode[mi].var2[0] == '+') {
		rsT << "\t\tli\t$t1\t" << midcode[mi].var2 << endl;
	}
	else {
		addr2 = varaddr(midcode[mi].var2);
		if (isglob)
			rsT << "\t\tlw\t$t1\t" << addr2 << "($t9)" << endl;
		else
			rsT << "\t\tlw\t$t1\t" << addr2 << "($fp)" << endl;
	}
	rsT << "\t\tslt\t$t0\t$t1\t$t0" << endl;
}

//    >=
void geqasm() {
	int addr1, addr2;
	if (isdigit(midcode[mi].var1[0]) || midcode[mi].var1[0] == '-' || midcode[mi].var1[0] == '+') {
		rsT << "\t\tli\t$t0\t" << midcode[mi].var1 << endl;
	}
	else {
		addr1 = varaddr(midcode[mi].var1);
		if (isglob)
			rsT << "\t\tlw\t$t0\t" << addr1 << "($t9)" << endl;
		else
			rsT << "\t\tlw\t$t0\t" << addr1 << "($fp)" << endl;
	}
	if (isdigit(midcode[mi].var2[0]) || midcode[mi].var2[0] == '-' || midcode[mi].var2[0] == '+') {
		rsT << "\t\tli\t$t1\t" << midcode[mi].var2 << endl;
	}
	else {
		addr2 = varaddr(midcode[mi].var2);
		if (isglob)
			rsT << "\t\tlw\t$t1\t" << addr2 << "($t9)" << endl;
		else
			rsT << "\t\tlw\t$t1\t" << addr2 << "($fp)" << endl;
	}
	rsT << "\t\tslt\t$t0\t$t0\t$t1" << endl;
	rsT << "\t\tli\t$t1\t1" << endl;
	rsT << "\t\tsub\t$t0\t$t1\t$t0" << endl;
}

//    <
void lssasm() {
	int addr1, addr2;
	if (isdigit(midcode[mi].var1[0]) || midcode[mi].var1[0] == '-' || midcode[mi].var1[0] == '+') {
		rsT << "\t\tli\t$t0\t" << midcode[mi].var1 << endl;
	}
	else {
		addr1 = varaddr(midcode[mi].var1);
		if (isglob)
			rsT << "\t\tlw\t$t0\t" << addr1 << "($t9)" << endl;
		else
			rsT << "\t\tlw\t$t0\t" << addr1 << "($fp)" << endl;
	}
	if (isdigit(midcode[mi].var2[0]) || midcode[mi].var2[0] == '-' || midcode[mi].var2[0] == '+') {
		rsT << "\t\tli\t$t1\t" << midcode[mi].var2 << endl;
	}
	else {
		addr2 = varaddr(midcode[mi].var2);
		if (isglob)
			rsT << "\t\tlw\t$t1\t" << addr2 << "($t9)" << endl;
		else
			rsT << "\t\tlw\t$t1\t" << addr2 << "($fp)" << endl;
	}
	rsT << "\t\tslt\t$t0\t$t0\t$t1" << endl;
}

//    <=
void leqasm() {
	int addr1, addr2;
	if (isdigit(midcode[mi].var1[0]) || midcode[mi].var1[0] == '-' || midcode[mi].var1[0] == '+') {
		rsT << "\t\tli\t$t0\t" << midcode[mi].var1 << endl;
	}
	else {
		addr1 = varaddr(midcode[mi].var1);
		if (isglob)
			rsT << "\t\tlw\t$t0\t" << addr1 << "($t9)" << endl;
		else
			rsT << "\t\tlw\t$t0\t" << addr1 << "($fp)" << endl;
	}
	if (isdigit(midcode[mi].var2[0]) || midcode[mi].var2[0] == '-' || midcode[mi].var2[0] == '+') {
		rsT << "\t\tli\t$t1\t" << midcode[mi].var2 << endl;
	}
	else {
		addr2 = varaddr(midcode[mi].var2);
		if (isglob)
			rsT << "\t\tlw\t$t1\t" << addr2 << "($t9)" << endl;
		else
			rsT << "\t\tlw\t$t1\t" << addr2 << "($fp)" << endl;
	}
	rsT << "\t\tslt\t$t0\t$t1\t$t0" << endl;
	rsT << "\t\tli\t$t1\t1" << endl;
	rsT << "\t\tsub\t$t0\t$t1\t$t0" << endl;
}

//    ==
void eqlasm() {
	int addr1, addr2;
	if (isdigit(midcode[mi].var1[0]) || midcode[mi].var1[0] == '-' || midcode[mi].var1[0] == '+') {
		rsT << "\t\tli\t$t0\t" << midcode[mi].var1 << endl;
	}
	else {
		addr1 = varaddr(midcode[mi].var1);
		if (isglob)
			rsT << "\t\tlw\t$t0\t" << addr1 << "($t9)" << endl;
		else
			rsT << "\t\tlw\t$t0\t" << addr1 << "($fp)" << endl;
	}
	if (isdigit(midcode[mi].var2[0]) || midcode[mi].var2[0] == '-' || midcode[mi].var2[0] == '+') {
		rsT << "\t\tli\t$t1\t" << midcode[mi].var2 << endl;
	}
	else {
		addr2 = varaddr(midcode[mi].var2);
		if (isglob)
			rsT << "\t\tlw\t$t1\t" << addr2 << "($t9)" << endl;
		else
			rsT << "\t\tlw\t$t1\t" << addr2 << "($fp)" << endl;
	}

	int t1 = tlabelnum++;
	int t2 = tlabelnum++;
	rsT << "\t\tbne\t$t0\t$t1\t__tLABEL" << t1 << endl;
	rsT << "\t\tli\t$t0\t1" << endl;
	rsT << "\t\tj\t__tLABEL" << t2 << endl;
	rsT << "__tLABEL" << t1 << ":" << endl;
	//cout << "__tLABEL" << t1 << ":" << endl;
	rsT << "\t\tli\t$t0\t0" << endl;
	rsT << "__tLABEL" << t2 << ":" << endl;
}

//    !=
void neqasm() {
	int addr1, addr2;
	if (isdigit(midcode[mi].var1[0]) || midcode[mi].var1[0] == '-' || midcode[mi].var1[0] == '+') {
		rsT << "\t\tli\t$t0\t" << midcode[mi].var1 << endl;
	}
	else {
		addr1 = varaddr(midcode[mi].var1);
		if (isglob)
			rsT << "\t\tlw\t$t0\t" << addr1 << "($t9)" << endl;
		else
			rsT << "\t\tlw\t$t0\t" << addr1 << "($fp)" << endl;
	}
	if (isdigit(midcode[mi].var2[0]) || midcode[mi].var2[0] == '-' || midcode[mi].var2[0] == '+') {
		rsT << "\t\tli\t$t1\t" << midcode[mi].var2 << endl;
	}
	else {
		addr2 = varaddr(midcode[mi].var2);
		if (isglob)
			rsT << "\t\tlw\t$t1\t" << addr2 << "($t9)" << endl;
		else
			rsT << "\t\tlw\t$t1\t" << addr2 << "($fp)" << endl;
	}
	int t1 = tlabelnum++;
	int t2 = tlabelnum++;
	rsT << "\t\tbeq\t$t0\t$t1\t__tLABEL" << t1 << endl;
	rsT << "\t\tli\t$t0\t1" << endl;
	rsT << "\t\tj\t__tLABEL" << t2 << endl;
	rsT << "__tLABEL" << t1 << ":" << endl;
	//cout << "__tLABEL" << t1 << ":" << endl;
	rsT << "\t\tli\t$t0\t0" << endl;
	rsT << "__tLABEL" << t2 << ":" << endl;
}

//    =
void assasm() {
	int addr1, addr2;
	if (isdigit(midcode[mi].var1[0]) || midcode[mi].var1[0] == '-' || midcode[mi].var1[0] == '+') {
		rsT << "\t\tli\t$t0\t" << midcode[mi].var1 << endl;
	}
	else {
		addr1 = varaddr(midcode[mi].var1);
		if (isglob)
			rsT << "\t\tlw\t$t0\t" << addr1 << "($t9)" << endl;
		else
			rsT << "\t\tlw\t$t0\t" << addr1 << "($fp)" << endl;
	}
	addr2 = varaddr(midcode[mi].var3);
	if (isglob)
		rsT << "\t\tsw\t$t0\t" << addr2 << "($t9)" << endl;
	else
		rsT << "\t\tsw\t$t0\t" << addr2 << "($fp)" << endl;
}

//    []= , a , i , t
void aassasm() {
	int addr1, addr2, addrt;
	addr1 = varaddr(midcode[mi].var1);
	int tisglob = isglob;
	if (isdigit(midcode[mi].var2[0])) {
		addr1 += (atoi(midcode[mi].var2) * 4);
		if (isdigit(midcode[mi].var3[0]) || midcode[mi].var3[0] == '-' || midcode[mi].var3[0] == '+') {
			rsT << "\t\tli\t$t0\t" << midcode[mi].var3 << endl;
		}
		else {
			addr2 = varaddr(midcode[mi].var3);
			if (isglob) {
				rsT << "\t\tlw\t$t0\t" << addr2 << "($t9)" << endl;
			}
			else
				rsT << "\t\tlw\t$t0\t" << addr2 << "($fp)" << endl;
		}
		if (isglob)
			rsT << "\t\tsw\t$t0\t" << addr1 << "($t9)" << endl;
		else
			rsT << "\t\tsw\t$t0\t" << addr1 << "($fp)" << endl;
	}
	else {
		//求数组元素a[i]地址
		addrt = varaddr(midcode[mi].var2);//addrt = &i
		if (isglob)
			rsT << "\t\tlw\t$t1\t" << addrt << "($t9)" << endl;    //t1 = i
		else
			rsT << "\t\tlw\t$t1\t" << addrt << "($fp)" << endl;    //t1 = i
		rsT << "\t\tmul\t$t1\t$t1\t4\n";    //t1 = t1 * 4 (t1 = offset)
		rsT << "\t\taddi\t$t1\t$t1\t" << addr1 << endl;    //t1 = &a[i] - $fp
		rsT << "\t\tadd\t$t1\t$t1\t$fp" << endl;//t1 = &a[i]
		if (isdigit(midcode[mi].var3[0]) || midcode[mi].var3[0] == '-' || midcode[mi].var3[0] == '+') {
			rsT << "\t\tli\t$t0\t" << midcode[mi].var3 << endl;
		}
		else {
			addr2 = varaddr(midcode[mi].var3);
			if (isglob) {
				rsT << "\t\tlw\t$t0\t" << addr2 << "($t9)" << endl;
			}
			else {
				rsT << "\t\tlw\t$t0\t" << addr2 << "($fp)" << endl;
			}

		}
		rsT << "\t\tsw\t$t0\t0($t1)" << endl;
	}
}

//geta, a, n, b
void assaasm() {
	int addr1 = varaddr(midcode[mi].var1);
	int tisglob = isglob;
	int addr2;
	if (isdigit(midcode[mi].var2[0]) || midcode[mi].var2[0] == '-' || midcode[mi].var2[0] == '+') {
		addr1 += (atoi(midcode[mi].var2) * 4);    //addr1 = &a[n]
		addr2 = varaddr(midcode[mi].var3);        //addr2 = &b
		if (tisglob) {
			rsT << "\t\tlw\t$t0\t" << addr1 << "($t9)" << endl;
		}
		else
			rsT << "\t\tlw\t$t0\t" << addr1 << "($fp)" << endl;
		if (isglob) {
			rsT << "\t\tsw\t$t0\t" << addr2 << "($t9)" << endl;
		}
		else
			rsT << "\t\tsw\t$t0\t" << addr2 << "($fp)" << endl;
	}
	else {
		//求数组元素a[i]地址
		int addrt = varaddr(midcode[mi].var2);//addrt = &i
		if (isglob) {
			rsT << "\t\tlw\t$t1\t" << addrt << "($t9)" << endl;    //t1 = i
		}
		else
			rsT << "\t\tlw\t$t1\t" << addrt << "($fp)" << endl;    //t1 = i
		rsT << "\t\tmul\t$t1\t$t1\t4\n";    //t1 = t1 * 4 (t1 = offset)
		rsT << "\t\taddi\t$t1\t$t1\t" << addr1 << endl;    //t1 = &a[i] - $fp
		rsT << "\t\tadd\t$t1\t$t1\t$fp" << endl;
		rsT << "\t\tlw\t$t1\t0($t1)\n";    //t1 = a[i]
		addr2 = varaddr(midcode[mi].var3);    //addr2 = &b
		if (isglob)
			rsT << "\t\tsw\t$t1\t" << addr2 << "($t9)" << endl;
		else
			rsT << "\t\tsw\t$t1\t" << addr2 << "($fp)" << endl;
	}
}

//scf ,   ,   , a
void scfasm() {
	int addr = varaddr(midcode[mi].var3);
	int ti = findvartable(midcode[mi].var3);
	int kind = addrtable[ti].kind;
	if (kind == INT) {
		rsT << "\t\tli\t$v0\t5" << endl;
		rsT << "\t\tsyscall" << endl;
		//rsT << "\t\tsubi\t$v0\t$v0\t" << ( int )'0' << endl;
		if (isglob)
			rsT << "\t\tsw\t$v0\t" << addr << "($t9)" << endl;
		else
			rsT << "\t\tsw\t$v0\t" << addr << "($fp)" << endl;
	}
	else {
		rsT << "\t\tli\t$v0\t12" << endl;
		rsT << "\t\tsyscall" << endl;
		if (isglob)
			rsT << "\t\tsw\t$v0\t" << addr << "($t9)" << endl;
		else
			rsT << "\t\tsw\t$v0\t" << addr << "($fp)" << endl;
	}
}

//prt, a, b, symb
void prtasm() {
	int addr;
	if (midcode[mi].var1[0] != '\0') {
		int len = strlen(midcode[mi].var1);
		for (int i = 0; i < len; i++) {
			rsT << "\t\tli\t$v0\t11" << endl;
			rsT << "\t\tli\t$a0\t" << int(midcode[mi].var1[i]) << endl;
			rsT << "\t\tsyscall" << endl;
		}
	}
	if (midcode[mi].var2[0] != ' ' && midcode[mi].var2[0] != '\0') {
		if (isdigit(midcode[mi].var2[0]) && strcmp(midcode[mi].var3, "char") == 0) {
			rsT << "\t\tli\t$v0\t11" << endl;
			rsT << "\t\tli\t$a0\t" << midcode[mi].var2 << endl;
			rsT << "\t\tsyscall" << endl;
			return;
		}
		else if (isdigit(midcode[mi].var2[0]) || midcode[mi].var2[0] == '-' || midcode[mi].var2[0] == '+') {
			rsT << "\t\tli\t$v0\t1" << endl;
			rsT << "\t\tli\t$a0\t" << midcode[mi].var2 << endl;
			rsT << "\t\tsyscall" << endl;
			return;
		}
		addr = varaddr(midcode[mi].var2);
		int ti = findvartable(midcode[mi].var2);
		int kind = addrtable[ti].kind;
		if (kind == INT || kind == WINT) {
			rsT << "\t\tli\t$v0\t1" << endl;
			if (isglob) {
				rsT << "\t\tlw\t$a0\t" << addr << "($t9)" << endl;
			}
			else
				rsT << "\t\tlw\t$a0\t" << addr << "($fp)" << endl;
			rsT << "\t\tsyscall" << endl;
		}
		else {
			rsT << "\t\tli\t$v0\t11" << endl;
			if (isglob)    rsT << "\t\tlw\t$a0\t" << addr << "($t9)" << endl;
			else    rsT << "\t\tlw\t$a0\t" << addr << "($fp)" << endl;
			rsT << "\t\tsyscall" << endl;
		}
	}
}

//fupa,   ,   , a     ==> a is a function parameter
void fupaasm() {
	if (isdigit(midcode[mi].var3[0])) {
		rsT << "\t\tli\t$t0\t" << midcode[mi].var3 << endl;
	}
	else {
		rsT << "\t\tlw\t$t0\t" << varaddr(midcode[mi].var3); //li    $t0 item
		if (isglob) {
			rsT << "($t9)" << endl;
		}
		else {
			rsT << "($fp)" << endl;
		}
	}
	rsT << "\t\tsw\t$t0\t($sp)" << endl;      //sw    $t0 $sp
	sp -= 4;
	rsT << "\t\tsubi\t$sp\t$sp\t4" << endl; //subi  $sp $sp 4  
}

//ret ,   ,   , (a)   ==> return a / return
void retasm() {
	if (midcode[mi].var3[0] != ' ' && midcode[mi].var3[0] != '\0') {
		if (isdigit(midcode[mi].var3[0]) || midcode[mi].var2[0] == '-' || midcode[mi].var2[0] == '+') {
			rsT << "\t\tli\t$v0\t" << midcode[mi].var3 << endl;
		}
		else {
			int addr2 = varaddr(midcode[mi].var3);
			if (isglob)
				rsT << "\t\tlw\t$v0\t" << addr2 << "($t9)" << endl;
			else
				rsT << "\t\tlw\t$v0\t" << addr2 << "($fp)" << endl;
		}
	}
	rsT << "\t\tj\t__FEND_LAB_" << funcnum << endl;//跳至结束
}

//para, int, , a == > f(int a, ...)
void paraasm() {
	paranum = 0;
	for (int i = mi; i < midcodeiter; i++) {
		if (strcmp(midcode[i].op, "para") == 0)
			paranum++;
		else
			break;
	}
	for (int i = 0; i < paranum; i++) {
		int kind = (strcmp(midcode[mi].var1, "int") == 0) ? INT : CHAR;
		insertaddress(kind, 4 * (paranum - i));
		mi++;
	}
	mi--;
}

void intcharasm() {
	if (isdigit(midcode[mi].var2[0])) {
		pushstack(midcode[mi].var2);
	}
	else {
		pushstack("0");
	}
	if (strcmp(midcode[mi].op, "int") == 0) {
		insertaddress(INT);
	}
	else {
		insertaddress(CHAR);
	}
}

void constdefasm() {
	//常量定义
	while (strcmp(midcode[mi].op, "const") == 0) {
		pushstack(midcode[mi].var2);
		if (strcmp(midcode[mi].var1, "int") == 0) {
			insertaddress(INT);
		}
		else {
			insertaddress(CHAR);
		}
		mi++;
	}
	mi--;
}

void intcharaasm() {
	//数组定义
	while (strcmp(midcode[mi].op, "inta") == 0 || strcmp(midcode[mi].op, "chara") == 0) {
		pushstack("0", atoi(midcode[mi].var2));
		if (strcmp(midcode[mi].op, "inta") == 0) {
			insertaddress(INT);
		}
		else {
			insertaddress(CHAR);
		}
		mi++;
	}
	mi--;

}

void cntopt() {
	//引用计数
	int tmi;
	if (OPTFLAG) {
		tmi = mi;
		while (strcmp(midcode[tmi].op, "end") != 0) {
			if (!strcmp(midcode[tmi].op, "=") || !strcmp(midcode[tmi].op, "+") || !strcmp(midcode[tmi].op, "-") || !strcmp(midcode[tmi].op, "*") || !strcmp(midcode[tmi].op, "/")) {
				cnt(midcode[tmi].var1);
				cnt(midcode[tmi].var2);
				cnt(midcode[tmi].var3);
			}
			tmi++;
		}
		sort(cnttable, cnttable + cntindex, cmpcnt);
		for (int i = 0; i < 8; i++) {
			varreg[cnttable[i].symbnum] = i;
			int addr = addrtable[cnttable[i].symbnum].address;
			rsT << "\t\tlw\t$s" << i << "\t" << addr << "($fp)" << endl;
		}
	}
}

void savesreg() {
	if (OPTFLAG) {
		for (int i = 0; i < 8; i++) {
			rsT << "\t\tsw\t$t" << i << "\t" << 4 * i << "($t8)" << endl;
		}
	}
}

void loadsreg() {
	if (OPTFLAG) {
		for (int i = 0; i < 8; i++) {
			rsT << "\t\tlw\t$t" << i << "\t" << 4 * i << "($t8)" << endl;
		}
	}
}

void cnt(char* name) {
	int t = ap - 1;
	isglob = 0;
	if (name[0] == '+' || name[0] == '-' || (name[0] >= '0'&& name[0] <= '9') || name[0] == ' ' || name[0] == '\t' || name[0] == '\0')
		return;
	while (t >= constedge) {
		if (strcmp(addrtable[t].name, name) == 0) {
			addrtable[t].cnt++;
			for (int q = 0; q <= cntindex; q++) {
				if (q == cntindex) {
					cnttable[cntindex].cnt = 1;
					cnttable[cntindex].symbnum = t;
					cntindex++;
					break;
				}
				if (cnttable[q].symbnum == t) {
					cnttable[q].cnt++;
					break;
				}
			}
		}
		t--;
	}
}