#include<stdio.h>
void main()
{
	char a[100][100];//测试注释
	int i = 0;
	float x = 0.001;
	x = 123abc * i;
	while(scanf("%s",a[i])!=EOF)
	{
		/*测试
		注释*/
		if(getchar()=='\n')
		{
			for(;i>=0;i--)
				printf("%s ",a[i]);
			putchar('\n');
		}
		i++;
		
	}
	return 0;
}
