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

#define MESGUSR1 "SIGN:1 "
#define MESGUSR2 "SIGN:2 "

char outputBuffer[BUFFER_SIZE];
uint32_t bytesWrote;
int32_t fd;


void user1(int sig)
{
		
	
	if ((bytesWrote = write(fd, MESGUSR1, strlen(MESGUSR1)-1)) == -1)
        {
		perror("write");
		
        }
        else
        {
		printf("writer: wrote %d bytes\n", bytesWrote);
		
        }
	

}

void user2(int sig)
{
       
       
	if ((bytesWrote = write(fd, MESGUSR2, strlen(MESGUSR2)-1)) == -1)
        {
		perror("write");
		
        }
        else
        {
		printf("writer: wrote %d bytes\n", bytesWrote);
		
        }
	
}




int main(void)
{
	//Configuración de receptores de señales del sistema
	struct sigaction sa;
	
	sa.sa_handler = user1;
	sa.sa_flags = 0; // SA_RESTART; //
	sigemptyset(&sa.sa_mask);

	sigaction(SIGUSR1,&sa,NULL);

        struct sigaction se;
        se.sa_handler = user2;
        se.sa_flags = 0; // SA_RESTART; //
        sigemptyset(&se.sa_mask);

        sigaction(SIGUSR2,&se,NULL);      


   
    	char outputBufferAux[BUFFER_SIZE];	
	
	int32_t returnCode;
	

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
		if(NULL!=(fgets(outputBufferAux, BUFFER_SIZE, stdin)))
        	{
			sprintf(outputBuffer,"DATA:%s",outputBufferAux);
		
			
			/* Write buffer to named fifo. Strlen - 1 to avoid sending \n char */
			if ((bytesWrote = write(fd, outputBuffer, strlen(outputBuffer)-1)) == -1)
			{
				perror("write");
			}
			else
			{
				printf("writer: wrote %d bytes\n", bytesWrote);
			}
		}
	}
	//close named fifo	
	close(fd);
	return 0;
}
