#include <oci.h>

char oci_err[1000];

int logon(char *username, char *password, char *dbname);
void logoff();
void error_proc(dvoid *errhp, sword status);
int insert_aqi(char *date,char *code,float value[],char *prov,char *city,char *station);
