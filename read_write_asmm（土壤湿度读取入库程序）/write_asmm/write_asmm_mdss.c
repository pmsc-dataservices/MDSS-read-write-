#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ocilib.h"
#include "public.h"
#include "public.c"
#include "ocilib.c"

#define LINE 500
#define COLUMN_NUM 30      //字段个数

/*
----- 主函数参数描述：argc=2 argv[1]：asmm数据文件名
*/
int main(int argc,char *argv[])
{
    char user[30], passwd[30], dbname[30];
    char buf[LINE];
    char temp[COLUMN_NUM][100];
    char asmm_date[13], bj_date[20];
    float value[COLUMN_NUM];
    FILE   *fp;
    int  j,ret,num=0,record=0;
    char asmm_filename[255];
    int  ct[10];
    
    memset(asmm_filename, '\0', sizeof(asmm_filename));
    if (argc==3)
    {
        sprintf(asmm_filename, "%s/data/asmm_data/asmm_%s_%s.dat", getenv("HOME"), argv[1], argv[2]);
    }
    else
    {
        numtime(ct, -1);
        sprintf(asmm_filename, "%s/data/asmm_data/asmm_%04d%02d%02d_%02d.dat", getenv("HOME"), ct[0], ct[1], ct[2], ct[3]);
    }
    
    
    /*
    ----- variable initialization
    */
    
    for(j=0;j<COLUMN_NUM;j++)
    	  value[j]=0;
      
    memset(temp,'\0',sizeof(temp));
    memset(buf,'\0',sizeof(buf));
    
    memset(user, '\0', sizeof(user));
    memset(passwd, '\0', sizeof(passwd));
    memset(dbname, '\0', sizeof(dbname));
    
//    strcpy(user, "process_user");
//    strcpy(passwd, "Cb5WU3By");
//    strcpy(dbname, "productdb_rac");

    strcpy(user, "process_user");
    strcpy(passwd, "process_user123");
    strcpy(dbname, "inner_database");
	
	/* 
    -----1. connect database 
    */
    ret = logon(user, passwd, dbname);
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
		
		fp = fopen(asmm_filename,"r");
    if (fp == NULL)
    { 
    	printf("open file error: %s\n",argv[1]);
      exit(-1);
    }
    
    /*
    -----3. process datafile
    */
    
    while(fgets(buf,LINE,fp))
    {
    	/*
    	--- process a line
    	*/
    	stringrtrim(buf);
    	num=0;
    	//printf("%s\n",buf);
    	num=stringsplit(buf,' ',COLUMN_NUM,temp);  			
			//printf("%d\n",num);
			
    	if(num==COLUMN_NUM)
    	{
    		/*
    		--- process date. formate: YYYYMMDD
    		*/
    		memset(asmm_date, '\0', sizeof(asmm_date));
    		memset(bj_date, '\0', sizeof(bj_date));
    		sprintf(asmm_date,"%04d",atoi(temp[4]));
    		sprintf(asmm_date+4,"%02d",atoi(temp[5]));
    		sprintf(asmm_date+6,"%02d",atoi(temp[6]));
    		sprintf(asmm_date+8,"%02d",atoi(temp[7]));
    		sprintf(asmm_date+10,"%02d",atoi(temp[8]));
      date_add(asmm_date, bj_date, 8);
    		strcat(bj_date, "00");
    		
    		for(j=0;j<COLUMN_NUM;j++)
    	  {
    		   value[j]=atof(temp[j]);
    		}
    		
    		ret=insert_asmm(bj_date, value);
    		if(ret!=0)
    		{
    			printf("call insert_abwa() failed.\n");
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

