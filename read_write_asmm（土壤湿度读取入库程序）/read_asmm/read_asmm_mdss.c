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

#define  FDATANUM  36000000    //�����ݿ��з��ص���������������record_num*column_float_num��asmf��90��ʵ���ֶΣ�90��40000=36000000
#define  CDATANUM    800000    //�����ݿ��з��ص��ַ�������������record_num*column_char_num��asmf��2��ʵ���ֶΣ�2��40000=800000
#define  IDENNUM     400000    //long ciden[],fiden[]�����С�������ݿ��з��صļ�¼������һ��վһ����12*3����¼��������1000��վ��12��3��10000=360000

#define C_COLUMN 2         //�ַ����ֶ���
#define F_COLUMN 91        //ʵ���ֶ���

/*
--- ��������������������ǰ���ڣ�(��ǰʱ��-1)����������
--- ����������������argc=3 argv[1]���������ڣ���ʽ��YYYYMMDD argv[2]��ʱ�� ��ʽ��HH
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

/*--- 1. �ַ�������������� cpar ---*/

    /*
    ----- �û���Ϣ�����ã�cpar[0-1]
    ----- cpar[0]�����ü������û���
    ----- cpar[1]�����ü������û����� 
    */
    strcpy(cpar[0],"lanhb");
    strcpy(cpar[1],"lanhb090728");

    /*
    ----- �������Ƶ����ã� cpar[2]
    ----- ���ü��������ϱ�������Ϊ��д 
    */
    strcpy(cpar[2],"RASMM");

    /*
    ----- ����Ҫ�ص����ã�cpar[3-4]
    ----- cpar[3]�����ü����ַ������ݵ�Ҫ������
    ----- cpar[4]�����ü������������ݵ�Ҫ������
    */
    strcpy(cpar[3],"ALL");
    /* /// LYM
    strcpy(cpar[4],"ALL-F");
    */
    strcpy(cpar[4],"V01000,V05001,V06001,V07001,V04001,V04002,V04003,V04004,V04005,V04006,V56208_010,V56208_020,V56208_030,V56208_040,V56208_050,V56208_060,V56208_070,V56208_080,V56208_090,V56208_100,V56208_110,V56208_120,V56208_130,V56208_140,V56208_150,V56208_160,V56208_170,V56208_180,V56208_190,V56208_200");

    /* ----- ����������������ã�cpar[5] */ 
    strcpy(cpar[5],"NULL");

    /* 
    ----- cpar ���������־��cpar[6] 
    */
    strcpy(cpar[6],"EOF");

/*--- 2. �����������������lpar ---*/

    /*
    ----- �������ڵ����ã�lpar[0-10]
    ----- lpar[0]���������ڵļ�����ʽ. 1��ʾ�������ڷ�Χ�ļ�����������Χ�����߽�ֵ��; 2��ʾ��ɢ���ڵļ���; 3��ʾ���������ڷ�Χ�ļ���
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
    ----- ����ʱ������ã�lpar[11-20]
    ----- lpar[11]���������ڵļ�������. 0��ʾ��������ʱ�������; 1��ʾ����ĳ���򼸸��۲�ʱ�ε�����; 2��ʾ����ĳ���۲�ʱ��ε�����
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
	  ----- ������������ã�lpar[21-50]
	  ----- lpar[21]�����������������, 0��ʾ�����������������; 1��ʾ����ĳ����γ�ȷ�Χ������; 2��ʾ����ĳ��վ�ŷ�Χ������; 3��ʾ����ĳЩ��վ�ŵ����� 
	  */
	  lpar[21] = 0;

    /* 
    ----- lpar �������������� :lpar[51]
    */
    lpar[51] = -999999;
    
    
/*--- 3. ������������ ---*/   

    /*
    ----- 
    */
    ciden[0] = CDATANUM;
    ciden[1] = IDENNUM;
    fiden[0] = FDATANUM;
    fiden[1] = IDENNUM;

/*--- 4. ��������ļ�  ---*/
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

    for (i=0; i<fiden[2]; i++) //�������ļ�¼����
    {
    	/*
			for(j=0; j<ciden[i+4]; j++) //ÿ����¼�ַ��������ܸ���
			{
	    	fprintf(fp,"  %s ",cdata[j+cdata_num]);
			}
      cdata_num += ciden[i+4];*/

 			for (j=0; j<fiden[i+4]; j++) //ÿ����¼ʵ�������ܸ��� 
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
