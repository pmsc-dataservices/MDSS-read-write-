#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#include "rdb_dq_c_comm.h"
#include "rdb_c_c_comm.h"

//int OutPut(char [],long [],long [],float [],char [][CF_LEN]);  
int numtime(int *ntime,time_t btime);
int date_add(char *old_date, char *new_date, int num);

#define  FDATANUM  200000   //从数据库中返回的整型数据总数：record_num*column_float_num。abwa有32个实型字段：71×400000=28400000
#define  CDATANUM  200000   ////从数据库中返回的字符型数据总数：record_num*column_char_num。abwa没有字符型数据 28×400000=
#define  IDENNUM   30000   //long ciden[],fiden[]数组大小：从数据库中返回的记录总数。一个站一年有12*3条记录。假设有10000个站：12×3×10000=360000
#define  ArrSize   100000

/*
--- 检索条件描述：检索
所有站一年的数据
--- 主函数参数描述：argc=3 argv[1]：检索时间范围下限；argv[2]：检索时间范围上限；格式：YYYYMMDD
*/

int main(int argc,char *argv[]) 
{
    int     ret;
    char    cpar[100][2048];
    long    lpar[100];
    int     ct[10], surf_start_date, surf_end_date;
    char    today[20], yesterday[20];
    
    
    
    /* get current time */
    numtime(ct, -1);
    memset(today, '\0', sizeof(today));
    memset(yesterday, '\0', sizeof(yesterday));
    sprintf(today, "%04d%02d%02d", ct[0], ct[1], ct[2]);
    date_add(today, yesterday, -24);
    
    sscanf(yesterday, "%d", &surf_start_date);
    sscanf(today, "%d", &surf_end_date);  
    
    
    printf("%d\n", surf_start_date);
    printf("%d\n", surf_end_date);

    /* allocate for arrays*/
    char * * cdata = malloc(ArrSize * CF_LEN);
    long *  ldata = malloc(ArrSize * sizeof(long));
    
/*--- 1. 字符型输入参数数组 cpar ---*/
    printf("fengyuxing1\n");
    /*
    ----- 用户信息的设置：cpar[0-1]
    ----- cpar[0]：设置检索的用户名
    ----- cpar[1]：设置检索的用户口令 
    */
    strcpy(cpar[0],"lanhb");
    strcpy(cpar[1],"lanhb090728");

    /*
    ----- 资料名称的??茫?cpar[2]
    ----- 设置检索的资料别名，均为大写 
    */
    strcpy(cpar[2],"RCAWN");
    
   
    /*
    ----- 资料类别：cpar[3-12]
    ----- cpar[3]：设置检索字符型数据的要素名称
    ----- cpar[4]：设置检索浮点型数据的要素名称
    */
       strcpy(cpar[3],"REG");
       strcpy(cpar[4],"OZO");
       strcpy(cpar[5],"NULL");

    /* 
    ----- 特殊检索条件的设置：cpar[23] 
    */
   strcpy(cpar[23],"NULL");
     /* 
    ----- 特殊文件路径：cpar[24] 
    */
   strcpy(cpar[24],"/home/nsmc_user/data/REG_cawn");
    
    /* 
    ----- cpar 数组结束标志：cpar[25] 
    */
    strcpy(cpar[25],"EOF");
    
/*--- 2. 长整型输入参数数组lpar ---*/

    /*
    ----- 检索日期的设置：lpar[0-10]
    ----- lpar[0]：设置日期的检索方式. 0表示检索所有时间的资料; 2表示某个或某几个小时的资料; 3表示检索某个时间范围的资料
    */
    /* 
    lpar[0] = 1;
    lpar[1] = surf_start_date;
    lpar[2] = surf_end_date;*/

    printf("fengyuxing3\n");

    /*lpar[0] = 1;
    lpar[1] = atoi(argv[1]);
    lpar[2] = atoi(argv[2]);*/
    
    /* 2.1 set YMD */
    lpar[0] = 2;
    lpar[1] = atoi(argv[1]);
    lpar[2] = -1;

    /* 2.2 set time */
    lpar[11] = 1;
    lpar[12] = atoi(argv[2]);
    lpar[13] = -1;

 
    
    /* 2.4 set end to lpar */
    lpar[30] = -999999;
    printf("fengyuxing4\n");

	
/*--- 3. 其它参数设置 ---*/   
    printf("fengyuxing6\n");

    /*
    ----- 
    */
    ldata[0] = ArrSize;
    ldata[1] = ArrSize;    

/*--- 4. 设置输出文件  ---*/
  
      ret = getproduct(1303, cpar, lpar, cdata, ldata);
       printf("fengyuxing8\n");
      /*printf filename and total number*/
       /*for(i=0;i<ldata[0];i++)
         {printf("cdata[%d]:%s\n",i.cdata[i]);
	  printf("total number: = %ld\n",ldata[0]);
         }*/
         printf("getgrib return error code =%d\n",ret);
        
	 printf("fengyuxing6\n");

	  free(cdata);
	  free(ldata);
	  	  
	  exit(0);
  
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

