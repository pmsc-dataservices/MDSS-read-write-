#include <oci.h>

char oci_err[1000];

int logon(char *username, char *password, char *dbname);
void logoff();
void error_proc(dvoid *errhp, sword status);
int insert_CRO1(char *date,char  *cymd,char  *chms,float value[]);
int insert_CRO2(char *date,char  *cymd,char  *chms,float value[]);
int insert_CRO3(char *date,char  *cymd,char  *chms,float value[]);
int insert_CRO4(char *date,char  *cymd,char  *chms,float value[]);
int insert_CRO5(char *date,char  *cymd,char  *chms,float value[]);
int insert_CRO6(char *date,char  *cymd,char  *chms,char  *des,float value[]);
int insert_CRO7(char *date,char  *cymd,char  *chms,float value[]);
int insert_SOO1(char *date,char  *cymd,char  *chms,float value[]);
int insert_SOO2(char *date,char  *cymd,char  *chms,float value[]);
int insert_SOO3(char *date,char  *cymd,char  *chms,float value[]);
int insert_SOO4(char *date,char  *cymd,char  *chms,float value[]);
int insert_SOO5(char *date,char  *cymd,char  *chms,float value[]);
int insert_PHO1(char *date,char  *cymd,char  *chms,float value[]);
int insert_PHO2(char *date,char  *cymd,char  *chms,float value[]);
int insert_PHO3(char *date,char  *cymd,char  *chms,float value[]);
int insert_PHO4(char *date,char  *cymd,char  *chms,float value[]);
