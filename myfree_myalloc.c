#include<stdio.h>
#include "myalloc.h"
struct node freee[10000];
struct node *head=NULL;
void sort_by_base(struct node *b)
{
	struct node *head1=head;
	if((b>head)&&(b<head->next))
	{
		b->next=head->next;
		head->next=b;
		return;
	}
	head1=head->next;
	while(head1!=head)
	{	
		if((head1<b)&&(b<head1->next))
		{
			b->next=head1->next;
			head1->next=b;
			return;
		}
		if((head1>b)&&(head1->next==head))
		{
			head=b;
			head1->next=b;
			b->next=head;
			return;
		}
		head1=head1->next;
	}
}
void free_node()
{
	struct node*temp1=head;
	while(temp1->next!=head)
	{
		if((temp1+temp1->size)==(temp1->next))
		{
			temp1->size=temp1->size+((temp1->next)->size);
			temp1->next=(temp1->next)->next;
		}
		else
		{
			temp1=temp1->next;
		}
	}
}
void myfree(void *a)
{
	struct node* temp=(struct node*)a-1;
	sort_by_base(temp);
	free_node(temp);
}
void *myalloc(unsigned n)
{
	struct node* temp;
	struct node*prev;
	unsigned nunits;
	nunits=((n+sizeof(struct node)-1)/sizeof(struct node))+1;
	if((prev=head)==NULL)
	{
		freee[0].next=head=prev=&freee[0];
		freee[0].size=10000;
	}
	temp=prev->next;
	while(1)
	{
		if(temp->size>=nunits)
		{
			if(temp->size==nunits)
			{
				prev->next=temp->next;
			}
			else
			{
				temp->size-=nunits;
				temp+=temp->size;
				temp->size=nunits;
			}
			head=prev;
			return(void*)(temp+1);
		}
		if(temp==head)
		{
			return NULL;
		}
		prev=temp;
		temp=temp->next;
	}
}



