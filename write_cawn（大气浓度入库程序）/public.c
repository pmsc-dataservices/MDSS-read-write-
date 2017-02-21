#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>
/********************************************************************
**
**    Function    :  read_pm_record()
**    Author      :  LanHaiBo
**
********************************************************************/
int read_pm_record25(char *data_dir,char *data_file,char  *date,int count)
{
	  
	  char cmd[1000];
	  
	  char file_name[255];
           char time[14];
	  char full_file_name[1000];
	  FILE *file_list;
          int  ct[10];
    int  filenum=0;
    	  
     memset(time,'\0',sizeof(time));
     
     date_add(date,time,-1);
     printf("%s\n",time);
      printf("%c",time[0]);
    if ((file_list=fopen(data_file,"w")) == NULL) 
    {
         printf("cannot write file %s\n", data_file);
         return -1;
    }
    if(count!=0)
     {
    	fprintf(file_list, "diamond 3 %c%c年%c%c月%c%c日%c%c时空气质量PM2.5浓度\n%c%c %c%c %c%c %c%c 1000 0 0 0 0\n  1 %d\n",time[2],time[3],time[4],time[5],time[6],time[7],time[8],time[9],time[2],time[3],time[4],time[5],time[6],time[7],time[8],time[9],count);
    	  	  
    	  
    }
        
    fclose(file_list);

    return filenum;
}
/********************************************************************
**
**    Function    :  read_pm_record()
**    Author      :  LanHaiBo
**
********************************************************************/
int read_pm_record10(char *data_dir,char *data_file,char  *date,int count)
{
	  
	  char cmd[1000];
	  
	  char file_name[255];
           char time[14];
	  char full_file_name[1000];
	  FILE *file_list;
          int  ct[10];
    int  filenum=0;
    	  
     memset(time,'\0',sizeof(time));
     
     date_add(date,time,-1);
     printf("%s\n",time);
      printf("%c",time[0]);

    if ((file_list=fopen(data_file,"w")) == NULL) 
    {
         printf("cannot write file %s\n", data_file);
         return -1;
    }
    if(count!=0)
     {
    	fprintf(file_list, "diamond 3 %c%c年%c%c月%c%c日%c%c时空气质量PM10浓度\n%c%c %c%c %c%c %c%c 1000 0 0 0 0\n  1 %d\n",time[2],time[3],time[4],time[5],time[6],time[7],time[8],time[9],time[2],time[3],time[4],time[5],time[6],time[7],time[8],time[9],count);
    	  	  
    	  
    }
        
    fclose(file_list);

    return filenum;
}


