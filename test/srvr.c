#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <assert.h>

#define NUM_BLOCKS 512
#define BLOCK_SIZE 1024
#define PILE_ATT 50


unsigned char file_data[NUM_BLOCKS][BLOCK_SIZE];
int copy_size;
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

int main(int argc, char **argv) {
    char *msgRecv, *msgSend;
    msgRecv = (char*)calloc(BLOCK_SIZE, sizeof(char));
    msgSend = (char*)calloc(BLOCK_SIZE, sizeof(char));
    struct sockaddr_in addrSrv;
    struct sockaddr_in addrClnt;
    addrSrv.sin_port = htons(8080);
    addrSrv.sin_family = AF_INET;
    memset(&addrSrv.sin_zero, '0', 8);
    inet_aton("10.1.9.111", &addrSrv.sin_addr);
    int dsc, lgAddrClt;
    int dss = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (dss == -1) {
        perror("Error in creation of socket!");
        exit(EXIT_FAILURE);
    }
    if (bind(dss, (struct sockaddr*)&addrSrv, sizeof(struct sockaddr)) == -1) {
        perror("Error in bind");
        exit(EXIT_FAILURE);
    }
    if (listen(dss, PILE_ATT) == -1) {
        perror("Error in listen!");
        exit(EXIT_FAILURE);
    }
    while (1)
    {
        dsc = accept(dss, (struct sockaddr*)&addrClnt, (socklen_t*)&lgAddrClt);
        printf("a client is connected\n");
        recv(dsc, msgRecv, BLOCK_SIZE, 0);
        read_from_file(msgRecv);
        for (int i = 0; i < block_index; i++) {
            send(dsc, file_data[i], strlen(file_data[i]), 0);
            //printf("sending data... block number: %d\n", i+1);
        }
        close(dsc);
    }
    close(dss);
    return 0;
}