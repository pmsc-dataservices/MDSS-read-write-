#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ocilib.h"
#include "public.h"
#include "public.c"

/*
----- 主函数参数描述：argc=2 argv[1]：cr01数据文件名
*/
int main(int argc,char *argv[])
{
    char user[30], passwd[30], dbname[30];
    char date[14],utctime[20],time[25];
    char data_dir[50];
    int   ct[10],type=1;

    FILE   *fp;
    int  j,ret,num=0,record=0;
    
    
    /*
    ----- variable initialization
    */
    numtime(ct, -1);

         
    memset(user, '\0', sizeof(user));
    memset(passwd, '\0', sizeof(passwd));
    memset(dbname, '\0', sizeof(dbname));
    
    //printf("fengyuxing1\n");
    strcpy(user, "process_user");
    strcpy(passwd, "process_user123");
    strcpy(dbname, "awstdb");
   /* 
    -----1. connect database 
    */
   
    //printf("fengyuxing2\n");
    ret = logon(user, passwd, dbname);
   // printf("%d\n",ret);

    if (ret != 0)
    {
    	printf("Connect Oracle Fail ...\n");
    	printf("%s\n", oci_err);
    	logoff();
    	exit(-1);
    }

    memset(date,'\0',sizeof(date)); 
    strcpy(date, argv[1]);
	    
    /*
    -----3. process datafile(降水)
    */ 
    memset(data_dir, '\0', sizeof(data_dir));
    sprintf(data_dir, "%s/mdss/demo/Z/write_awst_rain_new", getenv("HOME"));
        
    ret=select_awst(data_dir,date);
    //printf("%d",ret);
    if(ret!=0)
    {
     printf("call select_awst() failed.\n");
     exit(-1);
    } 
   
   /*
    -----3. process datafile（风速）
    */ 
    memset(data_dir, '\0', sizeof(data_dir));
    sprintf(data_dir, "%s/mdss/demo/Z/write_awst_rain_new", getenv("HOME"));
        
    ret=select_awst_wind(data_dir,date);
    //printf("%d",ret);
    if(ret!=0)
    {
     printf("call select_awst_wind() failed.\n");
     exit(-1);
    } 

    
    logoff(); 
    

    exit(0);
}