/********************************************************************
**
**    Function    :  read_o3_record()
**    Author      :  LanHaiBo
**
********************************************************************/
int read_o3_record(char *data_dir,char *data_file,char  *date)
{
	  
	  char cmd[1000];
	  
	  char file_name[255];
           char time[14];
	  char full_file_name[1000];
	  FILE *file_list;
          int  ct[10];
          int   count=1;
    int  filenum=0;
    	  
     memset(time,'\0',sizeof(time));
     
     date_add(date,time,-1);
     printf("%s\n",time);
      printf("%c",time[0]);

    if ((file_list=fopen(data_file,"w")) == NULL) 
    {
         printf("cannot write file %s\n", data_file);
         return -1;
    }
    if(count!=0)
     {
    	fprintf(file_list, "diamond 3 %c%c年%c%c月%c%c日%c%c时空气质量O3浓度\n%c%c %c%c %c%c %c%c 1000 0 0 0 0\n  1 5\n",time[2],time[3],time[4],time[5],time[6],time[7],time[8],time[9],time[2],time[3],time[4],time[5],time[6],time[7],time[8],time[9]);
    	  	  
    	  
    }
        
    fclose(file_list);

    return filenum;
}
/********************************************************************
**
**    Function    :  read_pm_record()
**    Author      :  LanHaiBo
**
********************************************************************/
int read_mul_record(char *data_dir,char *data_file1,char *data_file2,char *data_file3,char  *date)
{
	  
	  char cmd[1000];
	  
	  char file_name[255];
           char time[14];
	  char full_file_name[1000];
	  FILE *file_list;
          int  ct[10];
          int   count=1;
    int  filenum=0;
    	  
     memset(time,'\0',sizeof(time));
     
     date_add(date,time,-1);
     printf("%s\n",time);
      printf("%c",time[0]);

    if ((file_list=fopen(data_file1,"w")) == NULL) 
    {
         printf("cannot write file %s\n", data_file1);
         return -1;
    }
    if(count!=0)
     {
    	fprintf(file_list, "diamond 3 %c%c年%c%c月%c%c日%c%c时空气质量NO2浓度\n%c%c %c%c %c%c %c%c 1000 0 0 0 0\n  1 4\n",time[2],time[3],time[4],time[5],time[6],time[7],time[8],time[9],time[2],time[3],time[4],time[5],time[6],time[7],time[8],time[9]);
    	  	  
    	  
    }
        
    fclose(file_list);
    if ((file_list=fopen(data_file2,"w")) == NULL) 
    {
         printf("cannot write file %s\n", data_file2);
         return -1;
    }
    if(count!=0)
     {
    	fprintf(file_list, "diamond 3 %c%c年%c%c月%c%c日%c%c时空气质量SO2浓度\n%c%c %c%c %c%c %c%c 1000 0 0 0 0\n  1 4\n",time[2],time[3],time[4],time[5],time[6],time[7],time[8],time[9],time[2],time[3],time[4],time[5],time[6],time[7],time[8],time[9]);
    	  	  
    	  
    }
        
    fclose(file_list);
    if ((file_list=fopen(data_file3,"w")) == NULL) 
    {
         printf("cannot write file %s\n", data_file3);
         return -1;
    }
    if(count!=0)
     {
    	fprintf(file_list, "diamond 3 %c%c年%c%c月%c%c日%c%c时空气质量CO浓度\n%c%c %c%c %c%c %c%c 1000 0 0 0 0\n  1 4\n",time[2],time[3],time[4],time[5],time[6],time[7],time[8],time[9],time[2],time[3],time[4],time[5],time[6],time[7],time[8],time[9]);
    	  	  
    	  
    }
        
    fclose(file_list);



    return filenum;
}

/********************************************************************
**
**    Function    :  read_pm()
**    Author      :  LanHaiBo
**
********************************************************************/
int read_o3(char *data_dir,char *data_file,float value[])
{
	  
	  char cmd[1000];
	  
	  char file_name[255];
           char time[14];
	  char full_file_name[1000];
	  FILE *file_list;
          int  ct[10];
    int  filenum=0;
    	  
     
    if ((file_list=fopen(data_file,"a")) == NULL) 
    {
         printf("cannot write file %s\n", data_file);
         return -1;
    }
    if(value[0]!=0)
     {
    	fprintf(file_list, "%d %d 9999 9999 9999 9999 9999 9999 %.2f %.2f %.2f %.2f %.2f %.2f 9999\n",(int)value[0],(int)value[1],value[2],value[3],value[4],value[5],value[6],value[7]);    	  	    
    }
    
    fclose(file_list);

     return 0;
}

/********************************************************************
**
**    Function    :  read_pm()
**    Author      :  LanHaiBo
**
********************************************************************/
int read_58448(char *data_dir,char *data_file1,char *data_file2,char *data_file3,char *data_file4,float value[])
{
	  
	  char cmd[1000];
	  
	  char file_name[255];
           char time[14];
	  char full_file_name[1000];
	  FILE *file_list;
          int  ct[10];
    int  filenum=0;
    	  
     
    if ((file_list=fopen(data_file1,"a")) == NULL) 
    {
         printf("cannot write file %s\n", data_file1);
         return -1;
    }
    if(value[0]!=0)
     {
    	fprintf(file_list, "%d %d 9999 9999 9999 %.2f %.2f %.2f 9999\n",(int)value[0],(int)value[1],value[3],value[6],value[9]);    	  	    
    }
    
    fclose(file_list);
    if ((file_list=fopen(data_file2,"a")) == NULL) 
    {
         printf("cannot write file %s\n", data_file2);
         return -1;
    }
    if(value[0]!=0)
     {
    	fprintf(file_list, "%d %d 9999 9999 9999 %.2f %.2f %.2f 9999\n",(int)value[0],(int)value[1],value[2],value[5],value[8]);    	  	    
    }
    
    fclose(file_list);
    if ((file_list=fopen(data_file3,"a")) == NULL) 
    {
         printf("cannot write file %s\n", data_file3);
         return -1;
    }
    if(value[0]!=0)
     {
    	fprintf(file_list, "%d %d 9999 9999 9999 %.2f %.2f %.2f 9999\n",(int)value[0],(int)value[1],value[4],value[7],value[10]);    	  	    
    }
    
    fclose(file_list);
     if ((file_list=fopen(data_file4,"a")) == NULL) 
    {
         printf("cannot write file %s\n", data_file4);
         return -1;
    }
    if(value[0]!=0)
     {
    	fprintf(file_list, "%d %d 9999 9999 9999 %.2f %.2f %.2f 9999 9999 9999 9999 9999 9999 9999\n",(int)value[0],(int)value[1],value[11],value[12],value[13]);    	  	    
    }
    
    fclose(file_list);




     return 0;
}

