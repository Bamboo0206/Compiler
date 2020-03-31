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


/*����*/
void analysis(ifstream &fin)
{
	/*��ʼ��������*/
	buf[HALF_BUF_SIZE - 1] = EOF;//��뻺����ĩβ����ΪEOF
	buf[HALF_BUF_SIZE * 2 - 1] = EOF;//�Ұ뻺����ĩβ����ΪEOF
	beginPtr = buf;//��ʼָ��ָ�򻺳�����ʼ
	forwardPtr = buf;//��ǰָ��ָ�򻺳�����ʼ

	fill_buffer(fin, buf);//�����뻺����

	int state = 0;
	char c;
	bool finish = false;

	while (!finish)
	{
		switch (state)
		{
		case 0://��ʼ״̬
			cur = 0;
			memset(token, 0, sizeof(token));
			c = get_not_blank_char(fin);
			if (is_letter(c) || c == '_')//��ʶ��
			{
				state = 1;
			}
			else if (is_digit(c))//����
			{
				state = 2;
			}
			else if (c == EOF)
			{
				finish = true;
				break;
			}
			else//����Ͳ�����
			{
				/*���*/
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

				/*������*/

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

		case 1://��ʶ��
			cat(c);
			c = get_char(fin);
			if (is_letter(c)||is_digit(c) || c == '_')//��ʶ��
			{
				state = 1;
			}
			else //�ѵ����ʶ����β
			{
				state = 0;
				retreat();//��ǰָ�����
				token[cur] = '\0';
				int key_idx = is_keyword();
				if (key_idx >= 0)//�ǹؼ���
				{
					table_insert(KEYWORD, key_idx); //������ű�,����ӡ�Ǻ�����
				}
				else//��ͨ��ʶ��
				{
					table_insert(IDENTIFIER); //������ű�,����ӡ�Ǻ�����
				}
			}
			break;


		/*case 2-7 ʶ����*/
		case 2://����״̬
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
			else if (c == 'E' || c == 'e')//����ָ��״̬
			{
				state = 5;
			}
			else if (is_letter(c)||c=='_')//������
			{
				//�����������ı�ʶ��
				while (is_letter(c) || is_digit(c) || c == '_')
					c = get_char(fin);

				retreat();
				state = 0;
				error_report(INVALID_NUMBER);

			}
			else//ʶ�������
			{
				retreat();
				table_insert(INTEGER);
				state = 0;
			}
			break;

		case 3://С����״̬
			cat(c);
			c = get_char(fin);
			if (is_digit(c))
			{
				state = 4;
			}
			else//������
			{
				while (!is_blank(c))//��������λ��
				{
					c = get_char(fin);
				}
				state = 0;
				error_report(FLOAT_ERROR);
			}
			break;

		case 4:////С��״̬
			cat(c);
			c = get_char(fin);
			if (is_digit(c))
			{
				state = 4;
			}
			else if (c == 'E' || c == 'e')//����ָ��״̬
			{
				state = 5;
			}
			else if (is_letter(c) || c == '_')//������
			{
				//�����������ı�ʶ��
				while (is_letter(c) || is_digit(c) || c == '_')
					c = get_char(fin);

				retreat();
				state = 0;
				error_report(FLOAT_ERROR);

			}
			else//ʶ���С��
			{
				retreat();
				table_insert(FLOATDIG);
				state = 0;
			}
			break;

		case 5://ָ��״̬
			cat(c);
			c = get_char(fin);
			if (is_digit(c))
			{
				state = 7;
			}
			else if (c == '+' || c == '-')//����ָ��״̬
			{
				state = 6;
			}
			else//������
			{
				while (!is_blank(c))//��������λ��
				{
					c = get_char(fin);
				}
				state = 0;
				error_report(EXP_ERROR);
			}
			break;

		case 6://ָ���ķ��ź�ĵ�һ������״̬
			cat(c);
			c = get_char(fin);
			if (is_digit(c))
			{
				state = 7;
			}
			else//������
			{
				while (!is_blank(c))//��������λ��
				{
					c = get_char(fin);
				}
				state = 0;
				error_report(EXP_ERROR);
			}
			break;

		case 7://ָ�������ֲ���(���˵�һ���ַ���
			cat(c);
			c = get_char(fin);
			if (is_digit(c))
			{
				state = 7;
			}
			else if (is_letter(c) || c == '_')//������
			{
				//�����������ı�ʶ��
				while (is_letter(c) || is_digit(c) || c == '_')
					c = get_char(fin);

				retreat();
				state = 0;
				error_report(EXP_ERROR);

			}
			else//ʶ���С��
			{
				retreat();
				table_insert(FLOATDIG);
				state = 0;
			}
			break;
			
		
		/***************  case 8 - 22 ʶ�������  *****************/
		case 8://	+״̬
			c = get_char(fin);
			if (c == '=')
			{
				state = 0;
				//ʶ������+=
				table_insert(OPERATOR, 6);
			}
			else if (c == '+')
			{
				state = 0;
				//ʶ������++
				table_insert(OPERATOR, 5);
			}
			else
			{
				retreat();
				state = 0;
				//ʶ������+
				table_insert(OPERATOR, 4);
			}
			break;

		case 9://	-״̬
			c = get_char(fin);
			if (c == '=')
			{
				state = 0;
				//ʶ������-=
				table_insert(OPERATOR, 9);
			}
			else if (c == '-')
			{
				state = 0;
				//ʶ������--
				table_insert(OPERATOR, 8);
			}
			else if (c == '>')
			{
				state = 0;
				//ʶ������->
				table_insert(OPERATOR, 10);
			}
			else
			{
				retreat();
				state = 0;
				//ʶ������-
				table_insert(OPERATOR, 7);
			}
			break;

		case 10://	*״̬
			c = get_char(fin);
			if (c == '=')
			{
				state = 0;
				//ʶ������*=
				table_insert(OPERATOR, 12);
			}
			else
			{
				retreat();
				state = 0;
				//ʶ������*
				table_insert(OPERATOR, 11);
			}
			break;
		case 11://	/״̬
			c = get_char(fin);
			if (c == '=')
			{
				state = 0;
				//ʶ������/=
				table_insert(OPERATOR, 14);
			}

			/*����ע��*/
			else if (c == '/')
				//ע��״̬
			{
				while (c != '\n')
					c = get_char(fin);
				state = 0;
			}
			else if (c == '*')
				//ע��״̬
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
				//ʶ������/
				table_insert(OPERATOR, 13);
			}
			break;

		case 12://	%״̬
			c = get_char(fin);
			if (c == '=')
			{
				state = 0;
				//ʶ������%=
				table_insert(OPERATOR, 16);
			}
			else
			{
				retreat();
				state = 0;
				//ʶ������%
				table_insert(OPERATOR, 15);
			}
			break;
		case 13://	^״̬
			c = get_char(fin);
			if (c == '=')
			{
				state = 0;
				//ʶ������^=
				table_insert(OPERATOR, 18);
			}
			else
			{
				retreat();
				state = 0;
				//ʶ������^
				table_insert(OPERATOR, 17);
			}
			break;
		case 14://	&״̬
			c = get_char(fin);
			if (c == '&')
			{
				state = 0;
				//ʶ������&&
				table_insert(OPERATOR, 20);
			}
			else if (c == '=')
			{
				state = 0;
				//ʶ������&=
				table_insert(OPERATOR, 21);
			}
			else
			{
				retreat();
				state = 0;
				//ʶ������&
				table_insert(OPERATOR, 19);
			}
			break;

		case 15://	<״̬
			c = get_char(fin);
			if (c == '=')
			{
				state = 0;
				//ʶ������<=
				table_insert(OPERATOR, 23);
			}
			else if (c == '<')
			{
				state = 21;
				//ʶ������<<
			}
			else
			{
				retreat();
				state = 0;
				//ʶ������<
				table_insert(OPERATOR, 22);
			}
			break;
		case 16://	>״̬
			c = get_char(fin);
			if (c == '=')
			{
				state = 0;
				//ʶ������>=
				table_insert(OPERATOR, 27);
			}
			else if (c == '>')
			{
				state = 22;
				//ʶ������>>
			}
			else
			{
				retreat();
				state = 0;
				//ʶ������>
				table_insert(OPERATOR, 26);
			}
			break;
		case 17://	!״̬
			c = get_char(fin);
			if (c == '=')
			{
				state = 0;
				//ʶ������!=
				table_insert(OPERATOR, 31);
			}
			else
			{
				retreat();
				state = 0;
				//ʶ������!
				table_insert(OPERATOR, 30);
			}
			break;
		case 18://	=״̬
			c = get_char(fin);
			if (c == '=')
			{
				state = 0;
				//ʶ������==
				table_insert(OPERATOR, 33);
			}
			else
			{
				retreat();
				state = 0;
				//ʶ������=
				table_insert(OPERATOR, 32);
			}
			break;
		case 19://	|״̬
			c = get_char(fin);
			if (c == '|')
			{
				state = 0;
				//ʶ������||
				table_insert(OPERATOR, 35);
			}
			else if (c == '=')
			{
				state = 0;
				//ʶ������|=
				table_insert(OPERATOR,36 );
			}
			else
			{
				retreat();
				state = 0;
				//ʶ������|
				table_insert(OPERATOR, 34);
			}
			break;
		case 20: //#
			c = get_char(fin);
			if (c == '#')
			{
				state = 0;
				//ʶ������##
				table_insert(OPERATOR, 38);
			}
			else
			{
				retreat();
				state = 0;
				//ʶ������#
				table_insert(OPERATOR, 37);
			}
			break;

		case 21: // <<
			c = get_char(fin);
			if (c == '=')
			{
			state = 0;
			//ʶ������<<=
			table_insert(OPERATOR, 25);
			}
			else
			{
				retreat();
				state = 0;
				//ʶ������<<
				table_insert(OPERATOR, 24);
			}
			break;

		case 22: // >>
			c = get_char(fin);
			if (c == '=')
			{
				state = 0;
				//ʶ������>>=
				table_insert(OPERATOR, 29);
			}
			else
			{
				retreat();
				state = 0;
				//ʶ������>>
				table_insert(OPERATOR, 28);
			}
			break;
		case 23://	������ʶ�𵽷Ƿ��ַ�
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

	if (c == EOF || finish == true)//�ʷ��������
	{
		cout << "-------------------------------------------------------" << endl
			<< "�ʷ��������" << endl
			<< "����:\t\t" << line-1 << endl
			<< "�ַ�����:\t" << letter_num << endl
			<< "��ʶ��:\t\t" << identifier_table.size() << endl
			<< "����:\t\t" << int_table.size() << endl
			<< "������:\t\t" << float_table.size() << endl;
	}





}


/*���ļ�*/
void openfile(ifstream &fin)
{
	cout << "�������ļ�·��" << endl;
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
	//	cout << "������һ��Ҫ�����ĳ���" << endl;
	//	exit(0);
	//}
}


/*�����������*/
void fill_buffer(ifstream &fin, char* hulf_buf)
{
	char temp[HALF_BUF_SIZE] = { '\0' };
	fin.read(temp, HALF_BUF_SIZE - 1);
	int len = strlen(temp);//ʵ�ʶ����ַ���
	letter_num += len;
	strcpy(hulf_buf, temp);
	hulf_buf[len] = EOF;//��ĩβ��eof
}

char get_char(ifstream &fin)
{
	char c = *forwardPtr;
	if (c == EOF)//��ǰָ��ָ��EOF
	{
		if (forwardPtr == buf + HALF_BUF_SIZE - 1)//������յ�
		{
			fill_buffer(fin, buf + HALF_BUF_SIZE);//����Ұ뻺����
			forwardPtr++;
		}
		else if (forwardPtr == buf + HALF_BUF_SIZE * 2 - 1)//�Ұ����յ�
		{
			fill_buffer(fin, buf);//�����뻺����
			forwardPtr = buf;//��ǰָ���Ƶ���ʼ
		}
		else
		{
			return c;//�Ѷ����������룬��������������EOF
		}
		c = get_char(fin);//���������߽�ضϣ����ܷ���EOF����Ҫ��ȡ��һ���ַ�
	}
	else //�ڻ������ڲ�
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

int is_keyword()//�ж��Ƿ��ǹؼ��֣��Ƿ���key���±꣬���Ƿ���-1
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

void retreat()//��ǰָ���˻�һ���ַ�
{
	forwardPtr--;
	if (forwardPtr < buf)
	{
		cerr << "error in retreat";//����������������������������������������������

		forwardPtr = buf + HALF_BUF_SIZE * 2 - 1;
	}
	else if (*forwardPtr == EOF)//�����ϻ�����eof����Խ�簡������
	{
		cerr << "error in retreat";//����������������������������������������������
		forwardPtr--;//�Ұ����˵������

	}
	if (*forwardPtr == '\n')
	{
		line--;//��ֹ���з��������μӵ�line��
	}
}

//��ʶ����ļǺ���ӵ��Ǻ������Լ����У�ͬʱ����Ǻ�����
void table_insert(int type, int idx)//����idx���ڱ��е��±�
{
	Token temp;
	double floatnum;
	string str(token);


	
	temp.line = line;
	temp.type = type;
	

	switch (type)
	{
	case IDENTIFIER://��ͨ��ʶ��
		temp.index = identifier_table.size();
		identifier_table.push_back(token);//��ͨ��ʶ�������ʶ����
		cout << "<\t" << str << "\t\t����ͨ��ʶ��"<< "\t>" << endl;
		break;
	case KEYWORD://�ؼ���
		temp.index = idx;
		cout << "<\t" << str << "\t\t���ؼ���" << "\t>" << endl;
		//cout << "<\t�ؼ��֣�\t" << str << "\t\t" << line << "\t>" << endl;
		break;
	case INTEGER://����
		temp.index = int_table.size();
		int_table.push_back(atoi(token));
		cout << "<\t" << str << "\t\t������" << "\t\t>" << endl;
		//cout << "<\t������\t\t" << str << "\t\t" << line << "\t>" << endl;
		break;
	case FLOATDIG://������
		temp.index = float_table.size();
		float_table.push_back(atof(token));
		cout << "<\t" << str << "\t\t��������" << "\t>" << endl;
		//cout << "<\t��������\t" << str << "\t\t" << line << "\t>" << endl;
		break;
	case OPERATOR://������
		temp.index = idx;
		cout << "<\t" << op[idx] << "\t\t��������" << "\t>" << endl;
		//cout << "<\t��������\t" << op[idx] << "\t\t" << line << "\t>" << endl;
		break;
	case DELIMITER://���
		temp.index = idx;
		cout << "<\t" << delimiter[idx] << "\t\t�����" << "\t\t>" << endl;
		//cout << "<\t�����\t\t" << delimiter[idx] << "\t\t" << line << "\t>" << endl;
		break;
	default:
		error_report(TYPE_NOT_EXIST_ERROR);
		break;
	}

	token_table.push_back(temp);//����Ǻ�����
}

void error_report(ErrorCode code)
{
	switch (code)
	{
	case FLOAT_ERROR:
		cout << "����\t�кţ�" << line << "\t˵������������С�����Ĳ��ֲ����ϴʷ�" << endl;
		break;
	case EXP_ERROR:
		cout << "����\t�кţ�" << line << "\t˵����ָ�����ֲ����ϴʷ�" << endl;
		break;
	case INVALID_CHARACTER:
		cout << "����\t�кţ�" << line << "\t˵��������C���Բ�֧�ֵ��ַ�" << endl;
		break;
	case INVALID_IDENTIFIER:
		cout << "����\t�кţ�" << line << "\t˵�������������ֿ�ͷ�ı�ʶ������ʶ��Ӧ����ĸ���»��߿�ͷ" << endl;
		break;
	case INVALID_NUMBER:
		cout << "����\t�кţ�" << line << "\t˵���������г��ֲ��Ϸ��ַ�" << endl;
		break;
	case TYPE_NOT_EXIST_ERROR:
		cout << "����\t�кţ�" << line << "\t˵��������C���Բ����ڵļǺ�" << endl;
		break;
	case OTHER_ERROR:
	default:
		cout << "����\t�кţ�" << line << "\t˵����������������" << endl;
		break;
	}
}