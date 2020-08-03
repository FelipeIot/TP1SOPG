
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdint.h>

#define FIFO_NAME "myfifo"
#define BUFFER_SIZE 300
#define DESTINODATOS 	"Log.txt"
#define DESTINOSIGNAL 	"Sign.txt"
#define NCARACTERESFILTRO 4

int main(void)
{
	uint8_t inputBuffer[BUFFER_SIZE];
	int32_t bytesRead,bytesWrote, returnCode, fd,ftext,fl;


 	//uint8_t selector[NCARACTERESFILTRO];
	//uint8_t aux[NCARACTERESFILTRO];   

    	/* Create named fifo. -1 means already exists so no action if already exists */
    	if ( (returnCode = mknod(FIFO_NAME, S_IFIFO | 0777, 0) ) < -1  )
    	{
        	printf("Error creating named fifo: %d\n", returnCode);
        	exit(1);
    	}
    
    	/* Open named fifo. Blocks until other process opens it */
	printf("waiting for writers...\n");
	if ( (fd = open(FIFO_NAME, O_RDONLY) ) < 0 )
    	{
        	printf("Error opening named fifo file: %d\n", fd);
        	exit(1);
    	}
	/*Aqui creo si no encuentro  el archivo donde voy a escribir los datos recibidos*/
	if((fl = open(DESTINOSIGNAL,  O_CREAT | O_RDWR | O_APPEND, S_IRUSR | S_IWUSR))<0)
	{
		printf("ERROR CREACION Sign.txt");
		exit(0);
	}

        if((ftext = open(DESTINODATOS, O_CREAT | O_RDWR | O_APPEND, S_IRUSR | S_IWUSR))<0)
        {
                printf("ERROR CREACION Log.txt");
                exit(0);
        }


    	/* open syscalls returned without error -> other process attached to named fifo */
	printf("got a writer\n");

    	/* Loop until read syscall returns a value <= 0 */
	do
	{
        /* read data into local buffer */
		if ((bytesRead = read(fd, inputBuffer, BUFFER_SIZE)) == -1)
        	{
			perror("read");
        	}
        	else
		{
			inputBuffer[bytesRead] = '\0';
	
			if (strstr((char *)inputBuffer,"DATA") != NULL)
			{
				
				if((bytesWrote=write(ftext, (char *)inputBuffer, strlen((char *)inputBuffer)))==-1)
				{
					perror("write");
				}
				if((bytesWrote=write(ftext, "\r\n", strlen("\r\n")))==-1)
				{
					perror("write");
				}
			
				printf("reader: read %d bytes: \"%s\"\n", bytesRead, inputBuffer);
			}
			else if ( strstr((char *)inputBuffer, "SIGN") != NULL )
			{
                                
                          	
				if((bytesWrote=write(fl, (char *)inputBuffer, strlen((char *)inputBuffer)))==-1)
				{
					perror("write");
				}				
				if((bytesWrote=write(fl, "\r\n", strlen("\r\n")))==-1)
				{
					perror("write");
				}	
				printf("reader: read %d bytes: \"%s\"\n", bytesRead, inputBuffer);
	
			}
			else
			{	
				printf("caso raro \n\r");
			}
			
		}
	}
	while (bytesRead > 0);
	//close name fifo
	close(fd);
	//close files
	close(fl);
	close(ftext);

	return 0;
}
