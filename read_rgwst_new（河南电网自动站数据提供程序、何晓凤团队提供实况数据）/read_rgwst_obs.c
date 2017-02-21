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
int stringsplit(char *string, char split, int number, char newstr[][100]);
int stringltrim(char *string);
int stringtrim(char *string);


#define  FDATANUM  1500000000   //从数据库中返回的整型数据总数：record_num*column_float_num。abwa有32个实型字段：45×400000=28400000
#define  CDATANUM 5000000   ////从数据库中返回的字符型数据总数：record_num*column_char_num。abwa没有字符型数据 28×400000=
#define  IDENNUM   50000000   //long ciden[],fiden[]数组大小：从数据库中返回的记录总数。一个站一年有12*3条记录。假设有10000个站：12×3×10000=360000
#define LINE  30000
#define COLUMN_NUM 1 
/*
--- 检索条件描述：检索
所有站一年的数据
--- 主函数参数描述：argc=3 argv[1]：检索时间范围下限；argv[2]：检索时间范围上限；格式：YYYYMMDD
*/

int main(int argc,char *argv[]) 
{
    int     ret;
    char    cpar[20][2048];
    long    lpar[100];
    char    output[255],date[20],hour[10];
    char        select_time[300];
    int     ct[10], surf_start_date, surf_end_date;
    char    today[20], yesterday[20];
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
    
/*--- 1. 字符型输入参数数组 cpar ---*/
    
    /*
    ----- 用户信息的设置：cpar[0-1]
    ----- cpar[0]：设置检索的用户名
    ----- cpar[1]：设置检索的?没Э诹?
    */
    strcpy(cpar[0],"lanhb");
    strcpy(cpar[1],"lanhb090728");

    /*
    ----- 资料名称的??茫?cpar[2]
    ----- 设置检索的资料别名，均为大写 
    */
    strcpy(cpar[2],"RGWST");
        
   
    /*
    ----- 检索要素的设置：cpar[3-4]
    ----- cpar[3]：设置检索字符型数据的要素名称
    ----- cpar[4]：设置检索浮点型数据的要素名称
    */
    strcpy(cpar[3],"NULL");
    strcpy(cpar[4],"V01000,V05001,V06001,V07001,V12001,V13003,V11001,V11002,V13021,V13023,V20010,V20051,V12016,V12017,V20003,V10004,V12003,V20001_052_010,V13019,V13024,V13022,V10060_024,V13031,V20004,V20005,V20013,V20012_011,V20012_012,V20012_013,V20012_001,V20012_002,V20012_003,V20012_004,V20062");
   // strcpy(cpar[3],"ALL");
   // strcpy(cpar[4],"V01000,V05001,V06001,V07001,V04001,V04002,V04003,V04004,V04005,V04006,V13019");

    /* 
    ----- 特殊检索条件的设置：cpar[5] 
    */
  // sprintf(select_time,"C_IYMDHM between TO_DATE('%s','yyyymmddhh24miss') and TO_DATE('%s','yyyymmddhh24miss')",argv[2],argv[3]);

   strcpy(cpar[5],"V04005=0  and  V04006=0");
   //strcpy(cpar[5],"V01000=54511");
    
    /* 
    ----- cpar 数组结束标志：cpar[6] 
    */
    strcpy(cpar[6],"EOF");
    
/*--- 2. 长整型输入参数数组lpar ---*/

    /*
    ----- 检索日期的设置：lpar[0-10]
    ----- lpar[0]：设置日期的检索方式. 1表示连续的旬资料范围检索（检索范围包含边界值）; 2表示离散旬资料检索; 3表示不连续旬日期范围检索
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
    

	 /*--- 3. 其它参?柚?---*/   
    

    /*
    ----- 
    */
    ciden[0] = CDATANUM;
    ciden[1] = IDENNUM;
    fiden[0] = FDATANUM;
    fiden[1] = IDENNUM;
    
/*--- 4. 设置输出文件  ---*/
    sprintf(output,  "%s/data/rgwst_new_data/obs%s%s.dat", getenv("HOME"),argv[1],argv[2]);
    //sprintf(date,"%s",argv[1]);
        //sprintf(hour,"%s",argv[2]);


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
    FILE   *fp1;
    int     i, j,station,number,num=0;
    int   ct[10];
    char date[14],flag1[4],flag2[2],file[50];
    char buf[LINE];
    char temp[COLUMN_NUM][100]; 


    fdata_num=0;
    cdata_num=0;
     
   sprintf(file,"/usr/mdss_user/data/rgwst_new_data/obs_station.dat");


   fp1 = fopen(file,"r");
        
    if (fp1 == NULL)
    { 
    	printf("open file error\n");
      exit(-1);
    }

  while(fgets(buf,LINE,fp1))
    {
    	
    	/*
    	--- process a line
    	*/
    	num=0;
    	num=stringsplit(buf,' ',COLUMN_NUM,temp);  	
		
    	if(num==COLUMN_NUM)
    	{                
       station=atoi(temp[0]);
       printf("%d\n",station);
    	}

    	  // memset(buf,'\0',sizeof(buf));         l
       // memset(temp,'\0',sizeof(temp));
    } 
    fclose(fp1);
    number=station+fiden[2];
    fp = fopen(filename,"w");
    if (fp == NULL) 
        return -1;
    sscanf(filename, "%38s%10s%s", flag1, date, flag2);
     //printf("%s  %s\n",flag1, date);
     fprintf(fp,"obs_time %s station_numbers %ld element_numbers 36 default_value 999999.00\n",date,number);
     fprintf(fp,"站号 经度 纬度 高度 气温 相对湿度 瞬时风向 瞬时风速 观测前6小时降水量 观测前24小时降水量 总云量 低云量 24小时最高温度 24小时最低温度 天气现象 本站气压 露点温度 水平能见度 观测前1小时降水量 观测前3小时降水量 观测前12小时降水量 12小时最高气温 12小时最低气温 24小时气压变量 蒸发量 过去天气1 过去天气2 正点观测云高 低云状 中云状 高云状 云状1 云状2 云状3 云状4 地面状态\n");

    for (i=0; i<fiden[2]; i++) //检索到的记录个数
    {

   	 fprintf(fp," %.2f %.2f %.2f %.2f %.2f %.2f %.2f %.2f %.2f %.2f %.2f %.2f %.2f %.2f %.2f %.2f %.2f %.2f %.2f %.2f %.2f 999999.00 999999.00 %.2f %.2f %.2f %.2f %.2f %.2f %.2f %.2f %.2f %.2f %.2f %.2f %.2f\n",fdata[0+fdata_num],fdata[1+fdata_num],fdata[2+fdata_num],fdata[3+fdata_num],fdata[4+fdata_num],fdata[5+fdata_num],fdata[6+fdata_num],fdata[7+fdata_num],fdata[8+fdata_num],fdata[9+fdata_num],fdata[10+fdata_num],fdata[11+fdata_num],fdata[12+fdata_num],fdata[13+fdata_num],fdata[14+fdata_num],fdata[15+fdata_num],fdata[16+fdata_num],fdata[17+fdata_num],fdata[18+fdata_num],fdata[19+fdata_num],
   fdata[20+fdata_num],fdata[21+fdata_num],fdata[22+fdata_num],fdata[23+fdata_num],fdata[24+fdata_num],fdata[25+fdata_num],fdata[26+fdata_num],fdata[27+fdata_num],fdata[28+fdata_num],fdata[29+fdata_num],fdata[30+fdata_num],fdata[31+fdata_num],fdata[32+fdata_num],fdata[33+fdata_num]);	
     
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
/* function : stringsplit()                                       */
/******************************************************************/
int stringsplit(char *string, char split, int number, char newstr[][100])
{
    int  i, newstr_num=0;
    char char_one[2], char_more[50];
    
    if ((string==NULL) || (number<=0))
    {
    	 return 0;
    }
    memset(char_more, '\0', sizeof(char_more));
    for (i=0; i<strlen(string); i++)
    {
    	if ((string[i]==split) && (newstr_num<number-1))
    	{
    	    stringtrim(char_more);
    	    if (strlen(char_more)>0)
    	    {
    	       strcpy(newstr[newstr_num++], char_more);
    	    }
          memset(char_more, '\0', sizeof(char_more));
    	}
    	else
    	{
    	   memset(char_one, '\0', sizeof(char_one));
    	   sprintf(char_one, "%c", string[i]);
    	   strcat(char_more, char_one);
    	}
    }
    if (strlen(char_more)>0)
    {
	      stringtrim(char_more);
        strcpy(newstr[newstr_num++], char_more);
    }
    
    return newstr_num;
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
     new_hour  = (new_tmpt->tm_hour);


     memset(datestring, '\0', sizeof(datestring));
     sprintf(datestring, "%04d%02d%02d%02d", new_year, new_month, new_day, new_hour);
     strcpy(new_date, datestring);

     return 0;
}
/******************************************************************/
/* function : stringtrim()                                        */
/******************************************************************/
int stringrtrim(char *string)
{
    int i;
    for (i=strlen(string)-1; i>=0; i--)
    {
    	if (string[i]==' ' || string[i]==10 || string[i]==13)
    	{
    	    string[i]='\0';
    	}
    	else
    	{
    	    break;
    	}
    }
    return 1;
}

int stringltrim(char *string)
{
    int i;
    char *string_copy;
    
    string_copy=string;
    
    for (i=0; i<strlen(string_copy); i++)
    {
        if (string_copy[i]!=' ' && string_copy[i]!=10 && string_copy[i]!=13)
        {
            break;
        }
    }
    strcpy(string, string_copy+i);
    
    return 1;
}

int stringtrim(char *string)
{
    stringltrim(string);
    stringrtrim(string);
}



