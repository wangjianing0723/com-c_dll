#include "LogFunc.h"
#include "StdAfx.h"
static	char * basename(char * dir)
{
	char *p = NULL;

	if (dir == NULL)
	{
		return NULL;
	}
	p = strrchr(dir, '\\');
	if (p == NULL )
	{
		p = strrchr(dir, '/');
	}
	if( p != NULL ) 
		return p+1;
	else
		return dir;
}
static	char * dirname(char * dir)
{
	char * p = NULL;
	static char s[256];
		
	memset(s, 0, sizeof(s));
	strcpy(s, dir);
	p = strrchr(s, '\\');
	if (p == NULL )
	{
		p = strrchr(s, '/');
	}
	if( p != NULL )
	{
		s[p-s]='\0';
		return s;
	}

	return  NULL;
}

long f_getFileSize( char * file )
{
	struct 	stat 	sbuf;

	if( access(file, F_OK) == 0 )
	{
	    if(stat(file,&sbuf) == 0)
		{
			return sbuf.st_size;
		}
		else
		{
			printf("stat error:%s\n" , strerror(errno) );
			return -1;
		}
	}
	
	return 0;
}


/*
****************************************************************
*                                                            	*
*  NAME		：	f_GetTime   								    *
*  FUNCTION	：												    *
*		取当前实际日期                                    		*
*  PARAMETER	：												*
*		参数1	[输入/输出]	存储当前日期时间的字符串；			*
*		参数2	[输入]	是否格式化								*
*		参数3	[输入]	日期偏移								*
* 	RETURN VALUE：												*
*      NULL  - 失败                                             *
*      !NULL  - 成功                                            *
*																*
****************************************************************
*/
char * f_GetTime(char* szDate, int fmtflag, int dayOffset)
{
	time_t     ltime;
	struct tm  *tm_string;
	struct timeb t;
	static char szDateTime[30];
	char szTmp[30];
	int        ich;
	
	ftime(&t);
	ltime = t.time;
	
	ltime += dayOffset*24*60*60;

	tm_string = localtime(&ltime);
	memset(szTmp, 0, sizeof(szTmp) );
	memset(szDateTime, 0, sizeof(szDateTime) );
	if(fmtflag ==1){
		ich = strftime(szTmp, 20, "%Y-%m-%d %H:%M:%S", tm_string);
		if (ich < 0)
			return NULL;
		sprintf(szDateTime,"%s:%03d", szTmp, t.millitm);
	}
	if( fmtflag ==0){
		ich = strftime(szTmp, 15, "%Y%m%d%H%M%S", tm_string);
		if (ich < 0)
			return NULL;
		sprintf(szDateTime,"%s%03d", szTmp, t.millitm);
	}
	if( szDate != NULL )
		strcpy(szDate, szDateTime);
	
	return szDateTime;
}

void Sprintf( char * String, const char *format, ...)
{ 
	va_list  args;
	char szTmp[512];
	
	va_start(args, format);
	
	memset( szTmp, 0, sizeof(szTmp) );
	vsprintf( szTmp, format, args );
	memcpy( String, szTmp, strlen(szTmp) );
	
	va_end(args);

	return;
}

