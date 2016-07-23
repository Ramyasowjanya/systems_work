/*#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<conio.h>
#include<stdlib.h>
struct stack
{
	int data;
	struct stack *next;
};
struct stack* push(struct stack* top,int num)
{
	struct stack *temp=(struct stack*)malloc(sizeof(struct stack));
	temp->data=num;
	temp->next=top;
	top=temp;
	return top;
}
int pop(struct stack **top)
{
	if (top != NULL)
	{
		int no = (*top)->data;
		struct stack *temp = top;
		(*top) = (*top)->next;
		return no;
	}
	return -1;
}
struct stack* enqueue(struct stack *top,int num)
{
	return push(top,num);
}
int dequeue(struct stack *first)
{
	int no=0,ret;
	struct stack *second=NULL;
	while(first!=NULL&&no!=-1){
	no=pop(&first);
	second=push(second,no);	
	}
	ret=pop(&second);
	no = 0;
	while (second!=NULL&&no!=-1) {
		no = pop(&second);
		first = push(first, no);
	}
	return ret;
}
void main()
{
	int len,*arr,input;
	struct stack *root = NULL;
	printf("Enter length");
	scanf("%d",&len);
	while (len > 0)
	{
		scanf("%d",&input);
		root=enqueue(root,input);
		--len;
	}
	printf("%d ",dequeue(root));
	_getch();
}*/