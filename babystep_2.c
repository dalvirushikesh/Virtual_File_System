#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<fcntl.h>
#define blocksize (sizeof(long)*64)
struct file
{
	char filename[52];
	int size;
	int start;
	int last;
};
int countfileblock(int fd)
{
	int count=lseek(fd,0,SEEK_END)/blocksize;
	if(lseek(fd,0,SEEK_END)%blocksize==0)
	{
		return count;
	}
	else
	{
		return count+1;
	}
}
int readdiskblock(int fd,int blockno,void *buffer)
{
	int rd;
	int a=blockno*blocksize;
	if(lseek(fd,a,SEEK_SET)==-1)
		return -1;
	rd=read(fd,buffer,blocksize);
	return rd;
}
int writediskblock(int fd,int blockno,void *buffer)
{
	int wr;
	int a=blockno*blocksize;
	if(lseek(fd,a,SEEK_SET)==-1)
		return -1;
	wr=write(fd,buffer,blocksize);
	return wr;
}

/*int createdisk()
{
	off_t disksize=10485760L-1L;
	int fd=open("disk.teasage",O_RDWR | O_CREAT,00700);
	if(fd==-1)
	{
		return -1;
	}
	if(write(fd,"\0",1)!=1)
	{
		return -1;
	}
	if(lseek(fd,disksize,SEEK_SET)==-1)
	{
		return -1;
	}
	if(write(fd,"\0",1)!=1)
	{
		return -1;
	}
	struct file array[8];
	array[0].start=0;//in the starting no of files
	array[0].size=19480;//no of free blocks
	for(int j=1;j<8;j++)
	{
		array[j].size=-1;
	}
	writediskblock(fd,0,array);
	for(int j=0;j<8;j++)
	{
		array[j].size=-1;
	}
	for(int i=1;i<1000;i++)
	{
		writediskblock(fd,i,array);
	}
	return fd;
}*/
int vdcpto(char* file_path,char* file_name)
{
	int fd;
	fd=open("disk.teasage",O_RDWR);
	if(fd==-1)
	{
		/*fd=createdisk();
		if(fd==-1)*/
			return -1;
	}
	strcat(file_path,"/");
	strcat(file_path,file_name);
	int fd1=open(file_path,O_RDONLY);
	struct file array[8];
	char buffer[blocksize];
	readdiskblock(fd,0,array);
	int n,start,last,rd,j;
	n=array[0].size;
	int i=array[0].start;
	int k=countfileblock(fd1);
	if(lseek(fd1,0,SEEK_SET)==-1)
		return -2;
	if(n<k)
	{
		return -3;
	}

	if(i==0)
	{
		start = 1000;
	}
	else
	{
		readdiskblock(fd,i/8,array);
		start= array[i%8].start+array[i%8].size;
	}
	rd=read(fd1,buffer,blocksize);
	j=start;
	while(rd!=0)
	{
		if(writediskblock(fd,j,buffer)==-1)
		{
			close(fd);
			close(fd1);
			return -4;
		}
		j++;
		last=rd;
		if((rd=read(fd1,buffer,blocksize))==-1)
		{
			close(fd);
			close(fd1);
			return -5;
		}
	}			
	i++;
	readdiskblock(fd,i/8,array);
	strcpy((array[i%8].filename),file_name);
	array[i%8].start=start;
	array[i%8].size=k;
	array[i%8].last=last;
	if(writediskblock(fd,i/8,array)==-1)
		return -6;
	if(readdiskblock(fd,0,array)==-1)
		return -7;
	array[0].start+=1;
	array[0].size-=k;
	writediskblock(fd,0,array);
	return 1;
}
int vdcpfrom(char *filepath,char *filename)
{
	int fd=open("disk.teasage",O_RDWR);
	if(fd==-1)
	{
		return -8;
	}
	struct file array[8];
	if(readdiskblock(fd,0,array)==-1)
	{
		return -9;
	}
	if(array[0].start==0)
	{
		return -10;
	}
	strcat(filepath,"/");
	strcat(filepath,filename);
	readdiskblock(fd,0,array);
	int index=-1,i=0,n=array[0].start,wr;
	while(n>0)
	{
		readdiskblock(fd,i,array);
		for(int j=0;j<8;j++)
		{
			if(array[j].size==-1)
			{
				return -11;
			}
			if(strcmp(array[j].filename,filename)==0)
			{
				index=j;
				break;
			}
		}
		if(index!=-1)
		{
			break;
		}
		i++;
		n--;
	}
	if(n==0)
	{
		return -12;
	}
	int fd1=open(filepath,O_WRONLY|O_CREAT,00700);
	if(fd1==-1)
	{
		return -13;
	}
	char buff[blocksize];
	int k=array[index].start;
	while(k<array[index].size+array[index].start-1)
        {
                if(readdiskblock(fd,k,buff)==-1)
                {
                        close(fd);
                        close(fd1);
                        return -14;
                }
                if(wr=(write(fd1,buff,blocksize))==-1)
                {
                        close(fd1);
                        close(fd);
                        return -15;
                }
                k++;
        }
        readdiskblock(fd,k,buff);
        write(fd1,buff,array[index].last);
        close(fd);
        close(fd1);
	return 1;
}
int vdls(char *buf)
{
        struct file array[8];
        int fd=open("disk.teasage",O_RDONLY);
	if(readdiskblock(fd,0,array)==-1)
		return -1;
	int n= array[0].start;
	int ret=n;
	int i=0;
	int k=0;
	while(n>0)
	{
		readdiskblock(fd,i,array);
		for(int j=0;j<8;j++)
		{
			if(array[j].size==-1)
			{
				return ret;
			}
			if(i==0&& j==0)
				n++;
				continue;
			strcpy((buf+k*52),array[j].filename);
			k++;
		}
		i++;
		n-=8;
	}
	return ret;
}

int main()
{  
	int ch,k,i;
	char buf[2000][52];
	char file_name[50],file_path[50];
	do
	{
		printf("1.vdcpto\n2.vdcpfrom\n3.vdls\n0.exit\n");
		printf("enter the choice:\n");
		scanf("%d",&ch);
		switch(ch)
		{
			case 1:
					printf("\nenter the filepath\t:");
					scanf(" %[^\n]",file_path);
					printf("enter the filename\t");
					scanf(" %[^\n]",file_name);
					k=vdcpto(file_path,file_name);
					if(k==-1)
					{
						printf("%d",k);
						printf("error...\n");
					}

					break;
			case 2:
					printf("enter the filepath\t");
					scanf(" %[^\n]",file_path);
					printf("enter the filename:\t");
					scanf(" %[^\n]",file_name);
					k=vdcpfrom(file_path,file_name);
					if(k==-1)
					{
						printf("%d",k);
						printf("error...\n");
					}
					break;
			case 3:
					printf("showing all the files\n");
					int z=vdls(&buf[0][0]);
					if(z==-1)
					{
						printf("%d",z);
						printf("error...\n");
					}
					else
					{
						for(i=0;i<z;i++)
							printf("%d",z);
							printf("%s\n",buf[i]);
					}
					break;
			case 0:
					printf("you are exiting...\n");
					break;

			default:
					printf("enter the valid choice\n");
		}
	}while(ch!=0);
}

