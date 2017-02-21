#include <oci.h>

char oci_err[1000];

int logon(char *username, char *password, char *dbname);
void logoff();
void error_proc(dvoid *errhp, sword status);
int insert_special(int station,char *date,int tempe,int wd,int ws,int sd,float value[]);
