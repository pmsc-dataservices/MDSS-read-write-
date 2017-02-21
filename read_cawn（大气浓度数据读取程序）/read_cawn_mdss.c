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

#define  FDATANUM  200000   //�����ݿ��з��ص���������������record_num*column_float_num��abwa��32��ʵ���ֶΣ�71��400000=28400000
#define  CDATANUM  200000   ////�����ݿ��з��ص��ַ�������������record_num*column_char_num��abwaû���ַ������� 28��400000=
#define  IDENNUM   30000   //long ciden[],fiden[]�����С�������ݿ��з��صļ�¼������һ��վһ����12*3����¼��������10000��վ��12��3��10000=360000
#define  ArrSize   100000

/*
--- ������������������
����վһ�������
--- ����������������argc=3 argv[1]������ʱ�䷶Χ���ޣ�argv[2]������ʱ�䷶Χ���ޣ���ʽ��YYYYMMDD
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
    
/*--- 1. �ַ�������������� cpar ---*/
    printf("fengyuxing1\n");
    /*
    ----- �û���Ϣ�����ã�cpar[0-1]
    ----- cpar[0]�����ü������û���
    ----- cpar[1]�����ü������û����� 
    */
    strcpy(cpar[0],"lanhb");
    strcpy(cpar[1],"lanhb090728");

    /*
    ----- �������Ƶ�??ã?cpar[2]
    ----- ���ü��������ϱ�������Ϊ��д 
    */
    strcpy(cpar[2],"RCAWN");
    
   
    /*
    ----- �������cpar[3-12]
    ----- cpar[3]�����ü����ַ������ݵ�Ҫ������
    ----- cpar[4]�����ü������������ݵ�Ҫ������
    */
       strcpy(cpar[3],"REG");
       strcpy(cpar[4],"OZO");
       strcpy(cpar[5],"NULL");

    /* 
    ----- ����������������ã�cpar[23] 
    */
   strcpy(cpar[23],"NULL");
     /* 
    ----- �����ļ�·����cpar[24] 
    */
   strcpy(cpar[24],"/home/nsmc_user/data/REG_cawn");
    
    /* 
    ----- cpar ���������־��cpar[25] 
    */
    strcpy(cpar[25],"EOF");
    
/*--- 2. �����������������lpar ---*/

    /*
    ----- �������ڵ����ã�lpar[0-10]
    ----- lpar[0]���������ڵļ�����ʽ. 0��ʾ��������ʱ�������; 2��ʾĳ����ĳ����Сʱ������; 3��ʾ����ĳ��ʱ�䷶Χ������
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

	
/*--- 3. ������������ ---*/   
    printf("fengyuxing6\n");

    /*
    ----- 
    */
    ldata[0] = ArrSize;
    ldata[1] = ArrSize;    

/*--- 4. ��������ļ�  ---*/
  
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

