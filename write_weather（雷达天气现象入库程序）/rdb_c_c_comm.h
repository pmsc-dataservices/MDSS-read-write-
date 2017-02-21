/*
* defining return code
*/

/*
* common error 
*/

#define OPERATING_SUCCESS 0
#define SYS_ERROR_OS 1501
#define SYS_MORE_PROCESS 1502
#define SYS_NO_DISK_SPACE 1503
#define RDB_C_NOT_CONNECT_DB_SERVER 2001
#define RDB_C_FAIL_OPENFILE 2002
#define RDB_C_FAIL_MOVEFILE 2003
#define RDB_C_TOO_LONG_FILENAME 2004
#define RDB_C_TOO_LONG_PATHNAME 2005
#define RDB_C_FAIL_WRITEFILE 2006
#define RDB_C_NOT_DISCONNECT_DB_SERVER 2007
#define RDB_C_FILE_NOTEXIST 2008

static  time_t  prev;  /* the time when beginning to process bulletin */
static  unsigned int gap;  /* sleep time */

extern void WriteError(FILE *, char *);
extern void WriteMsg(FILE *, char *);
extern int NumTime(int *, time_t);
extern int WriteWake(FILE *, int []);
extern int WriteSleep(FILE *, int []);
extern void schedule(unsigned int);
extern void hiber();

