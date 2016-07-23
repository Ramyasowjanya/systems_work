#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<conio.h>
#include<stdlib.h>
struct node
{
	struct node *left;
	struct node *right;
	int data;
};
struct node* insertIntoTree(struct node *root,int data)
{
	struct node *newnode = (struct node*)malloc(sizeof(struct node));
	if (data != -1)
	{
		if (root == NULL)
		{
			newnode->data = data;
			newnode->right = NULL;
			newnode->left = NULL;
			root = newnode;
		}
		else if (root->data > data)
		{
			root->left=insertIntoTree(root->left,data);
		}
		else if (root->data < data)
		{
			root->right = insertIntoTree(root->right, data);
		}
	}
	return root;
}
void displayTree(struct node *head)
{
	if (head->left != NULL)
		displayTree(head->left);
	printf("%d ",head->data);
	if (head->right != NULL)
		displayTree(head->right); 
}
void findCommonAncestor(struct node *temp,int *ancestor,int in1,int in2,int *found,int *an1,int *an2,int level)
{
	if (*found != 2)
	{
		if (temp->left != NULL)
		{
			findCommonAncestor(temp->left, ancestor, in1, in2, found, an1, an2, level + 1);
		}
		if (temp->right != NULL)
		{
			findCommonAncestor(temp->right, ancestor, in1, in2, found, an1, an2, level + 1);
		}
		if (*found == 2)
		{
			if ((*an1) < (*an2) && (*an1) == level)
				*ancestor = temp->data;
			else if (*an1 >= (*an2) && (*an2) == level)
				*ancestor = temp->data;
			return;
		}
		else if (*found == 1)
		{
			if (*an1 ==level)
				(*an1)--;
			else if(*an2==level)
				(*an2)--;
		}
		if (temp->data == in1)
		{
			*an1 = level - 1;
			(*found)++;
		}
		if (temp->data == in2)
		{
			*an2 = level - 1;
			(*found)++;
		}
	}
}
void main()
{
	struct node *head = NULL, *temp;
	int ancestor=0;
	int in1, in2, found = 0, set = 1, index = 0;
	int input[] = {25,13,12,17,15,36,29,40,38,37};
	//printf("Enter input to terminate enter -1");
	while (index<10)
	{
		//scanf("%d",&input);
		head=insertIntoTree(head,input[index++]);
	}
	temp = head;
	displayTree(temp);
	temp = head; scanf("%d %d", &in1,&in2);
	int an1 = 0, an2 = 0,level=0;
	findCommonAncestor(temp,&ancestor,in1,in2,&found,&an1,&an2,level);
	printf("%d",ancestor);
	_getch();
}
