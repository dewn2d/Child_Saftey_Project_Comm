#include "comm.h"

/* 
* initialize_connection:
* open the pipeline to fona and initialize the setting of the pipeline
*
* return:
* 0 if successfully connected -1 if not
*/
int init_connection(void)
{
	struct termios pipe_opt;
	int fd = open("/dev/ttyS0", O_RDWR | O_NOCTTY | O_SYNC); // open pipeline to serial port 0;
	
	if ( fd == -1 ){
		printf("No connection established");
		return -1;
	}
 
	memset(&pipe_opt, 0, sizeof pipe_opt);
    if ( tcgetattr(fd, &pipe_opt) !=  0 ) { // get current options for port
                printf("error %d from tcgetattr\n", errno);
                return -1;
        }
	
	cfsetospeed (&pipe_opt, BUAD_RATE); // set baud rate
    cfsetispeed (&pipe_opt, BUAD_RATE);

    pipe_opt.c_cflag = (pipe_opt.c_cflag & ~CSIZE) | CS8;     //reciver recieves 8-bit chars
    // disable IGNBRK for mismatched speed tests; otherwise receive break
    // as \000 chars
    pipe_opt.c_iflag &= ~IGNBRK;         // disable break processing
    pipe_opt.c_lflag = 0;                // no signaling chars, no echo,
                                        // no canonical processing
    pipe_opt.c_oflag = 0;                // no remapping, no delays
    pipe_opt.c_cc[VMIN]  = 0;            // read doesn't block
    pipe_opt.c_cc[VTIME] = 5;            // 0.5 seconds read timeout

    pipe_opt.c_iflag &= ~(IXON | IXOFF | IXANY); // shut off xon/xoff ctrl

    pipe_opt.c_cflag |= (CLOCAL | CREAD);// ignore modem controls,
                                        // enable reading
    pipe_opt.c_cflag &= ~(PARENB | PARODD);      // shut off parity
    pipe_opt.c_cflag |= 0;		// set parity bit
    pipe_opt.c_cflag &= ~CSTOPB;
    pipe_opt.c_cflag &= ~CRTSCTS;

    if ( tcsetattr(fd, TCSANOW, &pipe_opt) != 0 )    {
            printf("error %d from tcsetattr", errno);
            return -1;
    }

    return fd;

}

FILE* init_log(void)
{

	FILE* fp = fopen( "log.txt", "a");
	if (fp == NULL)
		printf("log file not open");
		
	return fp;

}

/*
* send:
* send commands to fona
*
* return:
* 1 if successful 0 if not
*/
int send_cmd( char* cmd, char* ext, int fd, char** buffer, char** tmp_buffer  )
{
	
	char cmd2snd[30];
	memset(*tmp_buffer,0,100);
	
	if( ext == 0){
		strcpy(cmd2snd, cmd);
	}
	else
	{
		strcpy( cmd2snd, cmd );
		strcat( cmd2snd, ext );
	}

	int cmd_len = strlen(cmd2snd);
	
	memset(*buffer, 0, 100);
	
	write( fd, cmd2snd, cmd_len ); // send_cmd cmd
	write( fd, "\r", 1 );

	usleep ( (cmd_len + 10 + 25) * 300 ); // sleep enough to transmit the cmd plus	
	
	return 0;
}

int make_call( char* num, int fd, char** buffer, char** tmp_buffer )
{

	char cmd[30];
	memset(*tmp_buffer,0,100);
	
	strcpy( cmd, MK_CALL );
	strcat( cmd, num );
	strcat( cmd, ";" );
		
	int cmd_len = strlen(cmd);

	write( fd, cmd, cmd_len ); // send_cmd cmd
	write( fd, "\r", 1 );

	usleep ( (cmd_len + 10 + 25) * 300 ); // sleep enough to transmit the cmd plus	
	
	return 0;

}

int check_reply( char* wreply, char** tmp_buffer )
{

	char* reply;
	int i;
	int same = 1;

	/*
 	for ( i = 0; i < strlen(*tmp_buffer); i++)    
		printf("%d ", *tmp_buffer[i] );
	printf("\n");
	
 	for ( i = 0; i < strlen(wreply); i++)    
		printf("%d ", wreply[i] );
	printf("\n");
	*/

	reply = strstr( *tmp_buffer, wreply );
	if( reply == NULL)
		return 0;
	/*
	strtok(reply, "\r \n");
	
	printf("%s\n", reply);
	printf("%s\n", wreply);


	printf("%s", reply);


 	for ( i = 0; i < strlen(reply); i++)    
		printf("%d ", reply[i] );
	printf("reply \n");
 	for ( i = 0; i < strlen(wreply); i++)    
		printf("%d ", wreply[i] );
	printf("wreply \n");
	if( strncmp( wreply, reply, strlen(reply)) == 0 ){
		return 1;
	}
	*/	
	
 	for ( i = 0; i < strlen(wreply); i++) 
 	{  
		if(wreply[i] != reply[i])
		{
			same = 0;
			break;
		}
			
	}
		
	return same;

}
