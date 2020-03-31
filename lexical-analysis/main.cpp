#include"header.h"



int main(int argc, char* argv[])
{
	ifstream fin;
	openfile(fin);
	analysis(fin);

	fin.close();
	system("pause");
	return 0;
}


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
			cur = 0;
			memset(token, 0, sizeof(token));
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
			else//界符和操作符
			{
				/*界符*/
				switch (c)
				{
				case '[':state = 0; table_insert(DELIMITER, 0); break;
				case ']':state = 0; table_insert(DELIMITER, 1); break;
				case '(':state = 0; table_insert(DELIMITER, 2); break;
				case ')':state = 0; table_insert(DELIMITER, 3); break;
				case '{':state = 0; table_insert(DELIMITER, 4); break;
				case '}':state = 0; table_insert(DELIMITER, 5); break;
				case ';':state = 0; table_insert(DELIMITER, 6); break;
				case ',':state = 0; table_insert(DELIMITER, 7); break;
				case '\"':state = 0; table_insert(DELIMITER, 8); break;
				case '\'':state = 0; table_insert(DELIMITER, 9); break;
				case '\\':state = 0; table_insert(DELIMITER, 10); break;

				/*操作符*/

				case '.':state = 0; table_insert(OPERATOR,0 ); break;
				case ':':state = 0; table_insert(OPERATOR,1 ); break;
				case '?':state = 0; table_insert(OPERATOR,2 ); break;
				case '~':state = 0; table_insert(OPERATOR,3 ); break;

				case '+':state = 8; break;
				case '-':state = 9; break;
				case '*':state = 10; break;
				case '/':state = 11; break;
				case '%':state = 12; break;
				case '^':state = 13; break;
				case '&':state = 14; break;
				case '<':state = 15; break;
				case '>':state = 16; break;
				case '!':state = 17; break;
				case '=':state = 18; break;
				case '|':state = 19; break;
				case '#':state = 20; break;
				default:state = 23; break;
				}
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
				state = 0;
				retreat();//向前指针后退
				token[cur] = '\0';
				int key_idx = is_keyword();
				if (key_idx >= 0)//是关键字
				{
					table_insert(KEYWORD, key_idx); //加入符号表,并打印记号序列
				}
				else//普通标识符
				{
					table_insert(IDENTIFIER); //加入符号表,并打印记号序列
				}
			}
			break;


		/*case 2-7 识别常数*/
		case 2://常数状态
			cat(c);
			c = get_char(fin);
			if (is_digit(c))
			{
				state = 2;
			}
			else if(c=='.')
			{
				state = 3;
			}
			else if (c == 'E' || c == 'e')//进入指数状态
			{
				state = 5;
			}
			else if (is_letter(c)||c=='_')//错误处理
			{
				//跳过输入错误的标识符
				while (is_letter(c) || is_digit(c) || c == '_')
					c = get_char(fin);

				retreat();
				state = 0;
				error_report(INVALID_NUMBER);

			}
			else//识别出整数
			{
				retreat();
				table_insert(INTEGER);
				state = 0;
			}
			break;

		case 3://小数点状态
			cat(c);
			c = get_char(fin);
			if (is_digit(c))
			{
				state = 4;
			}
			else//错误处理
			{
				while (!is_blank(c))//跳过出错位置
				{
					c = get_char(fin);
				}
				state = 0;
				error_report(FLOAT_ERROR);
			}
			break;

		case 4:////小数状态
			cat(c);
			c = get_char(fin);
			if (is_digit(c))
			{
				state = 4;
			}
			else if (c == 'E' || c == 'e')//进入指数状态
			{
				state = 5;
			}
			else if (is_letter(c) || c == '_')//错误处理
			{
				//跳过输入错误的标识符
				while (is_letter(c) || is_digit(c) || c == '_')
					c = get_char(fin);

				retreat();
				state = 0;
				error_report(FLOAT_ERROR);

			}
			else//识别出小数
			{
				retreat();
				table_insert(FLOATDIG);
				state = 0;
			}
			break;

		case 5://指数状态
			cat(c);
			c = get_char(fin);
			if (is_digit(c))
			{
				state = 7;
			}
			else if (c == '+' || c == '-')//进入指数状态
			{
				state = 6;
			}
			else//错误处理
			{
				while (!is_blank(c))//跳过出错位置
				{
					c = get_char(fin);
				}
				state = 0;
				error_report(EXP_ERROR);
			}
			break;

		case 6://指数的符号后的第一个数字状态
			cat(c);
			c = get_char(fin);
			if (is_digit(c))
			{
				state = 7;
			}
			else//错误处理
			{
				while (!is_blank(c))//跳过出错位置
				{
					c = get_char(fin);
				}
				state = 0;
				error_report(EXP_ERROR);
			}
			break;

		case 7://指数的数字部分(除了第一个字符）
			cat(c);
			c = get_char(fin);
			if (is_digit(c))
			{
				state = 7;
			}
			else if (is_letter(c) || c == '_')//错误处理
			{
				//跳过输入错误的标识符
				while (is_letter(c) || is_digit(c) || c == '_')
					c = get_char(fin);

				retreat();
				state = 0;
				error_report(EXP_ERROR);

			}
			else//识别出小数
			{
				retreat();
				table_insert(FLOATDIG);
				state = 0;
			}
			break;
			
		
		/***************  case 8 - 22 识别操作符  *****************/
		case 8://	+状态
			c = get_char(fin);
			if (c == '=')
			{
				state = 0;
				//识别结果：+=
				table_insert(OPERATOR, 6);
			}
			else if (c == '+')
			{
				state = 0;
				//识别结果：++
				table_insert(OPERATOR, 5);
			}
			else
			{
				retreat();
				state = 0;
				//识别结果：+
				table_insert(OPERATOR, 4);
			}
			break;

		case 9://	-状态
			c = get_char(fin);
			if (c == '=')
			{
				state = 0;
				//识别结果：-=
				table_insert(OPERATOR, 9);
			}
			else if (c == '-')
			{
				state = 0;
				//识别结果：--
				table_insert(OPERATOR, 8);
			}
			else if (c == '>')
			{
				state = 0;
				//识别结果：->
				table_insert(OPERATOR, 10);
			}
			else
			{
				retreat();
				state = 0;
				//识别结果：-
				table_insert(OPERATOR, 7);
			}
			break;

		case 10://	*状态
			c = get_char(fin);
			if (c == '=')
			{
				state = 0;
				//识别结果：*=
				table_insert(OPERATOR, 12);
			}
			else
			{
				retreat();
				state = 0;
				//识别结果：*
				table_insert(OPERATOR, 11);
			}
			break;
		case 11://	/状态
			c = get_char(fin);
			if (c == '=')
			{
				state = 0;
				//识别结果：/=
				table_insert(OPERATOR, 14);
			}

			/*跳过注释*/
			else if (c == '/')
				//注释状态
			{
				while (c != '\n')
					c = get_char(fin);
				state = 0;
			}
			else if (c == '*')
				//注释状态
			{
				c = get_char(fin);
				while (true)
				{
					while (c != '*')
						c = get_char(fin);
					c = get_char(fin);
					if (c == '/')
						break;
				}
				state = 0;
			}
			else
			{
				retreat();
				state = 0;
				//识别结果：/
				table_insert(OPERATOR, 13);
			}
			break;

		case 12://	%状态
			c = get_char(fin);
			if (c == '=')
			{
				state = 0;
				//识别结果：%=
				table_insert(OPERATOR, 16);
			}
			else
			{
				retreat();
				state = 0;
				//识别结果：%
				table_insert(OPERATOR, 15);
			}
			break;
		case 13://	^状态
			c = get_char(fin);
			if (c == '=')
			{
				state = 0;
				//识别结果：^=
				table_insert(OPERATOR, 18);
			}
			else
			{
				retreat();
				state = 0;
				//识别结果：^
				table_insert(OPERATOR, 17);
			}
			break;
		case 14://	&状态
			c = get_char(fin);
			if (c == '&')
			{
				state = 0;
				//识别结果：&&
				table_insert(OPERATOR, 20);
			}
			else if (c == '=')
			{
				state = 0;
				//识别结果：&=
				table_insert(OPERATOR, 21);
			}
			else
			{
				retreat();
				state = 0;
				//识别结果：&
				table_insert(OPERATOR, 19);
			}
			break;

		case 15://	<状态
			c = get_char(fin);
			if (c == '=')
			{
				state = 0;
				//识别结果：<=
				table_insert(OPERATOR, 23);
			}
			else if (c == '<')
			{
				state = 21;
				//识别结果：<<
			}
			else
			{
				retreat();
				state = 0;
				//识别结果：<
				table_insert(OPERATOR, 22);
			}
			break;
		case 16://	>状态
			c = get_char(fin);
			if (c == '=')
			{
				state = 0;
				//识别结果：>=
				table_insert(OPERATOR, 27);
			}
			else if (c == '>')
			{
				state = 22;
				//识别结果：>>
			}
			else
			{
				retreat();
				state = 0;
				//识别结果：>
				table_insert(OPERATOR, 26);
			}
			break;
		case 17://	!状态
			c = get_char(fin);
			if (c == '=')
			{
				state = 0;
				//识别结果：!=
				table_insert(OPERATOR, 31);
			}
			else
			{
				retreat();
				state = 0;
				//识别结果：!
				table_insert(OPERATOR, 30);
			}
			break;
		case 18://	=状态
			c = get_char(fin);
			if (c == '=')
			{
				state = 0;
				//识别结果：==
				table_insert(OPERATOR, 33);
			}
			else
			{
				retreat();
				state = 0;
				//识别结果：=
				table_insert(OPERATOR, 32);
			}
			break;
		case 19://	|状态
			c = get_char(fin);
			if (c == '|')
			{
				state = 0;
				//识别结果：||
				table_insert(OPERATOR, 35);
			}
			else if (c == '=')
			{
				state = 0;
				//识别结果：|=
				table_insert(OPERATOR,36 );
			}
			else
			{
				retreat();
				state = 0;
				//识别结果：|
				table_insert(OPERATOR, 34);
			}
			break;
		case 20: //#
			c = get_char(fin);
			if (c == '#')
			{
				state = 0;
				//识别结果：##
				table_insert(OPERATOR, 38);
			}
			else
			{
				retreat();
				state = 0;
				//识别结果：#
				table_insert(OPERATOR, 37);
			}
			break;

		case 21: // <<
			c = get_char(fin);
			if (c == '=')
			{
			state = 0;
			//识别结果：<<=
			table_insert(OPERATOR, 25);
			}
			else
			{
				retreat();
				state = 0;
				//识别结果：<<
				table_insert(OPERATOR, 24);
			}
			break;

		case 22: // >>
			c = get_char(fin);
			if (c == '=')
			{
				state = 0;
				//识别结果：>>=
				table_insert(OPERATOR, 29);
			}
			else
			{
				retreat();
				state = 0;
				//识别结果：>>
				table_insert(OPERATOR, 28);
			}
			break;
		case 23://	错误处理：识别到非法字符
			error_report(INVALID_CHARACTER);
			state = 0;
			break;

		default:
			if (c == EOF) { finish = true; break; }
			error_report(OTHER_ERROR);
			state = 0;
			break;

		}
	}

	if (c == EOF || finish == true)//词法分析完成
	{
		cout << "-------------------------------------------------------" << endl
			<< "词法分析完成" << endl
			<< "行数:\t\t" << line-1 << endl
			<< "字符总数:\t" << letter_num << endl
			<< "标识符:\t\t" << identifier_table.size() << endl
			<< "整数:\t\t" << int_table.size() << endl
			<< "浮点数:\t\t" << float_table.size() << endl;
	}





}


