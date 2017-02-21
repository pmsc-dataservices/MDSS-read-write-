#include <oci.h>

char oci_err[1000];

int logon(char *username, char *password, char *dbname);
void logoff();
void error_proc(dvoid *errhp, sword status);
int insert_GRO1(char *date,char  *cymd,char  *chms,float value[]);
int insert_GRO2(char *date,char  *cymd,char  *chms,float value[]);
int insert_GRO3(char *date,char  *cymd,char  *chms,float value[]);
int insert_GRO4(char *date,char  *cymd,char  *chms,float value[]);
int insert_GRO5(char *date,char  *cymd,char  *chms,float value[]);
int insert_GRO6(char *date,char  *cymd,char  *chms,float value[]);
int insert_DIO1(char *date,char  *cymd,char  *chms,char  *des,float value[]);
int insert_DIO2(char *date,char  *cymd,char  *chms,char  *des,float value[]);
int insert_DIO3(char *date,char  *cymd,char  *chms,char  *des,float value[]);
int insert_DIO4(char *date,char  *cymd,char  *chms,char  *des,float value[]);
int insert_TENS(char *date,float value[]);
int insert_MONT(char *date,float value[]);
