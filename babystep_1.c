#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<fcntl.h>
#include<errno.h>
#include<unistd.h>
#include "myalloc.h"
#define blocksize (sizeof(long)*64)
struct file
{
	char filename[52];
	int size;
	int start;
	int last;
};
int readdiskblock(int fd,int blockno,void *buffer)
{
	int rd;
	long c=blockno*blocksize;
	if(lseek(fd,c,SEEK_SET)==-1)
	{
		return -7;
	}
	rd=read(fd,buffer,blocksize);
	if(rd==-1)
	{
		return -2;
	}
	else
	{
		return rd;
	}
}
int writediskblock(int fd,int blockno,void *buffer)
{
	int wr;
	long c=blockno*blocksize;
	if(lseek(fd,c,SEEK_SET)==-1)
	{
		return -7;
	}
	wr=write(fd,buffer,blocksize);
	if(wr==-1)
	{
		return -3;
	}
	else
	{
		return wr;
	}
}
int storefileontodisk(char *a)
{
	int fd,fd1,rd,wr,start,last;
	char buffer[blocksize];
	struct file *block=(struct file*)myalloc(sizeof(struct file));
	fd1=open(a,O_RDONLY);
	 if(fd1==-1)
        {
                return -1;
        }

	int i=1;
	block->start=i;
	strcpy(block->filename,a);
	fd=open("disk.teasage",O_WRONLY);
	if(fd==-1)
	{
		return -4;
	}
	long dsize,filesize;
	dsize=lseek(fd,0,SEEK_END);
	if(dsize==-1)
	{
		return -7;
	}
	long freeblock=(dsize/512);
	filesize=lseek(fd1,0,SEEK_END);
	if(filesize==-1)
	{
		return -7;
	}
	long fileblock=(filesize/512);
	if(fileblock>freeblock)
	{
		return -11;
	}
	if((lseek(fd1,0,SEEK_SET))==-1)
	{
		return -7;
	}
	rd=read(fd1,buffer,blocksize);
	while(rd!=0)
	{
		wr=writediskblock(fd,i,buffer);
		if(wr<=-1)
		{
			return -10;
		}
		last=rd;
		rd=read(fd1,buffer,blocksize);
		if(rd==-1)
		{
			return -5;
		}
		i++;
	}
	block->size=i-1;
	block->last=last;
	writediskblock(fd,0,block);
	close(fd);
	close(fd1);
       	myfree(block);
	return 1;
}
int getfilefromdisk(char *a)
{
	int fd,fd1,rd,wr,start,last;
	char *buffer=(char*)myalloc(sizeof(blocksize));
	fd=open("disk.teasage",O_RDONLY);
	if(fd==-1)
	{
		return -4;
	}
	struct file *block=(struct file*)myalloc(sizeof(struct file));
	readdiskblock(fd,0,block);
	if(strcmp(block->filename,a)!=0)
	{
		return -8;
	}
	fd1=open(block->filename,O_WRONLY|O_CREAT,00700);
	if(fd1==-1)
	{
		return -4;
	}
	int i=block->start;
	while(i<block->size+block->start-1)
	{
		rd=readdiskblock(fd,i,buffer);
		if(rd==-1)
		{
			return -9;
		}
		wr=write(fd1,buffer,blocksize);
		if(wr==-1)
		{
			return -6;
		}
		i++;
	}
	readdiskblock(fd,i,buffer);
	write(fd1,buffer,block->last);
	close(fd1);
	close(fd);
	myfree(block);
	myfree(buffer);
	return 1;
}
void main()
{
	int ch,k,p;
	char fname[30];
	do
	{
		printf("enter the choice\n");
		printf("1.Storefileontodisk\n2.Getfilefromdisk\n3.Exit\n");
		scanf("%d",&ch);
		switch(ch)
		{
			case 1:
				printf("enter the file name:\t");
				scanf("%s",fname);
				k=storefileontodisk(fname);
				if(k<=-1)
				{
					printf("error...\n");
				}
				break;

			case 2:
				printf("enter the file name:\t");
				scanf("%s",fname);
				p=getfilefromdisk(fname);
				if(p<=-1)
				{
					printf("error...\n");
				}
				break;
			case 3:
				printf("exit...\n");
				break;
			default:
				printf("enter the valid choice\n");
				
		}
	}while(ch!=3);
}


