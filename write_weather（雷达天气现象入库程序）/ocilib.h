#include <oci.h>

char oci_err[1000];

int logon(char *username, char *password, char *dbname);
void logoff();
void error_proc(dvoid *errhp, sword status);
int insert_weather(char *name,char *bjtime,char *source,char *weather,char *update,float value[0]);
