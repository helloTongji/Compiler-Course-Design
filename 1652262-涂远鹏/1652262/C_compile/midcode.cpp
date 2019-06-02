//
//  midcode.cpp
//  exc0compiler
//
//  Created by sciencefans on 14/11/29.
//  Copyright (c) 2014年 sciencefans. All rights reserved.
//



#include "glob.h"

fourvarcode midcode[MAXMIDCODE];
int midcodeiter = 0;
int labelcnt = 0;
int varcnt = 0;
vector <char*> hisvar;

char* itoa(int i) {
	char *temp = (char*)malloc(sizeof(char) * 10);
	sprintf(temp, "%d", i);
	return temp;
}

//插入中间代码
void insmidcode(char* op, char* t1, char* t2, char* t3) {
	if (strcmp(op, "func") == 0) {
		midcoderst << endl << endl;
	}
	midcoderst << "\t\t";
	midcoderst << op << "\t," << t1 << "\t," << t2 << "\t," << t3 << endl;
	strcpy(midcode[midcodeiter].op, op);
	strcpy(midcode[midcodeiter].var1, t1);
	strcpy(midcode[midcodeiter].var2, t2);
	strcpy(midcode[midcodeiter].var3, t3);
	midcodeiter++;
}
void insmidcode(char* op, int t1, int t2, char* t3) {
	midcoderst << "\t\t";
	midcoderst << op << "\t," << t1 << "\t," << t2 << "\t," << t3 << endl;
	strcpy(midcode[midcodeiter].op, op);
	char *t;

	strcpy(midcode[midcodeiter].var1, strcmp(t = itoa(t1), "0") == 0 ? "  " : t);
	strcpy(midcode[midcodeiter].var2, t = itoa(t2));
	strcpy(midcode[midcodeiter].var3, t3);
	midcodeiter++;
	free(t);
}
void insmidcode(char* op, char* t1, int t2, char* t3) {
	midcoderst << "\t\t";
	midcoderst << op << "\t," << t1 << "\t," << t2 << "\t," << t3 << endl;
	strcpy(midcode[midcodeiter].op, op);
	char *t;
	strcpy(midcode[midcodeiter].var1, t1);
	strcpy(midcode[midcodeiter].var2, t = itoa(t2));
	strcpy(midcode[midcodeiter].var3, t3);
	midcodeiter++;
	free(t);
}
//生成新的标记
char* nextlab() {
	char *label = (char*)malloc(sizeof(char) * 20);
	strcpy(label, "_LABEL_");
	sprintf(label, "_LABEL_%d", labelcnt++);
	return label;
}

//生成新的变量
char* nextvar()
{
	char *var = (char*)malloc(sizeof(char) * 20);
	sprintf(var, "$_%d", varcnt++);
	//insmidcode("int", "  ", "  ", var);
	hisvar.push_back(var);
	return var;
}

void freetempvar() {
	for (int i = 0; i < hisvar.size(); i++) {
		free(hisvar[i]);
	}
}