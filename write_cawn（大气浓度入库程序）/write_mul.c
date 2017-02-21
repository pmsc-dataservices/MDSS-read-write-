#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ocilib.h"
#include "public.h"

#define LINE  1000
#define COLUMN_NUM 35
#define COLUMN_NUMS 26     //字段个数

/*
----- 主函数参数描述：argc=2 argv[1]：cr01数据文件名
*/
int main(int argc,char *argv[])
{
    char user[30], passwd[30], dbname[30];
    char buf[LINE];
    char temp[COLUMN_NUM][1000];
    char data_dir[50],data_file[80],data_file1[80],data_file2[80],data_file3[80],data_file4[80]; 
    char date[14],time[25];
    char  flag[1];
    int   ct[10],type=1;
    float value[COLUMN_NUM];
    FILE   *fp;
    int  j,ret,num=0,count1=0,count2=0,count3=0,count4=0;    
    /* 
     * 1.1 读取pm2.5数据
     */
    numtime(ct, -1);
    memset(data_dir, '\0', sizeof(data_dir));
  

    memset(data_file1, '\0', sizeof(data_file1));
     memset(data_file2, '\0', sizeof(data_file2));
     memset(data_file3, '\0', sizeof(data_file3));
     memset(data_file4, '\0', sizeof(data_file4));

    memset(date,'\0',sizeof(date));
    sprintf(date,"%4d%02d%02d%02d00",ct[0],ct[1],ct[2],ct[3]);
    sprintf(data_dir, "/home/nsmc_user/data/cawn_mul");
  
    sprintf(data_file1, "%s/MSP3_PMSC_ENVACM_NO2_L88_CHN_%s.TXT",data_dir,date);
    sprintf(data_file2, "%s/MSP3_PMSC_ENVACM_SO2_L88_CHN_%s.TXT",data_dir,date);
    sprintf(data_file3, "%s/MSP3_PMSC_ENVACM_CO_L88_CHN_%s.TXT",data_dir,date);
    sprintf(data_file4, "%s/MSP3_PMSC_ENVACM_O3_L88_CHN_%s.TXT",data_dir,date);


      
    
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
                value[3]=atof(temp[9]);
                value[4]=atof(temp[13]);
                value[5]=atof(temp[17]);
                value[6]=atof(temp[19]);
                value[7]=atof(temp[21]);
                value[8]=atof(temp[23]);
                value[9]=atof(temp[25]);
                value[10]=atof(temp[27]);
                value[11]=atof(temp[15]);
                value[12]=atof(temp[22]);
                value[13]=atof(temp[28]);
               // printf("%f\n",value[27]);
    		count1++;
    		//printf("\n");
               ret = read_58448(data_dir,data_file1,data_file2,data_file3,data_file4,value);
              
    	}

    	memset(buf,'\0',sizeof(buf));         
        memset(temp,'\0',sizeof(temp));
    }
    
    printf("processed records: %d\n",count1);
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
                value[3]=atof(temp[9]);
                value[4]=atof(temp[13]);
                value[5]=atof(temp[17]);
                value[6]=atof(temp[19]);
                value[7]=atof(temp[21]);
                value[8]=atof(temp[23]);
                value[9]=atof(temp[25]);
                value[10]=atof(temp[27]);
                value[11]=atof(temp[15]);
                value[12]=atof(temp[22]);
                value[13]=atof(temp[28]);

               // printf("%f\n",value[0]);
    		count2++;
    		//printf("\n");
               ret = read_54421(data_dir,data_file1,data_file2,data_file3,data_file4,value);
              
    	}

    	memset(buf,'\0',sizeof(buf));         
        memset(temp,'\0',sizeof(temp));
    }
    
    printf("processed records: %d\n",count2);
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
    	
    	/*
    	--- process a line
    	*/
    	num=0;
    	//printf("%s\n",buf);
    	num=stringsplit(buf,',',COLUMN_NUMS,temp);  			
        //printf("%d\n",num);
			
	//printf("fengyuxing6\n");
		
    	if(num==COLUMN_NUMS)
    	{                            
                value[0]=atof(temp[0]);
                value[1]=atof(temp[4]);
                value[2]=atof(temp[9]);
                value[3]=atof(temp[15]);
                value[4]=atof(temp[18]);
                value[5]=atof(temp[21]);

               // printf("%f\n",value[0]);
    		count3++;
    		//printf("\n");
               ret = read_51058(data_dir,data_file1,data_file2,data_file3,data_file4,value);
              
    	}

    	memset(buf,'\0',sizeof(buf));         
        memset(temp,'\0',sizeof(temp));
    }
    
    printf("processed records: %d\n",count3);
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
    	
    	/*
    	--- process a line
    	*/
    	num=0;
    	//printf("%s\n",buf);
    	num=stringsplit(buf,',',COLUMN_NUMS,temp);  			
        //printf("%d\n",num);
			
	//printf("fengyuxing6\n");
		
    	if(num==COLUMN_NUMS)
    	{                            
                value[0]=atof(temp[0]);
                value[1]=atof(temp[4]);
                value[2]=atof(temp[9]);
                value[3]=atof(temp[15]);
                value[4]=atof(temp[18]);
                value[5]=atof(temp[21]);
               // printf("%f\n",value[0]);
    		count4++;
    		//printf("\n");
               ret = read_54084(data_dir,data_file1,data_file2,data_file3,data_file4,value);
              
    	}

    	memset(buf,'\0',sizeof(buf));         
        memset(temp,'\0',sizeof(temp));
    }
    
    printf("processed records: %d\n",count4);
    fclose(fp);        
      

    exit(0);
}

