#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ocilib.h"
#include "public.h"
#include "public.c"

#define LINE  30000
#define COLUMN_NUM 35     //字段个数

/*
----- 主函数参数描述：argc=2 argv[1]：cr01数据文件名
*/
int main(int argc,char *argv[])
{
    char user[30], passwd[30], dbname[30];
    char buf[LINE];
    char temp[COLUMN_NUM][100]; 
    float value[COLUMN_NUM];
    char date[14],bjtime[20],time[25],station[6];
    char  flag[20];
    int   ct[10],type=1;
    int  ret2;

    FILE   *fp;
    int  j,ret,num=0,record=0;
    
    
    /*
    ----- variable initialization
    */
    numtime(ct, -1);

    for(j=0;j<COLUMN_NUM;j++)
    	value[j]=0;
    memset(date,'\0',sizeof(date));
    memset(bjtime,'\0',sizeof(bjtime));
    memset(station,'\0',sizeof(station));
    memset(flag,'\0',sizeof(flag));
    memset(time,'\0',sizeof(time));
    memset(temp,'\0',sizeof(temp));
    memset(buf,'\0',sizeof(buf));
    
    memset(user, '\0', sizeof(user));
    memset(passwd, '\0', sizeof(passwd));
    memset(dbname, '\0', sizeof(dbname));
    
    //printf("fengyuxing1\n");
    strcpy(user, "process_user");
    strcpy(passwd, "process_user123");
    strcpy(dbname, "dmz_151");
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
    //printf("begin  write datas: %4d/%d/%d %02d:%02d:%02d \n",ct[0],ct[1],ct[2],ct[3],ct[4],ct[5]);
    
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
                
                sprintf(date,"%04d",atoi(temp[1]));
    		          sprintf(date+4,"%02d",atoi(temp[2]));
    		          sprintf(date+6,"%02d",atoi(temp[3]));
                 sprintf(date+8,"%02d",atoi(temp[4]));
                 sprintf(date+10,"%02d",atoi(temp[5]));
                 sprintf(date+12,"%02d",atoi(temp[6]));
    		                 date_add(date, bjtime,8);
                   sprintf(flag,"%s%s",bjtime,station);
                 // printf("%s\n",flag);
                 // printf("%s\n",time);

                 
    		 /*
    		--- process date. formate: YYYYMMDD
    		*/
    		value[0]=atof(temp[0]);
    		for(j=7;j<COLUMN_NUM;j++)
    	        {
       		  value[j]=atof(temp[j]);
    		     		}                         
                 
    		
               
    		ret=insert_awst(bjtime,value);
                 

                //printf("%d",ret);
               // printf("fengyuxing9\n");

    		if(ret!=0)
    		{
    			printf("call insert_gwst() failed.\n");
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

   // printf("write datas is over: %4d/%d/%d %02d:%02d:%02d \n",ct[0],ct[1],ct[2],ct[3],ct[4],ct[5]);
   // ret=execute_generate_info(bjtime);
    if(ret!=0)
    		{
    		 printf("call execute_generate_info() failed.\n");
    		 exit(-1);
    		}

    logoff();    
    exit(0);
}