#ifndef WIN32
char * f_getPgmName()
{
	static char szPgmName[256];
    int rc=-1;
	pid_t pid=-1;

	memset(szPgmName, 0, sizeof(szPgmName) );
	pid = getpid();

#ifdef __AIX__
	{
	    struct procsinfo ProcessBuffer;
	    struct fdsinfo FileBuffer;
		int Count = 1;
	
	    rc = getprocs( &ProcessBuffer, sizeof(struct procsinfo), &FileBuffer,
	                sizeof(struct fdsinfo), &pid, Count);
		memset(szPgmName, 0, sizeof(szPgmName) );
		strcpy(szPgmName, ProcessBuffer.pi_comm);
		return szPgmName;
	}
#endif
#ifdef __NCR__
	{
		char szName[256];
        prpsinfo_t p;
        int fd =-1;


        sprintf(szName, "/proc/%05d", pid);
        fd = open(szName, O_RDONLY);
        rc = ioctl(fd,  PIOCPSINFO,  &p );
		if( rc != 0 )
		{
			perror("ioctl error: ");
			close(fd);
			return NULL;
		}
        strcpy(szPgmName, p.pr_fname);
        close(fd);

		return szPgmName;
	}
#endif
#ifdef __LINUX__
	{
		char szName[256];
        int fd =-1;


        sprintf(szName, "/proc/%d/cmdline", pid);
        fd = open(szName, O_RDONLY);
        rc = read(fd,  szName,  sizeof(szName) );
		if( rc < 0 )
		{
			perror("read error: ");
			close(fd);
			return NULL;
		}
		strcpy(szPgmName, basename(szName));
        close(fd);

		return szPgmName;

	}
#endif
}
#endif

int f_getBackupFileName(char * logfile, char * szBackupFile)
{
	int i;
	char szDirName[256];
	char szBaseName[256];
	char szTmp[256];
	char szTmp2[256];
	char * p =NULL;

	memset(szDirName, 0, sizeof(szDirName));
	memset(szBaseName, 0, sizeof(szBaseName));
	p = dirname(logfile);
	if( p != NULL ) strcpy(szDirName, p);	
	strcpy(szBaseName, basename(logfile));
	if( szBaseName != NULL )
    {
        p = strchr(szBaseName, '.');
    }

	for(i=0; i<100; i++)
	{
		if( p != NULL )
		{
			sprintf(szTmp, "%.*s%.8s%02d%s",  
						p-szBaseName, szBaseName, f_GetTime(NULL,0,0), i, p);
		}
		else
		{
			sprintf(szTmp, "%s%.8s%02d",szBaseName, f_GetTime(NULL,0,0), i );
		}
		if( szDirName[0] != 0 )
		{
			sprintf(szTmp2, "%s/%s", szDirName, szTmp);
		}
		else
		{
			strcpy(szTmp2, szTmp);
		}
		if( access(szTmp2, F_OK) != 0)
		{
			break;
		}
	}
	strcpy(szBackupFile, szTmp2);

	return 0;
}

