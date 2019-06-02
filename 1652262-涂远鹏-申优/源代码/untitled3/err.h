#ifndef exc0compiler_err_h
#define exc0compiler_err_h

#define NOSUCHFILE            999
#define FILEINCOMPLETE      0
#define DOUBLEQUOTESLACK    1
#define UNACCEPTATLECHAR    2
#define SINGLEQUOTESLACK    3
#define OUTOFTABLE          4
#define SYMBOLCONFLICT      5
#define CSTDEFINEFAIL       6
#define VARNOTINIT          7
#define UNKNOWRIGHTSYM      8
#define SEMICOLONLACK       9
#define KEYWORDERROR        10
#define IDENTIFIERLACK      11
#define RIGHTBRACKLACK      12
#define FUNCTIONNOTFOUND    13
#define FORMALPARANUMUNMATCH 14
#define VARNOTDEFINE        15
#define LEFTPARENTLACK      16
#define RIGHTPARENTLACK     17
#define IMMLACK             18
#define RIGHTBRACELACK      19
#define FUNCTIONRETURNNULL  20
#define EXPRESSIONERROR     21
#define UNACCEPTABLESENTENCE 22
#define ASSIGNCONST         23
#define LEFTBRACELACK       24
#define NONERETURN          25
#define PLUSMINULACK        26
#define MAINLACK            27
#define MOREWORD            28
#define CONSTNOTINIT        29

extern void error(int _errsig, int signal = 0);
extern int errnum;
#endif