/********************************************************************
**
**    Function    :  read_pm()
**    Author      :  LanHaiBo
**
********************************************************************/
int read_54421(char *data_dir,char *data_file1,char *data_file2,char *data_file3,char *data_file4,float value[])
{
	  
	  char cmd[1000];
	  
	  char file_name[255];
           char time[14];
	  char full_file_name[1000];
	  FILE *file_list;
          int  ct[10];
    int  filenum=0;
    	  
     
    if ((file_list=fopen(data_file1,"a")) == NULL) 
    {
         printf("cannot write file %s\n", data_file1);
         return -1;
    }
    if(value[0]!=0)
     {
    	fprintf(file_list, "%d %d 9999 9999 9999 %.2f %.2f %.2f 9999\n",(int)value[0],(int)value[1],value[3],value[6],value[9]);    	  	    
    }
    
    fclose(file_list);
    if ((file_list=fopen(data_file2,"a")) == NULL) 
    {
         printf("cannot write file %s\n", data_file2);
         return -1;
    }
    if(value[0]!=0)
     {
    	fprintf(file_list, "%d %d 9999 9999 9999 %.2f %.2f %.2f 9999\n",(int)value[0],(int)value[1],value[2],value[5],value[8]);    	  	    
    }
    
    fclose(file_list);
    if ((file_list=fopen(data_file3,"a")) == NULL) 
    {
         printf("cannot write file %s\n", data_file3);
         return -1;
    }
    if(value[0]!=0)
     {
    	fprintf(file_list, "%d %d 9999 9999 9999 %.2f %.2f %.2f 9999\n",(int)value[0],(int)value[1],value[4],value[7],value[10]);    	  	    
    }
    
    fclose(file_list);
     if ((file_list=fopen(data_file4,"a")) == NULL) 
    {
         printf("cannot write file %s\n", data_file4);
         return -1;
    }
    if(value[0]!=0)
     {
    	fprintf(file_list, "%d %d 9999 9999 9999 %.2f %.2f %.2f 9999 9999 9999 9999 9999 9999 9999\n",(int)value[0],(int)value[1],value[11],value[12],value[13]);    	  	    
    }
    
    fclose(file_list);



     return 0;
}
/********************************************************************
**
**    Function    :  read_pm()
**    Author      :  LanHaiBo
**
********************************************************************/
int read_51058(char *data_dir,char *data_file1,char *data_file2,char *data_file3,char *data_file4,float value[])
{
	  
	  char cmd[1000];
	  
	  char file_name[255];
           char time[14];
	  char full_file_name[1000];
	  FILE *file_list;
          int  ct[10];
    int  filenum=0;
    	  
     
    if ((file_list=fopen(data_file1,"a")) == NULL) 
    {
         printf("cannot write file %s\n", data_file1);
         return -1;
    }
    if(value[0]!=0)
     {
    	fprintf(file_list, "%d %d 9999 9999 9999 9999 9999 9999 %.2f \n",(int)value[0],(int)value[1],value[2]);    	  	    
    }
    
    fclose(file_list);
    if ((file_list=fopen(data_file2,"a")) == NULL) 
    {
         printf("cannot write file %s\n", data_file2);
         return -1;
    }
    if(value[0]!=0)
     {
    	fprintf(file_list, "%d %d 9999 9999 9999 9999 9999 9999 %.2f \n",(int)value[0],(int)value[1],value[3]);    	  	    
    }
    
    fclose(file_list);
    if ((file_list=fopen(data_file3,"a")) == NULL) 
    {
         printf("cannot write file %s\n", data_file3);
         return -1;
    }
    if(value[0]!=0)
     {
    	fprintf(file_list, "%d %d 9999 9999 9999 9999 9999 9999 %.2f \n",(int)value[0],(int)value[1],value[4]);       	  	    
    }
    
    fclose(file_list);
    if ((file_list=fopen(data_file4,"a")) == NULL) 
    {
         printf("cannot write file %s\n", data_file4);
         return -1;
    }
    if(value[0]!=0)
     {
    	fprintf(file_list, "%d %d 9999 9999 9999 9999 9999 9999 9999 9999 9999 9999 9999 9999 %.2f \n",(int)value[0],(int)value[1],value[5]);     	  	    
    }
    
    fclose(file_list);



     return 0;
}
/********************************************************************
**
**    Function    :  read_pm()
**    Author      :  LanHaiBo
**
********************************************************************/
int read_54084(char *data_dir,char *data_file1,char *data_file2,char *data_file3,char *data_file4,float value[])
{
	  
	  char cmd[1000];
	  
	  char file_name[255];
           char time[14];
	  char full_file_name[1000];
	  FILE *file_list;
          int  ct[10];
    int  filenum=0;
    	  
     
    if ((file_list=fopen(data_file1,"a")) == NULL) 
    {
         printf("cannot write file %s\n", data_file1);
         return -1;
    }
    if(value[0]!=0)
     {
    	fprintf(file_list, "%d %d 9999 9999 9999 9999 9999 9999 %.2f \n",(int)value[0],(int)value[1],value[2]);    	  	    
    }
    
    fclose(file_list);
    if ((file_list=fopen(data_file2,"a")) == NULL) 
    {
         printf("cannot write file %s\n", data_file2);
         return -1;
    }
    if(value[0]!=0)
     {
    	fprintf(file_list, "%d %d 9999 9999 9999 9999 9999 9999 %.2f \n",(int)value[0],(int)value[1],value[3]);    	  	    
    }
    
    fclose(file_list);
    if ((file_list=fopen(data_file3,"a")) == NULL) 
    {
         printf("cannot write file %s\n", data_file3);
         return -1;
    }
    if(value[0]!=0)
     {
    	fprintf(file_list, "%d %d 9999 9999 9999 9999 9999 9999 %.2f \n",(int)value[0],(int)value[1],value[4]);   	  	    
    }
    
    fclose(file_list);
    if ((file_list=fopen(data_file4,"a")) == NULL) 
    {
         printf("cannot write file %s\n", data_file4);
         return -1;
    }
    if(value[0]!=0)
     {
    	fprintf(file_list, "%d %d 9999 9999 9999 9999 9999 9999 9999 9999 9999 9999 9999 9999 %.2f \n",(int)value[0],(int)value[1],value[5]);     	  	    
    }
    
    fclose(file_list);



     return 0;
}

