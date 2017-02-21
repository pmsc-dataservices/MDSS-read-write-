#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#include "typh_babj.h"

int getfile(char *directory);
int checkfile(char *filename, char *filetime);
int getfilehash(char *filename, char *hash_value);
int parse_typh_file(char *filename, TYPH_KEY typh_key[MAX_REP_NUM], int *report_num);
int output_typh(TYPH_KEY *key);
int move_file2trash(char *filename);

int main()
{
	  char data_path[255], typh_file[255], typh_full_file[255], cur_file_time[20], file_hash[50];
	  int  i, j, k, ret, file_num=0, report_num=0, ct[10];
	  char logname[255], procname[20];
	  char orausr[30],orapasswd[30],oradbname[30];
	  char process_log[1000];
	  
	  TYPH_KEY *typh_key, key[MAX_REP_NUM];
	  TYPH_ELE *typh_ele;
	  
	  /* 1. set envirment */
	  memset(data_path, '\0', sizeof(data_path));
	  sprintf(data_path, "%s/data/typh", getenv("HOME"));
	  
    /*
     * 2. open log file
     */
    numtime(ct, -1);
    memset(logname, '\0', sizeof(logname));
    sprintf(logname, "%s/log/proc_db_typh_2014new_%04d%02d%02d.log", getenv("HOME"), ct[0], ct[1], ct[2]);

    if ((fplog=fopen(logname,"a+")) == NULL) 
    {
         printf("cannot open %s\n", logname);
         exit(-1);
    }
    setvbuf(fplog,NULL,_IONBF,0);

    signal(SIGINT, SIG_IGN);
    signal(SIGTERM, SIG_IGN);
    signal(SIGHUP, SIG_IGN);
    
    /* 
     * 3. process control
     */     
    sprintf(procname, "%s", "proc_db_typh_babj_2014new");
    ret = IsProcessExist(procname);
    if (ret == 0)
    {
        fprintf(fplog, "################################################################################\n");
	      fprintf(fplog, "#warning: process %s second starting fail {%04d-%02d-%02d %02d:%02d:%02d}    #\n", procname, ct[0], ct[1], ct[2], ct[3], ct[4], ct[5]);
        fprintf(fplog, "################################################################################\n");
        fflush(fplog);
        fclose(fplog);
	      exit(1);
    }
    else
    {
    	  fprintf(fplog, "#########################################\n");
    	  fprintf(fplog, "# process %s started     #\n", procname);
    	  fprintf(fplog, "#       {%04d-%02d-%02d %02d:%02d:%02d}           #\n", ct[0], ct[1], ct[2], ct[3], ct[4], ct[5]);
    	  fprintf(fplog, "#########################################\n");
        fflush(fplog);
    }
	  
    /* 
     * 4. connect database 
     */
    memset(orausr, '\0', sizeof(orausr));
    strcpy(orausr, "process_user");
    memset(orapasswd, '\0', sizeof(orapasswd));
    strcpy(orapasswd, "process_user123");
    memset(oradbname, '\0', sizeof(oradbname));
    strcpy(oradbname, "weatherdb_public");
    if (logon(orausr, orapasswd, oradbname) != 0)
    {
        fprintf(fplog,"Cannot connect to ORACLE as %s@%s\n", orausr, orapasswd, oradbname);
        fflush(fplog);
        fclose(fplog);
        logoff();
        exit(-1);
    }

	  /* 5. get typh file */
	  file_num=getfile(data_path);
	  printf("file num = %d\n", file_num);
	  for (i=0; i<file_num; i++)
	  {
	  	   memset(typh_full_file, '\0', sizeof(typh_full_file));
	  	   sprintf(typh_full_file, "%s/%s", data_path, TYPH_FILE[i]);
	  	   memset(cur_file_time, '\0', sizeof(cur_file_time));
	  	   fprintf(fplog, "FILE: %s\n", TYPH_FILE[i]);
	  	   printf("file = %s\n", TYPH_FILE[i]);
	  	   fprintf(fplog, "----------------------------\n");
	  	   fprintf(fplog, "check file\t");
	  	   if (checkfile(typh_full_file, cur_file_time)==1)
	  	   {
	  	       fprintf(fplog, "[ok]\ncheck hash\t");
	  	   	   memset(file_hash, '\0', sizeof(file_hash));
	  	   	   if (getfilehash(typh_full_file, file_hash) == 1)
	  	   	   {
	  	           fprintf(fplog, "[skip]\n");
	  	           move_file2trash(TYPH_FILE[i]);
	  	   	   	   continue;
	  	   	   }
	  	       fprintf(fplog, "[ok]\nparse file\t");
	  	   	   if (parse_typh_file(typh_full_file, key, &report_num)==1)
	  	   	   {
	  	           fprintf(fplog, "[ok]\ndb process\n");
	  	   	   	   for (j=0; j<report_num; j++)
	  	   	   	   {
	  	   	   	   	    typh_key = &key[j];
	  	   	            strcpy(typh_key->hash, file_hash);
	  	   	            strcpy(typh_key->fname, TYPH_FILE[i]);
	  	   	            strcpy(typh_key->ftime, cur_file_time);
 	   	                numtime(ct, -1);
	  	   	            sprintf(typh_key->itime, "%04d%02d%02d%02d%02d%02d", ct[0], ct[1], ct[2], ct[3], ct[4], ct[5]);
	  	   	            strcpy(typh_key->ty_status, "active");
	  	   	            
	  	   	   	        /* check typh status  */
	  	   	   	        select_typh_time_status(typh_key);
	  	   	   	        if (strncmp(typh_key->ty_status, "stop", 4)==0)
	  	   	   	        {
	  	   	   	        	  fprintf(fplog, "check status [stop]\n");
	  	   	   	        	  continue;
	  	   	   	        }
	  	   	   	        else
	  	   	   	        {
	  	   	   	        	  fprintf(fplog, "check status [active]\n");
	  	   	   	        }
	  	   	   	        fprintf(fplog, "[ typh name = %s\t typh code = %d\t typh time = %s]\n", 
	  	   	   	                       key->tyname, key->code_cn, key->ty_bjtime);

	  	   	   	        for (k=0; k<typh_key->vti_num; k++)
	  	   	   	        {
						   if(typh_key->ele[k].vti <= 120)
							{
   	  	   	   	   	         fprintf(fplog, "vti = %d\t", typh_key->ele[k].vti);

	  	   	   	   	         ret = insert_typh_babj(typh_key, &typh_key->ele[k]);
	  	   	   	   	         if (ret==0)
	  	   	   	   	             fprintf(fplog, "[ok]");
	  	   	   	   	         else
	  	   	   	   	             fprintf(fplog, "[fail]");

   	   	   	   	             ret = insert_ele_typh(typh_key, &typh_key->ele[k]);
	  	   	   	   	         if (ret==0)
	  	   	   	   	             fprintf(fplog, "[ok]\n");
	  	   	   	   	         else
	  	   	   	   	             fprintf(fplog, "[fail]\n");
							}
							else
							{
   	  	   	   	   	           fprintf(fplog, "Action! this vti = %d not insert to db...\t", typh_key->ele[k].vti);

							}
	  	   	   	        }
	  	   	   	   }
	  	   	   	   /* output data to log */
	  	   	   	   /*
	  	   	   	   for (j=0; j<report_num; j++)
	  	   	   	   {
	  	   	   	   	    output_typh(&key[j]);
	  	   	   	   }*/
	  	   	   	   /* process data to db */
	  	   	   }
	  	   	   else
	  	   	   {
	  	           fprintf(fplog, "fail\n");
	  	       }
	  	   }
	  	   else
	  	   {
	  	       fprintf(fplog, "fail\n");
	  	   }
	  	   move_file2trash(TYPH_FILE[i]);
	  }/* end for */
	  
	  
	  update_typh_status();
    /* 
     * 6. distinct connect database
     */
    logoff();
    
    /*
     * 7. close log file */
    fclose(fplog);
    
    exit(1);
}

