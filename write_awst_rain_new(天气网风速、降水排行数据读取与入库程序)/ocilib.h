#include <oci.h>

char oci_err[1000];

int logon(char *username, char *password, char *dbname);
void logoff();
void error_proc(dvoid *errhp, sword status);
int select_awst(char  *data_dir,char  *time);
int write_awst(char *date,float value[]);
int select_awst_wind(char  *data_dir,char  *time);
int write_awst_wd(char *date,float value[]);
