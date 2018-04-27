#include "comm.h"
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 8080
int end = 0;

char* buffer;
char* tmp_buffer;

struct Coordinates
{
	char x[100];
	char y[100];
};

struct Coordinates get_gps( int fd )
{
	
	struct Coordinates currCoords;
	
	char gps[500];
	int x = 0, i;
	char str[3] = " :,";
	char* token;	
		
	send_cmd(AT, GPS_CHK ,fd, &buffer, &tmp_buffer);
	x = check_reply( "3D Fix", &tmp_buffer );
	
	if( x == 0){
		while(x == 0)
		{
		
			send_cmd(AT, GPS_CHK ,fd, &buffer, &tmp_buffer);
			x = check_reply( "3D Fix", &tmp_buffer );
		
		}
	}
		
	send_cmd(AT, GPS_GET ,fd, &buffer, &tmp_buffer);
	strcpy(gps, tmp_buffer);
	
	token = strtok( gps, str );
	
	for( i  = 0; i < 4; i++ )
	{
		
		if( i == 2 ){
			strcpy(currCoords.x, token );
			printf("%s\n",token);
		}
		if( i == 3 ){
			strcpy(currCoords.y, token );
			printf("%s\n",token);
		}
		token = strtok(NULL, str);
		
	} 
	
	return currCoords;
}

void play_msg(int fd, int gps)
{
	int i = 0;
	struct Coordinates coord;
	
	system("play AUDIO/MESSAGE.mp3");
	
	if ( gps == 1 )
	{
		usleep(1000);
		system("play AUDIO/COORDINATES.mp3");
		
		coord = get_gps(fd);
		
		system("play AUDIO/LONGITUDE.mp3");
		
		for( i = 0; i < strlen( coord.x ); i++ )
		{	
			switch( coord.x[i] )
			{
				case '0':
					system("play AUDIO/ZERO.mp3");
					break;
				case '1':
					system("play AUDIO/ONE.mp3");
					break;
				case '2':
					system("play AUDIO/TWO.mp3");
					break;
				case '3':
					system("play AUDIO/THREE.mp3");
					break;
				case '4':
					system("play AUDIO/FOUR.mp3");
					break;
				case '5':
					system("play AUDIO/FIVE.mp3");
					break;
				case '6':
					system("play AUDIO/SIX.mp3");
					break;
				case '7':
					system("play AUDIO/SEVEN.mp3");
					break;
				case '8':
					system("play AUDIO/EIGHT.mp3");
					break;
				case '9':
					system("play AUDIO/NINE.mp3");
					break;
				case '.':
					system("play AUDIO/POINT.mp3");
					break;
				default:
					break;
				
			}
		}
		
		system("play AUDIO/LATITUDE.mp3");
		
		for( i = 0; i < strlen( coord.y ); i++ )
		{	
			switch( coord.y[i] )
			{
				case '0':
					system("play AUDIO/ZERO.mp3");
					break;
				case '1':
					system("play AUDIO/ONE.mp3");
					break;
				case '2':
					system("play AUDIO/TWO.mp3");
					break;
				case '3':
					system("play AUDIO/THREE.mp3");
					break;
				case '4':
					system("play AUDIO/FOUR.mp3");
					break;
				case '5':
					system("play AUDIO/FIVE.mp3");
					break;
				case '6':
					system("play AUDIO/SIX.mp3");
					break;
				case '7':
					system("play AUDIO/SEVEN.mp3");
					break;
				case '8':
					system("play AUDIO/EIGHT.mp3");
					break;
				case '9':
					system("play AUDIO/NINE.mp3");
					break;
				
			}
		}
	}
	send_cmd( END_CALL, 0, fd, &buffer, &tmp_buffer);
	
}

void* readfona( void* ptr)
{
	
	int i;
	int fd;
	
	fd = *((int *) ptr);
	
	buffer = (char *) malloc(100);
	tmp_buffer = (char *) malloc(100);
	
	while ( end != 1)
	{	

		//strcpy(tmp_buffer, buffer);
		int read_len = read(fd, buffer, sizeof(buffer));

		for ( i = 0; i < read_len; i++)
		{    
			printf("%c", buffer[i] );
			tmp_buffer[strlen(tmp_buffer)] = buffer[i];
		}	
		
		if(check_reply( KEY_IN, &tmp_buffer ) == 1 )
		{
				printf("recieved key");
				send_cmd( END_CALL, 0, fd, &buffer, &tmp_buffer);
		}
		
	}

	pthread_exit(0);

}

