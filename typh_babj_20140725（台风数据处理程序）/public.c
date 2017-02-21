#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>

/******************************************************************/
/* function : numtime()                                           */
/******************************************************************/
int numtime(int *ntime,time_t btime)
{
    struct tm *tblock;

    time_t lt;
    lt = time(NULL);
    tblock = localtime(&lt);

    ntime[0] = tblock->tm_year+1900;
    ntime[1] = tblock->tm_mon+1;
    ntime[2] = tblock->tm_mday; 
    ntime[3] = tblock->tm_hour; 
    ntime[4] = tblock->tm_min; 
    ntime[5] = tblock->tm_sec; 

    return 0;
}
int filetime(int *ntime,time_t btime)
{
    struct tm *tblock;
    tblock = localtime( &btime );

    /* convert time_t to struct tm */

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
	
  return ret;
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
/* function : stringfind()                                       */
/******************************************************************/
int stringfind(char source_str[][100], int source_str_num, char *string)
{
	  int i;
	  
	  for (i=0; i<source_str_num; i++)
	  {
	  	  if (strncmp(source_str[i], string, strlen(string))==0)
	  	  {
	  	  	  return i;
	  	  }
	  }
	  return 0;
}

/******************************************************************/
/* function : msg_to_data()                                       */
/******************************************************************/
int msg_to_data(float *attri, char *mms, int left, int right)
{
	char nmms[10];
	char *omms;

	omms = mms+left;
	
	memset(nmms, '\0', sizeof(nmms));	
	strncpy(nmms, omms, strlen(omms)-right);

	*attri = atof(nmms);
	
	return 0;
}

/******************************************************************/
/* function : get_substr()                                         */
/* get substring of string from start with length to dest   */
/******************************************************************/
int get_substr(char *string, int strstart, int lng, char *dest)
{
	char char_one[2], char_more[5];
	int in, start, end;
	start = strstart;
	if(string == NULL || start > strlen(string))	return 0;
	if(start < 0) start = 0;
	
	memset(char_more, '\0', sizeof(char_more));	
	if((start + lng) > strlen(string)) end = strlen(string);
		else end = start + lng;

	in = start;
	while (in < end)
	{

		memset(char_one, '\0', sizeof(char_one));
	   sprintf(char_one, "%c", string[in]);
	   strcat(char_more, char_one);
		
		in++;
	}
	
	strncpy(dest, char_more, lng);
	return 0;
}


/*****************************************************
**		Function: 	dateadd()
**		Description:	add del hours to ctime (del may be an negative)
*****************************************************/
int dateadd(char ctime[], int del, char *dateadded)
{
    struct tm *tmfile, tmfl;
    time_t timeby=0;
    time_t timenum=0;
    
    int ret_year, ret_month, ret_day, ret_hour, ret_minute, ret_sec;
    int rep_year, rep_month, rep_day, rep_hour, rep_minute, rep_sec;
    int i, ret=-1;

    sscanf(ctime, "%4d%2d%2d%2d%2d", &ret_year, &ret_month, &ret_day, &ret_hour, &ret_minute);
    ret_sec = 0;
    
    tmfile = &tmfl;
    tmfile->tm_year=(ret_year-1900);
    tmfile->tm_mon=ret_month-1;
    tmfile->tm_mday=ret_day;
    tmfile->tm_hour=ret_hour;
    tmfile->tm_min=ret_minute;
    tmfile->tm_sec=ret_sec;
    
    timenum=mktime(tmfile);
    timeby = timenum + del*3600; // add the del hours
    tmfile = localtime(&timeby);
    
    rep_year = tmfile->tm_year + 1900;
    rep_month = tmfile->tm_mon +1;
    rep_day = tmfile->tm_mday;
    rep_hour = tmfile->tm_hour;
    rep_minute = tmfile->tm_min;
    rep_sec = tmfile->tm_sec;
    
    sprintf(dateadded, "%4d%02d%02d%02d%02d", rep_year, rep_month, rep_day, rep_hour, rep_minute); 
    
    return 0;   
}

/*****************************************************
**		Function: 	datuma()
**		Description:	add del hours to ctime (del may be an negative)
*****************************************************/
int datuma(char rtime[], char *dd, char hh[], char mm[], char *codate, char cotime[])
{
	
    struct tm *tmfile, tmfl;
    time_t timeby=0;
    time_t timenum=0;
    
    int ret_year, ret_month, ret_day, ret_hour, ret_minute;
    int rep_year, rep_month, rep_day, rep_hour, rep_minute;
    int timeout = 10;        /* minutes */
    int timein  = 20;        /* days */
    int i, ret=-1;
    
    rep_day = atoi(dd);
    rep_hour = atoi(hh);
    rep_minute = atoi(mm);

    sscanf(rtime, "%4d%2d%2d%2d%2d", &ret_year, &ret_month, &ret_day, &ret_hour, &ret_minute);

    tmfile = &tmfl;
    tmfile->tm_year=(ret_year-1900);
    tmfile->tm_mon=ret_month-1;
    tmfile->tm_mday=ret_day;
    tmfile->tm_hour=ret_hour;
    tmfile->tm_min=ret_minute;
    tmfile->tm_sec=0;
    
    timenum=mktime(tmfile);
    timenum=timenum+timeout*60;
     for (i=0; i<timein; i++)
    {
        timeby = timenum - (i*24*60*60);
        tmfile = localtime(&timeby);
        if (tmfile->tm_mday==rep_day)
        {
            rep_year=tmfile->tm_year+1900;
            rep_month=tmfile->tm_mon+1;
            ret = 0;
            break;
        } 
    }

    if (ret==0)
    {
        sprintf(codate, "%4d%02d%02d", rep_year, rep_month, rep_day);
        codate[8]='\0';
        sprintf(cotime, "%02d%02d", rep_hour, rep_minute);
        cotime[4]='\0';
    }
    return ret;
}

/*****************************************************
**		Function: 	typh_direct()
**		Description:	add del hours to ctime (del may be an negative)
*****************************************************/
int typh_direct(char *direct, float *dvalue)
{
	float value;
	
	if(strcmp(direct, "N")==0)
			value = 0;
	else if(strcmp(direct, "NNE")==0)
			value = 22.5;
	else if(strcmp(direct, "NE")==0)
			value = 45;
	else if(strcmp(direct, "ENE")==0)
			value = 67.5;
	else if(strcmp(direct, "E")==0)
			value = 90;
	else if(strcmp(direct, "ESE")==0)
			value = 112.5;
	else if(strcmp(direct, "SE") == 0)
			value = 135;
	else if(strcmp(direct, "S")==0)
			value = 180;
	else if(strcmp(direct, "SSW")==0)
			value = 202.5;
	else if(strcmp(direct, "SW")==0)
			value = 225;
	else if(strcmp(direct, "WSW")==0)
			value = 247.5;
	else if(strcmp(direct, "W")==0)
			value = 270;
	else if(strcmp(direct, "WNW")==0)
			value = 292.5;
	else if(strcmp(direct, "NW")==0)
			value = 315;
	else if(strcmp(direct, "NNW")==0)
			value = 337.5;
	else if(strcmp(direct, "ALMOST")==0)
			value = 999999.0;

	*dvalue = value;
	return 0;
}
