#ifndef COMM_H__
#define COMM_H__

#include <stdio.h>   /* Standard input/output definitions */
#include <stdlib.h>
#include <string.h>  /* String function definitions */
#include <unistd.h>  /* UNIX standard function definitions */
#include <fcntl.h>   /* File control definitions */
#include <errno.h>   /* Error number definitions */
#include <termios.h> /* POSIX terminal control definitions */
#include <pthread.h>

//------------------------ Command codes ------------------------
#define	AT		((unsigned char *)"AT") // used for checking connection
#define	ATE0		((unsigned char *)"ATE0") // no echo
#define MK_CALL		((unsigned char *)"ATD") // make a call
#define REDIAL		((unsigned char *)"ATDL")
#define END_CALL	((unsigned char *)"ATH") // end call/ hang upat

//------------------------ AT extensions --------------------------
#define SD_SMS		((unsigned char *)"+CMGS") // send text
#define RD_SMS		((unsigned char *)"+CMGR") // read text
#define SET_FM		((unsigned char *)"+CMGF") // set format for text
#define ICCID		((unsigned char *)"+CCID")
#define SERV_CHK	((unsigned char *)"+COPS?")
#define GEN_ERROR	((unsigned char *)"+COP7")
#define CALL_STAT	((unsigned char *)"+CLCC")
#define SH_ERROR	((unsigned char *)"+CMEE=2")
#define TONE_DECT	((unsigned char *)"+DDET=1")
#define MIC_GAIN	((unsigned char *)"+CMIC=0,3")
#define GPS_CHK		((unsigned char *)"+CGPSSTATUS?")
#define GPS_SET 	((unsigned char *)"+CGPSPWR=1")
#define GPS_GET 	((unsigned char *)"+CGPSINF=0")

//------------------------ Fona responses --------------------------
#define OK			((unsigned char *)"OK") // fona response if operation successful 
#define NO_ANS		((unsigned char *)"NO ANSWER") 
#define NO_DT		((unsigned char *)"NO DIALTONE")
#define BUSY		((unsigned char *)"BUSY")
#define NO_ANS		((unsigned char *)"NO ANSWER")
#define KEY_IN		((unsigned char *)"+DTMF: 1")

#define	MAX			100
#define BUAD_RATE 	B115200

#ifndef CRTSCTS
#ifdef CNEW_RTSCTS
#define CRTSCTS CNEW_RTSCTS
#else
#define CRTSCTS 0x80000000
#endif /* CNEW_RTSCTS */
#endif /* !CRTSCTS */

int init_connection(void);
FILE* init_log( void);
int send_cmd( char* cmd, char* ext, int fd, char** buffer, char** tmp_buffer );
int make_call( char* num, int fd, char** buffer, char** tmp_buffer);
int check_reply( char* wreply, char** tmp_buffer );

#endif
