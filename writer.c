#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdint.h>
#include <signal.h>


#define FIFO_NAME "myfifo"
#define BUFFER_SIZE 300

char outputBuffer[BUFFER_SIZE];
uint32_t bytesWrote;
int32_t fd;
uint8_t permiso;
void pepito(int sig)
{
	permiso=0;	
	sprintf(outputBuffer,"SIGN:1 ");
	

}



int main(void)
{
	struct sigaction sa;
	permiso=1;
	sa.sa_handler = pepito;
	sa.sa_flags = 0; // SA_RESTART; //
	sigemptyset(&sa.sa_mask);

	sigaction(SIGUSR1,&sa,NULL);   


    	//char outputBuffer[BUFFER_SIZE];
    	char outputBufferAux[BUFFER_SIZE];	
	//uint32_t bytesWrote;
	int32_t returnCode;
	//int32_t fd;

    	/* Create named fifo. -1 means already exists so no action if already exists */
    	if ( (returnCode = mknod(FIFO_NAME, S_IFIFO | 0666, 0) ) < -1 )
    	{
        	printf("Error creating named fifo: %d\n", returnCode);
        	exit(1);
    	}

    /* Open named fifo. Blocks until other process opens it */
	printf("waiting for readers...\n");
	if ( (fd = open(FIFO_NAME, O_WRONLY) ) < 0 )
    {
        printf("Error opening named fifo file: %d\n", fd);
        exit(1);
    }
    
    /* open syscalls returned without error -> other process attached to named fifo */
	printf("got a reader--type some stuff\n");

    /* Loop forever */
	while (1)
	{
        /* Get some text from console */
		fgets(outputBufferAux, BUFFER_SIZE, stdin);
        	if(permiso)
		{
			sprintf(outputBuffer,"DATA:%s",outputBufferAux);
		}
		
        /* Write buffer to named fifo. Strlen - 1 to avoid sending \n char */
		if ((bytesWrote = write(fd, outputBuffer, strlen(outputBuffer)-1)) == -1)
        {
			perror("write");
			permiso=1;
        }
        else
        {
			printf("writer: wrote %d bytes\n", bytesWrote);
			permiso=1;
        }
	}
	return 0;
}
