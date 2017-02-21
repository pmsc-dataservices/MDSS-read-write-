#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ocilib.h"
#include "public.h"
#include "public.c"

#define LINE  10000
#define COLUMN_NUM 45     //字段个数

/*
----- 主函数参数描述：argc=2 argv[1]：cr01数据文件名
*/
int main(int argc,char *argv[])
{
    char user[30], passwd[30], dbname[30];
    char buf[LINE];
    char  data_dir[50];
    char temp[COLUMN_NUM][100]; 
    float value[COLUMN_NUM];
    char a[4],date[14],utctime[20],time[25],times[25],dates[25];
    int   ct[10],type=1;
    int     b,c,d,e;

    FILE   *fp;
    int  j,ret,num=0,record=0;
    
    
    /*
    ----- variable initialization
    */
    numtime(ct, -1);

    for(j=0;j<COLUMN_NUM-29;j++)
    	value[j]=0;
    memset(date,'\0',sizeof(date));
    memset(times,'\0',sizeof(times));
     memset(dates,'\0',sizeof(dates));

   
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
	//printf("fengyuxing4\n");
	
		fp = fopen(argv[1],"r");
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
       // printf("%d\n",num);
			
	//printf("fengyuxing6\n");
		
    	if(num==COLUMN_NUM)
    	{
          //printf("fengyuxing7\n");
              
                sprintf(date,"%04d",atoi(temp[5]));
    		sprintf(date+4,"%02d",atoi(temp[6]));
    		sprintf(date+6,"%02d",atoi(temp[7]));
                 sprintf(date+8,"%02d",atoi(temp[8]));
                 sprintf(date+10,"%02d",atoi(temp[9]));
                 sprintf(date+12,"%02d",atoi(temp[10]));
    		// printf("%s\n",date);
                
                 
    		 /*
    		--- process date. formate: YYYYMMDD
    		*/
    		
    		for(j=0;j<COLUMN_NUM;j++)
    	       {
    		  //printf("%f ",atof(temp[j]));
    		  value[j]=atof(temp[j]);
    		  //printf("%f ",value[j]);
    		}
                 
                 
    		//printf("fengyuxing8\n");
               
    		ret=insert_spml(date,value);
                //printf("%d",ret);
               // printf("fengyuxing9\n");

    		if(ret!=0)
    		{
    			printf("call insert_spml() failed.\n");
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

    /* 
    -----1. process datafile 
    */
         numtime(ct, -1);
         memset(time,'\0',sizeof(time));
         sprintf(time,"%4d%02d%02d%02d0000",ct[0],ct[1],ct[2],ct[3]);
         printf("%s\n",time);
         date_add(time, date,-9);
         printf("%s\n",date);
        // sprintf(a,"%04d",ct[0]);
                 printf("%d",ct[0]);
                 
                  c=ct[0]%100;     
                  printf("%d\n",c); 

         sprintf(times,"%d%02d%02d%02d",c,ct[1],ct[2],ct[3]);
         printf("%s\n",times);

      memset(data_dir, '\0', sizeof(data_dir));
      sprintf(data_dir, "%s/data/spml_pm_data", getenv("HOME"));
      
     ret=select_spml(times,date,data_dir);
    
     if(ret!=0)
    {
     printf("call select_spml() failed.\n");
     exit(-1);
    } 

      ret=select_spml_data(times,date,data_dir);
      if(ret!=0)
    {
     printf("call select_spml_data() failed.\n");
     exit(-1);
    } 

       
    logoff();    
    exit(0);
}

