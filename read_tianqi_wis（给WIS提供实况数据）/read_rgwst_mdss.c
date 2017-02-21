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

#define  FDATANUM  1500000000   //�����ݿ��з��ص���������������record_num*column_float_num��abwa��32��ʵ���ֶΣ�45��400000=28400000
#define  CDATANUM 5000000   ////�����ݿ��з��ص��ַ�������������record_num*column_char_num��abwaû���ַ������� 28��400000=
#define  IDENNUM   50000000   //long ciden[],fiden[]�����С�������ݿ��з��صļ�¼������һ��վһ����12*3����¼��������10000��վ��12��3��10000=360000

/*
--- ������������������
����վһ�������
--- ����������������argc=3 argv[1]������ʱ�䷶Χ���ޣ�argv[2]������ʱ�䷶Χ���ޣ���ʽ��YYYYMMDD
*/

int main(int argc,char *argv[]) 
{
    int     ret;
    char    cpar[20][2048];
    long    lpar[100];
    char    output[255];
    char        select_time[300];
    int     ct[10], surf_start_date, surf_end_date;
    char    today[20], yesterday[20];
    char logname[255];
    FILE *fplog;
    
    
    /* get current time */
    numtime(ct, -1);
    memset(select_time, '\0', sizeof(select_time));
    memset(logname,'\0',sizeof(logname));
    //sprintf(logname, "%s/log/product_awst_log/awst_%04d%02d%02d.log", getenv("HOME"),ct[0],ct[1],ct[2]);
    /*if ((fplog=fopen(logname,"a+")) == NULL) 
    {
         printf("cannot open %s\n", logname);
         exit(-1);
    }*/
    
    /* allocate for arrays*/
    char * * cdata = malloc(CDATANUM * CF_LEN);
    float * fdata = malloc(FDATANUM * sizeof(float));
    long *  ciden = malloc(IDENNUM * sizeof(long));
    long *  fiden = malloc(IDENNUM * sizeof(long));
    
/*--- 1. �ַ�������������� cpar ---*/
    
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
    strcpy(cpar[2],"RGWST");
        
   
    /*
    ----- ����Ҫ�ص����ã�cpar[3-4]
    ----- cpar[3]�����ü����ַ������ݵ�Ҫ������
    ----- cpar[4]�����ü������������ݵ�Ҫ������
    */
    strcpy(cpar[3],"NULL");
    strcpy(cpar[4],"V01000,V04001,V04002,V04003,V04004,V04005,V04006,V20003,V20010,V20051,V20011");
   // strcpy(cpar[3],"ALL");
   // strcpy(cpar[4],"V01000,V04001,V04002,V04003,V04004,V04005,V04006,V13019");

    /* 
    ----- ����������������ã�cpar[5] 
    */
  // sprintf(select_time,"C_IYMDHM between TO_DATE('%s','yyyymmddhh24miss') and TO_DATE('%s','yyyymmddhh24miss')",argv[2],argv[3]);

   //strcpy(cpar[5],"NULL");
   strcpy(cpar[5],"V04005=0  and  V04006=0  ");
    
    /* 
    ----- cpar ���������־��cpar[6] 
    */
    strcpy(cpar[6],"EOF");
    
/*--- 2. �����������������lpar ---*/

    /*
    ----- �������ڵ����ã�lpar[0-10]
    ----- lpar[0]���������ڵļ�����ʽ. 1��ʾ������Ѯ���Ϸ�Χ������������Χ�����߽�ֵ��; 2��ʾ��ɢѮ���ϼ���; 3��ʾ������Ѯ���ڷ�Χ����
    */

     /* 2.1 set YMD */
    lpar[0] = 2;
    lpar[1] = atoi(argv[1]);
    lpar[2] = -1;

    /* 2.2 set time */
     lpar[11] = 3;
    lpar[12] = atoi(argv[2]);
    lpar[13] = -1;

    lpar[21] = 0;
    
    /* 2.4 set end to lpar */
    lpar[51] = -999999;
    

	 /*--- 3. ������?����?---*/   
    

    /*
    ----- 
    */
    ciden[0] = CDATANUM;
    ciden[1] = IDENNUM;
    fiden[0] = FDATANUM;
    fiden[1] = IDENNUM;
    
/*--- 4. ��������ļ�  ---*/
    sprintf(output,  "%s/data/tianqi_wis/rgwst_%s_%s.txt", getenv("HOME"),argv[1],argv[2]);
     

       ret = elemsea(1107, cpar, lpar, cdata, fdata, ciden, fiden);
   fprintf(fplog, "************* Begin a new query ***************\n");
   fprintf(fplog, "# begin to query   {%04d-%02d-%02d %02d:%02d:%02d}    #\n", ct[0], ct[1], ct[2], ct[3], ct[4], ct[5]);
   fprintf(fplog,"elemsea return code = %d\n",ret);

	  printf("elemsea return code = %d\n",ret);
	  ret = OutPut(output,fiden,ciden,fdata,cdata);
     if(ret!=0)
    		{
       fprintf(fplog,"call rdb_rac failed.\n");
    		}
     else
           {
             fprintf(fplog,"call rdb_rac  success.\n");

           }

   fprintf(fplog, "# query is over.  {%04d-%02d-%02d %02d:%02d:%02d}    #\n\n\n", ct[0], ct[1], ct[2], ct[3], ct[4], ct[5]);

	  free(cdata);
	  free(fdata);
	  free(ciden);
	  free(fiden);

	  fclose(fplog);
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

    for (i=0; i<fiden[2]; i++) //�������ļ�¼����
    {

 			for (j=0; j<fiden[i+4]; j++) //ÿ����¼ʵ�������ܸ��� 
 			{
	    	if(j%46==0 && j!=0)
	    		fprintf(fp,"\n");
	    	fprintf(fp," %f ",fdata[j+fdata_num]);	
			}
			fdata_num += fiden[i+4];
			
			//fprintf(fp,"\n");

			fprintf(fp,"\n\n");
    }
    printf("record number is = %ld\n",fiden[2]);
    

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
