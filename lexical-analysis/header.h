#pragma once
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
/*#define optnum 16			//����������
#define endnum 13			//�������
#define othernum 12			//������������*/

#define IDENTIFIER 1		//��ͨ��ʶ��
#define KEYWORD 2			//�ؼ���
#define INTDIG 3			//����
#define FLOATDIG 4			//������
#define OPERATOR 5			//��������
#define END 6				//���
#define OTHER 7				//��������





//�ؼ���
const char* key[KEY_NUM] =
{
	"auto","break","case","char","const","continue","default","do","double",
	"else","enum","extern","float","for","goto","if","inline","int","long",
	"register","restrict","return","short","signed","sizeof","static","struct",
	"switch","typedef","union","unsigned","void","volatile","while","_Bool",
	"_Complex","_Imaginary"
};

//������
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
