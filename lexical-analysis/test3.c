#include<stdio.h>
#include<string.h>
void filtrate(char *sourceFile,char *destFile); 
main()
{
	filtrate("p12in.c","p12out.c");
	return 0;
 } 
void filtrate(char *sourceFile,char *destFile)
{
	char ch,ch2=0;
	int flag=0,quote=0;//flag=0则复制，1为//注释内，2为/*注释内。quote 1为单引号内，2为双引号内，0不在引号内 
	FILE *sourcefPtr,*destfPtr;
	if((sourcefPtr=fopen(sourceFile,"r"))==NULL)
	printf("ERROR!");
	else if((destfPtr=fopen(destFile,"w"))==NULL)
	printf("ERROR!");
	else
	{
		while((ch=fgetc(sourcefPtr))!=EOF)
		{
			if(ch=='/'&&flag==0&&quote==0)
			{
				ch2=fgetc(sourcefPtr);
				if(ch2=='\''&&flag==0&&quote==0)
				{
					if(quote==0) quote=1;
					else quote=0;
				}
				else if(ch2=='\"'&&flag==0&&quote==0) 
				{
					if(quote==0) quote=2;
					else quote=0;
				}
				if(ch2=='/'&&flag==0&&quote==0)
				flag=1;
				else if(ch2=='*'&&flag==0&&quote==0)
				flag=2;
			}
			else if(ch=='\n')//到达行末 
			{
				if(flag==1) flag=0; //双斜杠 
				quote=0;//引号只在本行起作用 
			}
			else if(ch=='*'&&quote==0&&flag==2)
			{
				ch2=fgetc(sourcefPtr);
				if(ch2=='/')//是*/  
				{
					flag=0;
					ch=0;
					ch2=0;
				}
			}
			else if(flag==0&&quote!=2&&ch=='\'')
			{
				if(quote==0) quote=1;
				else quote=0;
			}
			else if(flag==0&&quote!=1&&ch=='\"') 
			{
				if(quote==0) quote=2;
				else quote=0;
			}
			if(flag==0)
			{
				if(ch>0) fputc(ch,destfPtr);
				if(ch2>0) fputc(ch2,destfPtr);
			}
			ch2=0;
		}
		fclose(destfPtr);
		fclose(sourcefPtr);
	}
}
