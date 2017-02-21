#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include "rdb_dq_c_comm.h"
#include "rdb_c_c_comm.h"

int OutPut(char [],long [],long [],float [],char [][CF_LEN]);  
int numtime(int *ntime,time_t btime);
int date_add(char *old_date, char *new_date, int num);

#define  FDATANUM  10000000   //从数据库中返回的整型数据总数：record_num*column_float_num
#define  CDATANUM  500000   //从数据库中返回的字符型数据总数：record_num*column_char_num
#define  IDENNUM   50000   //long ciden[],fiden[]数组大小：从数据库中返回的记录总数

#define C_COLUMN 5         //字符型字段数
//#define F_COLUMN 28        //实型字段数
#define F_COLUMN 32        //实型字段数

//检索10000个站整点（某个时次）一个月，有10000*30=300000条记录。

/*
--- 检索条件描述：检索一个自然月的数据
--- 主函数参数描述：argc=5 
argv[1]：检索日期范围下限；argv[2]：检索日期范围上限；格式：YYYYMMDD 
argv[3]；小时值；格式：HH 
argv[4] 站号 格式：FFFFF
*/


//executeprogram 20080101 20101231 12 54511
int main(int argc,char *argv[]) 
{
    int     ret;
    char    cpar[20][2048];
    long    lpar[100];
    char    output[255];
    int     ct[10], temp_start_date, temp_end_date;
    char    today[20], yesterday[20];
    
    
    
    /* get current time */
    numtime(ct, -1);
    memset(today, '\0', sizeof(today));
    memset(yesterday, '\0', sizeof(yesterday));
    sprintf(today, "%04d%02d%02d", ct[0], ct[1], ct[2]);
    date_add(today, yesterday, -24);
    
    sscanf(yesterday, "%d", &temp_start_date);
    sscanf(today, "%d", &temp_end_date);  
    
    //printf("%d\n", temp_start_date);
    //printf("%d\n", temp_end_date);

    /* allocate for arrays*/
    char **cdata = malloc(CDATANUM * CF_LEN);
    float *fdata = malloc(FDATANUM * sizeof(float));
    long *ciden = malloc(IDENNUM * sizeof(long));
    long *fiden = malloc(IDENNUM * sizeof(long));

/*--- 1. 字符型输入参数数组 cpar ---*/

    /*
    ----- 用户信息的设置：cpar[0-1]
    ----- cpar[0]：设置?焖鞯挠没?
    ----- cpar[1]：设置检索的用户口令 
    */
    strcpy(cpar[0],"lanhb");
    strcpy(cpar[1],"lanhb090728");

    /*
    ----- 资料名称的设置： cpar[2]
    ----- 设置检索的资料别名，均为大写 
    */
    strcpy(cpar[2],"RLBDM");


    /*
    ----- 检索要素的设置：cpar[3-4]
    ----- cpar[3]：设置检索字符型数据的要素名称
    ----- cpar[4]：设置检索浮点型数据的要素名称
    */
    strcpy(cpar[3],"ALL");

    //strcpy(cpar[3],"NULL");
    strcpy(cpar[4],"ALL-F");
    

    /* 
    ----- 特殊检索条件的设置：cpar[5] 
    */
    strcpy(cpar[5],"NULL");
    //strcpy(cpar[5],"V01000 >0"); 

    /* 
    ----- cpar 数组结束标志：cpar[6] 
    */
    strcpy(cpar[6],"EOF");

/*--- 2. 长整型输入参数数组lpar ---*/

    /*
    ----- 检索日期的设置：lpar[0-10]
    ----- lpar[0]：设置日期的检索方式. 1表示连续日期范围的检索（检索范围包含边界值）; 2表示离散日期的检索; 3表示不连续日期范围的检索
    */


	
    lpar[0] = 2;
    lpar[1] = atoi(argv[1]);
    lpar[2] = -1;
    

    /* 
    ----- 检索时间的设置：lpar[11-20]
    ----- lpar[11]：设置日期的检索条件. 0表示检索所有时间的数据; 1表示检索某个或几个观测时次的数据; 2表示检索某个观测时间段的数据
    */
	//只检索00、12两个时次的数据
    //lpar[11] = 0;    

    lpar[11] = 1;    
    lpar[12] = atoi(argv[2]);     
    lpar[13] = -1;    

	/* 
	----- 检索区域的设置：lpar[21-50]
	----- lpar[21]：设置区域检索条件, 0表示检索所有区域的数据; 1表示检索某个经纬度范围的数据; 2表示检索某区站号范围的数据; 3表示检索某些区站号的数据
	*/
	lpar[21] = 0;
	//区号范围在50000 到 60000 之间

/*	
	lpar[21] = 2;
	lpar[22] = 50000;
	lpar[23] = 60000;
*/	 
	


	
	/* 
    ----- lpar 数组结束标记设置
    */
    lpar[26] = -999999;
    
    
/*--- 3. 其它参数设置 ---*/   

    /*
    ----- 
    */
    ciden[0] = CDATANUM;
    ciden[1] = IDENNUM;
    fiden[0] = FDATANUM;
    fiden[1] = IDENNUM;

/*--- 4. 设置输出文件  ---*/
    sprintf(output, "%s/data/temp_data/lbdm_%s_%s.dat", getenv("HOME"),argv[1],argv[2]);
	printf("call elemsea() function, please wait......\n");
    ret = elemsea(1102, cpar, lpar, cdata, fdata, ciden, fiden);
	  printf("elemsea return code = %d\n",ret);
	  ret = OutPut(output,fiden,ciden,fdata,cdata);
	  
	  free(cdata);
	  free(fdata);
	  free(ciden);
	  free(fiden);
	  
	  exit(0);
}


