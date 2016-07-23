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
struct node* deleteFromTree(struct node *head,int data)
{
	struct node *parent=head,*temp=head,*child=NULL;
	while (temp!=NULL)
	{
		if (temp->data == data)
		{
			if (temp->left == NULL&&temp->right == NULL)
			{
				if(parent->left==temp)
				parent->left = NULL;
				if (parent->right == temp)
				parent->right = NULL;
			}
			else if (temp->left == NULL&&temp->right != NULL)
			{
				if (parent->left == temp)
					parent->left = temp->right;
				if (parent->right == temp)
					parent->right = temp->right;
			}
				else if (temp->left != NULL&&temp->right == NULL)
				{
					if (parent->left == temp)
						parent->left = temp->left;
					if (parent->right == temp)
						parent->right = temp->left;
				}
				else if (temp->left != NULL&&temp->right != NULL)
				{
					child = temp->left;
					while (child->right != NULL)
					{
						parent = child;
						child = child->right;
					}
					parent->right = NULL;
					temp->data = child->data;
					free(child);
				}
				//free(temp);
		}
		else if (temp->data>data)
		{
			parent = temp;
			temp = temp->left;
		}
		else if (temp->data < data)
		{
			parent = temp;
			temp = temp->right;
		}
	}
	return head;
}
void main()
{
	struct node *head=NULL,*temp;
	int input=0;
	printf("Enter input to terminate enter -1");
	while (input!=-1)
	{
		scanf("%d",&input);
		head=insertIntoTree(head,input);
	}
	temp = head;
	displayTree(temp);
	scanf("%d",&input);
	temp = head;
	deleteFromTree(temp,input);
	displayTree(temp);
	_getch();
}*/