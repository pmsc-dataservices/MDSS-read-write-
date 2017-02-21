#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ocilib.h"
#include "public.h"
#include "public.c"

#define LINE  1000
#define COLUMN_NUM 6     //字段个数

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
    char name[30],bjtime[14],source[10],weather[30],update[30];
    int   ct[10],type=1;
    int     b,c,d,e;

    FILE   *fp;
    int  j,ret,num=0,record=0;
    
    
    /*
    ----- variable initialization
    */
    numtime(ct, -1);

    for(j=0;j<COLUMN_NUM;j++)
    	value[j]=0;
    memset(bjtime,'\0',sizeof(bjtime));
    memset(source,'\0',sizeof(source));
    memset(weather,'\0',sizeof(weather));
    memset(update,'\0',sizeof(update));


   
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
    	num=stringsplit(buf,',',COLUMN_NUM,temp);  			
       // printf("%d\n",num);
			
	//printf("fengyuxing6\n");
		
    	if(num==COLUMN_NUM)
    	{
                if((strcmp(temp[0],"stationId"))==0)
                {
                   printf("请继续下一行\n");
                 }
                 else
                {
                value[0]=atof(temp[0]);
                sprintf(name,"%s",temp[1]);
                sprintf(bjtime,"%s",temp[2]);
                sprintf(source,"%s",temp[3]);
                sprintf(weather,"%s",temp[4]);
                sprintf(update,"%s",temp[5]);
                
               // printf("%f ",value[0]);printf("%s ",name);printf("%s ",bjtime);
    		         // printf("%s ",source);printf("%s ",weather);printf("%s ",update);
                
    		if((strcmp(weather,"无法判别"))!=0) 
      {
       ret=insert_weather(name,bjtime,source,weather,update,value);
      	if(ret!=0)
    		{
    			printf("call insert_weather() failed.\n");
    			exit(-1);
    		}      
     }   
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