int OutPut(char filename[],long fiden[],long ciden[],float fdata[],char cdata[][CF_LEN])
{
    long    fdata_num;
    long    cdata_num;
    FILE   *fp;
    int     i, j;
    //char CYMD[9],CHMS[7],CHM[5],*CID;
    //int decpt,sign;

    fdata_num=0;
    cdata_num=0;

    fp = fopen(filename,"w");
    if (fp == NULL) 
        return -1;

    //504条记录
    for (i=0; i<fiden[2]; i++) //fiden[2]检索到的记录个数
    {
           //数据的个数,即字段数
 			for (j=0; j<fiden[i+4]; j++) 
			//fiden[4]以后为每个测站的数据的个数, 即fiden[4]为第一个测站的数据个数,fiden[5]为第二个测站的数据个数.
 			{
	    	   fprintf(fp," %f ",fdata[j+fdata_num]);	
			//fdata[0],fdata[1],fdata[2]................
			}
			fdata_num += fiden[i+4];
			
			for(j=0; j<ciden[i+4]; j++) //每条记录字符型数据总个数
			{

	    	   fprintf(fp,"  %s ",cdata[j+cdata_num]);
			}
            cdata_num += ciden[i+4];

			fprintf(fp,"\n");
			fprintf(fp,"\n");

    }
	//printf("fiden[2] = %ld\n",fiden[2]);
	printf("fdata_num = %ld\n",fdata_num);
	printf("cdata_num = %ld\n",cdata_num);
    fclose(fp);

    return 0;
}
																										

/******************************************************************/
/* function : numtime()                                           */
/******************************************************************/
int numtime(int *ntime,time_t btime)
{
    struct tm *tblock;

    /* the value of btime is false */ 
    if( btime < -1 )
	return -1;

    /* get current time */ 
    if( btime == -1 )
	btime = time(NULL);

    /* convert time_t to struct tm */
    tblock = localtime( &btime );

    ntime[0] = tblock->tm_year+1900;
    ntime[1] = tblock->tm_mon+1;
    ntime[2] = tblock->tm_mday; 
    ntime[3] = tblock->tm_hour; 
    ntime[4] = tblock->tm_min; 
    ntime[5] = tblock->tm_sec; 

    return 0;
}

/******************************************************************/
/* function : date_add()                                          */
/******************************************************************/
int date_add(char *old_date, char *new_date, int num)
{
     int    old_year, old_month, old_day;
     int    new_year, new_month, new_day;
     struct tm *old_tmpt, old_tm, *new_tmpt;
     time_t time_num=0;
     char   datestring[13];

     sscanf(old_date, "%4d%2d%2d", &old_year, &old_month, &old_day);

     old_tmpt = &old_tm;
     old_tmpt->tm_year=(old_year-1900);
     old_tmpt->tm_mon =(old_month-1);
     old_tmpt->tm_mday=old_day;
     old_tmpt->tm_hour=0;
     old_tmpt->tm_min =0;
     old_tmpt->tm_sec =0;

     time_num=mktime(old_tmpt);

     time_num=(time_num+num*3600);

     new_tmpt = localtime(&time_num);

     new_year  = (new_tmpt->tm_year + 1900);
     new_month = (new_tmpt->tm_mon + 1);
     new_day   = (new_tmpt->tm_mday);

     memset(datestring, '\0', sizeof(datestring));
     sprintf(datestring, "%04d%02d%02d", new_year, new_month, new_day);
     strcpy(new_date, datestring);

     return 0;
}