/**************************
 * getfile 
 **************************/ 
int getfile(char *directory)
{
    struct dirent *directp;
    DIR *dirp;
    int  i, retnum=0;
    char filename[255];
    
    dirp = opendir(directory);
    if(dirp == NULL)
    {
       fprintf(fplog,"\nCan't open directory %s\n",directory);
       fflush(fplog);
       return 0;
    }
    else
    {
			 while((directp = readdir( dirp )) != NULL)
	     {
	    		 memset(filename, '\0', sizeof(filename));
	    		 strcpy(filename, directp->d_name);
	    		 for (i=0; i<strlen(filename); i++)
    				 	 filename[i]=tolower(filename[i]);
    			 if (strcmp(filename+strlen(filename)-4, ".bck")==0)
    			 {
    			 	   if (retnum >= MAX_FILE_NUM)
    			 	   	   break;
    			 	   else
    			 	   	   strcpy(TYPH_FILE[retnum++],directp->d_name); 
    			 }
	     }
    }
    closedir(dirp);
    
    return retnum;
}

/**************************
 * checkfile 
 **************************/ 
int checkfile(char *filename, char *file_time)
{
	  FILE *tempfp;
    struct stat status;
    int ret = 1, ct[10];
    
	  if(stat(filename, &status)==-1) 
		{
			 fprintf(fplog, "file %s status failed\n", filename);
		   fflush(fplog);
		   return -1;
    }
    
	  if(!S_ISREG(status.st_mode))
		{
		   fprintf(fplog, "file %s mode failed\n", filename);
		   fflush(fplog);
		   return -1;
		}
		
		if (status.st_size==0)
		{
		   fprintf(fplog, "file %s is null\n", filename);
		   fflush(fplog);
		   return -1;
		}
	
	  if ((tempfp=fopen(filename, "rb")) == NULL)
	  {
		   fprintf(fplog, "file %s read failed\n", filename);
		   fflush(fplog);
		   ret = -1;
	  }
	  fclose(tempfp);
	  
		filetime(ct,status.st_mtime);
		sprintf(file_time, "%4d%02d%02d%02d%02d%02d", ct[0], ct[1], ct[2], ct[3], ct[4], ct[5]);
		
		return ret;
}


/**************************
 * getfilehash 
 **************************/ 
