#include"header.h"

int main(int argc, char* argv[])
{
	ifstream fin;
	openfile(fin);


	fin.close();
	return 0;
}

/*打开文件*/
void openfile(ifstream &fin)
{
	cout << "请输入文件路径" << endl;
	string path;
	cin >> path;
	//if (argc == 2)
	//{

	fin.open(path, ios::in);
	if (!fin)
		cerr << "File Open Error" << endl;

	//}
	//else
	//{
	//	cout << "请输入一个要分析的程序" << endl;
	//	exit(0);
	//}
}

int line = 0, word_num = 0, letter_num = 0;//行号，单词个数统计，字符个数统计
char buf[HALF_BUF_SIZE * 2] = { '\0' };//双缓冲区
char *beginPtr, *forwardPtr;//开始指针，前向指针
char token[HALF_BUF_SIZE];//存放单词符号//最大长度是？？？？？？？？？？？
int cur = 0;//token的位置指针


/*分析*/
void analysis(ifstream &fin)
{
	/*初始化缓冲区*/
	buf[HALF_BUF_SIZE - 1] = EOF;//左半缓冲区末尾设置为EOF
	buf[HALF_BUF_SIZE * 2 - 1] = EOF;//右半缓冲区末尾设置为EOF
	beginPtr = buf;//开始指针指向缓冲区起始
	forwardPtr = buf;//向前指针指向缓冲区起始

	fill_buffer(fin, buf);//填充左半缓冲区

	int state = 0;
	char c;
	bool finish = false;

	while (!finish)
	{
		switch (state)
		{
		case 0://初始状态
			c = get_not_blank_char(fin);
			if (is_letter(c) || c == '_')//标识符
			{
				state = 1;
			}
			else if (is_digit(c))//数字
			{
				state = 2;
			}
			else if (c == EOF)
			{
				finish = true;
				break;
			}
			else//界符和运算符
			{

			}
			break;

		case 1://标识符
			cat(c);
			c = get_char(fin);
			if (is_letter(c)||is_digit(c) || c == '_')//标识符
			{
				state = 1;
			}
			else //已到达标识符结尾
			{
				retreat();//向前指针后退
				token[cur] = '\0';
				if (is_keyword() >= 0)//是关键字
				{
					add//加入符号表
				}
				else//普通标识符
				{

				}
			}




		default:
			break;
		}
	}






}

/*填充半个缓冲区*/
void fill_buffer(ifstream &fin, char* hulf_buf)
{
	char temp[HALF_BUF_SIZE] = { '\0' };
	fin.read(temp, HALF_BUF_SIZE - 1);
	int len = strlen(temp);//实际读入字符数
	letter_num += len;
	strcpy(hulf_buf, temp);
	hulf_buf[len] = EOF;//在末尾加eof//？？？？？？？？？？？？？？？？？
}

char get_char(ifstream &fin)
{
	char c = *forwardPtr;
	if (c == EOF)//向前指针指向EOF
	{
		if (forwardPtr == buf + HALF_BUF_SIZE - 1)//左半区终点
		{
			fill_buffer(fin, buf + HALF_BUF_SIZE);//填充右半缓冲区
			forwardPtr++;
		}
		else if (forwardPtr == buf + HALF_BUF_SIZE * 2 - 1)//右半区终点
		{
			fill_buffer(fin, buf);//填充左半缓冲区
			forwardPtr = buf;//向前指针移到开始
		}
		else
		{
			return c;//已读完所有输入，结束分析，返回EOF
		}
		c = get_char(fin);//被缓冲区边界截断，不能返回EOF，需要获取下一个字符
	}
	else //在缓冲区内部
		forwardPtr++;

	if (c == '\n')
		line++;
	return c;
}

char get_not_blank_char(ifstream &fin)
{
	char c = get_char(fin);
	while (c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\0')
	{
		c = get_char(fin);
	}
	return c;
}

void cat(char c)
{
	token[cur++] = c;
}
bool is_digit(char c)
{
	return (c >= '0' && c <= '9');
}
bool is_letter(char c)
{
	return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

int is_keyword()//判断是否是关键字，是返回key的下标，不是返回-1
{
	for (int i = 0; i < KEY_NUM; i++)
	{
		if (strcmp(token, key[i]) == 0)
		{
			return i;
		}
	}
	return -1;
}

void retreat()//向前指针退回一个字符
{
	forwardPtr--;
	if (forwardPtr < buf)
	{
		cerr << "error in retreat";//？？？？？？？？？？？？？？？？？？？？？？？

		forwardPtr = buf + HALF_BUF_SIZE * 2 - 1;
	}
	else if (*forwardPtr == EOF)//理论上会碰到eof还会越界啊？？？
	{
		cerr << "error in retreat";//？？？？？？？？？？？？？？？？？？？？？？？
		forwardPtr--;//右半区退到左半区
		
	}
}