int f_setLogRcd(st_logRcd_t * logRcd,char * appName,char * level,char *msgCode, char * msg)
{
	char buffer[256] = {0};
	char chExeName[256] = {0};
#ifndef WIN32
	char *p = NULL;
	char cmd[256];
	int pid=-1;
	char szPgm[128];
	struct utsname Name;
#endif

	memset(buffer, 0, sizeof(buffer) );

	Sprintf(logRcd->timeStamp, "%.*s", sizeof(logRcd->timeStamp)-1, f_GetTime(NULL, 0, 0) );
	logRcd->timeStamp[sizeof(logRcd->timeStamp)-1] = FIELD_DELIMIT;
	Sprintf(logRcd->msglevel, "%.*s", sizeof(logRcd->msglevel)-1, level);
	logRcd->msglevel[sizeof(logRcd->msglevel)-1] = FIELD_DELIMIT;
	Sprintf(logRcd->msgCode, "%.*s", sizeof(logRcd->msgCode)-1, msgCode);
	logRcd->msgCode[sizeof(logRcd->msgCode)-1] = FIELD_DELIMIT;
	Sprintf(logRcd->appName, "%.*s", sizeof(logRcd->appName)-1, appName);
	logRcd->appName[sizeof(logRcd->appName)-1] = FIELD_DELIMIT;
	Sprintf(logRcd->message, "%.*s", sizeof(logRcd->message)-1, msg);
	logRcd->message[sizeof(logRcd->message)-1] = FIELD_DELIMIT;

#if defined(WIN32)
	Sprintf(logRcd->hostName, "%.*s", sizeof(logRcd->hostName)-1, getenv( "USERDOMAIN") );
	logRcd->hostName[sizeof(logRcd->hostName)-1] = FIELD_DELIMIT;
	Sprintf(logRcd->usrName, "%.*s", sizeof(logRcd->usrName)-1, getenv( "USERNAME") );
	logRcd->usrName[sizeof(logRcd->usrName)-1] = FIELD_DELIMIT;
	GetModuleFileName(NULL, (LPWSTR)chExeName, 256);
//	Sprintf(logRcd->pgmName, "%.*s", sizeof(logRcd->pgmName)-1, basename(_pgmptr));
	Sprintf(logRcd->pgmName, "%.*s", sizeof(logRcd->pgmName)-1, basename(chExeName));
	logRcd->pgmName[sizeof(logRcd->pgmName)-1] = FIELD_DELIMIT;
	logRcd->rcdDelimit[0] = '\r';
	logRcd->rcdDelimit[1] = '\n';
#else
	uname(&Name);
	
	Sprintf(logRcd->hostName, "%.*s", sizeof(logRcd->hostName)-1, Name.nodename);
	logRcd->hostName[sizeof(logRcd->hostName)-1] = FIELD_DELIMIT;
	
	Sprintf(logRcd->usrName, "%.*s", sizeof(logRcd->usrName)-1, getenv("LOGNAME"));
	logRcd->usrName[sizeof(logRcd->usrName)-1] = FIELD_DELIMIT;

	Sprintf(logRcd->pgmName, "%.*s", sizeof(logRcd->pgmName)-1, f_getPgmName());
	logRcd->pgmName[sizeof(logRcd->pgmName)-1] = FIELD_DELIMIT;
	logRcd->rcdDelimit[0] = '\n';	
#endif
	
	return 0;
}


int f_getLogIdLen(char * jzjklog)
{
	FILE * fp = NULL;
	char str[2048];
	int rc  =-1;

	fp = fopen( jzjklog, "r");
	if( fp == NULL )
	{
		fprintf(stderr, "fopen %s %s\n" , jzjklog, strerror(errno));
		return -1; 
	}
	fgets(str, sizeof(str), fp );
	rc = ftell(fp);

	fclose(fp);
	
	return rc;
}