int getfilehash(char *filename, char *hash_value)
{
	  char sha1_command[300];
	  int ret, pret;
	  
	  FILE *sha1ret;
	  
	  memset(sha1_command, '\0', sizeof(sha1_command));
	  sprintf(sha1_command, "/usr/bin/sha1sum %s", filename);
	  if( (sha1ret = popen(sha1_command, "r")) == NULL )
	  {
       pclose(sha1ret);
       fprintf(fplog, "\nWaring: hash code error ...\n");
		   fflush(fplog);
		   strcpy(hash_value, "ffffffffffffffffffffffffffffffffffffffff");
		   return -1; 
	  }
	  else
	  {
		   fgets(hash_value, 40, sha1ret);
  		 pclose(sha1ret);
	  }
	  
	  ret = select_typh_file_hash(hash_value);
	  if(ret == 1)
	  {	
	  	 /* file has already been processed */
		   fprintf(fplog, "\nWarning: file %s has already been processed\n", filename);
		   fflush(fplog);		
  		 return 1;
	  }		
	  else  
	  {
	  	 /* this is a new file to process */
		   /*fprintf(fplog, "hash = %s\n", hash_value);
		   fflush(fplog);	*/
		   return 0;
	  }
}

/**************************
 * parse_typh_file 
 **************************/
