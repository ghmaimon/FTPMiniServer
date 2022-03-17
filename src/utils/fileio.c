#include "../../headers/fileio.h"

unsigned char file_data[NUM_BLOCKS][BLOCK_SIZE];
int copy_size=0;
int block_index = 0;


void read_from_file(char* filename) {
  int status;
  struct stat buf;
  status =  stat( filename, &buf );
  printf("%s\n", filename);
  printf("%d\n", status);

  if( status != -1 )
  {
    FILE *ifp = fopen ( filename, "r" ); 
    printf("Reading %d bytes from %s\n", (int) buf . st_size, filename );
    copy_size   = buf . st_size;
    int offset      = 0;               
    block_index = 0;
    printf("%d\n", copy_size);

    while( copy_size > 0 )
    {

      fseek( ifp, offset, SEEK_SET );
      int bytes  = fread( file_data[block_index], BLOCK_SIZE, 1, ifp );

      if( bytes == 0 && !feof( ifp ) )
      {
        printf("An error occured reading from the input file.\n");
        return;
      }

      clearerr( ifp );

      copy_size -= BLOCK_SIZE;
      
      offset    += BLOCK_SIZE;

      block_index ++;
    }

    fclose( ifp );
    }
}

void write_to_file(char *filename, int last){
    FILE *ofp;
    ofp = fopen(filename, "w");
    copy_size = (copy_size - 1)*BLOCK_SIZE + last;
    if( ofp == NULL )
    {
      printf("Could not open output file: %s\n", filename );
      perror("Opening output file returned");
      return;
    }
    int block_index = 0;
    int offset      = 0;

    printf("Writing %d bytes to %s\n", copy_size, filename );

    while( copy_size > 0 )
    { 

      int num_bytes;
      if( copy_size < BLOCK_SIZE )
      {
        num_bytes = copy_size;
      }
      else 
      {
        num_bytes = BLOCK_SIZE;
      }

      fwrite( file_data[block_index], num_bytes, 1, ofp ); 

      copy_size -= BLOCK_SIZE;
      offset    += BLOCK_SIZE;
      block_index ++;

      fseek( ofp, offset, SEEK_SET );
    }

    // Close the output file, we're done. 
    fclose( ofp );
}