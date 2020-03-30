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
#define PUNC_NUM 54			//标点数量
#define OP_NUM 39			//操作符数量
#define LIM_NUM 11			//界符数量
//#define othernum 12			//其他符号数量

#define IDENTIFIER 1		//普通标识符
#define KEYWORD 2			//关键字
#define INTEGER 3			//整数
#define FLOATDIG 4			//浮点数
#define OPERATOR 5			//操作符号
#define DELIMITER 6			//界符
//#define OTHER 7				//其他符号

enum ErrorCode { FLOAT_ERROR, EXP_ERROR, INVALID_NUMBER, INVALID_CHARACTER, Other_wrong, INVALID_IDENTIFIER, TYPE_NOT_EXIST_ERROR
};


//关键字
const char* key[KEY_NUM] =
{
	"auto","break","case","char","const","continue","default","do","double",
	"else","enum","extern","float","for","goto","if","inline","int","long",
	"register","restrict","return","short","signed","sizeof","static","struct",
	"switch","typedef","union","unsigned","void","volatile","while","_Bool",
	"_Complex","_Imaginary"
};

/*//标点符号
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
vector<string>identifier_table;			//标识符表
vector<Token>token_table;	//记号序列

int line = 1, word_num = 0, letter_num = 0;//行号，单词个数统计，字符个数统计
char buf[HALF_BUF_SIZE * 2] = { '\0' };//双缓冲区
char *beginPtr, *forwardPtr;//开始指针，前向指针
char token[HALF_BUF_SIZE];//存放单词符号//最大长度是？？？？？？？？？？？
int cur = 0;//token的位置指针

/*函数声明*/
void openfile(ifstream &fin);/*打开文件*/
void fill_buffer(ifstream &fin, char* hulf_buf);/*填充半个缓冲区*/
char get_char(ifstream &fin);
char get_not_blank_char(ifstream &fin);
void cat(char c);
bool is_digit(char c);
bool is_letter(char c);
bool is_blank(char c);
int is_keyword();//判断是否是关键字，是返回key的下标，不是返回-1
void retreat();//向前指针退回一个字符
void table_insert(int type, int idx = -1);//将识别出的记号添加到记号序列以及表中，同时输出记号序列//参数idx是在表中的下标
void error_report(ErrorCode code);
void analysis(ifstream &fin);

