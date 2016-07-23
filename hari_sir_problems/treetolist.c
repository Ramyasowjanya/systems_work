/*#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<conio.h>
#include<stdlib.h>
struct node
{
	struct node *left;
	struct node *right;
	int data;
};
struct node* insertIntoTree(struct node *root, int data)
{
	struct node *newnode;
	if (data != -1)
	{
		if (root == NULL)
		{
			newnode = (struct node*)malloc(sizeof(struct node));
			newnode->data = data;
			newnode->right = NULL;
			newnode->left = NULL;
			root = newnode;
		}
		else if (root->data > data)
		{
			root->left = insertIntoTree(root->left, data);
		}
		else if (root->data < data)
		{
			root->right = insertIntoTree(root->right, data);
		}
	}
	return root;
}

void displayTree(struct node *head, struct node **prev)
{
	if (head->left != NULL)
		displayTree(head->left, prev);
	head->left=*prev;
	if (*prev != NULL)
		(*prev)->right = head;
	*prev = head;
	if (head->right != NULL)
		displayTree(head->right, prev);
}
void main()
{
	struct node *head = NULL, *temp,*temp1;
	int input[] = { 25,13,12,17,15,36,29 },index=0;
	//head = (struct node*)malloc(sizeof(struct node));
	//printf("Enter input to terminate enter -1");
	while (index<7)
	{
		//scanf("%d",&input);
		head = insertIntoTree(head, input[index++]);
	}
	temp = head;
	temp1 = NULL;
	struct node *temp2 = NULL;
	displayTree(temp, &temp2);
	temp = head;
	while (temp->left != NULL)
	{
		temp = temp->left;
	}
	head = temp;
	while (temp != NULL)
	{
		printf("%d ",temp->data);
		temp = temp->right;
	}
	_getch();
}*/