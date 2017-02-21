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

#define  FDATANUM  36000000    //从数据库中返回的整型数据总数：record_num*column_float_num。asmf有90个实型字段：90×40000=36000000
#define  CDATANUM    800000    //从数据库中返回的字符型数据总数：record_num*column_char_num。asmf有2个实型字段：2×40000=800000
#define  IDENNUM     400000    //long ciden[],fiden[]数组大小：从数据库中返回的记录总数。一个站一年有12*3条记录。假设有1000个站：12×3×10000=360000

#define C_COLUMN 2         //字符型字段数
#define F_COLUMN 91        //实型字段数

/*
--- 检索条件描述：检索当前日期，(当前时次-1)的所有数据
--- 主函数参数描述：argc=3 argv[1]：检索日期；格式：YYYYMMDD argv[2]：时次 格式：HH
*/

int main(int argc,char *argv[]) 
{
    int     ret;
    char    cpar[20][2048];
    long    lpar[100];
    char    output[255];
    int     ct[10], asmm_date, asmm_time, temp_time;
    char    asmmdatetime[20], bjdate[10], bjtime[10], bjdatetime[20];
    
    /* get current time */
    memset(bjdatetime, '\0', sizeof(bjdatetime));
    memset(bjdate, '\0', sizeof(bjdate));
    memset(bjtime, '\0', sizeof(bjtime));
    if (argc==3)
    {
        strncpy(bjdate,argv[1],8);
        strncpy(bjtime,argv[2],2);
        sprintf(bjdatetime, "%s%s0000", bjdate, bjtime);
    }
    else
    {
       numtime(ct, -1);
       sprintf(bjdate, "%04d%02d%02d", ct[0], ct[1], ct[2]);
       sprintf(bjtime, "%02d", ct[3]);
       sprintf(bjdatetime, "%04d%02d%02d%02d0000", ct[0], ct[1], ct[2], ct[3]);
    }
    date_add(bjdatetime, asmmdatetime, -8);
    
    sscanf(asmmdatetime, "%8d%2d%4d", &asmm_date, &asmm_time, &temp_time);
    
    /* allocate for arrays*/
    char * * cdata = malloc(CDATANUM * CF_LEN);
    float * fdata = malloc(FDATANUM * sizeof(float));
    long *  ciden = malloc(IDENNUM * sizeof(long));
    long *  fiden = malloc(IDENNUM * sizeof(long));

/*--- 1. 字符型输入参数数组 cpar ---*/

    /*
    ----- 用户信息的设置：cpar[0-1]
    ----- cpar[0]：设置检索的用户名
    ----- cpar[1]：设置检索的用户口令 
    */
    strcpy(cpar[0],"lanhb");
    strcpy(cpar[1],"lanhb090728");

    /*
    ----- 资料名称的设置： cpar[2]
    ----- 设置检索的资料别名，均为大写 
    */
    strcpy(cpar[2],"RASMM");

    /*
    ----- 检索要素的设置：cpar[3-4]
    ----- cpar[3]：设置检索字符型数据的要素名称
    ----- cpar[4]：设置检索浮点型数据的要素名称
    */
    strcpy(cpar[3],"ALL");
    /* /// LYM
    strcpy(cpar[4],"ALL-F");
    */
    strcpy(cpar[4],"V01000,V05001,V06001,V07001,V04001,V04002,V04003,V04004,V04005,V04006,V56208_010,V56208_020,V56208_030,V56208_040,V56208_050,V56208_060,V56208_070,V56208_080,V56208_090,V56208_100,V56208_110,V56208_120,V56208_130,V56208_140,V56208_150,V56208_160,V56208_170,V56208_180,V56208_190,V56208_200");

    /* ----- 特殊检索条件的设置：cpar[5] */ 
    strcpy(cpar[5],"NULL");

    /* 
    ----- cpar 数组结束标志：cpar[6] 
    */
    strcpy(cpar[6],"EOF");

/*--- 2. 长整型输入参数数组lpar ---*/

    /*
    ----- 检索日期的设置：lpar[0-10]
    ----- lpar[0]：设置日期的检索方式. 1表示连续日期范围的检索（检索范围包含边界值）; 2表示离散日期的检索; 3表示不连续日期范围的检索
    */
    /* /// LYM
    lpar[0] = 1;
    lpar[1] = 20110210;
    lpar[2] = 20110212;
    */
    lpar[0] = 2;
    lpar[1] = asmm_date;
    lpar[2] = -1;
    
    /* 
    ----- 检索时间的设置：lpar[11-20]
    ----- lpar[11]：设置日期的检索条件. 0表示检索所有时间的数据; 1表示检索某个或几个观测时次的数据; 2表示检索某个观测时间段的数据
    */
    /* /// LYM
    lpar[11] = 0;
    lpar[12] = 2;
    lpar[13] = -1;
    */
    lpar[11] = 3;
    lpar[12] = asmm_time;
    lpar[13] = -1;
	  /* 
	  ----- 检索区域的设置：lpar[21-50]
	  ----- lpar[21]：设置区域检索条件, 0表示检索所有区域的数据; 1表示检索某个经纬度范围的数据; 2表示检索某区站号范围的数据; 3表示检索某些区站号的数据 
	  */
	  lpar[21] = 0;

    /* 
    ----- lpar 数组结束标记设置 :lpar[51]
    */
    lpar[51] = -999999;
    
    
/*--- 3. 其它参数设置 ---*/   

    /*
    ----- 
    */
    ciden[0] = CDATANUM;
    ciden[1] = IDENNUM;
    fiden[0] = FDATANUM;
    fiden[1] = IDENNUM;

/*--- 4. 设置输出文件  ---*/
    sprintf(output, "%s/data/asmm_data/asmm_%s_%s.dat", getenv("HOME"),bjdate,bjtime);

printf("%s\n", output);
    ret = elemsea(1107, cpar, lpar, cdata, fdata, ciden, fiden);
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

    fdata_num=0;
    cdata_num=0;

    fp = fopen(filename,"w");
    if (fp == NULL) 
        return -1;

    for (i=0; i<fiden[2]; i++) //检索到的记录个数
    {
    	/*
			for(j=0; j<ciden[i+4]; j++) //每条记录字符型数据总个数
			{
	    	fprintf(fp,"  %s ",cdata[j+cdata_num]);
			}
      cdata_num += ciden[i+4];*/

 			for (j=0; j<fiden[i+4]; j++) //每条记录实型数据总个数 
 			{
	    	fprintf(fp,"%.3f ",fdata[j+fdata_num]);	
			}
			fdata_num += fiden[i+4];
			
			fprintf(fp,"\n");
    }

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
     int    old_year, old_month, old_day, old_hour, old_minutes, old_seconds;
     int    new_year, new_month, new_day, new_hour, new_minutes, new_seconds;
     struct tm *old_tmpt, old_tm, *new_tmpt;
     time_t time_num=0;
     char   datestring[15];

     sscanf(old_date, "%4d%2d%2d%2d%2d%2d", &old_year, &old_month, &old_day, &old_hour, &old_minutes, &old_seconds);

     old_tmpt = &old_tm;
     old_tmpt->tm_year=(old_year-1900);
     old_tmpt->tm_mon =(old_month-1);
     old_tmpt->tm_mday=old_day;
     old_tmpt->tm_hour=old_hour;
     old_tmpt->tm_min =old_minutes;
     old_tmpt->tm_sec =0;

     time_num=mktime(old_tmpt);

     time_num=(time_num+num*3600);

     new_tmpt = localtime(&time_num);

     new_year  = (new_tmpt->tm_year + 1900);
     new_month = (new_tmpt->tm_mon + 1);
     new_day   = (new_tmpt->tm_mday);
     new_hour  = (new_tmpt->tm_hour);
     new_minutes = (new_tmpt->tm_min);

     memset(datestring, '\0', sizeof(datestring));
     sprintf(datestring, "%04d%02d%02d%02d%02d00", new_year, new_month, new_day, new_hour, new_minutes);
     strcpy(new_date, datestring);

     return 0;
}