int parse_typh_file(char *filename, TYPH_KEY typh_key[MAX_REP_NUM], int *report_num)
{
	  TYPH_KEY *key;
	  TYPH_ELE *ele;
	  int      i, j, rep_num, key_num, word_num, word_position, ct[10], dateret;
	  FILE    *typhfp;
	  char     one_line[255], word[120][100], cur_year[5], typh_code_in[5];
	  char     report[MAX_REP_NUM][2000];
	  char     left_parenthesis, right_parenthesis;
	  char     utc_dd[4], utc_hh[4], utc_mi[4], utc_datetime[20], bj_datetime[20], utc_date[10], utc_time[10];
	  float    move_12_1,move_12_2,value_30kts_1,value_30kts_2,value_30kts_3,value_30kts_4, value_50kts_1, value_50kts_2, value_50kts_3, value_50kts_4, value_64kts_1, value_64kts_2, value_64kts_3, value_64kts_4;
	  
    /* read typh file to report .... */
		if((typhfp=fopen(filename, "r"))==NULL)
		{
			 fprintf(fplog, "open file %s error\n", filename);
			 fflush(fplog);
			 fclose(typhfp);
			 return -1;
		}
		else
		{
  	   rep_num = 0;
       memset(report[rep_num], '\0', sizeof(report[rep_num]));
              
   		 while(!feof(typhfp))
			 {
				     memset(one_line, '\0', sizeof(one_line));
             fgets(one_line, 255, typhfp);
             stringrtrim(one_line);
             if (strncmp(one_line, "NNNN", 4)==0)
             {
                 rep_num++;
                 break;
             }

             if (one_line[strlen(one_line)-1] == '=')
             {
               	 one_line[strlen(one_line)-1]='\0';
                 strcat(report[rep_num], " ");	
                 strcat(report[rep_num], one_line);	
                 rep_num++;
                 memset(report[rep_num], '\0', sizeof(report[rep_num]));
             }
             else
             {
                 strcat(report[rep_num], " ");	
                 strcat(report[rep_num], one_line);	
             }
			 }
		}
 	  fclose(typhfp);
 	  
 	  for (i=0; i<rep_num; i++)
 	  {
 	  	  stringtrim(report[i]);
 	  	  printf("%d\n%s\n", i, report[i]);
 	  }
	  
	  /* parse report to struct key and ele */  
		for (i=0,key_num=0; i<rep_num; i++)
		{
				 key = &typh_key[key_num];
				 memset(key, '\0', sizeof(key));

		 	   for (j=0; j<100; j++)
		 	   {
		 	   	  memset(word, '\0', sizeof(word));
		 	   }
				 word_num = stringsplit(report[i], ' ', 120, word);
				 word_position = -1;
				 for (j=0; j<word_num; j++)
				 {
				      stringtrim(word[j]);
				      if ((strncmp(word[j], "WT", 2)==0) && (word_position==-1))
				      {
				      	  word_position = j;
				      	  break;
				      }
				 }
				 if (word_position == -1)
				 {
				 	   if (word_num > 10)
				 	       fprintf(fplog, "report head error...\n");
				 	   continue;
				 }
				 else
				 {
				     strcpy(key->cccc, word[word_position+1]);
				 }
				 
				 word_position = stringfind(word, word_num, "FORECAST");
				 if (word_position == 0)
				 {
				 	  continue;
				 }
				 else
				 {
				 	  /* TD, TS, STS, TY, STY, */
				 	  strcpy(key->ty_type, word[word_position+1]); 
fprintf(fplog, "--- key->ty_type = %s\n", key->ty_type);
				 	  /* typh name */
				    strcpy(key->tyname, word[word_position+2]);  
fprintf(fplog, "--- key->tyname = %s\n", key->tyname);
				    /* typh china no */
				    numtime(ct, -1);
				    memset(cur_year, '\0', sizeof(cur_year));
				    sprintf(cur_year, "%02d", ct[0]-2000);
				    if (strncmp(word[word_position+3], cur_year, 2)==0)
				        key->code_cn = atoi(word[word_position+3]);  
				    else
				    	  continue;
fprintf(fplog, "--- key->code_cn = %d\n", key->code_cn);
				    	  
				    /* typh internel no */
				    left_parenthesis = word[word_position+4][0];
				    right_parenthesis = word[word_position+4][strlen(word[word_position+4])-1];
				    if (left_parenthesis=='(' && right_parenthesis==')')
				    {
				    	  strncpy(typh_code_in, word[word_position+4]+1, 4);
				    	  key->code_in = atoi(typh_code_in);
				    }
fprintf(fplog, "--- key->code_in = %d\n", key->code_in);
				    /* typh time */
				    word_position = stringfind(word, word_num, "TIME");
				    if (word_position == 0)
				 	      continue;
				    else
				    {
				    	  memset(utc_dd, '\0', sizeof(utc_dd));
				    	  memset(utc_hh, '\0', sizeof(utc_hh));
				    	  memset(utc_mi, '\0', sizeof(utc_mi));
				    	  memset(utc_datetime, '\0', sizeof(utc_datetime));
				    	  memset(bj_datetime, '\0', sizeof(bj_datetime));
				    	  memset(utc_date, '\0', sizeof(utc_date));
				    	  memset(utc_time, '\0', sizeof(utc_time));
				 	      get_substr(word[word_position+1], 0, 2, utc_dd);
				        get_substr(word[word_position+1], 2, 2, utc_hh);
				        get_substr(word[word_position+1], 4, 2, utc_mi);
				        numtime(ct, -1);
				        sprintf(bj_datetime, "%4d%02d%02d%02d%02d%02d", ct[0], ct[1], ct[2], ct[3], ct[4], ct[5]);
				        dateadd(bj_datetime, -8, utc_datetime);
				        dateret = datuma(utc_datetime, utc_dd, utc_hh, utc_mi, utc_date, utc_time);
				        if (dateret == -1) 
				        {
				    	      fprintf(fplog, "current time = %s\n", bj_datetime);
				    	      fprintf(fplog, "report  time = %s\n", word[word_position+1]);
				    	      fprintf(fplog, "Time error ... \n");
				    	      continue;
				        }
				        sprintf(key->ty_utime, "%s%s", utc_date, utc_time);		
				        dateadd(key->ty_utime, 8, key->ty_bjtime);
				    }
fprintf(fplog, "--- key->ty_utime = %s\n", key->ty_utime);
				    key->vti_num=0;
				    /* 00 HOUR */
				    word_position = stringfind(word, word_num, "00HR");
				    if (word_position > 0)
				    {
				 	     ele = &key->ele[key->vti_num];
				 	     ele->vti = 0;
						   msg_to_data(&ele->v05001, word[word_position+1], 0, 1);  /* 中心纬度   */
						   msg_to_data(&ele->v06001, word[word_position+2], 0, 1); /* 中心经度   */
						   msg_to_data(&ele->v10004, word[word_position+3], 0, 3); /* 中心气压   */
						   msg_to_data(&ele->v11041, word[word_position+4], 0, 3); /* 中心附近最大风速 */
    					 ele->v55010_1 = DEFAULT_VALUE;  /* ≥7 级风东北半径 */
          ele->v55010_2 = DEFAULT_VALUE;  /* ≥7 级风东南半径 */
          ele->v55010_3 = DEFAULT_VALUE;  /* ≥7 级风西南半径 */
          ele->v55010_4 = DEFAULT_VALUE;  /* ≥7 级风西北半径 */
						   ele->v55011_1 = DEFAULT_VALUE;  /* ≥10 级风东北半径 */
          ele->v55011_2 = DEFAULT_VALUE;  /* ≥10 级风东南半径 */
          ele->v55011_3 = DEFAULT_VALUE;  /* ≥10 级风西南半径 */
          ele->v55011_4 = DEFAULT_VALUE;  /* ≥10 级风西北半径 */
           ele->v55012_1 = DEFAULT_VALUE;  /* ≥12 级风东北半径 */
          ele->v55012_2 = DEFAULT_VALUE;  /* ≥12 级风东南半径 */
          ele->v55012_3 = DEFAULT_VALUE;  /* ≥12 级风西南半径 */
          ele->v55012_4 = DEFAULT_VALUE;  /* ≥12 级风西北半径 */
            word_position = stringfind(word, word_num, "MOVE");
				    if (word_position > 0)
				    {
				    	 if (strlen(word[word_position+1])>0 && strlen(word[word_position+2])> 0)
				    	 {
				 	        ele = &key->ele[key->vti_num];
				 	        ele->vti = 0;
				 	     
						      typh_direct(word[word_position+1], &ele->v11224);;     /* 未来移向(12时效)*/
						      msg_to_data(&ele->v11225, word[word_position+2], 0, 4);/* 未来移速(12时效)*/
						     
						    						   }
				    }
						  
						   key->vti_num++;
						   
fprintf(fplog, "--- ele->v05001 = %f\n", ele->v05001);
fprintf(fplog, "--- ele->v06001 = %f\n", ele->v06001);
fprintf(fplog, "--- ele->v10004 = %f\n", ele->v10004);
fprintf(fplog, "--- ele->v11041= %f\n", ele->v11041);
fflush(fplog);

				    }
				    /* MOVE */
       

				   
          /*12 HOUR */
				    word_position = stringfind(word, word_num, "P+12HR");
				    if (word_position > 0)
				    {
				 	     ele = &key->ele[key->vti_num];
				 	     ele->vti = 12;
						   msg_to_data(&ele->v05001, word[word_position+1], 0, 1);  /* 中心纬度   */
						   msg_to_data(&ele->v06001, word[word_position+2], 0, 1); /* 中心经度   */
						   msg_to_data(&ele->v10004, word[word_position+3], 0, 3); /* 中心气压   */
						   msg_to_data(&ele->v11041, word[word_position+4], 0, 3); /* 中心附近最大风速 */
    					ele->v55010_1 = DEFAULT_VALUE;  /* ≥7 级风东北半径 */
          ele->v55010_2 = DEFAULT_VALUE;  /* ≥7 级风东南半径 */
          ele->v55010_3 = DEFAULT_VALUE;  /* ≥7 级风西南半径 */
          ele->v55010_4 = DEFAULT_VALUE;  /* ≥7 ?斗缥鞅卑刖?*/
						   ele->v55011_1 = DEFAULT_VALUE;  /* ≥10 级风东北半径 */
          ele->v55011_2 = DEFAULT_VALUE;  /* ≥10 级风东南半径 */
          ele->v55011_3 = DEFAULT_VALUE;  /* ≥10 级风西南半径 */
          ele->v55011_4 = DEFAULT_VALUE;  /* ≥10 级风西北半径 */
           ele->v55012_1 = DEFAULT_VALUE;  /* ≥12 级风东北半径 */
          ele->v55012_2 = DEFAULT_VALUE;  /* ≥12 级?缍习刖?*/
          ele->v55012_3 = DEFAULT_VALUE;  /* ≥12 级风西南半径 */
          ele->v55012_4 = DEFAULT_VALUE;  /* ≥12 级风西北半径 */
          ele->v11224 = DEFAULT_VALUE;  /* 未来移向(12时效)*/
						   ele->v11225 = DEFAULT_VALUE;  /* 未来移向(12时效)*/
						  	 key->vti_num++;
				    }

				    /* 24 HOUR */
				    word_position = stringfind(word, word_num, "P+24HR");
				    if (word_position > 0)
				    {
				 	     ele = &key->ele[key->vti_num];
				 	     ele->vti = 24;
						   msg_to_data(&ele->v05001, word[word_position+1], 0, 1);  /* 中心纬度   */
						   msg_to_data(&ele->v06001, word[word_position+2], 0, 1); /* 中心经度   */
						   msg_to_data(&ele->v10004, word[word_position+3], 0, 3); /* 中心气压   */
						   msg_to_data(&ele->v11041, word[word_position+4], 0, 3); /* 中心附近最大风速 */
    					ele->v55010_1 = DEFAULT_VALUE;  /* ≥7 级风东北半径 */
          ele->v55010_2 = DEFAULT_VALUE;  /* ≥7 级风东南半径 */
          ele->v55010_3 = DEFAULT_VALUE;  /* ≥7 级风西南半径 */
          ele->v55010_4 = DEFAULT_VALUE;  /* ≥7 级风西北半径 */
						   ele->v55011_1 = DEFAULT_VALUE;  /* ≥10 级风东北半径 */
          ele->v55011_2 = DEFAULT_VALUE;  /* ≥10 级风东南半径 */
          ele->v55011_3 = DEFAULT_VALUE;  /* ≥10 级风西南半径 */
          ele->v55011_4 = DEFAULT_VALUE;  /* ≥10 级风西北半径 */
           ele->v55012_1 = DEFAULT_VALUE;  /* ≥12 级风东北半径 */
          ele->v55012_2 = DEFAULT_VALUE;  /* ≥12 级风东南半径 */
          ele->v55012_3 = DEFAULT_VALUE;  /* ≥12 级风西南半径 */
          ele->v55012_4 = DEFAULT_VALUE;  /* ≥12 级风西北???*/
						   ele->v11224 = DEFAULT_VALUE;  /* 未来移向(12时效)*/
						   ele->v11225 = DEFAULT_VALUE;  /* 未来移向(12时效)*/
						   key->vti_num++;
				    }
          /* 36 HOUR */
				    word_position = stringfind(word, word_num, "P+36HR");
				    if (word_position > 0)
				    {
				 	     ele = &key->ele[key->vti_num];
				 	     ele->vti = 36;
						   msg_to_data(&ele->v05001, word[word_position+1], 0, 1);  /* 中心纬度   */
						   msg_to_data(&ele->v06001, word[word_position+2], 0, 1); /* 中心经度   */
						   msg_to_data(&ele->v10004, word[word_position+3], 0, 3); /* 中心气压   */
						   msg_to_data(&ele->v11041, word[word_position+4], 0, 3); /* 中心附近最大风速 */
    					ele->v55010_1 = DEFAULT_VALUE;  /* ≥7 级风东北半径 */
          ele->v55010_2 = DEFAULT_VALUE;  /* ≥7 级风东南半径 */
          ele->v55010_3 = DEFAULT_VALUE;  /* ≥7 级风西南半径 */
          ele->v55010_4 = DEFAULT_VALUE;  /* ≥7 级风西北半径 */
						   ele->v55011_1 = DEFAULT_VALUE;  /* ≥10 级风东北半径 */
          ele->v55011_2 = DEFAULT_VALUE;  /* ≥10 级风东南半径 */
          ele->v55011_3 = DEFAULT_VALUE;  /* ≥10 级风西南半径 */
          ele->v55011_4 = DEFAULT_VALUE;  /* ≥10 级风西北半径 */
           ele->v55012_1 = DEFAULT_VALUE;  /* ≥12 级风东北半径 */
          ele->v55012_2 = DEFAULT_VALUE;  /* ≥12 级风东南半径 */
          ele->v55012_3 = DEFAULT_VALUE;  /* ≥12 级风西南半径 */
          ele->v55012_4 = DEFAULT_VALUE;  /* ≥12 级风西北半径 */
						   ele->v11224 = DEFAULT_VALUE;  /* 未来移向(12时效)*/
						   ele->v11225 = DEFAULT_VALUE;  /* 未来移向(12时效)*/
						   key->vti_num++;
				    }

				    /* 48 HOUR */
				    word_position = stringfind(word, word_num, "P+48HR");
				    if (word_position > 0)
				    {
				 	     ele = &key->ele[key->vti_num];
				 	     ele->vti = 48;
						   msg_to_data(&ele->v05001, word[word_position+1], 0, 1);  /* ?行奈扯?  */
						   msg_to_data(&ele->v06001, word[word_position+2], 0, 1); /* 中心经度   */
						   msg_to_data(&ele->v10004, word[word_position+3], 0, 3); /* 中心气??  */
						   msg_to_data(&ele->v11041, word[word_position+4], 0, 3); /* 中心附近最大风速 */
    					 ele->v55010_1 = DEFAULT_VALUE;  /* ≥7 级风东北半径 */
          ele->v55010_2 = DEFAULT_VALUE;  /* ≥7 级风东南半径 */
          ele->v55010_3 = DEFAULT_VALUE;  /* ≥7 级风西南半径 */
          ele->v55010_4 = DEFAULT_VALUE;  /* ≥7 级风西北半径 */
						   ele->v55011_1 = DEFAULT_VALUE;  /* ≥10 级风东北半径 */
          ele->v55011_2 = DEFAULT_VALUE;  /* ≥10 级风东南半径 */
          ele->v55011_3 = DEFAULT_VALUE;  /* ≥10 级风西南半径 */
          ele->v55011_4 = DEFAULT_VALUE;  /* ≥10 级风西北半径 */
           ele->v55012_1 = DEFAULT_VALUE;  /* ≥12 级风东北半径 */
          ele->v55012_2 = DEFAULT_VALUE;  /* ≥12 级风东南半径 */
          ele->v55012_3 = DEFAULT_VALUE;  /* ≥12 级风西南半径 */
          ele->v55012_4 = DEFAULT_VALUE;  /* ≥12 级风西北半径 */
						   ele->v11224 = DEFAULT_VALUE;  /* ?蠢匆葡?12时效)*/
						   ele->v11225 = DEFAULT_VALUE;  /* 未来移向(12时效)*/
						   key->vti_num++;
				    }

          /* 60 HOUR */
				    word_position = stringfind(word, word_num, "P+60HR");
				    if (word_position > 0)
				    {
				 	     ele = &key->ele[key->vti_num];
				 	     ele->vti = 60;
						   msg_to_data(&ele->v05001, word[word_position+1], 0, 1);  /* ?行奈扯?  */
						   msg_to_data(&ele->v06001, word[word_position+2], 0, 1); /* 中心经度   */
						   msg_to_data(&ele->v10004, word[word_position+3], 0, 3); /* 中心气??  */
						   msg_to_data(&ele->v11041, word[word_position+4], 0, 3); /* 中心附近最大风速 */
    					 ele->v55010_1 = DEFAULT_VALUE;  /* ≥7 级风东北半径 */
          ele->v55010_2 = DEFAULT_VALUE;  /* ≥7 级风东南半径 */
          ele->v55010_3 = DEFAULT_VALUE;  /* ≥7 级风西南半径 */
          ele->v55010_4 = DEFAULT_VALUE;  /* ≥7 级风西北半径 */
						   ele->v55011_1 = DEFAULT_VALUE;  /* ≥10 级风东北半径 */
          ele->v55011_2 = DEFAULT_VALUE;  /* ≥10 级风???习?*/
          ele->v55011_3 = DEFAULT_VALUE;  /* ≥10 级风西南半径 */
          ele->v55011_4 = DEFAULT_VALUE;  /* ≥10 级风西北半径 */
           ele->v55012_1 = DEFAULT_VALUE;  /* ≥12 级风东北半径 */
          ele->v55012_2 = DEFAULT_VALUE;  /* ≥12 级风东南半径 */
          ele->v55012_3 = DEFAULT_VALUE;  /* ≥12 级风西南半径 */
          ele->v55012_4 = DEFAULT_VALUE;  /* ≥12 级风西北半径 */
						   ele->v11224 = DEFAULT_VALUE;  /* 未来移向(12时效)*/
						   ele->v11225 = DEFAULT_VALUE;  /* 未来移向(12时效)*/
						   key->vti_num++;
				    }

				    /* 72 HOUR */
				    word_position = stringfind(word, word_num, "P+72HR");
				    if (word_position > 0)
				    {
				 	     ele = &key->ele[key->vti_num];
				 	     ele->vti = 72;
						   msg_to_data(&ele->v05001, word[word_position+1], 0, 1);  /* 中心纬度   */
						   msg_to_data(&ele->v06001, word[word_position+2], 0, 1); /* 中心经度   */
						   msg_to_data(&ele->v10004, word[word_position+3], 0, 3); /* 中心气压   */
						   msg_to_data(&ele->v11041, word[word_position+4], 0, 3); /* 中心附近最大风速 */
    					 ele->v55010_1 = DEFAULT_VALUE;  /* ≥7 级风东北半径 */
          ele->v55010_2 = DEFAULT_VALUE;  /* ≥7 级风东南半径 */
          ele->v55010_3 = DEFAULT_VALUE;  /* ≥7 级风西南半径 */
          ele->v55010_4 = DEFAULT_VALUE;  /* ≥7 级风西北半径 */
						   ele->v55011_1 = DEFAULT_VALUE;  /* ≥10 级风东北半径 */
          ele->v55011_2 = DEFAULT_VALUE;  /* ≥10 级风东南半径 */
          ele->v55011_3 = DEFAULT_VALUE;  /* ≥10 级风西南半径 */
          ele->v55011_4 = DEFAULT_VALUE;  /* ≥10 级风西北半径 */
           ele->v55012_1 = DEFAULT_VALUE;  /* ≥12 级风东北半径 */
          ele->v55012_2 = DEFAULT_VALUE;  /* ≥12 级风东南半径 */
          ele->v55012_3 = DEFAULT_VALUE;  /* ≥12 级风西南半径 */
          ele->v55012_4 = DEFAULT_VALUE;  /* ≥12 级风西北半径 */
						   ele->v11224 = DEFAULT_VALUE;  /* 未来移向(12时效)*/
						   ele->v11225 = DEFAULT_VALUE;  /* 未来移向(12时效)*/
						   key->vti_num++;
				    }
				    /* 96 HOUR */
				    word_position = stringfind(word, word_num, "P+96HR");
				    if (word_position > 0)
				    {
				 	     ele = &key->ele[key->vti_num];
				 	     ele->vti = 96;
						   msg_to_data(&ele->v05001, word[word_position+1], 0, 1);  /* 中心纬度   */
						   msg_to_data(&ele->v06001, word[word_position+2], 0, 1); /* 中心经度   */
						   msg_to_data(&ele->v10004, word[word_position+3], 0, 3); /* 中心气压   */
						   msg_to_data(&ele->v11041, word[word_position+4], 0, 3); /* 中心附近最大风速 */
    					 ele->v55010_1 = DEFAULT_VALUE;  /* ≥7 级风东北半径 */
          ele->v55010_2 = DEFAULT_VALUE;  /* ≥7 级风东南半径 */
          ele->v55010_3 = DEFAULT_VALUE;  /* ≥7 级风西南半径 */
          ele->v55010_4 = DEFAULT_VALUE;  /* ≥7 级风西北半径 */
						   ele->v55011_1 = DEFAULT_VALUE;  /* ≥10 级风东北半径 */
          ele->v55011_2 = DEFAULT_VALUE;  /* ≥10 级风东南半径 */
          ele->v55011_3 = DEFAULT_VALUE;  /* ≥10 级风西南半径 */
          ele->v55011_4 = DEFAULT_VALUE;  /* ≥10 级风西北半径 */
           ele->v55012_1 = DEFAULT_VALUE;  /* ≥12 级风东北半径 */
          ele->v55012_2 = DEFAULT_VALUE;  /* ≥12 级风东南半径 */
          ele->v55012_3 = DEFAULT_VALUE;  /* ≥12 级风西南半径 */
          ele->v55012_4 = DEFAULT_VALUE;  /* ≥12 级风西北半径 */
						   ele->v11224 = DEFAULT_VALUE;  /* 未来移向(12时效)*/
						   ele->v11225 = DEFAULT_VALUE;  /* 未来移向(12时效)*/
						   key->vti_num++;
				    }
				    /* 120 HOUR */
				    word_position = stringfind(word, word_num, "P+120HR");
				    if (word_position > 0)
				    {
				 	     ele = &key->ele[key->vti_num];
				 	     ele->vti = 120;
						   msg_to_data(&ele->v05001, word[word_position+1], 0, 1);  /* 中心纬度   */
						   msg_to_data(&ele->v06001, word[word_position+2], 0, 1); /* 中心经度   */
						   msg_to_data(&ele->v10004, word[word_position+3], 0, 3); /* 中心气压   */
						   msg_to_data(&ele->v11041, word[word_position+4], 0, 3); /* 中心附近最大风速 */
    					 ele->v55010_1 = DEFAULT_VALUE;  /* ≥7 级风东北半径 */
          ele->v55010_2 = DEFAULT_VALUE;  /* ≥7 级风东南半径 */
          ele->v55010_3 = DEFAULT_VALUE;  /* ≥7 级风西南半径 */
          ele->v55010_4 = DEFAULT_VALUE;  /* ≥7 级风西北半径 */
						   ele->v55011_1 = DEFAULT_VALUE;  /* ≥10 级风东北半径 */
          ele->v55011_2 = DEFAULT_VALUE;  /* ≥10 级风东南半径 */
          ele->v55011_3 = DEFAULT_VALUE;  /* ≥10 级风西南半径 */
          ele->v55011_4 = DEFAULT_VALUE;  /* ≥10 级风西北半径 */
           ele->v55012_1 = DEFAULT_VALUE;  /* ≥12 级风东北半径 */
          ele->v55012_2 = DEFAULT_VALUE;  /* ≥12 级风东南半径 */
          ele->v55012_3 = DEFAULT_VALUE;  /* ≥12 级风西南半径 */
          ele->v55012_4 = DEFAULT_VALUE;  /* ≥12 级风西北半径 */
						   ele->v11224 = DEFAULT_VALUE;  /* 未来移向(12时效)*/
						   ele->v11225 = DEFAULT_VALUE;  /* 未来移向(12时效)*/
						   key->vti_num++;
				    }
				    /* 30KTS and 50KTS */
				    value_30kts_1 = DEFAULT_VALUE;
         value_30kts_2 = DEFAULT_VALUE;
         value_30kts_3 = DEFAULT_VALUE;
         value_30kts_4 = DEFAULT_VALUE;
				    value_50kts_1 = DEFAULT_VALUE;
         value_50kts_2 = DEFAULT_VALUE;
         value_50kts_3 = DEFAULT_VALUE;
         value_50kts_4 = DEFAULT_VALUE;
         value_64kts_1 = DEFAULT_VALUE;
         value_64kts_2 = DEFAULT_VALUE;
         value_64kts_3 = DEFAULT_VALUE;
         value_64kts_4 = DEFAULT_VALUE;

				    word_position = stringfind(word, word_num, "30KTS");
				    if (word_position > 0)
				    {
				    	  msg_to_data(&value_30kts_1, word[word_position+2], 0, 2);
           msg_to_data(&value_30kts_2, word[word_position+4], 0, 2);
           msg_to_data(&value_30kts_3, word[word_position+6], 0, 2);
           msg_to_data(&value_30kts_4, word[word_position+8], 0, 2);
				    }
				    word_position = stringfind(word, word_num, "50KTS");
				    if (word_position > 0)
				    {
				    	  msg_to_data(&value_50kts_1, word[word_position+2], 0, 2);
           msg_to_data(&value_50kts_2, word[word_position+4], 0, 2);
           msg_to_data(&value_50kts_3, word[word_position+6], 0, 2);
           msg_to_data(&value_50kts_4, word[word_position+8], 0, 2);

				    }
         word_position = stringfind(word, word_num, "64KTS");
				    if (word_position > 0)
				    {
				    	  msg_to_data(&value_64kts_1, word[word_position+2], 0, 2);
           msg_to_data(&value_64kts_2, word[word_position+4], 0, 2);
           msg_to_data(&value_64kts_3, word[word_position+6], 0, 2);
           msg_to_data(&value_64kts_4, word[word_position+8], 0, 2);

				    }

				    if ((value_30kts_1 != DEFAULT_VALUE) || (value_50kts_1 != DEFAULT_VALUE) || (value_64kts_1 != DEFAULT_VALUE))
				    {
				    	  for (j=0; j<key->vti_num; j++)
				    	  {
				    	  	   ele = &key->ele[j];
				    	  	   if (ele->vti == 0)
				    	  	   {
				    	  	   	    ele->v55010_1 = value_30kts_1;
                  ele->v55010_2 = value_30kts_2;
                  ele->v55010_3 = value_30kts_3;
                  ele->v55010_4 = value_30kts_4;

				    	  	   	   ele->v55011_1 = value_50kts_1;
                  ele->v55011_2 = value_50kts_2;
                 ele->v55011_3 = value_50kts_3;
                ele->v55011_4 = value_50kts_4;
             
                 ele->v55012_1 = value_64kts_1;
                  ele->v55012_2 = value_64kts_2;
                 ele->v55012_3 = value_64kts_3;
                ele->v55012_4 = value_64kts_4;

				    	  	   }
				    	  }
				    }
				 }
				 if (key->vti_num > 0)
				     key_num++;
		} /* end for */
		*report_num = key_num;
		if (key_num==0)
			  return -1;
		else
        return 1;
}

