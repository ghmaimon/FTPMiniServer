#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <stdio.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <assert.h>

#define NUM_BLOCKS 512
#define BLOCK_SIZE 1024

unsigned char file_data[NUM_BLOCKS][BLOCK_SIZE];
int copy_size=0;

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

int main(int argc, char** argv) {
	struct sockaddr_in addr;
    inet_aton("127.0.0.1", &(addr.sin_addr));
	addr.sin_port = htons(8080);
	addr.sin_family = AF_INET;
	memset(&(addr.sin_zero), '0', 8);

	int ds = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (ds == -1) {
		printf("Erreur lors de l'ouverture du socket!\n");
		exit(EXIT_FAILURE);
	} else {
        if (connect(ds, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
            printf("Erreur lors de la connection!\n");
            exit (EXIT_FAILURE);
        }
        printf("what is the file to send?   ");
		char *msgSend = calloc(1024, sizeof(char));
        scanf("%s", msgSend);
		char *msgRecv = calloc(1024, sizeof(char));
		if (send(ds, msgSend, strlen(msgSend), 0) == -1) {
            printf("Erreur lors de la transmission du message");
            exit(EXIT_FAILURE);
        }
        copy_size=0;
    int n = recv(ds, file_data[copy_size], BLOCK_SIZE, 0);
    printf("%d\n", n);
    int last;
		while (n > 0){
            last=n;
            printf("receiving data... block number: %d\n", copy_size+1);
            copy_size++;
            n = recv(ds, file_data[copy_size], BLOCK_SIZE, 0);
        }
        write_to_file("something", last);
        close(ds);
	}
	return 0;
}