#include <oci.h>

char oci_err[1000];

int logon(char *username, char *password, char *dbname);
void logoff();
void error_proc(dvoid *errhp, sword status);
int insert_spml(char *date,float value[]);
int select_spml(char  *time,char  *date,char  *data_dir);
int select_spml_data(char  *time,char  *date,char  *data_dir);
