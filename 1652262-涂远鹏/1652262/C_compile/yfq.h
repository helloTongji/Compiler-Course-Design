#include <fstream>
using namespace std;

class yfq
{
private:
	int end_flag;       //���������־
	int read_main;      //�Ƿ��Ѿ�����main����
	int return_flag;
	char return_words[100];
	char c;             //c = fin.get()����ʱ�������
	char bl_name[100];        //��ʱ��¼������(��'\0'����)
	char hs_name[100];        //��ʱ��¼����������'\0'������
	int len_hs;              //�Ӻ�������')'�ĳ���
	int available_T;         //Ti,i=available_T��Ϊ��ǰ���õ�i
	int available_L;         //Li,i=available_L
	char var_name[10][100];    //��¼������
	int kh[100];             //��¼������������
	int len_kh;              //
	char headwords[100];     //�����ַ�
	int is_hs_head;        //�Ƿ������ھ��ױ�־���ж��Ƿ���Ҫreturn��
	int is_hs_dy;          //�Ƿ�������
	ifstream fin;
	ofstream fout1;         //����м����
	ofstream fout2;         //���������
public:
	yfq();
	~yfq();
	bool zs_handle();                       //�ж��Ƿ�Ϊ��Чע�� && ȥ��ע��
											//bool u_handle(char u[], int len_u,int mode);     //���Ӵ�����
											//bool s_handle(char c[], int len_c);     //�ӷ����ʽ������
	int fh_rank(char fh);                   //������ŵȼ�
	bool r_handle(char c[], int len_c, int mode);      //���ʽ��������mode: 0-> = r;  1-> if( r )  2-> while( r )
	bool bl_handle();                       //����Ϊ�����Ĵ�����
	bool void_handle();                     //����Ϊvoid�Ĵ�����
	bool int_handle();                      //����Ϊint�Ĵ�����
	int get_kind(char c[], int len_c);      //�Ϸ����������Ϸ����֣�
	void error_rep();                       //������ش�����
	int readhead();                         //������
	void analyze();
};
