#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ocilib.h"
#include "public.h"
#include "public.c"

#define LINE  10000
#define COLUMN_NUM 24     //字段个数

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
    char a[4],date[14],code[5],prov[20],city[30],station[30];
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
    memset(date,'\0',sizeof(date));
    memset(code,'\0',sizeof(code));
     memset(prov,'\0',sizeof(prov));
     memset(city,'\0',sizeof(city));
     memset(station,'\0',sizeof(station));

   
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
          //printf("fengyuxing7\n");
              
      sprintf(date,"%04d",atoi(temp[0]));
    		sprintf(date+4,"%02d",atoi(temp[1]));
    		sprintf(date+6,"%02d",atoi(temp[2]));
      sprintf(date+8,"%02d",atoi(temp[3]));
                 
    		// printf("%s\n",date);
      sprintf(code,"%s",temp[4]);
            sprintf(prov,"%s",temp[21]);
            sprintf(city,"%s",temp[22]);
            sprintf(station,"%s",temp[23]);
           // printf("%s\n",code);printf("%s\n",prov);printf("%s\n",city);printf("%s\n",station);
            	for(j=5;j<21;j++)
    	       {	           
    		               value[j-5]=atof(temp[j]);
    		              //printf("%f ",value[j]);
        		}
                
    		ret=insert_aqi(date,code,value,prov,city,station);
               
    		if(ret!=0)
    		{
    			printf("call insert_aqi() failed.\n");
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

