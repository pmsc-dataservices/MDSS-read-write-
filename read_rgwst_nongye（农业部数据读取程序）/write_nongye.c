#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "public.c"

#define LINE  30000
#define COLUMN_NUM 17    //字段个数

/*
----- 主函数参数描述：argc=2 argv[1]：cr01数据文件名
*/
int main(int argc,char *argv[])
{
   
    char buf[LINE];
    char temp[COLUMN_NUM][100]; 
    char  cvalue[29][100];
    float value[COLUMN_NUM];
    char date[14],utctime[20],time[25];
    char  flag[1];
    int   ct[10],type=1;
    int  ret2;
    char file_name[50];
	char filename_path[100];
	char file_path[50];

    FILE   *fp;
    int  i,j,ret,num=0,record=0;
    FILE *hua_fp;
    
    /*
    ----- variable initialization
    */
    numtime(ct, -1);

    for(j=0;j<COLUMN_NUM;j++)
    	value[j]=0;
    memset(date,'\0',sizeof(date));
    memset(utctime,'\0',sizeof(utctime));
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

                               
    		 /*
    		--- process date. formate: YYYYMMDD
    		*/
    		memset(file_name, '\0', sizeof(file_name));
	     sprintf(file_name,"nongye_%s.dat",argv[2]);
  memset(filename_path, '\0', sizeof(filename_path));
	sprintf(filename_path,"/usr/mdss_user/data/nongye_data/%s",file_name);

	//创建实况文件
	
	if((hua_fp=fopen(filename_path,"a"))==NULL)//打开文件 没有就创建
	{
		printf("can not create awst txt file!\n");
	}
     //fprintf(hua_fp,"站号，北京观测时间，纬度，经度，海拔高度，气温，降水，蒸发量，相对湿度，风速风速，风向，地面温度\n");
	           
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
    	
                 date_add(date, utctime,8);
                //  printf("%s\n",utctime);
                
       value[0]=atof(temp[0]);
       value[1]=atof(temp[7]);
       value[2]=atof(temp[8]);
       value[3]=atof(temp[9]);
       value[4]=atof(temp[10]);
       value[5]=atof(temp[11]);
        value[6]=atof(temp[12]);
       value[7]=atof(temp[13]);
       value[8]=atof(temp[14]);
       value[9]=atof(temp[15]);
      value[10]=atof(temp[16]);
      //value[14]=atof(temp[20]);
      //value[15]=atof(temp[21]);value[16]=atof(temp[22]);value[17]=atof(temp[23]);value[18]=atof(temp[22]);value[19]=atof(temp[23]);
        
   		  fprintf(hua_fp,"%.2f,%s,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f\n",value[0],utctime,value[1],value[2],value[3],value[4],value[5],value[6],value[7],value[8],value[9],value[10]);
	                
        	}

    	memset(buf,'\0',sizeof(buf));         
        memset(temp,'\0',sizeof(temp));
    } 
    //printf("processed records: %d\n",record);
    fclose(fp); 
      fclose(hua_fp);  
          exit(0);
}

