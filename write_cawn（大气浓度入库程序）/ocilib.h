#include <oci.h>

char oci_err[1000];

int logon(char *username, char *password, char *dbname);
void logoff();
void error_proc(dvoid *errhp, sword status);
int read_pm25(char *data_dir,char *data_file,float value[]);
int read_pm10(char *data_dir,char *data_file,float value[]);

