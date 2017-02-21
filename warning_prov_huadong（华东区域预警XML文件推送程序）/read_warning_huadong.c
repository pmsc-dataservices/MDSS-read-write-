#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/stat.h>
#include "warning.h"
#include "public.h"
#include "public.c"

int main(int argc,char *argv[])
{
	  char procname[50], fcdate[10];
    char orausr[30], orapasswd[30], oradbname[30];
    int  ct[10], warning_number, ret;
  	char logname[255];
    
    /*
     * 1. open log file
     */
    numtime(ct, -1);
	  memset(logname,'\0',sizeof(logname));
    sprintf(logname, "%s/log/create_warning_huadong_xml%04d%02d%02d.log", getenv("HOME"),ct[0],ct[1],ct[2]);
    if ((fplog=fopen(logname,"a+")) == NULL) 
    {
         printf("cannot open %s\n", logname);
         exit(-1);
    }
    setvbuf(fplog,NULL,_IONBF,0);

    signal(SIGINT, SIG_IGN);
    signal(SIGTERM, SIG_IGN);
    signal(SIGHUP, SIG_IGN);

    memset(fcdate, '\0', sizeof(fcdate));
    if (argc==2)
    {
        sprintf(fcdate, "%s", argv[1]);
    }
    else
    {
        numtime(ct, -1);
        sprintf(fcdate, "%04d%02d%02d", ct[0], ct[1], ct[2]);
        fprintf(fplog, "fcdate = %s \n",fcdate);
    }

    /* 
     * 2. process control
     */
    sprintf(procname, "%s", "create_warning_huadong_xml");
    ret = IsProcessExist(procname);
    if (ret == 0)
     {
         fprintf(fplog, "####################################################\n");
         fprintf(fplog, "#warning: process %s second starting fail {%04d-%02d-%02d %02d:%02d:%02d}#\n", procname, ct[0], ct[1], ct[2], ct[3], ct[4], ct[5]);
         fprintf(fplog, "################################################ ###\n");
         fflush(fplog);
         fclose(fplog);
         exit(1);   
     }
     else
     {
         fprintf(fplog, "#########################################\n");
         fprintf(fplog, "# process %s started #\n", procname);
         fprintf(fplog, "#       {%04d-%02d-%02d %02d:%02d:%02d}           #\n", ct[0], ct[1], ct[2], ct[3], ct[4], ct[5]);
         fprintf(fplog, "#########################################\n");
         fflush(fplog);
     }
     
    /* 
     * 3. connect source database 
     */
    memset(orausr, '\0', sizeof(orausr));
    sprintf(orausr,"%s","process_user");
    memset(orapasswd, '\0', sizeof(orapasswd));
    sprintf(orapasswd,"%s","process_user123");
    memset(oradbname, '\0', sizeof(oradbname));
    sprintf(oradbname,"%s","dmz_151");

    if (logon(orausr, orapasswd, oradbname) != 0)
    {
    	 fprintf(fplog, "[error]\n");
		   fprintf(fplog,"Cannot connect to ORACLE as %s/%s@%s\n", orausr, orapasswd, oradbname);
       fflush(fplog);
       fclose(fplog);
       logoff();
       exit(-1);
    }
    else
    {
    	 fprintf(fplog, "[ok]\n");
       fflush(fplog);
    }
    
    /* 4. read warning province and create xml */
    fprintf(fplog, "begin read warning province ...\n");
    warning_number = read_warning_province(fcdate);
    fprintf(fplog, "warning number = [%d]\n", warning_number);
    
    /* 
     * 5. distinct connect database
     */
    logoff();
    
    /*
     * 6. close log file */
    fclose(fplog);
    
    exit(1);

}
