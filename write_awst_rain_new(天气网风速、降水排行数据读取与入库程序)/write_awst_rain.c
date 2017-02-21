#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ocilib.h"
#include "public.h"

#define LINE 500
#define COLUMN_NUM 3     //字段个数

/*
----- 主函数参数描述：argc=2 argv[1]：cr01数据文件名
*/
int main(int argc,char *argv[])
{
    char user[30], passwd[30], dbname[30];
    char buf[LINE];
    char date[14];
    char temp[COLUMN_NUM][100];   
    float value[COLUMN_NUM];
    FILE   *fp;
    int  j,ret,num=0,record=0;
    
    /*
    ----- variable initialization
    */
    
    for(j=0;j<COLUMN_NUM;j++)
    	value[j]=0;
      
    memset(date,'\0',sizeof(date));
    memset(temp,'\0',sizeof(temp));
    memset(buf,'\0',sizeof(buf));
    
    memset(user, '\0', sizeof(user));
    memset(passwd, '\0', sizeof(passwd));
    memset(dbname, '\0', sizeof(dbname));
    
    //printf("fengyuxing1\n");
    strcpy(user, "name_user");
    strcpy(passwd, "name_user123");
    strcpy(dbname, "weatherdb_dmz");
   /* 
    -----1. connect database 
    */
   
    //printf("fengyuxing2\n");
    ret = logon(user, passwd, dbname);
    //printf("fengyuxing3\n");

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
			//printf("%d\n",num);
			
	//printf("fengyuxing6\n");
		
    	if(num==COLUMN_NUM)
    	{	    	      
                  sprintf(date,"%s",temp[1]);
                  //printf("%s\n",date);
                  value[0]=atof(temp[0]);
                  value[1]=atof(temp[2]);
    		  //printf("%f\n",value[0]);
                  //printf("%f\n",value[1]);
    		
    		  ret=write_awst(date,value);
                

    		if(ret!=0)
    		{
    			printf("call write_awst() failed.\n");
    			exit(-1);
    		}
    		record++;
    		
    	}

    	memset(buf,'\0',sizeof(buf));         
      memset(temp,'\0',sizeof(temp));
    }
    
    printf("processed records: %d\n",record);
    fclose(fp);        
    logoff();    
    exit(0);
}

