#include<stdio.h>
void 123main()
{
	char a[100][100];
	int i=0;
	while(scanf("%s",a[i])!=EOF)
	{
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
