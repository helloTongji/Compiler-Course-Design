#include "err.h"
#include "glob.h"

#define QUIT 1
#define DONOTHING 2
#define ICV    3
#define CICVIFIRSP 4
#define CS    5
#define CLR 6
#define IWFRSPIFCV 7
#define IWFLIRSPE 8
#define IWFXXXANE 9
int errnum = 0;

void error(int _errsig, int signal) {
	int errclass = -1;
	errnum++;
	printf("Error %d: LINE %d, COLUMN %d: Near \"%s\" : ", errnum, cnum == 0 ? lnum - 1 : lnum, cnum == 0 ? prllen : cnum + 1, prid.c_str());
	switch (_errsig) {
	case  NOSUCHFILE:            errclass = QUIT;    printf("NO SUCH FILE\n");    break;
	case  FILEINCOMPLETE:        errclass = DONOTHING;    printf("FILEINCOMPLETE\n");    break;
	case  DOUBLEQUOTESLACK:        errclass = DONOTHING;    printf("DOUBLEQUOTESLACK\n");    break;
	case  UNACCEPTATLECHAR:        errclass = DONOTHING;    printf("UNACCEPTATLECHAR\n");    break;
	case  SINGLEQUOTESLACK:        errclass = DONOTHING;    printf("SINGLEQUOTESLACK\n");    break;
	case  OUTOFTABLE:            errclass = QUIT;    printf("OUT OF TABLE\n");    break;
	case  SYMBOLCONFLICT:        errclass = DONOTHING;    printf("SYMBOL CONFLICT\n");    break;
	case  CSTDEFINEFAIL:        errclass = CS;    printf("CST DEFINE FAIL\n");    break;
	case  VARNOTINIT:            errclass = DONOTHING;    printf("VARNOTINIT\n");    break;
	case  UNKNOWRIGHTSYM:        errclass =
		signal == 0 ? CS :
		signal == 1 ? IWFLIRSPE :
		-1;
		printf("UNKNOWRIGHTSYM\n");    break;
	case  SEMICOLONLACK:        errclass =
		signal == 0 ? CICVIFIRSP :
		signal == 1 ? IWFRSPIFCV :
		signal == 2 ? IWFLIRSPE :
		-1;
		printf("SEMICOLONLACK\n");
		break;
	case  KEYWORDERROR:
		errclass = signal == 0 ? CICVIFIRSP :
			signal == 1 ? CLR :
			signal == 2 ? IWFRSPIFCV :
			printf("KEYWORDERROR\n");
		break;
	case  IDENTIFIERLACK:        errclass =
		signal == 0 ? ICV :
		signal == 1 ? CLR :
		signal == 2 ? IWFRSPIFCV :
		signal == 3 ? IWFLIRSPE :
		signal == 4 ? IWFXXXANE :
		-1;
		printf("IDENTIFIER LACK\n");
		break;
	case  RIGHTBRACKLACK:        errclass =
		signal == 0 ? IWFXXXANE :
		signal == 1 ? IWFRSPIFCV :
		-1;
		printf("RIGHT BRACK LACK\n");
		break;
	case  FUNCTIONNOTFOUND:        errclass =
		signal == 0 ? IWFLIRSPE :
		signal == 1 ? IWFXXXANE :
		-1;
		printf("FUNCTION NOT FOUND\n");    break;
	case  FORMALPARANUMUNMATCH:    errclass =
		signal == 0 ? DONOTHING :
		signal == 1 ? IWFLIRSPE :
		-1;
		printf("FORMAL PARA NUM UNMATCH\n");    break;
	case  VARNOTDEFINE:            errclass =
		signal == 0 ? DONOTHING :
		signal == 1 ? IWFLIRSPE :
		-1;
		printf("VAR NOT DEFINE\n");    break;
	case  LEFTPARENTLACK:        errclass =
		signal == 0 ? ICV :
		signal == 1 ? IWFLIRSPE :
		signal == 2 ? IWFXXXANE :
		-1;
		printf("LEFT PARENT LACK\n");    break;
	case  RIGHTPARENTLACK:        errclass =
		signal == 0 ? ICV :
		signal == 1 ? IWFLIRSPE :
		signal == 2 ? IWFXXXANE :
		-1;
		printf("RIGHT PARENT LACK\n");    break;
	case  IMMLACK:                errclass = IWFRSPIFCV;    printf("IMM LACK\n");    break;
	case  RIGHTBRACELACK:        errclass =
		signal == 0 ? ICV :
		signal == 1 ? IWFLIRSPE :
		-1;    printf("RIGHT BRACE LACK\n");    break;
	case  FUNCTIONRETURNNULL:    errclass = IWFXXXANE;    printf("FUNCTION RETURN NULL\n");    break;
	case  EXPRESSIONERROR:        errclass = IWFXXXANE;    printf("EXPRESSION ERROR\n");    break;
	case  UNACCEPTABLESENTENCE:    errclass = IWFLIRSPE;    printf("UNACCEPTABLE SENTENCE\n");    break;
	case  ASSIGNCONST:            errclass =
		signal == 0 ? IWFLIRSPE :
		signal == 1 ? IWFLIRSPE :
		-1;
		printf("ASSIGN CONST\n");    break;
	case  LEFTBRACELACK:        errclass = ICV;    printf("LEFT BRACE LACK\n");    break;
	case  NONERETURN:            errclass = ICV;    printf("NON ERETURN\n");    break;
	case  PLUSMINULACK:            errclass = IWFLIRSPE;    printf("PLUS or MINU LACK\n");    break;
	case  MAINLACK:                errclass = DONOTHING;    printf("MAIN LACK\n");    break;
	case  MOREWORD:                errclass = DONOTHING;    printf("MORE WORD\n");    break;
	case  CONSTNOTINIT:        errclass = CS;    printf("CONST NOT INIT\n");    break;
	default:                    errclass = QUIT;    printf("Unknow error occurs! [error code: %d]\n", _errsig);
	}
	/*
	laxrst.close();
	asmrst.close();
	midcoderst.close();
	symtablehis.close();
	fclose(src);
	system("pause");
	exit(0);
	*/
	switch (errclass) {
	case QUIT:
		system("pause");
		exit(0);
		break;
	case DONOTHING:
		break;
	case ICV:
		while (symid != INTTK && symid != CHARTK && symid != VOIDTK) {
			if (symid == -1)
			{
				system("pause");    exit(0);
			}
			getsym();
		}
		break;
	case CICVIFIRSP:
		while (symid != CONSTTK && symid != INTTK && symid != CHARTK && symid != VOIDTK && symid != IFTK && symid != WHILETK
			&& symid != FORTK && symid != IDEN && symid != RETURNTK && symid != SCANFTK && symid != PRINTFTK) {
			if (symid == -1) { system("pause");    exit(0); }
			getsym();
		}
		break;
	case CS:
		while (symid != COMMA && symid != SEMICN) {
			if (symid == -1) { system("pause");    exit(0); }
			getsym();
		}
		break;
	case CLR:
		while (symid != COMMA && symid != LPARENT && symid != RPARENT) {
			if (symid == -1) { system("pause");    exit(0); }
			getsym();
		}
		break;
	case IWFRSPIFCV:
		while (symid != IFTK && symid != WHILETK && symid != FORTK && symid != RETURNTK && symid != SCANFTK
			&& symid != PRINTFTK && symid != INTTK && symid != CHARTK && symid != VOIDTK) {
			if (symid == -1) { system("pause");    exit(0); }
			getsym();
		}
		break;
	case IWFLIRSPE:
		while (symid != IFTK && symid != WHILETK && symid != FORTK && symid != LBRACK && symid != IDEN
			&& symid != RETURNTK && symid != SCANFTK && symid != PRINTFTK && symid != ELSETK && symid != RBRACE) {
			if (symid == -1) { system("pause");    exit(0); }
			getsym();
		}
		break;
	case IWFXXXANE:
		while (symid != IFTK && symid != WHILETK && symid != FORTK && symid != LBRACK && symid != IDEN && symid != RETURNTK
			&& symid != SCANFTK && symid != PRINTFTK && symid != SEMICN && symid != ELSETK && symid != RPARENT
			&& symid != COMMA && symid != PLUS && symid != MINU && symid != MULT && symid != DIV
			&& symid != LSS && symid != LEQ && symid != GRE && symid != GEQ && symid != NEQ && symid != EQL) {
			if (symid == -1) { system("pause");    exit(0); }
			getsym();
		}
		break;
	default:
		break;
	}

}