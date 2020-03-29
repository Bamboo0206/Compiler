#include"header.h"

int main(int argc, char* argv[])
{
	ifstream fin;
	openfile(fin);


	fin.close();
	return 0;
}

/*���ļ�*/
void openfile(ifstream &fin)
{
	cout << "�������ļ�·��" << endl;
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
	//	cout << "������һ��Ҫ�����ĳ���" << endl;
	//	exit(0);
	//}
}

int line = 0, word_num = 0, letter_num = 0;//�кţ����ʸ���ͳ�ƣ��ַ�����ͳ��
char buf[HALF_BUF_SIZE * 2] = { '\0' };//˫������
char *beginPtr, *forwardPtr;//��ʼָ�룬ǰ��ָ��
char token[HALF_BUF_SIZE];//��ŵ��ʷ���//��󳤶��ǣ���������������������
int cur = 0;//token��λ��ָ��


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
			else//����������
			{

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
				retreat();//��ǰָ�����
				token[cur] = '\0';
				if (is_keyword() >= 0)//�ǹؼ���
				{
					add//������ű�
				}
				else//��ͨ��ʶ��
				{

				}
			}




		default:
			break;
		}
	}






}

/*�����������*/
void fill_buffer(ifstream &fin, char* hulf_buf)
{
	char temp[HALF_BUF_SIZE] = { '\0' };
	fin.read(temp, HALF_BUF_SIZE - 1);
	int len = strlen(temp);//ʵ�ʶ����ַ���
	letter_num += len;
	strcpy(hulf_buf, temp);
	hulf_buf[len] = EOF;//��ĩβ��eof//����������������������������������
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
}