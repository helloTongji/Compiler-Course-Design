#include "err.h"
#include "glob.h"
#include<QDebug>
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
    WrongLineMessage=WrongLineMessage+"Error ";
    WrongLineMessage=WrongLineMessage+QString::number(errnum);
    WrongLineMessage=WrongLineMessage+": LINE ";
    int Line_ERROR=(cnum == 0 ? lnum - 1 : lnum);
    int Column_ERROR=(cnum == 0 ? prllen : cnum + 1);
    WrongLineMessage=WrongLineMessage+QString::number(Line_ERROR);
    WrongLineMessage=WrongLineMessage+", COLUMN ";
    WrongLineMessage=WrongLineMessage+QString::number(Column_ERROR);
    WrongLineMessage=WrongLineMessage+": Near '";
    WrongLineMessage=WrongLineMessage+QString::fromStdString(prid);
    WrongLineMessage=WrongLineMessage+"' : ";



    switch (_errsig) {
    case  NOSUCHFILE:            errclass = QUIT;       WrongLineMessage=WrongLineMessage+"NO SUCH FILE\n";         break;
    case  FILEINCOMPLETE:        errclass = DONOTHING;  WrongLineMessage=WrongLineMessage+"FILEINCOMPLETE\n";       break;
    case  DOUBLEQUOTESLACK:      errclass = DONOTHING;  WrongLineMessage=WrongLineMessage+"DOUBLEQUOTESLACK\n";     break;
    case  UNACCEPTATLECHAR:      errclass = DONOTHING;  WrongLineMessage=WrongLineMessage+"UNACCEPTATLECHAR\n";     break;
    case  SINGLEQUOTESLACK:      errclass = DONOTHING;  WrongLineMessage=WrongLineMessage+"SINGLEQUOTESLACK\n";     break;
    case  OUTOFTABLE:            errclass = QUIT;       WrongLineMessage=WrongLineMessage+"OUT OF TABLE\n";         break;
    case  SYMBOLCONFLICT:        errclass = DONOTHING;  WrongLineMessage=WrongLineMessage+"SYMBOL CONFLICT\n";      break;
    case  CSTDEFINEFAIL:         errclass = CS;         WrongLineMessage=WrongLineMessage+"CST DEFINE FAIL\n";      break;
    case  VARNOTINIT:            errclass = DONOTHING;  WrongLineMessage=WrongLineMessage+"VARNOTINIT\n";           break;
	case  UNKNOWRIGHTSYM:        errclass =
		signal == 0 ? CS :
		signal == 1 ? IWFLIRSPE :
		-1;
        WrongLineMessage=WrongLineMessage+"UNKNOWRIGHTSYM\n";
        break;
	case  SEMICOLONLACK:        errclass =
		signal == 0 ? CICVIFIRSP :
		signal == 1 ? IWFRSPIFCV :
		signal == 2 ? IWFLIRSPE :
		-1;
        WrongLineMessage=WrongLineMessage+"SEMICOLONLACK\n";
		break;
	case  KEYWORDERROR:
		errclass = signal == 0 ? CICVIFIRSP :
			signal == 1 ? CLR :
            signal == 2 ? IWFRSPIFCV :printf("KEYWORDERROR\n");
            WrongLineMessage=WrongLineMessage+"KEYWORDERROR\n";
		break;
	case  IDENTIFIERLACK:        errclass =
		signal == 0 ? ICV :
		signal == 1 ? CLR :
		signal == 2 ? IWFRSPIFCV :
		signal == 3 ? IWFLIRSPE :
		signal == 4 ? IWFXXXANE :
		-1;
        WrongLineMessage=WrongLineMessage+"IDENTIFIER LACK\n";
		break;
	case  RIGHTBRACKLACK:        errclass =
		signal == 0 ? IWFXXXANE :
		signal == 1 ? IWFRSPIFCV :
		-1;
        WrongLineMessage=WrongLineMessage+"RIGHT BRACK LACK\n";
		break;
	case  FUNCTIONNOTFOUND:        errclass =
		signal == 0 ? IWFLIRSPE :
		signal == 1 ? IWFXXXANE :
		-1;
        WrongLineMessage=WrongLineMessage+"FUNCTION NOT FOUND\n";
        break;
	case  FORMALPARANUMUNMATCH:    errclass =
		signal == 0 ? DONOTHING :
		signal == 1 ? IWFLIRSPE :
		-1;
        WrongLineMessage=WrongLineMessage+"FORMAL PARA NUM UNMATCH\n";
        break;
	case  VARNOTDEFINE:            errclass =
		signal == 0 ? DONOTHING :
		signal == 1 ? IWFLIRSPE :
		-1;
        WrongLineMessage=WrongLineMessage+"VAR NOT DEFINE\n";
        break;
	case  LEFTPARENTLACK:        errclass =
		signal == 0 ? ICV :
		signal == 1 ? IWFLIRSPE :
		signal == 2 ? IWFXXXANE :
		-1;
        WrongLineMessage=WrongLineMessage+"LEFT PARENT LACK\n";
        break;
	case  RIGHTPARENTLACK:        errclass =
		signal == 0 ? ICV :
		signal == 1 ? IWFLIRSPE :
		signal == 2 ? IWFXXXANE :
		-1;
        WrongLineMessage=WrongLineMessage+"RIGHT PARENT LACK\n";
        break;
    case  IMMLACK:               errclass = IWFRSPIFCV;  WrongLineMessage=WrongLineMessage+"IMM LACK\n";   break;
	case  RIGHTBRACELACK:        errclass =
		signal == 0 ? ICV :
		signal == 1 ? IWFLIRSPE :
        -1;
        WrongLineMessage=WrongLineMessage+"RIGHT BRACE LACK\n";
        break;
    case  FUNCTIONRETURNNULL:    errclass = IWFXXXANE;    WrongLineMessage=WrongLineMessage+"FUNCTION RETURN NULL\n";   break;
    case  EXPRESSIONERROR:       errclass = IWFXXXANE;   WrongLineMessage=WrongLineMessage+"EXPRESSION ERROR\n";  break;
    case  UNACCEPTABLESENTENCE:  errclass = IWFLIRSPE;  WrongLineMessage=WrongLineMessage+"UNACCEPTABLE SENTENCE\n";    break;
    case  ASSIGNCONST:           errclass =
		signal == 0 ? IWFLIRSPE :
		signal == 1 ? IWFLIRSPE :
		-1;
        WrongLineMessage=WrongLineMessage+"ASSIGN CONST\n";
        break;
    case  LEFTBRACELACK:        errclass = ICV;    WrongLineMessage=WrongLineMessage+"LEFT BRACE LACK\n";   break;
    case  NONERETURN:            errclass = ICV;   WrongLineMessage=WrongLineMessage+"NON ERETURN\n";   break;
    case  PLUSMINULACK:            errclass = IWFLIRSPE;    WrongLineMessage=WrongLineMessage+"PLUS or MINU LACK\n";   break;
    case  MAINLACK:                errclass = DONOTHING;    WrongLineMessage=WrongLineMessage+"MAIN LACK\n";   break;
    case  MOREWORD:                errclass = DONOTHING;    WrongLineMessage=WrongLineMessage+"MORE WORD\n";   break;
    case  CONSTNOTINIT:        errclass = CS;    WrongLineMessage=WrongLineMessage+"CONST NOT INIT\n";  break;
    default:                    errclass = QUIT;
        WrongLineMessage=WrongLineMessage+"Unknow error occurs! [error code: ";
        WrongLineMessage=WrongLineMessage+QString::number(_errsig);
        WrongLineMessage=WrongLineMessage+"]\n";
	}
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
