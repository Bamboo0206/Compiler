#pragma once
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
/*#define optnum 16			//操作符数量
#define endnum 13			//界符数量
#define othernum 12			//其他符号数量*/

#define IDENTIFIER 1		//普通标识符
#define KEYWORD 2			//关键字
#define INTDIG 3			//整数
#define FLOATDIG 4			//浮点数
#define OPERATOR 5			//操作符号
#define END 6				//界符
#define OTHER 7				//其他符号





//关键字
const char* key[KEY_NUM] =
{
	"auto","break","case","char","const","continue","default","do","double",
	"else","enum","extern","float","for","goto","if","inline","int","long",
	"register","restrict","return","short","signed","sizeof","static","struct",
	"switch","typedef","union","unsigned","void","volatile","while","_Bool",
	"_Complex","_Imaginary"
};

//标点符号
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
};
