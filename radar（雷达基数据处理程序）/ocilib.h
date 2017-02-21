#include <oci.h>

char oci_err[1000];

int logon(char *username, char *password, char *dbname);
void logoff();
void error_proc(dvoid *errhp, sword status);
int insert_radr(char *itime,char *ptime,char *sdir,char *tdir,char *filename,char *filesize,char *pname,char *sip,char *sname,char *tip,char *tname);


