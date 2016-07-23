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
//struct node* insertIntoTree(struct node *root, int data)
//{
//	struct node *newnode = (struct node*)malloc(sizeof(struct node));
//	if (data != -1)
//	{
//		if (root == NULL)
//		{
//			newnode->data = data;
//			newnode->right = NULL;
//			newnode->left = NULL;
//			root = newnode;
//		}
//		else if (root->data > data)
//		{
//			root->left = insertIntoTree(root->left, data);
//		}
//		else if (root->data < data)
//		{
//			root->right = insertIntoTree(root->right, data);
//		}
//	}
//	return root;
//}

void displayTree(struct node *head,int *before)
{
	if (head->left != NULL)
		displayTree(head->left,before);
	if (head->data < *before)
	{
		printf("not valid"); return;
	}
	printf("%d ", head->data);
	*before = head->data;
	if (head->right != NULL)
		displayTree(head->right,before);
}
void main()
{
	struct node *head = NULL, *temp;
	int  ancestor = 0, in1, in2, found = 0, set = 1, index = 0;
	int input[] = { 25,13,12,17,15,36,29 };
	head= (struct node*)malloc(sizeof(struct node));
	head->data = 6;
	head->left = (struct node*)malloc(sizeof(struct node));
	head->right = (struct node*)malloc(sizeof(struct node));
	head->right->data = 1;
	head->left->data = 10;
	head->left->left = NULL;
	head->left->right = NULL;
	head->right->left = NULL; 
	head->right->right = NULL;

	//printf("Enter input to terminate enter -1");
	//while (index<7)
	//{
	//	//scanf("%d",&input);
	//	head = insertIntoTree(head, input[index++]);
	//}

	temp = head;
	index = 0;
	displayTree(temp,&index);
	temp = head; 
//	printf("%d", ancestor);
	_getch();
}*/