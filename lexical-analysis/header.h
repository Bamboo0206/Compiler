#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<vector>
#include<string>
#include<fstream>
#include<cstring>
using namespace std;

/*��������*/
#define HALF_BUF_SIZE 1024	//���໺��������
#define buf_left 0			//��໺����
#define buf_right 1			//�Ҳ໺����

#define KEY_NUM 37			//�ؼ�������
#define PUNC_NUM 54			//�������
#define OP_NUM 39			//����������
#define LIM_NUM 11			//�������
//#define othernum 12			//������������

#define IDENTIFIER 1		//��ͨ��ʶ��
#define KEYWORD 2			//�ؼ���
#define INTEGER 3			//����
#define FLOATDIG 4			//������
#define OPERATOR 5			//��������
#define DELIMITER 6			//���
//#define OTHER 7				//��������

enum ErrorCode { FLOAT_ERROR, EXP_ERROR, INVALID_NUMBER, INVALID_CHARACTER, Other_wrong, INVALID_IDENTIFIER, TYPE_NOT_EXIST_ERROR
};


//�ؼ���
const char* key[KEY_NUM] =
{
	"auto","break","case","char","const","continue","default","do","double",
	"else","enum","extern","float","for","goto","if","inline","int","long",
	"register","restrict","return","short","signed","sizeof","static","struct",
	"switch","typedef","union","unsigned","void","volatile","while","_Bool",
	"_Complex","_Imaginary"
};

/*//������
const char* punctuator[PUNC_NUM] =
{
	"[","]","(",")","{","}",
	".","->",
	"++","--","&","*"," + "," - ","~","!"," / ","%",
	" << "," >> ",
	"<",">"," <= "," >= "," == "," != ","^",
	" | ","&&"," || ",
	" ? "," : ","; ","..."," = "," *= "," /= ",
	" %= "," += "," -= "," <<= "," >>= "," &= "," ^= "," |= ",
	",","#","##","<:"," : >","<%","%>","%:","% : % :"
};*/

const char delimiter[LIM_NUM]=
{
	'[', ']', '(', ')', '{', '}', ';', ',', '\"', '\'', '\\'
};
const char* op[OP_NUM] =
{
	".", ":", " ? ", "~", " + ", "++", " += ", " - ", "--", 
	" -= ", "->", "*", " *= ", " / ", " /= ", "%", " %= ", "^", 
	" ^= ", "&", "&&", " &= ", "<", " <= ", " << ", " <<= ", ">", 
	" >= ", " >> ", " >>= ", "!", " != ", " = ", " == ", " | ", " || ", 
	" |= ", "#", "##"
};

//�ඨ��
class Token
{
public:
	Token();
	~Token();

	int type;			//�Ǻ�
	int index;			//����
	int line;			//��������
private:

};

Token::Token()
{
	type = 0;
	index = 0;
	line = 0;
}

Token::~Token()
{
}

/*ȫ�ֱ�������*/
vector<long long>int_table;		//������
vector<double>float_table;		//��������
vector<string>identifier_table;			//��ʶ����
vector<Token>token_table;	//�Ǻ�����

int line = 1, word_num = 0, letter_num = 0;//�кţ����ʸ���ͳ�ƣ��ַ�����ͳ��
char buf[HALF_BUF_SIZE * 2] = { '\0' };//˫������
char *beginPtr, *forwardPtr;//��ʼָ�룬ǰ��ָ��
char token[HALF_BUF_SIZE];//��ŵ��ʷ���//��󳤶��ǣ���������������������
int cur = 0;//token��λ��ָ��

/*��������*/
void openfile(ifstream &fin);/*���ļ�*/
void fill_buffer(ifstream &fin, char* hulf_buf);/*�����������*/
char get_char(ifstream &fin);
char get_not_blank_char(ifstream &fin);
void cat(char c);
bool is_digit(char c);
bool is_letter(char c);
bool is_blank(char c);
int is_keyword();//�ж��Ƿ��ǹؼ��֣��Ƿ���key���±꣬���Ƿ���-1
void retreat();//��ǰָ���˻�һ���ַ�
void table_insert(int type, int idx = -1);//��ʶ����ļǺ���ӵ��Ǻ������Լ����У�ͬʱ����Ǻ�����//����idx���ڱ��е��±�
void error_report(ErrorCode code);
void analysis(ifstream &fin);