/*
****************************************************************
*                                                            	*
*  NAME		：	f_doBackup   								    *
*  FUNCTION	：												    *
*		备份日志文件                                    		*
*  PARAMETER	：												*
*		参数1	[输入]	日志文件名								*
*		参数2	[输入]	备份日志文件名							*
*		参数3	[输入]	备份方法								*
*						1: 备份二日之前的日志					*
*						2: 备份一半日志记录						*
* 	RETURN VALUE：												*
*      <0    - 失败                                             *
*      =0    - 成功                                            *
*																*
****************************************************************
*/
int f_doBackup(char * logfile, char * backupfile, int iStrategy)
{
	int 	fd		= -1;
	int		fd_read	= -1;	
	int		fd_bkup	= -1;	
	int		bkup_flag= 0;	
	long  	fileLen = 0;
	int 	rcdLen = 0;
	int		rcdNum = 0;
	int		rcdMoved=0;
	int		logIdLen = 0;
	int 	rc = -1;
	st_logRcd_t stLogRcd;
	char	szTimeStamp[20];

	rcdLen = sizeof(stLogRcd);
	fileLen = f_getFileSize(logfile);
	logIdLen = f_getLogIdLen(logfile);

	 #if defined(WIN32)
        fd = _sopen(logfile, _O_WRONLY|_O_CREAT|_O_BINARY, _SH_DENYWR,
                        _S_IREAD|_S_IWRITE );
		if( fd == -1 )
		{
			if( errno == EACCES )
			{
				close(fd);
				return 1;
			}
			else
			{
				printf("open file [%s] error:%s\n" ,logfile, strerror(errno) );
				close(fd);
				return -1;
			}
		}
		fd_read = _sopen(logfile, _O_RDONLY|_O_BINARY,_SH_DENYNO);
    #else
		fd = open(logfile, O_WRONLY|O_CREAT,
                     S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH);
		if( fd == -1 )
		{
			printf("open file [%s] error:%s\n" , logfile, strerror(errno) );
			return -1;
		}

		rc = lockf(fd, F_TLOCK, 0 );
		if( rc == -1 )
		{
			if( errno == EACCES || errno == EAGAIN )
			{
				close(fd);
				return 1;
			}
			else
			{
				printf("lockf file [%s] error:%s:%d\n" ,logfile, strerror(errno),errno );
				close(fd);
				return -1;
			}
		}

		fd_read = open(logfile, O_RDONLY);
    #endif

		
	if( fd_read == -1 )
	{
		printf("open file [%s] error:%s\n" , logfile, strerror(errno) );
		close(fd);
		return -1;
	}
	/* 跳过日志文件标识行 */
	lseek(fd, logIdLen, SEEK_SET);
	lseek(fd_read, logIdLen, SEEK_SET);

	memset( &stLogRcd, ' ', sizeof(stLogRcd) );
	memset( szTimeStamp, 0, sizeof(szTimeStamp) );
	f_GetTime(szTimeStamp, 0, BACKUP_OFFSET_DAY);
	while( read( fd_read, &stLogRcd, rcdLen) == rcdLen ) 
	{
		rcdNum++;
		rc = strncmp( stLogRcd.timeStamp, szTimeStamp, 8);
		if( iStrategy == 1 && rcdNum == 1  && rc > 0 )
		{
			break;
		}
		if( bkup_flag == 0 && 
				((iStrategy == 1 && rc <= 0)||iStrategy == 2))
		{
		#if defined(WIN32)
			fd_bkup = _sopen( backupfile, _O_WRONLY|_O_CREAT|_O_APPEND|_O_BINARY,_SH_DENYNO,
				_S_IREAD|_S_IWRITE);
		#else
			fd_bkup = open( backupfile, O_WRONLY|O_CREAT|O_APPEND,
						0664);
		#endif
			if( fd_bkup == -1 )
			{
				printf("open file [%s] err:%s\n",backupfile,strerror(errno));
				close(fd_read);
				close(fd);
				return -1;
			}
			bkup_flag = 1;
		}
		if( ( iStrategy == 1 && rc <= 0)||
			( iStrategy == 2 && rcdNum <= fileLen/(rcdLen*2) ) )
		{
			if(write(fd_bkup, &stLogRcd, rcdLen) != rcdLen)
			{
				printf("write file [%s] err:%s\n",backupfile,strerror(errno));
				close(fd_read);
				close(fd_bkup);
				close(fd);
				return -1;
			}
		}
		else
		{
			rcdMoved++;
			if(write(fd, &stLogRcd, rcdLen) != rcdLen)
			{
				printf("write file [%s] err:%s\n",backupfile,strerror(errno));
				close(fd_read);
				close(fd_bkup);
				close(fd);
				return -1;
			}
		}
		memset( &stLogRcd, ' ', rcdLen );
	}
	if( bkup_flag == 1 )
	{
		rc = ftruncate(fd,  rcdMoved * rcdLen+logIdLen);
		if( rc == -1 )
		{
			printf("ftruncate [%s] error:%s\n",logfile,strerror(errno) );
			close(fd_read);
			close(fd_bkup);
			close(fd);
			return -1;
		}
	}
	
	if( fd_bkup > 0 )
	{
		close(fd_bkup);
	}
	close(fd_read);
	close(fd);

	return 0;

}