/********************************************************************
**
**    Function    :  stringrtrim()
**    Author      :  LanHaiBo
**
********************************************************************/
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

/******************************************************************/
/* function : stringsplit()                                       */
/******************************************************************/
int stringsplit(char *string, char split, int number, char newstr[][1000])
{
    int  i, newstr_num=0;
    char char_one[2000], char_more[2000];
    //printf("fengyuxing9\n");
    if ((string==NULL) || (number<=0))
    {
    	 return 0;
    }
    memset(char_more, '\0', sizeof(char_more));
     //printf("fengyuxing10\n");

    for (i=0; i<strlen(string); i++)
    {   
        //printf("fengyuxing11\n");

    	if ((string[i]==split) && (newstr_num<number-1))
    	{
             //printf("fengyuxing12\n");

    	    stringtrim(char_more);
    	    if (strlen(char_more)>0)
    	    {
    	       strcpy(newstr[newstr_num++], char_more);
                 //printf("fengyuxing13\n");

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
    // printf("fengyuxing14\n");

    if (strlen(char_more)>0)
    {
        // printf("fengyuxing145\n");

	      stringtrim(char_more);
        strcpy(newstr[newstr_num++], char_more);
    }
    //printf("fengyuxing15\n");

    return newstr_num;
    //printf("fengyuxing16\n");

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
/* function : IsProcessExist()                                    */
/******************************************************************/
int IsProcessExist(char *ProcName)
{
    char cmd[255], bufline[255], username[50];
    FILE *pcfp;
    char cmd_procname[100];
    
    int  ret=-1, num=0;
	
    if (getenv("LOGNAME")!= NULL)
        strcpy(username, getenv("LOGNAME"));
    else
	      strcpy(username, "product");

  memset(cmd, '\0', sizeof(cmd));
  sprintf(cmd, "ps -fu %s | grep %s | grep -v grep", username, ProcName);

  if((pcfp=popen(cmd,"r"))==NULL)
  {
    	return -1;
  }
  while(!feof(pcfp))
  {
      memset(bufline, '\0', sizeof(bufline));
 	    if(!fgets(bufline, 255, pcfp))
	       continue;
	    
      stringtrim(bufline);
      memset(cmd_procname, '\0', sizeof(cmd_procname));   
	    strcpy(cmd_procname, bufline+strlen(bufline)-strlen(ProcName));
	
	    if(strcmp(cmd_procname, ProcName)==0)
	    {
	      num++;
	      if (num > 1)
	      {
	         ret = 0;
    	       break;
    	  }
	    }
  }
  pclose(pcfp);
  
  /*
  if(ret==0)
     printf("%s process has been existed!\n", ProcName);
  else
     printf("%s process doesnot exist!\n", ProcName);*/
	
  return ret;
}


/******************************************************************/
/* function : file_size()                                         */
/******************************************************************/
int file_size(char *filename)
{
    struct stat    status;
    int    ct[10];
    
    if (stat(filename,&status) == -1) 
        return 0;
    if (!S_ISREG(status.st_mode))	
        return 0;
    
    return status.st_size;
}

/******************************************************************/
/* function : date_add()                                          */
/******************************************************************/
int date_add(char *old_date, char *new_date, int num)
{
     int    old_year, old_month, old_day, old_hour, old_minutes, old_secs;
     int    new_year, new_month, new_day, new_hour, new_minutes, new_secs;
     struct tm *old_tmpt, old_tm, *new_tmpt;
     time_t time_num=0;
     char   datestring[20];

     sscanf(old_date, "%4d%02d%02d%02d%02d%02d", &old_year, &old_month, &old_day, &old_hour, &old_minutes, &old_secs);
     old_tmpt = &old_tm;
     old_tmpt->tm_year=(old_year-1900);
     old_tmpt->tm_mon =(old_month-1);
     old_tmpt->tm_mday=old_day;
     old_tmpt->tm_hour=old_hour;
     old_tmpt->tm_min =old_minutes;
     old_tmpt->tm_sec =old_secs;

     time_num=mktime(old_tmpt);

     time_num=(time_num+num*3600);

     new_tmpt = localtime(&time_num);

     new_year  = (new_tmpt->tm_year + 1900);
     new_month = (new_tmpt->tm_mon + 1);
     new_day   = (new_tmpt->tm_mday);
     new_hour  = (new_tmpt->tm_hour);
     new_minutes = (new_tmpt->tm_min);
      new_secs = (new_tmpt->tm_sec);

     memset(datestring, '\0', sizeof(datestring));
     sprintf(datestring, "%4d%02d%02d%02d%02d", new_year, new_month, new_day, new_hour, new_minutes);
     strcpy(new_date, datestring);

     return 0;
}

/******************************************************************/
/* function : date_adds()                                          */
/******************************************************************/
int date_adds(char *old_date, char *new_date, int num)
{
     int    old_year, old_month, old_day, old_hour, old_minutes;
     int    new_year, new_month, new_day, new_hour, new_minutes;
     struct tm *old_tmpt, old_tm, *new_tmpt;
     time_t time_num=0;
     char   datestring[13];

     sscanf(old_date, "%4d%2d%2d%2d%2d", &old_year, &old_month, &old_day, &old_hour, &old_minutes);

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
     sprintf(datestring, "%04d%02d%02d%02d%02d", new_year, new_month, new_day, new_hour, new_minutes);
     strcpy(new_date, datestring);

     return 0;
}
