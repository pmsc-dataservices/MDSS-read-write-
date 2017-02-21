#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ocilib.h"
#include "public.h"
#include "public.c"

#define LINE  1000
#define COLUMN_NUM 9     //�ֶθ���

/*
----- ����������������argc=2 argv[1]��cr01�����ļ���
*/
int main(int argc,char *argv[])
{
    char user[30], passwd[30], dbname[30];
    char buf[LINE];
    char temp[COLUMN_NUM][100]; 
    float value[COLUMN_NUM];
    char date[14];
    char  flag[1];
    int   ct[10],type=1;
    int   station,tempe,wd,ws,sd;

    FILE   *fp;
    int  j,ret,num=0,record=0;
    
    
    /*
    ----- variable initialization
    */
    numtime(ct, -1);
    for(j=0;j<COLUMN_NUM;j++)
    	value[j]=0;

    memset(date,'\0',sizeof(date));
    memset(temp,'\0',sizeof(temp));
    memset(buf,'\0',sizeof(buf));
    
    memset(user, '\0', sizeof(user));
    memset(passwd, '\0', sizeof(passwd));
    memset(dbname, '\0', sizeof(dbname));
    
    //printf("fengyuxing1\n");
    strcpy(user, "process_user");
    strcpy(passwd, "process_user123");
    strcpy(dbname, "inner_database");
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
    
    
		/*
		-----2. open datafile
		*/
	
     fp = fopen(argv[1],"r");
     printf("fengyuxing1\n");
          
          
    if (fp == NULL)
    { 
      printf("open file error: %s\n",argv[1]);
      exit(-1);
    }
    
    
    /*
    -----3. process datafile
    */    
    //printf("fengyuxing5\n");

    while(fgets(buf,LINE,fp))
    {
    	
    	/*
    	--- process a line
    	*/
    	num=0;
    	//printf("%s\n",buf);
    	num=stringsplit(buf,' ',COLUMN_NUM,temp);  			
        //printf("%d\n",num);
			
	//printf("fengyuxing6\n");
		
    	if(num==COLUMN_NUM)
    	{
          //printf("fengyuxing7\n");
           
                 
    		 /*
    		--- process date. formate: YYYYMMDD
    		*/
    		
    		
    		  //printf("%f ",atof(temp[j]));
                  station=atoi(temp[0]);
                  strcpy(date,temp[1]);
                  printf("%s\n",date);
                  tempe=atoi(temp[2]);
                                    wd=atoi(temp[3]);
                                    ws=atoi(temp[4]);
                                    sd=atoi(temp[5]);
    		  value[0]=atof(temp[6]);
                   value[1]=atof(temp[7]);
                   value[2]=atof(temp[8]);
                   printf("%d\n",tempe);
    	         printf("%d\n",station);
     
                             
    		printf("fengyuxing8\n");
               
    		ret=insert_special(station,date,tempe,wd,ws,sd,value);
                 

                printf("%d",ret);
               // printf("fengyuxing9\n");

    		if(ret!=0)
    		{
    			printf("call insert_special() failed.\n");
    			exit(-1);
    		}
    		record++;
    		//printf("\n");
    	}

    	memset(buf,'\0',sizeof(buf));         
        memset(temp,'\0',sizeof(temp));
    } 
    printf("processed records: %d\n",record);
    fclose(fp); 
    logoff();    
    exit(0);
}