/*
****************************************************************
*                                                            	*
*  NAME		：	f_backUpJzjkLog									* 
*  FUNCTION	：													*	
*		写日志文件        										* 
*  PARAMETER	：												*	
*		参数1	[输入]	日志文件名								*	
*  RETURN VALUE：												*	
*     	-1  - 失败    	                                        *
*     	0  - 成功                                               *
*     	1  - 日志文件正在被使用，不能备份.                      *
****************************************************************
*/
int f_backUpJzjkLog(char * logfile )
{
	int rc = -1;
	char 	szBackupFile[256];

	f_getBackupFileName(logfile, szBackupFile);

	/* 日志备份处理 */
	if( f_getFileSize(logfile) > MAX_LOG_FILE_LEN )
	{
		/* 备份两个自然日前的日志 */
		rc = f_doBackup(logfile, szBackupFile, 1);	
	}
	while( f_getFileSize(logfile) > MAX_LOG_FILE_LEN )
	{
		/* 备份当前日志文件的前半部分 */
		 rc = f_doBackup(logfile, szBackupFile, 2);
		if( rc == 1 )
		{
			break;
		}	
	}

	return rc;

}

/*
****************************************************************
*                                                            	*
*  NAME		：	f_wrtJzjkLog									* 
*  FUNCTION	：													*	
*		写日志文件        										* 
*  PARAMETER	：												*	
*	    参数1   [输入]  消息级别								*
*		参数2	[输入]	消息代码								*	
*		参数3	[输入]	应用名									*	
*		参数4	[输入]	消息									*	
*		参数5	[输入]	日志文件名								*	
*						在WIN32环境下支持两种格式的文件名:		*
*						1. 如: "d:\\projects\\logname.log"		*
*						2. 如: "d:/projects/logname.log"		*
* 	RETURN VALUE：												*	
*     	<0  - 失败    	                                        *
*     	0  - 成功                                               *
*																*
****************************************************************
*/
int f_wrtJzjkLog(char * level,char * msgcode, char * appname, char * msg, char * logfile)
{
	int 	fd		= -1;
	int 	rcdLen = 0;
	int 	rc=-1, i=0;
	st_logRcd_t stLogRcd;

	rcdLen = sizeof(stLogRcd);
	
	/* 填写日志记录信息 */
	memset(&stLogRcd, ' ', rcdLen);
	if( f_setLogRcd(&stLogRcd, appname, level, msgcode, msg) < 0 )
	{
		return -1;
	}
	
	/* 写日志文件 */
	for( i = 0; i< 100; i++)
	{
	#if defined(WIN32)
        fd = _sopen(logfile, _O_WRONLY|_O_CREAT|_O_APPEND|_O_BINARY, _SH_DENYNO,
                        _S_IREAD|_S_IWRITE );
		if( fd == -1 )
		{
			if( errno == EACCES )
			{
				Sleep(3);
				continue;
			}
			else
			{
				printf("open file [%s] error:%s\n" , logfile, strerror(errno) );
				return -1;
			}
		}
	#else
		fd = open( logfile, O_WRONLY|O_CREAT|O_APPEND,
				0664);
		if( fd == -1 )
		{
			printf("open file [%s] error:%s\n" , logfile, strerror(errno) );
			return -1;
		}
		rc = lockf(fd, F_TEST, 0 );
		if( rc == -1 )
		{
			if( errno == EACCES || errno == EAGAIN )
			{
				sleep(3);
				continue;
			}
			else
			{
				printf("lockf file [%s] error:%s:%d\n" ,logfile, strerror(errno), errno );
				return -1;
			}
		}
	#endif
		if( fd >= 0 )
		{
			break;
		}
	}
	if(write(fd, &stLogRcd, rcdLen) != rcdLen)
	{
		printf("write file [%s] error:%s\n" , logfile, strerror(errno) );
		close(fd);
		return -1;
	}
	
	close( fd );

	return 0;
}

