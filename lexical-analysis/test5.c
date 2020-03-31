#include<windows.h>   //创建线程函数需要操作系统函数
#include<process.h>   //创建线程函数头文件
#include <stdio.h>  
#include <string.h> 
#include <stdlib.h> 
unsigned __stdcall getInput(void* pArguments); 
int main(){
    //HANDLE hThread1;       //线程的访问句柄
    unsigned ThreadID;   
    //线程标识号,不用变量赋值，则只能为NULL
    /*创建一个对应于getInput函数的线程 */
    _beginthreadex(NULL,0, getInput,NULL,0,&ThreadID); 
    if (  hThread1 == 0 )  
        printf("Failed to create thread 1\n"); 
    int i;
    for(i=30;i>=0;i--) 
        printf("main thread is running\n"); //自定义的主函数体
    WaitForSingleObject(ThreadID,INFINITE);//等待线程结束
    CloseHandle(ThreadID);//无须控制线程时删除此句柄，可防止内核对象泄露。
    system("pause");
    return 0;
}
unsigned __stdcall getInput(void* pArguments)
//线程函数的返回值是指定类型，形参也只能有一个
{
	int i;
	for(i=30;i>=0;i--) 
	printf("hThreadl is running\n");//自定义的函数体

     	_endthreadex(0);//线程完成时要调用_endthread
	return 0;
}

