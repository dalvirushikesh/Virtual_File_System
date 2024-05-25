#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#define blocksize (sizeof(long)*64)
void createdisk()
{
	off_t disksize=10485760L-1L;
	int fd=open("disk.teasage",O_WRONLY|O_CREAT,00700);
	if(fd==-1)
	{
		printf("disk open error...\n");
		return;
	}
	if(write(fd,"\0",1)!=1)
	{
		printf("disk write error\n");
		return;
	}
	if(lseek(fd,disksize,SEEK_SET)==-1)
	{
		printf("error..\n");
		return;
	}
	if(write(fd,"\0",1)!=1)
	{
		printf("disk write error\n");
		return;
	}
	printf("disk created successfully\n");
	return;	
}
void main()
{
	createdisk();
}
