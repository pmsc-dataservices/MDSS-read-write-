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
     char    bjtime[20], date[20];
    char logname[255];
    FILE *fplog;
    
    
    /* get current time */
    numtime(ct, -1);
    memset(select_time, '\0', sizeof(select_time));
    memset(logname,'\0',sizeof(logname));
    sprintf(logname, "%s/log/product_awst_log/awst_%04d%02d%02d.log", getenv("HOME"),ct[0],ct[1],ct[2]);
    if ((fplog=fopen(logname,"a+")) == NULL) 
    {
         printf("cannot open %s\n", logname);
         exit(-1);
    }
    
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
    strcpy(cpar[4],"V01000,V12001,V13003,V13019,V11043,V11041");

    /* 
    ----- ����������������ã�cpar[5] 
    */
  // sprintf(select_time,"C_IYMDHM between TO_DATE('%s','yyyymmddhh24miss') and TO_DATE('%s','yyyymmddhh24miss')",argv[2],argv[3]);

   strcpy(cpar[5],"V04005=0  and  V04006=0   and V01000 in (57056,57051,57067,57156,57261,57066,57077,57063,57169,57065,57175,57271,57074,57176,57071,57078,57178,53978,57162,57072,57076,57273,57075,57173,53972,57187,57080,57179,57274,57070,57082,57180,53982,57281,57079,57181,57184,57085,57081,57285,53987,53984,57088,57084,53979,57182,57177,57185,57083,53988,57086,57087,53985,53889,57089,53986,57183,53989,57189,57090,57294,57290,53997,53974,57094,57194,57091,57197,57297,57188,53991,57295,53898,57098,53983,57190,57191,57186,53995,53992,57193,57298,57198,57195,57292,53998,53994,57390,57296,57096,57093,57099,57192,57196,57396,57299,53993,57293,57095,54900,54901,58100,58007,58008,58207,58301,58205,54903,58101,58104,58208,58005,58001,54817,58006,54902,58004,58111,58017,53990,57171)");   

   /* 
      ----- cpar ���������־��cpar[6] 
    */
    strcpy(cpar[6],"EOF");
    
/*--- 2. �����������������lpar ---*/

    /*
    ----- �������ڵ����ã�lpar[0-10]
    ----- lpar[0]���������ڵļ�����ʽ. 1��ʾ������Ѯ���Ϸ�Χ������������Χ�����߽�ֵ��; 2��ʾ��ɢѮ���ϼ���; 3��ʾ������Ѯ���ڷ�Χ����
    */
    /* 
    lpar[0] = 1;
    lpar[1] = surf_start_date;
    lpar[2] = surf_end_date;*/

  
    
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
    sprintf(date,  "%s%s",argv[1],argv[2]);
    date_add(date, bjtime,8);
    
     sprintf(output,  "%s/data/rgwst_henan_data/%s00.TXT", getenv("HOME"),bjtime);
     

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
    float  temp,rain,wind;
    char   rains[8],hum[8],windd[8],winds[8];
        

    fdata_num=0;
    cdata_num=0;

    fp = fopen(filename,"w");
    if (fp == NULL) 
        return -1;
           
        for (i=0; i<fiden[2]; i++) //�������ļ�¼����
    {
        temp=fdata[1+fdata_num]*10;
        rain=fdata[3+fdata_num]*10;
        wind=fdata[5+fdata_num]*10;
        if(fdata[2+fdata_num]<100)
            { 
              sprintf(hum,"0%.f",fdata[2+fdata_num]);  
            }
          else 
            { 
             sprintf(hum,"%.f",fdata[2+fdata_num]); 
            }

          if(rain<10)
             {
                  sprintf(rains,"000%.f",rain); 
              }
     else if (rain>10  &&  rain<100)
        { 
          sprintf(rains,"00%.f",rain); 
         }
     else if (rain>100  &&  rain<1000)
        { 
          sprintf(rains," 0%.f",rain);  
         }
 
         
      
     if(fdata[4+fdata_num]<10)
              { sprintf(windd,"00%.f",fdata[4+fdata_num]);  }
     else if(fdata[4+fdata_num]>10  && fdata[4+fdata_num]<100)
              {sprintf(windd,"0%.f",fdata[4+fdata_num]); }
     else if(fdata[4+fdata_num]>100)
              {sprintf(windd,"%.f",fdata[4+fdata_num]); }


     if(wind<10)
        { sprintf(winds,"00%.f",wind);  }
     else if (wind>10  &&  wind<100)
        { sprintf(winds,"0%.f",wind);  }
       
      fprintf(fp,"%.f %.f %s %s %s %s\n",fdata[0+fdata_num],temp,hum,rains,windd,winds);	
                
      //fprintf(fp,"%.2f %.2f %.2f %.2f %.2f %.2f\n",fdata[0+fdata_num],fdata[1+fdata_num],fdata[2+fdata_num],fdata[3+fdata_num],fdata[4+fdata_num],fdata[5+fdata_num]);	
 	
            
                    
			fdata_num += fiden[i+4];
			
			
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
     int    old_year, old_month, old_day, old_hour;
     int    new_year, new_month, new_day, new_hour;
     struct tm *old_tmpt, old_tm, *new_tmpt;
     time_t time_num=0;
     char   datestring[13];

     sscanf(old_date, "%4d%2d%2d%2d", &old_year, &old_month, &old_day, &old_hour);

     old_tmpt = &old_tm;
     old_tmpt->tm_year=(old_year-1900);
     old_tmpt->tm_mon =(old_month-1);
     old_tmpt->tm_mday=old_day;
     old_tmpt->tm_hour=old_hour;
     old_tmpt->tm_min =0;
     old_tmpt->tm_sec =0;

     time_num=mktime(old_tmpt);

     time_num=(time_num+num*3600);

     new_tmpt = localtime(&time_num);

     new_year  = (new_tmpt->tm_year + 1900);
     new_month = (new_tmpt->tm_mon + 1);
     new_day   = (new_tmpt->tm_mday);
     new_hour   = (new_tmpt->tm_hour);


     memset(datestring, '\0', sizeof(datestring));
     sprintf(datestring, "%04d%02d%02d%02d", new_year, new_month, new_day, new_hour);
     strcpy(new_date, datestring);

     return 0;
}
