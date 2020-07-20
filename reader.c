
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

int main(void)
{
	uint8_t inputBuffer[BUFFER_SIZE];
	int32_t bytesRead, returnCode, fd;
	FILE * ftext;
 	uint8_t selector[5];   
    	/* Create named fifo. -1 means already exists so no action if already exists */
    	if ( (returnCode = mknod(FIFO_NAME, S_IFIFO | 0666, 0) ) < -1  )
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
	ftext = fopen("Log.txt", "a");
	/*if(ftext)
	{
		printf("El archivo existe \n");
	}
	else
	{
		ftext = fopen("test.txt", "w");//creo el archivo si no existe
	}*/
    
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
			strncpy(selector,inputBuffer,4);
			selector[4]='\0';
			if(selector[0] == 'D')
			{
				printf("reader: read %d bytes: \"%s\"\n", bytesRead, inputBuffer);
				fprintf (ftext, "%s \n",inputBuffer);
				printf("%s \n\r",inputBuffer);
			}
			else
			{	
				printf("caso raro \n\r");
			}
			
		}
	}
	while (bytesRead > 0);

	return 0;
}
