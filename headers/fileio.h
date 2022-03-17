#if !defined(FILEIO)
#define FILEIO

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <netdb.h>
#include <unistd.h>
#include <sys/stat.h>


#define NUM_BLOCKS 512
#define BLOCK_SIZE 1024

extern unsigned char file_data[NUM_BLOCKS][BLOCK_SIZE];
extern int copy_size;
extern int block_index;


#endif // FILEIO