int cmd_menu( int cmd_index, int fd )
{

	int i;
	int chk;
	
	switch (cmd_index)
	{

	case 0:
		end = 1;
		break;
	
	case 1:
		send_cmd(AT, GPS_CHK ,fd, &buffer, &tmp_buffer);
		chk = check_reply( "3D Fix", &tmp_buffer );
		if(chk ==1)
		{
			printf("good \n");
			//get_gps(fd);
			play_msg(fd, 1);
		}
		else	
			printf("no good \n");
		break;

	case 2:
		system("play AUDIO/ZERO.mp3");
		break;

	case 3:
		send_cmd( AT, SERV_CHK, fd, &buffer, &tmp_buffer );
		break;
	
	case 4:
/*
		make_call( "3145469107", fd, &buffer, &tmp_buffer );
		chk = check_for_pickup( fd );
		if(chk ==1)
		{
			printf("good \n");
			play_msg(fd, 0);
		}
		else	
			printf("no good \n");
*/
		mkcall();
		break;

	defualt:
		break;

	}

	return 0;
}


int check_for_pickup( int fd )
{
	
	int calling = 1;
	int answered = 0;
	
	while( calling )
	{
		send_cmd( AT, CALL_STAT, fd, &buffer, &tmp_buffer );
		usleep(1000000);
		
		if(check_reply( "1,0,2,0,0,", &tmp_buffer) == 1)
			printf("DAILING... \n");
				
		else if(check_reply( "1,0,3,0,0,", &tmp_buffer) == 1 )
			printf("RINGING... \n");
		
		else if(check_reply( "1,0,0,0,0,", &tmp_buffer) == 1)
		{
			printf("ANSWERED... \n");
			answered = 1;
			calling = 0;
		}
		
		else if(check_reply( "+CLCC:", &tmp_buffer)  == 0 ) 
			calling = 0;

	}
	
	if( answered == 1 )
		return 1;
		
	else 
		return 0;
}

int mkcall( int fd )
{
	
	FILE* fp;
	fp = fopen("~/work/cars_data/Emergancy_Contacts.txt","r");
	
	int numofcon;
	int num = 0, chk = 0;
	char name[40];
	char number[40];
	char relate[40];

	fscanf(fp, "%d", &numofcon);
	
	while( num < numofcon)
	{
	
		fscanf(fp, "%s", name);
		fscanf(fp, "%s", number);
		fscanf(fp, "%s", relate);
		make_call( number, fd, &buffer, &tmp_buffer );
		
		if( check_for_pickup( fd ) == 1 )
		{
			play_msg(fd,);
			break;
		}

		num++;

	}
}

int main(void)
{
	
	int menu, fd;
	FILE* fp;
	pthread_t thrd1;

	fd = init_connection();
	fp = init_log();
	
	if(fd == -1)
		return 0;
		
	pthread_create( &thrd1, NULL, (void *)&readfona, (void *)&fd );
	
	usleep(100000);
	send_cmd(AT, SH_ERROR ,fd, &buffer, &tmp_buffer);
	usleep(100000);
	send_cmd(AT, TONE_DECT ,fd, &buffer, &tmp_buffer);
	usleep(100000);
	send_cmd(AT, MIC_GAIN ,fd, &buffer, &tmp_buffer);
	usleep(100000);
	send_cmd(AT, GPS_SET ,fd, &buffer, &tmp_buffer);
	usleep(100000);
	
	int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
     
    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {	
		printf("0");
        return 0;
    }
    
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );
      
    // Forcefully attaching socket to the port  8080
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0)
    {
		printf("1");
        return 0;
    }
    if (listen(server_fd, 3) < 0)
    {
		printf("2");
        return 0;
    }
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)
    {
		printf("3");
        return 0;
    }
	printf("CONNECT\n");
	
	while( end != 1)
	{
		valread = read(new_socket, buffer, 1024);
		cmd_menu( atoi(buffer), fd );
	}
	
	usleep(100000);
	fclose(fp);
	close(fd);
	return 0;

}
