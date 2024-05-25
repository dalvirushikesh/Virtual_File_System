FileSystem
Code is written in C language

This code works on a virtual disk named as "disk.teasage".
Functanalities available in current code are:

    Store a File into Disk
    Retrieve a File from Disk
    List all Files available on Disk
  

About the Code:

Disk has been divided mainly into two parts.
The first part is approximately 12% of the disk size.This part is being used to store the meta data,the structures and the free block bit array.
The structres conatins all the relevant data of the file stored in the disk.
Struct is as follow:

struct File  
{  
	char filename[48];  
	unsigned int start;  
	unsigned int size;  
	unsigned int last;  
	unsigned int next;  
}; 

    filename :stores the name of the file.
    start : stores the starting block number where the files's data is stored.
    size : stores number of continous block from the starting block.
    last : stores either the number of relevant data in the last block of the file or 0 which indicates that this is not the last structure of this file.
    next : stores either the index of the next file after this or the index of the next struct of the same file when the last==0

The first structure is being used to store the Disk information.
Free Bit array has been implemented to know which blocks from the second part are free( -1 indicates the block is free and vice versa).

Main Subroutines:

    Stores File into Virtual Disk:
    vdcpto(char* filepath,char* filename):
    filepath: is file path of file on actual disk which is to be copied on the disk, filename: is the name to be used to stored the file. It returns negative value on error and 1 for sucessfull storing the file.
    Copies File from Virtual Disk into Actual Disk.
    vdcpfrom(char* filepath,char* filename):
    filepath:is the path where the file has to be stored, filename:is the name of the file in virtual disk which is to be stored. It returns negative value on occurence of error and 1 for successful completion of the subroutine.
    filename:is the name of the file to be deleted from the virtual disk.
    It returns negative value on occurence of error and 1 for successful completion of the subroutine.
    Displays File present on Virtual Disk.
    vdls(char* buff):
    buff: is the buffer in which the filename is to be stored. The buffers first 8 bytes must be reserved(First 4bytes indicates the index of the struct to be read and next 4 bytes indicates the size of the buffer).
    It returns the number of files read and negative value at occurence of Error.