/*
****************************************************************
*                                                            	*
*  NAME		：	f_jzjkLog										* 
*  FUNCTION	：													*	
*		写日志文件        										* 
*  PARAMETER	：												*	
*	    参数1   [输入]  消息级别								*
*		参数2	[输入]	消息代码								*	
*		参数3	[输入]	应用名									*	
*		参数4	[输入]	消息									*	
*		参数5	[输入]	日志文件名								*	
*						在WIN32环境下支持两种格式的文件名:		*
*						1. 如: "d:\\projects\\logname.log"		*
*						2. 如: "d:/projects/logname.log"		*
* 	RETURN VALUE：												*	
*     	<0  - 失败    	                                        *
*     	0  - 成功                                               *
*																*
****************************************************************
*/
int f_jzjkLog(char * level,char * msgcode, char * appname, char * msg, char * logfile)
{
	int 	fd		= -1;
	int 	rcdLen = 0;
	int 	rc=-1, i=0;
	st_logRcd_t stLogRcd;
	char 	szBackupFile[256];

	rcdLen = sizeof(stLogRcd);
	f_getBackupFileName(logfile, szBackupFile);

	/* 日志备份处理 */
	if( f_getFileSize(logfile) > MAX_LOG_FILE_LEN )
	{
		/* 备份两个自然日前的日志 */
		f_doBackup(logfile, szBackupFile, 1);	
	}
	while( f_getFileSize(logfile) > MAX_LOG_FILE_LEN )
	{
		/* 备份当前日志文件的前半部分 */
		if( f_doBackup(logfile, szBackupFile, 2)==1 )
		{
			break;
		}	
	}
	
	/* 填写日志记录信息 */
	memset(&stLogRcd, ' ', rcdLen);
	if( f_setLogRcd(&stLogRcd, appname, level, msgcode, msg) < 0 )
	{
		return -1;
	}
	
	/* 写日志文件 */
	for( i = 0; i< 100; i++)
	{
	#if defined(WIN32)
        fd = _sopen(logfile, _O_WRONLY|_O_CREAT|_O_APPEND|_O_BINARY, _SH_DENYNO,
                        _S_IREAD|_S_IWRITE );
		if( fd == -1 )
		{
			if( errno == EACCES )
			{
				Sleep(3);
				continue;
			}
			else
			{
				printf("open file [%s] error:%s\n" , logfile, strerror(errno) );
				return -1;
			}
		}
	#else
		fd = open( logfile, O_WRONLY|O_CREAT|O_APPEND,
				0664);
		if( fd == -1 )
		{
			printf("open file [%s] error:%s\n" , logfile, strerror(errno) );
			return -1;
		}
		rc = lockf(fd, F_TEST, 0 );
		if( rc == -1 )
		{
			if( errno == EACCES || errno == EAGAIN )
			{
				sleep(3);
				continue;
			}
			else
			{
				printf("lockf file [%s] error:%s:%d\n" ,logfile, strerror(errno) ,errno);
				return -1;
			}
		}
	#endif
		if( fd >= 0 )
		{
			break;
		}
	}
	rcdLen = sizeof(stLogRcd);
	if(write(fd, &stLogRcd, rcdLen) != rcdLen)
	{
		printf("write file [%s] error:%s\n" , logfile, strerror(errno) );
		close(fd);
		return -1;
	}
	
	close( fd );

	return 0;
}

int DSS_Log(int level,int msgcode,char *msg)
{
	char loglevel[16] = {0};
	char logcode[16] = {0};
	char appname[] = "Suresec";
	char logfile[] = LOGFILE;

	//判断日志文件夹是否存在，不存在直接返回
	if( (_access( LOGPATH, 0 )) != 0 )   
	{     
		return 0;
	}


	switch(level)
	{
	case DSS_DEBUG:
		strcpy(loglevel,"00");
		break;
	case DSS_INFO:
		strcpy(loglevel,"01");
		break;
	case DSS_WARN:
		strcpy(loglevel,"02");
		break;
	case DSS_ERROR:
		strcpy(loglevel,"03");
		break;
	case DSS_FATAL:
		strcpy(loglevel,"04");
		break;
	default:
		return -1;
		break;
	}

	itoa(msgcode,logcode,16);

	f_jzjkLog(loglevel,logcode,appname,msg,logfile);

	return 0;
}