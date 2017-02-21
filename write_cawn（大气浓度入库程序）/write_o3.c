#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ocilib.h"
#include "public.h"

#define LINE  1000
#define COLUMN_NUM 17     //字段个数

/*
----- 主函数参数描述：argc=2 argv[1]：cr01数据文件名
*/
int main(int argc,char *argv[])
{
    char user[30], passwd[30], dbname[30];
    char buf[LINE];
    char temp[COLUMN_NUM][1000];
    char data_dir[50],data_file[80],data_file1[80]; 
    char date[14],time[25];
    char  flag[1];
    int   ct[10],type=1;
    float value[COLUMN_NUM];
    FILE   *fp;
    int  j,ret,num=0,record=0,count=0;
    
    /* 
     * 1.1 读取o3
     */
    numtime(ct, -1);
    memset(data_dir, '\0', sizeof(data_dir));
    memset(data_file, '\0', sizeof(data_file));
    memset(date,'\0',sizeof(date));
    sprintf(date,"%4d%02d%02d%02d00",ct[0],ct[1],ct[2],ct[3]);
    sprintf(data_dir, "/home/nsmc_user/data/cawn_mul");
    sprintf(data_file, "%s/MSP3_PMSC_ENVACM_O3_L88_CHN_%s.TXT",data_dir,date);

         
    
    /*
    ----- variable initialization
    */
    

    for(j=0;j<COLUMN_NUM;j++)
    	value[j]=0;
    
    memset(time,'\0',sizeof(time));
    memset(temp,'\0',sizeof(temp));
    memset(buf,'\0',sizeof(buf));
     

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
        //printf("%d\n",num);
			
	//printf("fengyuxing6\n");
		
    	if(num==COLUMN_NUM)
    	{                            
                value[0]=atof(temp[0]);
                value[1]=atof(temp[4]);
                value[2]=atof(temp[5]);
                value[3]=atof(temp[6]);
                value[4]=atof(temp[9]);
                value[5]=atof(temp[10]);
                 value[6]=atof(temp[11]);
                value[7]=atof(temp[12]);

               // printf("%f\n",value[0]);
    		count++;
    		//printf("\n");
               ret = read_o3(data_dir,data_file,value);
              
    	}

    	memset(buf,'\0',sizeof(buf));         
        memset(temp,'\0',sizeof(temp));
    }
    
    printf("processed records: %d\n",count);
    fclose(fp);        
    
    exit(0);
}

