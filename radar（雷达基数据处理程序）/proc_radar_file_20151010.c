#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/stat.h>
#include <dirent.h>
#include "ocilib.h"
#include "public.h"
#include "ocilib.c"

#define MAX_FILES 1000
char RadarFile[MAX_FILES][100];

int main(int argc, char *argv[])
{
	  int  ct[10], i, j,ret, str_num=0, file_num=0,size=0;
	  char logname[255], gz_file_path[255], str[11][100];
   char user[30], passwd[30], dbname[30];
    char itime[14],ptime[14],sdir[300],full_filename[300],tdir1[300],tdir2[300],fname[500],filesize[50],pname[50],sip[20],sname[50],tip1[20],tip2[20],tname1[50],tname2[50];
    char data_dir[50],data_file[50],data_file1[50];
    FILE *file_list;
     FILE *file_list1;
	  FILE *fplog;
	  char radar_number[10];
	  char cmd[1000],cmd1[1000],cmd2[1000], argc_radarno[10];
	  
    if (argc > 1)
    {
        memset(argc_radarno, '\0', sizeof(argc_radarno));
        stringtrim(argv[1]);
        strcpy(argc_radarno, argv[1]);
    }
    else
    {
        printf("arguments error ... \n");
        exit(-1);
    }

    /*
     * 1. open log file
     */
    numtime(ct, -1);
    memset(logname, '\0', sizeof(logname));
    sprintf(logname, "%s/log/radar_base/radar_%04d%02d%02d.log", getenv("HOME"), ct[0], ct[1], ct[2]);
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
    -----2. connect database 
      */
   
    memset(user, '\0', sizeof(user));
    memset(passwd, '\0', sizeof(passwd));
    memset(dbname, '\0', sizeof(dbname));
    

    strcpy(user, "process_user");
    strcpy(passwd, "process_user123");
    strcpy(dbname, "awstdb");
    
    ret = logon(user, passwd, dbname);

    if (ret != 0)
    {
    	printf("Connect Oracle Fail ...\n");
    	printf("%s\n", oci_err);
    	logoff();
    	exit(-1);
    }

	  
    /* 
     * 3. get radar gz file  and unzip
     */
    memset(gz_file_path,'\0',sizeof(gz_file_path));
    sprintf(gz_file_path,"%s/radardata", getenv("HOME"));
    sprintf(data_file, "%s/bin/ftp_radar_240/240_data_%s.sh", getenv("HOME"), argv[1]);
     sprintf(data_file1, "%s/bin/ftp_radar_249/249_data_%s.sh", getenv("HOME"), argv[1]);

     
    if ((file_list=fopen(data_file,"w")) == NULL) 
    {
         fprintf(fplog, "cannot write file %s\n", data_file);
         return -1;
    }
        fprintf(file_list,"source /home/radar_user/.bash_profile\n");             
    	   fprintf(file_list,"FTP_PRODUCT_FILE=/home/radar_user/log/ftp_radar_%s.log\n",argv[1]);  
        fprintf(file_list,"echo \"user nmic_provider  nmic_provider123\" > $FTP_PRODUCT_FILE\n");  
                fprintf(file_list,"echo \"bin\" >> $FTP_PRODUCT_FILE\n");    
               fprintf(file_list,"echo \"prom\" >> $FTP_PRODUCT_FILE\n");   

    if ((file_list1=fopen(data_file1,"w")) == NULL) 
    {
         fprintf(fplog, "cannot write file %s\n", data_file1);
         return -1;
    }
        fprintf(file_list1,"source /home/radar_user/.bash_profile\n");             
    	   fprintf(file_list1,"FTP_PRODUCT_FILE=/home/radar_user/log/ftp_radar_249_%s.log\n",argv[1]);  
        fprintf(file_list1,"echo \"user radarbase  radarbase123\" > $FTP_PRODUCT_FILE\n");  
         fprintf(file_list1,"echo \"bin\" >> $FTP_PRODUCT_FILE\n");    
         fprintf(file_list1,"echo \"prom\" >> $FTP_PRODUCT_FILE\n");    
           
    while(1)
    {
       file_num = getfile(gz_file_path, argc_radarno);

       if (file_num == 0)
       	   break;
       
       
       for(i=0; i<file_num; i++)
       {
   	      /* parse file name */
          for (j=0; j<10; j++)
   	           memset(str[j], '\0', sizeof(str[j]));
    	               
          str_num = stringsplit(RadarFile[i], '_', 9, str);
          memset(radar_number, '\0', sizeof(radar_number));
          sprintf(itime, "%04d%02d%02d%02d%02d%02d", ct[0], ct[1], ct[2], ct[3], ct[4], ct[5]);

          
          
          strcpy(radar_number, str[3]+1);
           
       
          memset(cmd, '\0', sizeof(cmd));
          sprintf(cmd, "mkdir -p %s/%s;mv %s/%s %s/%s; cd %s/%s; bzip2 -d %s/%s/%s", 
                       gz_file_path, radar_number, gz_file_path, RadarFile[i], gz_file_path, radar_number, gz_file_path, radar_number,
                       gz_file_path, radar_number, RadarFile[i]);
          system(cmd); 
          
          fprintf(fplog,"%s rqpf-0 1 O \"%s_%s#%s#Z_RADR_I_%s_%s_%s_%s_%s_CAP.bin\" \n", itime,str[3],str[4],RadarFile[i],str[3],str[4],str[5],str[6],str[7]);
          fflush(fplog);            
    	            
            fprintf(file_list,"echo \"cd /mnt/data_nfs/cma/nmic/radar_base/bin/%s\" >> $FTP_PRODUCT_FILE\n",radar_number); 
          fprintf(file_list,"echo \"lcd /home/radar_user/radardata/%s\" >> $FTP_PRODUCT_FILE\n",radar_number);       
           fprintf(file_list,"echo \"mput Z_RADR_I_%s_%s_%s_%s_%s_CAP.bin\" >> $FTP_PRODUCT_FILE\n",str[3],str[4],str[5],str[6],str[7]);   
          
           fprintf(file_list1,"echo \"mkdir /%s\" >> $FTP_PRODUCT_FILE\n",radar_number);     
           fprintf(file_list1,"echo \"cd /%s\" >> $FTP_PRODUCT_FILE\n",radar_number); 
          fprintf(file_list1,"echo \"lcd /home/radar_user/radardata/%s\" >> $FTP_PRODUCT_FILE\n",radar_number);       
           fprintf(file_list1,"echo \"mput Z_RADR_I_%s_%s_%s_%s_%s_CAP.bin\" >> $FTP_PRODUCT_FILE\n",str[3],str[4],str[5],str[6],str[7]);   

            sprintf(itime, "%04d%02d%02d%02d%02d%02d", ct[0], ct[1], ct[2], ct[3], ct[4], ct[5]);
            sprintf(ptime, "%s",str[4]);
            sprintf(sdir, "%s/%s",gz_file_path,radar_number);
            sprintf(tdir1, "/mnt/data_nfs/cma/nmic/radar_base/bin/%s",radar_number);
              sprintf(tdir2, "/%s",radar_number);

            sprintf(fname, "Z_RADR_I_%s_%s_%s_%s_%s_CAP.bin",str[3],str[4],str[5],str[6],str[7]);
            sprintf(full_filename, "%s/%s",sdir,fname);
              
            size=file_size(full_filename)/1024;
            sprintf(filesize,"%d KB",size);
             // printf("%s\n",filesize);


              
            sprintf(pname, "RADAR_BASE_BIN");
            sprintf(sip, "10.10.31.135");
            sprintf(sname, "radar_user");
            sprintf(tip1, "10.30.16.240");
            sprintf(tname1, "nmic_provider");
              sprintf(tip2, "10.30.16.249");
            sprintf(tname2, "radarbase");
             ret=insert_radr(itime,ptime,sdir,tdir1,fname,filesize,pname,sip,sname,tip1,tname1);
    
    		        if(ret!=0)
    		         {
    			           printf("call insert_surf_surx() failed.\n");
    			           exit(-1);
    		         }
               //ret=insert_radr(itime,ptime,sdir,tdir2,fname,filesize,pname,sip,sname,tip2,tname2);
    
    		        /*if(ret!=0)
    		         {
    			           printf("call insert_surf_surx() failed.\n");
    			           exit(-1);
    		         }*/


       }
    }

       fprintf(file_list,"ftp -nv 10.30.16.240< $FTP_PRODUCT_FILE\n\n");  
         fprintf(file_list1,"ftp -nv 10.30.16.249< $FTP_PRODUCT_FILE\n\n");  
    
    /*
     * 3. close log file */
    fclose(fplog);
     logoff();    

     fclose(file_list);
      fclose(file_list1);
 
          memset(cmd1, '\0', sizeof(cmd1));
          sprintf(cmd1, "sh %s/bin/ftp_radar_240/240_data_%s.sh", getenv("HOME"), argv[1]);
          system(cmd1); 

          memset(cmd2, '\0', sizeof(cmd2));
          sprintf(cmd2, "sh %s/bin/ftp_radar_249/249_data_%s.sh", getenv("HOME"), argv[1]);
          system(cmd2); 


    
    exit(1);
}


/**************************
 * getfile 
 **************************/ 
int getfile(char* directory, char* radar_no)
{
    struct dirent *directp;
    DIR *dirp;
    int  i, retnum=0;
    char filter[50];
    
    memset(filter, '\0', sizeof(filter));
    sprintf(filter, "Z_RADR_I_Z%s_", radar_no);
    
    dirp = opendir(directory);
    if(dirp == NULL)
    {
    	  /*fprintf(fplog,"\nCan't open directory %s\n", directory);
 	      fflush(fplog);*/
    }
    else
    {
    	  while((directp = readdir( dirp )) != NULL)
        {

           if ( (strcmp(directp->d_name,".")!=0) && (strcmp(directp->d_name,"..")!=0) &&
                (strstr(directp->d_name,".tmp")==0) && (strstr(directp->d_name,".TMP")==0))
           {
           	   if (strncmp(directp->d_name, filter, strlen(radar_no)+10)==0)
           	   {
           	   	
            	    strcpy(RadarFile[retnum++],directp->d_name);
	                if(retnum > (MAX_FILES-1))
  	                 break;
	            }
           }
        }
    }
    closedir(dirp);
       
    return retnum;
}
