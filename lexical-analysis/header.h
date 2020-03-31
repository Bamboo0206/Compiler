#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<vector>
#include<string>
#include<fstream>
#include<cstring>
using namespace std;

/*常量定义*/
#define HALF_BUF_SIZE 1024	//单侧缓存区长度
#define buf_left 0			//左侧缓存区
#define buf_right 1			//右侧缓存区
#define KEY_NUM 37			//关键字数量
#define OP_NUM 39			//操作符数量
#define LIM_NUM 11			//界符数量

#define IDENTIFIER 1		//普通标识符
#define KEYWORD 2			//关键字
#define INTEGER 3			//整数
#define FLOATDIG 4			//浮点数
#define OPERATOR 5			//操作符号
#define DELIMITER 6			//界符

enum ErrorCode { FLOAT_ERROR, EXP_ERROR, INVALID_NUMBER, INVALID_CHARACTER, INVALID_IDENTIFIER, TYPE_NOT_EXIST_ERROR, OTHER_ERROR};


/*关键字*/
const char* key[KEY_NUM] =
{
	"auto","break","case","char","const","continue","default","do","double",
	"else","enum","extern","float","for","goto","if","inline","int","long",
	"register","restrict","return","short","signed","sizeof","static","struct",
	"switch","typedef","union","unsigned","void","volatile","while","_Bool",
	"_Complex","_Imaginary"
};

/*界符*/
const char delimiter[LIM_NUM]=
{
	'[', ']', '(', ')', '{', '}', ';', ',', '\"', '\'', '\\'
};

/*操作符*/
const char* op[OP_NUM] =
{
	".", ":", "? ", "~", "+ ", "++", "+= ", "- ", "--", 
	"-= ", "->", "*", "*= ", "/ ", "/= ", "%", "%= ", "^", 
	"^= ", "&", "&&", "&= ", "<", "<= ", "<< ", "<<= ", ">", 
	">= ", ">> ", ">>= ", "!", "!= ", "= ", "== ", "| ", "|| ", 
	"|= ", "#", "##"
};

//类定义
class Token
{
public:
	Token();
	~Token();

	int type;			//记号
	int index;			//属性
	int line;			//所在行数
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

/*全局变量定义*/
vector<long long>int_table;		//整数表
vector<double>float_table;		//浮点数表
vector<string>identifier_table;	//标识符表
vector<Token>token_table;		//记号序列

int line = 1, word_num = 0, letter_num = 0;//行号，单词个数统计，字符个数统计
char buf[HALF_BUF_SIZE * 2] = { '\0' };//双缓冲区
char *beginPtr, *forwardPtr;//开始指针，前向指针
char token[HALF_BUF_SIZE];//存放单词符号
int cur = 0;//token的位置指针

/*函数声明*/
void openfile(ifstream &fin);/*打开文件*/
void fill_buffer(ifstream &fin, char* hulf_buf);/*填充半个缓冲区*/
char get_char(ifstream &fin);//读取一个字符
char get_not_blank_char(ifstream &fin);//读取一个非空字符
void cat(char c);//每次调用把当前C中的字符与token中的字符串连接起来。（追加字符）
bool is_digit(char c);//判断C中的字符是否为数字，若是则返回true，否则返回false。
bool is_letter(char c);//判断C中的字符是否为字母，若是则返回true，否则返回false。
bool is_blank(char c);//判断C中的字符是否为空字符，若是则返回true，否则返回false。
int is_keyword();//判断是否是关键字，是返回key的下标，不是返回-1
void retreat();//向前指针退回一个字符
void table_insert(int type, int idx = -1);//将识别出的记号添加到记号序列以及表中，同时输出记号序列//参数idx是在表中的下标
void error_report(ErrorCode code);//错误处理过程
void analysis(ifstream &fin);//词法分析主程序

