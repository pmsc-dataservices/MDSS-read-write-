#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ocilib.h"
#include "public.h"

#define LINE  1000
#define COLUMN_NUM 35  
#define COLUMN_NUMS 26    //字段个数

/*
----- 主函数参数描述：argc=2 argv[1]：cr01数据文件名
*/
int main(int argc,char *argv[])
{
    char user[30], passwd[30], dbname[30];
    char buf[LINE];
    char temp[COLUMN_NUM][1000];
    char data_dir[50],data_file[80],data_file1[80],data_file2[80],data_file3[80]; 
    char date[14],time[25];
    char  flag[1];
    int   ct[10],type=1;
    float value[COLUMN_NUM];
    FILE   *fp;
    int  j,ret,num=0,record=0,record1=0,record2=0,record3=0,record4=0,count=0;
    
    /* 
     * 1. 读取pm数据
     */
    numtime(ct, -1);
    memset(data_dir, '\0', sizeof(data_dir));
  

    memset(data_file1, '\0', sizeof(data_file1));
     memset(data_file2, '\0', sizeof(data_file2));
     memset(data_file3, '\0', sizeof(data_file3));
    memset(date,'\0',sizeof(date));
    sprintf(date,"%4d%02d%02d%02d00",ct[0],ct[1],ct[2],ct[3]);
    sprintf(data_dir, "/home/nsmc_user/data/cawn_mul");
  
    sprintf(data_file1, "%s/MSP3_PMSC_ENVACM_NO2_L88_CHN_%s.TXT",data_dir,date);
    sprintf(data_file2, "%s/MSP3_PMSC_ENVACM_SO2_L88_CHN_%s.TXT",data_dir,date);
    sprintf(data_file3, "%s/MSP3_PMSC_ENVACM_CO_L88_CHN_%s.TXT",data_dir,date);

     
    
    /*
    ----- variable initialization
    */
    

    for(j=0;j<COLUMN_NUM;j++)
    	value[j]=0;
    
    memset(time,'\0',sizeof(time));
    memset(temp,'\0',sizeof(temp));
    memset(buf,'\0',sizeof(buf));
     

    /*
    -----2.1 open datafile
     */
	//printf("fengyuxing4\n");
	
    fp = fopen(argv[1],"r");
    if (fp == NULL)
    { 
    	printf("open file error: %s\n",argv[1]);
      exit(-1);
    }
    
    
    /*
    -----2.2 process datafile
    */    
    //printf("fengyuxing5\n");
    while(fgets(buf,LINE,fp))
        {
              num=0;
    	//printf("%s\n",buf);
    	num=stringsplit(buf,',',COLUMN_NUM,temp);  			
               // printf("%d\n",num);
			
	//printf("fengyuxing6\n");
		
    	if(num==COLUMN_NUM)
                {
                     record1++;
                }
          memset(buf,'\0',sizeof(buf));         
          memset(temp,'\0',sizeof(temp));
         }
         printf("processed records: %d\n",record1);
        fclose(fp);   
    /*
    -----3.1 open datafile
     */
	//printf("fengyuxing4\n");
	
    fp = fopen(argv[2],"r");
    if (fp == NULL)
    { 
    	printf("open file error: %s\n",argv[2]);
      exit(-1);
    }
    
    
    /*
    -----3.2 process datafile
    */    
    //printf("fengyuxing5\n");
    while(fgets(buf,LINE,fp))
        {
              num=0;
    	//printf("%s\n",buf);
    	num=stringsplit(buf,',',COLUMN_NUM,temp);  			
               // printf("%d\n",num);
			
	//printf("fengyuxing6\n");
		
    	if(num==COLUMN_NUM)
                {
                     record2++;
                }
          memset(buf,'\0',sizeof(buf));         
          memset(temp,'\0',sizeof(temp));
         }
         printf("processed records: %d\n",record2);
        fclose(fp);  
       /*
    -----4.1 open datafile
     */
	//printf("fengyuxing4\n");
	
    fp = fopen(argv[3],"r");
    if (fp == NULL)
    { 
    	printf("open file error: %s\n",argv[3]);
      exit(-1);
    }
    
    
    /*
    -----4.2 process datafile
    */    
    //printf("fengyuxing5\n");
    while(fgets(buf,LINE,fp))
        {
              num=0;
    	//printf("%s\n",buf);
    	num=stringsplit(buf,',',COLUMN_NUMS,temp);  			
               // printf("%d\n",num);
			
	//printf("fengyuxing6\n");
		
    	if(num==COLUMN_NUMS)
                {
                     record3++;
                }
          memset(buf,'\0',sizeof(buf));         
          memset(temp,'\0',sizeof(temp));
         }
         printf("processed records: %d\n",record3);
        fclose(fp);   
    /*
    -----5.1 open datafile
     */
	//printf("fengyuxing4\n");
	
    fp = fopen(argv[4],"r");
    if (fp == NULL)
    { 
    	printf("open file error: %s\n",argv[4]);
      exit(-1);
    }
    
    
    /*
    -----5.2 process datafile
    */    
    //printf("fengyuxing5\n");
    while(fgets(buf,LINE,fp))
        {
              num=0;
    	//printf("%s\n",buf);
    	num=stringsplit(buf,',',COLUMN_NUMS,temp);  			
               // printf("%d\n",num);
			
	//printf("fengyuxing6\n");
		
    	if(num==COLUMN_NUMS)
                {
                     record4++;
                }
          memset(buf,'\0',sizeof(buf));         
          memset(temp,'\0',sizeof(temp));
         }
         printf("processed records: %d\n",record4);
        fclose(fp);        
      
        record=record1+record2+record3+record4;   

        ret = read_mul_record(data_dir, data_file1,data_file2,data_file3,date);
        
    exit(0);
}

