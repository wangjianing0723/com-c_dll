#ifndef _LOG_FUNC_H_
#define _LOG_FUNC_H_

#pragma warning( disable: 4996 )
#pragma warning( disable: 4267 )

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <memory.h>
#include <time.h>
#include <stdarg.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/timeb.h>
#if defined(WIN32)
	#include <Windows.h>
	#include <io.h>
	#include <share.h>
	#include <sys/locking.h>

	#define F_OK 0
	#define popen _popen
	#define pclose _pclose
	#define ftruncate _chsize
#else
	#include <unistd.h>
	#include <sys/utsname.h>
	#ifdef __AIX__
		#include <procinfo.h>
	#endif
	#ifdef __NCR__
		#include <sys/procfs.h>
	#endif
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define LOGPATH "C:\\SuresecLog"
#define LOGFILE "C:\\SuresecLog\\SuresecControl.log"

#define DSS_DEBUG	0
#define DSS_INFO	1
#define DSS_WARN	2
#define DSS_ERROR	3
#define DSS_FATAL	4

#define MAX_LOG_FILE_LEN    64*1024*1024
//#define LOG_NAME_PREFIX		"SureLog"
#define BACKUP_OFFSET_DAY	-2
#define FIELD_DELIMIT 	'|'


typedef struct
{
	char timeStamp[20+1];
	char msglevel[2+1];
	char msgCode[10+1];
	char hostName[20+1];
	char appName[20+1];
	char pgmName[32+1];
	char usrName[10+1];
	char message[128+1];
	char reserved[128];
#if defined(WIN32)
	char rcdDelimit[2];
#else
	char rcdDelimit[1];
#endif
}st_logRcd_t;


int f_backUpJzjkLog(char * logfile );

int f_wrtJzjkLog(char * level,char * msgcode, char * appname, 
					char * msg, char * logfile);

int f_jzjkLog(char * level,char * msgcode, char * appname, 
					char * msg, char * logfile);

int DSS_Log(int level,int msgcode,char *msg);

#ifdef __cplusplus
}
#endif

#endif