/*打开文件*/
void openfile(ifstream &fin)
{
	cout << "请输入文件路径" << endl;
	string path;
	cin >> path;
	//path = "test4.c";
	//if (argc == 2)
	//{

	fin.open(path, ios::in);
	if (!fin)
	{
		cerr << "File Open Error" << endl;
		exit(0);
	}

	//}
	//else
	//{
	//	cout << "请输入一个要分析的程序" << endl;
	//	exit(0);
	//}
}


/*填充半个缓冲区*/
void fill_buffer(ifstream &fin, char* hulf_buf)
{
	char temp[HALF_BUF_SIZE] = { '\0' };
	fin.read(temp, HALF_BUF_SIZE - 1);
	int len = strlen(temp);//实际读入字符数
	letter_num += len;
	strcpy(hulf_buf, temp);
	hulf_buf[len] = EOF;//在末尾加eof
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
	while (is_blank(c))
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
bool is_blank(char c)
{
	return c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\0';
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
	if (*forwardPtr == '\n')
	{
		line--;//防止换行符被算两次加到line里
	}
}

//将识别出的记号添加到记号序列以及表中，同时输出记号序列
void table_insert(int type, int idx)//参数idx是在表中的下标
{
	Token temp;
	double floatnum;
	string str(token);


	
	temp.line = line;
	temp.type = type;
	

	switch (type)
	{
	case IDENTIFIER://普通标识符
		temp.index = identifier_table.size();
		identifier_table.push_back(token);//普通标识符加入标识符表
		cout << "<\t" << str << "\t\t，普通标识符"<< "\t>" << endl;
		break;
	case KEYWORD://关键字
		temp.index = idx;
		cout << "<\t" << str << "\t\t，关键字" << "\t>" << endl;
		//cout << "<\t关键字，\t" << str << "\t\t" << line << "\t>" << endl;
		break;
	case INTEGER://整数
		temp.index = int_table.size();
		int_table.push_back(atoi(token));
		cout << "<\t" << str << "\t\t，整数" << "\t\t>" << endl;
		//cout << "<\t整数，\t\t" << str << "\t\t" << line << "\t>" << endl;
		break;
	case FLOATDIG://浮点数
		temp.index = float_table.size();
		float_table.push_back(atof(token));
		cout << "<\t" << str << "\t\t，浮点数" << "\t>" << endl;
		//cout << "<\t浮点数，\t" << str << "\t\t" << line << "\t>" << endl;
		break;
	case OPERATOR://操作符
		temp.index = idx;
		cout << "<\t" << op[idx] << "\t\t，操作符" << "\t>" << endl;
		//cout << "<\t操作符，\t" << op[idx] << "\t\t" << line << "\t>" << endl;
		break;
	case DELIMITER://界符
		temp.index = idx;
		cout << "<\t" << delimiter[idx] << "\t\t，界符" << "\t\t>" << endl;
		//cout << "<\t界符，\t\t" << delimiter[idx] << "\t\t" << line << "\t>" << endl;
		break;
	default:
		error_report(TYPE_NOT_EXIST_ERROR);
		break;
	}

	token_table.push_back(temp);//加入记号序列
}

void error_report(ErrorCode code)
{
	switch (code)
	{
	case FLOAT_ERROR:
		cout << "错误：\t行号：" << line << "\t说明：浮点数的小数点后的部分不符合词法" << endl;
		break;
	case EXP_ERROR:
		cout << "错误：\t行号：" << line << "\t说明：指数部分不符合词法" << endl;
		break;
	case INVALID_CHARACTER:
		cout << "错误：\t行号：" << line << "\t说明：出现C语言不支持的字符" << endl;
		break;
	case INVALID_IDENTIFIER:
		cout << "错误：\t行号：" << line << "\t说明：出现以数字开头的标识符，标识符应以字母或下划线开头" << endl;
		break;
	case INVALID_NUMBER:
		cout << "错误：\t行号：" << line << "\t说明：常数中出现不合法字符" << endl;
		break;
	case TYPE_NOT_EXIST_ERROR:
		cout << "错误：\t行号：" << line << "\t说明：出现C语言不存在的记号" << endl;
		break;
	case OTHER_ERROR:
	default:
		cout << "错误：\t行号：" << line << "\t说明：出现其他错误" << endl;
		break;
	}
}