/**************************
 * output typh
 **************************/
int output_typh(TYPH_KEY *key)
{
	  int i;
	  TYPH_ELE *ele;
	  
	  fprintf(fplog, "=====================================\n");
	  fprintf(fplog, "hash = %s\n", key->hash);
	  fprintf(fplog, "tyname  = %s\t cccc   = %s\t bjtime = %s\n",key->tyname, key->cccc, key->ty_bjtime);
	  fprintf(fplog, "ty_type = %s\t status = %s\t utime = %s\t\n",key->ty_type, key->ty_status, key->ty_utime);
	  fprintf(fplog, "code_cn = %d\t code_in= %d\n",key->code_cn, key->code_in);
	  fprintf(fplog, "-------------------------------------\n");
	  
	  for (i=0; i<key->vti_num; i++)
	  {
	  	   ele = &key->ele[i];
	  	   fprintf(fplog, "vti = %d\n", ele->vti);
	  	   fprintf(fplog, "v05001 = %f\t v06001 = %f\n", ele->v05001, ele->v06001);
	  	   fprintf(fplog, "v11041 = %f\t v10004 = %f\n", ele->v11041, ele->v10004);
	  	   fprintf(fplog, "v55010_1 = %f\t v55010_2 = %f\tv55010_3 = %f\tv55010_4 = %f\t v55011_1 = %f\t   v55011_2 = %f\t  v55011_3 = %f\t   v55011_4 = %f\t   v55012_1 = %f\t   v55012_2 = %f\t  v55012_3 = %f\t   v55012_4 = %f\t\n", ele->v55010_1,ele->v55010_2,ele->v55010_3,ele->v55010_4, ele->v55011_1, ele->v55011_2, ele->v55011_3, ele->v55011_4, ele->v55012_1, ele->v55012_2, ele->v55012_3, ele->v55012_4);
	  	   fprintf(fplog, "v11224 = %f\t v11225 = %f\n", ele->v11224, ele->v11225);
	       fprintf(fplog, "-------------------------------------\n");
	  }
	  fflush(fplog);
}

/**************************
 * output typh
 **************************/
int move_file2trash(char *filename)
{
	  char move_command[255];
	  
	  memset(move_command, '\0', sizeof(move_command));
	  sprintf(move_command, "mv %s/data/typh/%s %s/data.bak/typh", getenv("HOME"), filename, getenv("HOME"));
	  system(move_command);
	 
	  return 0;
}
