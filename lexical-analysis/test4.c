#include<stdio.h>
#include<stdlib.h>
struct chain{
	int number;
	struct chain *next;
}; 
struct chain *AddList(struct chain *head,int num);
void PrintList(struct chain *current);
void FreeList(struct chain *head);
main()
{
	struct chain *headA=NULL,*headB=NULL,*currentA,*currentB,*preA,*preB,*tempPtr;
	int temp;
	while(scanf("%d",&temp)&&temp!=-1)
	headA=AddList(headA,temp);
	while(scanf("%d",&temp)&&temp!=-1)
	headB=AddList(headB,temp);
	currentB=headB;
	while(currentB!=NULL)
	{
		currentA=headA;
		while(currentA!=NULL&&currentA->number<=currentB->number)
		{
			preA=currentA;
			currentA=currentA->next;
		}
		if(preA->number<currentB->number)//已找到插入位置 
		{
			tempPtr=currentB;//拿出节点 
			if(currentB==headB) //从B删除
			{
				headB=headB->next;
				currentB=headB;
			}
			else
			{
				preB->next=currentB->next;
				currentB=currentB->next;
			}
			tempPtr->next=currentA; //把节点插入A 
			preA->next=tempPtr;
		}
		else
		{
			preB=currentB;
			currentB=currentB->next; 
		}
	}
	printf("A is : ");
	PrintList(headA);
	printf("\nB is : ");
	PrintList(headB);
	FreeList(headA);
	FreeList(headB);
	return 0;
}
struct chain *AddList(struct chain *head,int num)
{
	struct chain *current,*pre;
	for(current=head;current!=NULL;pre=current,current=current->next);
	current=(struct chain*)malloc(sizeof(struct chain));
	if(head==NULL)
	head=current;
	else
	pre->next=current; 
	current->number=num;
	current->next=NULL;
	return head;//非常关键 
 } 
void PrintList(struct chain *current)
{
	while(current!=NULL)
	{
		printf("%d ",current->number);
		current=current->next; 
	}
 } 
void FreeList(struct chain *head)
{
	struct chain *current;
	while(head!=NULL)
	{
		current=head;
		free(current);
		head=head->next;
	}
}
