#pragma once
#include <string>
#include <cstring>
#define _bl 1
#define _hs 2
#define _void 3
#define _int 4
#define _if 5
#define _while 6
#define _return 7
#define _ydkh 8
#define error -1
#define is_bl 9
#define is_hs 10
#define is_num 11
#define is_main 12
using namespace std;

struct hs_info
{
	char hs_name[20];
	int f_return;
	char w_return[20];

	int par_num;
};

struct bl_info
{
	char bl_name[20];
};