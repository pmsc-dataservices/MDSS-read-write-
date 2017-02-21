#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <oci.h>
#include "ocilib.h"
#include "public.h"

OCIEnv         *envhp;
OCIError       *errhp;
OCIServer      *srvhp;
OCISvcCtx      *svchp;
OCISession     *authp;
OCIStmt        *stmthp;
OCIStmt        *stmthp1;
OCIStmt        *stmthp2;

/*********************************************************************
**
**    Function    :  logon() 
**    Description :  begin session with database and allocate resource
**    Author      :  LanHaiBo 
**
**********************************************************************/
int logon(char *username, char *password, char *dbname)
{
  sword status;
  
  /* 1  
   * create OCI environment */
  OCIEnvCreate(&envhp,
               OCI_DEFAULT,
               (dvoid *) 0,
               (dvoid * (*)(dvoid *, size_t)) 0,
               (dvoid * (*)(dvoid *, dvoid *, size_t)) 0,
               (void (*)(dvoid *, dvoid *)) 0,
               0,
               (dvoid **) 0);

  /* 2.1 
   * allocating ERROR handle */
  OCIHandleAlloc((dvoid *) envhp,
                 (dvoid **) &errhp,
                 OCI_HTYPE_ERROR,
                 (size_t) 0,
                 (dvoid **) 0);
  /* 2.2 
   * allocating SERVER handle */
  OCIHandleAlloc((dvoid *) envhp,
                 (dvoid **) &srvhp,
                 OCI_HTYPE_SERVER,
                 (size_t) 0,
                 (dvoid **) 0);
  /* 2.3 
   * allocating SERVICE CONTEXT handle */
  OCIHandleAlloc((dvoid *) envhp,
                 (dvoid **) &svchp,
                 OCI_HTYPE_SVCCTX,
                 (size_t) 0,
                 (dvoid **) 0);

  /* 3
   * connect database */
  status = OCIServerAttach(srvhp, errhp,
                           (text *)dbname, (ub4) strlen(dbname), 0);
  if (status != OCI_SUCCESS)
  {
     return -1;
  }
                 
  /* 4
   * set SERVICE CONTEXT relating SERVER property */
  OCIAttrSet((dvoid *) svchp,
             OCI_HTYPE_SVCCTX,
             (dvoid *) srvhp,
             (ub4) 0,
             OCI_ATTR_SERVER,
             errhp);
                 

  /* 5.1 
   * allocating SESSION handle */
  OCIHandleAlloc((dvoid *) envhp,
                 (dvoid **) &authp,
                 OCI_HTYPE_SESSION,
                 (size_t) 0,
                 (dvoid **) 0);
  /* 
   * 5.2 set SESSION property */
  OCIAttrSet((dvoid *) authp,
             OCI_HTYPE_SESSION,
             (text *)username,
             (ub4) strlen(username),
             OCI_ATTR_USERNAME,
             errhp);
  OCIAttrSet((dvoid *) authp,
             OCI_HTYPE_SESSION,
             (text *)password,
             (ub4) strlen(password),
             OCI_ATTR_PASSWORD,
             errhp);

  /* 6 
   * set SERVICE CONTEXT (SESSION) property */
  OCIAttrSet((dvoid *) svchp,
             OCI_HTYPE_SVCCTX,
             (dvoid *) authp,
             (ub4) 0,
             OCI_ATTR_SESSION,
             errhp);

  /* 7
   * allocating STATMENT handle */
  OCIHandleAlloc((dvoid *) envhp,
                 (dvoid **) &stmthp,
                 OCI_HTYPE_STMT,
                 (size_t) 0,
                 (dvoid **) 0);
  OCIHandleAlloc((dvoid *) envhp,
                 (dvoid **) &stmthp1,
                 OCI_HTYPE_STMT,
                 (size_t) 0,
                 (dvoid **) 0);
  OCIHandleAlloc((dvoid *) envhp,
                 (dvoid **) &stmthp2,
                 OCI_HTYPE_STMT,
                 (size_t) 0,
                 (dvoid **) 0);               
             
  /* 8
   * begin session */
  status = OCISessionBegin(svchp, errhp, authp, 
                           OCI_CRED_RDBMS, (ub4) OCI_DEFAULT);
  if (status != OCI_SUCCESS)
  {
     return -1;
  }
  
  return 0;
}

/********************************************************************
**
**    Function    :  logoff()
**    Description :  end session with database and release resource
**    Author      :  LanHaiBo
**
********************************************************************/
void logoff()
{
  OCISessionEnd(svchp, errhp, authp, (ub4) 0);
  OCIServerDetach(srvhp, errhp, (ub4) OCI_DEFAULT);
  OCIHandleFree((dvoid *) envhp, OCI_HTYPE_ENV);
}


/*********************************************************************
**
**    Function    :  error_proc() 
**    Author      :  LanHaiBo 
**
**********************************************************************/
void error_proc(dvoid *errhp, sword status)
{
  text errbuf[512];
  sb4  errcode;
  
  memset(oci_err, '\0', sizeof(oci_err));  
  switch (status)
  {
    case OCI_SUCCESS:
         break;
    case OCI_SUCCESS_WITH_INFO:
         strcpy(oci_err, "OCI ERROR CODE: OCI_SUCCESS_WITH_INFO\n");
         break;
    case OCI_NEED_DATA:
         strcpy(oci_err, "OCI ERROR CODE: OCI_NEED_DATA\n");
         break;
    case OCI_NO_DATA:
         strcpy(oci_err, "OCI ERROR CODE: OCI_NO_DATA\n");
         break;
    case OCI_ERROR:
         OCIErrorGet((dvoid *) errhp, 
                     (ub4) 1, 
                     NULL,
                     &errcode,
                     errbuf, (ub4) sizeof(errbuf), 
                     OCI_HTYPE_ERROR);
         sprintf(oci_err, "OCI ERROR CODE:%d\n OCI ERROR INFO:%s\n", errcode, errbuf);
         break;
    case OCI_INVALID_HANDLE:
         strcpy(oci_err, "OCI ERROR CODE: OCI_INVALID_HANDLE\n");
         break;
    case OCI_STILL_EXECUTING:
         strcpy(oci_err, "OCI ERROR CODE: OCI_STILL_EXECUTING\n");
         break;
    default:
         strcpy(oci_err, "OCI UNKNOW ERROR\n");
         break;      
  }
  
}
/*********************************************************************
**
**    Function    :  select_awst() 
**    Author      :  FengYuXing 
**
**********************************************************************/
int select_awst(char  *data_dir,char  *time)
{
    char insert_statement[500],select_statement[255],select_statement2[800],delete_statement[255];
    int count=0,ret=0;
     FILE *file_list;
     char  rownum[5];
     double station,rain;
     char  prov[30];
      char data_file[1000];
     int   ct[10];
    sword  status;
    OCIDefine *def1hp,*def2hp,*def3hp;
    OCIBind   *bnd1hp, *bnd2hp, *bnd3hp, *bnd4hp, *bnd5hp, *bnd6hp, *bnd7hp, *bnd8hp,*bnd9hp, *bnd10hp, *bnd11hp, *bnd12hp;
        
     /*
    --- select
    */
    // printf("fengyxuing1\n");
    memset(select_statement, '\0', sizeof(select_statement));
    sprintf(select_statement,"select distinct province from observe.dict_station_area");
    // printf("%s\n",select_statement);
   // printf("fengyxuing2\n");
    status = OCIStmtPrepare(stmthp1, errhp,
                            select_statement, (ub4) strlen(select_statement),
                            (ub4) OCI_NTV_SYNTAX, (ub4) OCI_DEFAULT);
    
    status |= OCIDefineByPos(stmthp1, &def1hp, errhp, 1, 
                             (ub1 *)&prov, (sword) sizeof(prov), SQLT_STR, 
                             (dvoid *) 0, (ub2 *) 0, (ub2 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIStmtExecute(svchp, stmthp1, errhp, (ub4) 0, (ub4) 0,
                              NULL, NULL, (ub4) OCI_DEFAULT);
    if (status != OCI_SUCCESS)
    { 
       // printf("fengyxuing3\n");
    	error_proc(errhp, status);
    	printf("%s\n",oci_err); 
    	return -1;
    }
         
     
    while ((status = OCIStmtFetch(stmthp1, errhp, 1, OCI_FETCH_NEXT, OCI_DEFAULT)) != OCI_NO_DATA)
    {  
        //printf("%s\n",prov);
        numtime(ct, -1);
         //memset(time,'\0',sizeof(time));
         //sprintf(time,"%4d%02d%02d%02d0000",ct[0],ct[1],ct[2],ct[3]);
         //printf("%s\n",time);
           memset(data_file,'\0',sizeof(data_file));
          sprintf(data_file, "%s/awst_rain_%s.dat",data_dir,time);
    	 // printf("%s\n",data_file);

                
     if ((file_list=fopen(data_file,"w")) == NULL) 
     {
           fprintf("cannot write file %s\n", data_file);
           return -1;
     }
      fclose(file_list);
     file_list=fopen(data_file,"a");

          /*
          --- select
           */
    // printf("fengyxuing1\n");
    memset(select_statement2, '\0', sizeof(select_statement2));
    sprintf(select_statement2,"select v01000,v13019,rownum  r from (select a.v01000,a.v13019 ,a.c_bjtime ,b.province "\
                                  " from observe.ele_awst_area a,observe.dict_station_area b  "\ 
                                  " where c_bjtime='%s' and  a.v01000=b.stationid   and v13019<3000  order by v13019 desc)"\ 
                                  " where province='%s' and rownum<21",time,prov);     
     //printf("%s\n",select_statement2);
   // printf("fengyxuing2\n");
    status = OCIStmtPrepare(stmthp2, errhp,
                            select_statement2, (ub4) strlen(select_statement2),
                            (ub4) OCI_NTV_SYNTAX, (ub4) OCI_DEFAULT);
    
    status |= OCIDefineByPos(stmthp2, &def1hp, errhp, 1, 
                             (ub1 *)&station,(sword) sizeof(double), SQLT_FLT, 
                             (dvoid *) 0, (ub2 *) 0, (ub2 *) 0, (ub4) OCI_DEFAULT);
      status |= OCIDefineByPos(stmthp2, &def2hp, errhp, 2, 
                             (ub1 *)&rain, (sword) sizeof(double), SQLT_FLT, 
                             (dvoid *) 0, (ub2 *) 0, (ub2 *) 0, (ub4) OCI_DEFAULT);
      status |= OCIDefineByPos(stmthp2, &def3hp, errhp, 3, 
                             (ub1 *)&rownum, (sword) sizeof(rownum), SQLT_STR, 
                             (dvoid *) 0, (ub2 *) 0, (ub2 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIStmtExecute(svchp, stmthp2, errhp, (ub4) 0, (ub4) 0,
                              NULL, NULL, (ub4) OCI_DEFAULT);
    if (status != OCI_SUCCESS)
    { 
       printf("fengyxuing3\n");
    	error_proc(errhp, status);
    	printf("%s\n",oci_err); 
    	return -1;
    }
    while ((status = OCIStmtFetch(stmthp2, errhp, 1, OCI_FETCH_NEXT, OCI_DEFAULT)) != OCI_NO_DATA)
    {  
        //printf("%f\n",station);
       // printf("%f\n",rain);
       // printf("%s\n",rownum); 
         fprintf(file_list,"%f %s %f",station,time,rain);
         fprintf(file_list,"\n");
    }

    }
    fclose(file_list);
   return 0;
}


/*********************************************************************
**
**    Function    :  write_awst() 
**    Author      :  FengYuXing 
**
**********************************************************************/
int write_awst(char *date,float value[])
{
    char insert_statement[255],select_statement[255],delete_statement[255];
    int count=0,ret=0;
    sword  status;
    OCIDefine *def1hp;
    OCIBind   *bnd1hp, *bnd2hp, *bnd3hp, *bnd4hp,*bnd5hp, *bnd6hp, *bnd7hp, *bnd8hp,*bnd9hp, *bnd10hp, *bnd11hp, *bnd12hp;
    OCIBind   *bnd13hp, *bnd14hp, *bnd15hp, *bnd16hp,*bnd17hp;
    
    
    /*
    --- select
    */
    memset(select_statement, '\0', sizeof(select_statement));
   
    sprintf(select_statement,"select count(*) as num from observe.ele_awst_rain@RACDB where v01000 = :1 and c_bjtime = :2");
   

    status = OCIStmtPrepare(stmthp1, errhp,
                            select_statement, (ub4) strlen(select_statement),
                            (ub4) OCI_NTV_SYNTAX, (ub4) OCI_DEFAULT);
    
    status |= OCIBindByPos(stmthp1, &bnd1hp, errhp, 1,
                           (dvoid *)&value[0], (sword)sizeof(value[0]), SQLT_FLT, 
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
     status |= OCIBindByPos(stmthp1, &bnd2hp, errhp, 2,
                           (dvoid *)date, strlen(date)+1, SQLT_STR,
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIDefineByPos(stmthp1, &def1hp, errhp, 1, 
                             (ub1 *)&count, (sword)sizeof(count), SQLT_INT, 
                             (dvoid *) 0, (ub2 *) 0, (ub2 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIStmtExecute(svchp, stmthp1, errhp, (ub4) 0, (ub4) 0,
                              NULL, NULL, (ub4) OCI_DEFAULT);
    if (status != OCI_SUCCESS)
    { 
    	error_proc(errhp, status);
    	printf("%s\n",oci_err); 
    	return -1;
    }
    while ((status = OCIStmtFetch(stmthp1, errhp, 1, OCI_FETCH_NEXT, OCI_DEFAULT)) != OCI_NO_DATA)
    {
    	ret = count;
    }
    //printf("%d\n",ret);
    /*
    --- delect
    */
    if(ret>0)
    {    
       //printf("fengyuxing1\n");  
      memset(delete_statement,'\0',sizeof(delete_statement));
      sprintf(delete_statement,"update  observe.ele_awst_rain@RACDB set rain = :3 where v01000 = :1 and c_bjtime = :2");
      
       
      status = OCIStmtPrepare(stmthp2, errhp,
                              delete_statement, (ub4) strlen(delete_statement),
                              (ub4) OCI_NTV_SYNTAX, (ub4) OCI_DEFAULT);
     
      status |= OCIBindByPos(stmthp2, &bnd1hp, errhp, 1,
                           (dvoid *)&value[0], (sword)sizeof(value[0]), SQLT_FLT, 
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
        status |= OCIBindByPos(stmthp2, &bnd2hp, errhp, 2,
                           (dvoid *)date, strlen(date)+1, SQLT_STR,
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
       status |= OCIBindByPos(stmthp2, &bnd3hp, errhp, 3,
                           (dvoid *)&value[1], (sword)sizeof(value[1]), SQLT_FLT, 
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    
      status |= OCIStmtExecute(svchp, stmthp2, errhp, (ub4) 1, (ub4) 0,
                                NULL, NULL, (ub4) OCI_DEFAULT);
      if (status != OCI_SUCCESS)
      { 
      	error_proc(errhp, status);
      	printf("%s\n",oci_err); 
      	return -1;
      }
    }
    else{
    /*
    --- insert
    */
  
    memset(insert_statement, '\0', sizeof(insert_statement));
    sprintf(insert_statement, "insert into observe.ele_awst_rain@RACDB values(:1,:2,:3)");
    

    status = OCIStmtPrepare(stmthp, errhp, 
                            insert_statement, (ub4) strlen(insert_statement),
                            (ub4) OCI_NTV_SYNTAX, (ub4) OCI_DEFAULT);
    
    
    status |= OCIBindByPos(stmthp, &bnd1hp, errhp, 1,
                           (dvoid *)&value[0], (sword)sizeof(value[0]), SQLT_FLT,
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
     status |= OCIBindByPos(stmthp, &bnd2hp, errhp, 2,
                           (dvoid *)date, strlen(date)+1, SQLT_STR,
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp, &bnd3hp, errhp, 3,
                           (dvoid *)&value[1], (sword)sizeof(value[1]), SQLT_FLT,
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
        
    status |= OCIStmtExecute(svchp, stmthp, errhp, (ub4) 1, (ub4) 0,
                             (CONST OCISnapshot *) 0,
                             (OCISnapshot *) 0, 
                             (ub4) OCI_DEFAULT);
    if (status != OCI_SUCCESS)
    { 
    	error_proc(errhp, status);
    	printf("%s\n",oci_err); 
    	return -1;
    }
    
   } 
    /*
    --- transaction commit
    */
    status = OCITransCommit(svchp, errhp, 0);
    if (status != OCI_SUCCESS)
    { 
      error_proc(errhp, status);
      printf("%s\n",oci_err); 
      return -1;
    }
    return 0;
}



/*********************************************************************
**
**    Function    :  select_awst_wind() 
**    Author      :  FengYuXing 
**
**********************************************************************/
int select_awst_wind(char  *data_dir,char  *time)
{
    char insert_statement[500],select_statement[255],select_statement2[800],delete_statement[255];
    int count=0,ret=0;
     FILE *file_list;
     char  rownum[5];
     double station,ws,wd;
     char  prov[30];
      char data_file[1000];
     int   ct[10];
    sword  status;
    OCIDefine *def1hp,*def2hp,*def3hp,*def4hp;
    OCIBind   *bnd1hp, *bnd2hp, *bnd3hp, *bnd4hp, *bnd5hp, *bnd6hp, *bnd7hp, *bnd8hp,*bnd9hp, *bnd10hp, *bnd11hp, *bnd12hp;
        
     /*
    --- select
    */
    // printf("fengyxuing1\n");
    memset(select_statement, '\0', sizeof(select_statement));
    sprintf(select_statement,"select distinct province from observe.dict_station_area");
    // printf("%s\n",select_statement);
   // printf("fengyxuing2\n");
    status = OCIStmtPrepare(stmthp1, errhp,
                            select_statement, (ub4) strlen(select_statement),
                            (ub4) OCI_NTV_SYNTAX, (ub4) OCI_DEFAULT);
    
    status |= OCIDefineByPos(stmthp1, &def1hp, errhp, 1, 
                             (ub1 *)&prov, (sword) sizeof(prov), SQLT_STR, 
                             (dvoid *) 0, (ub2 *) 0, (ub2 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIStmtExecute(svchp, stmthp1, errhp, (ub4) 0, (ub4) 0,
                              NULL, NULL, (ub4) OCI_DEFAULT);
    if (status != OCI_SUCCESS)
    { 
       // printf("fengyxuing3\n");
    	error_proc(errhp, status);
    	printf("%s\n",oci_err); 
    	return -1;
    }
         
     
    while ((status = OCIStmtFetch(stmthp1, errhp, 1, OCI_FETCH_NEXT, OCI_DEFAULT)) != OCI_NO_DATA)
    {  
        //printf("%s\n",prov);
        // printf("%s\n",time);
           memset(data_file,'\0',sizeof(data_file));
          sprintf(data_file, "%s/awst_wd_%s.dat",data_dir,time);
    	 // printf("%s\n",data_file);

                
     if ((file_list=fopen(data_file,"w")) == NULL) 
     {
           fprintf("cannot write file %s\n", data_file);
           return -1;
     }
      fclose(file_list);
     file_list=fopen(data_file,"a");

          /*
          --- select
           */
    // printf("fengyxuing1\n");
    memset(select_statement2, '\0', sizeof(select_statement2));
    sprintf(select_statement2,"select v01000,v11041,v11043,rownum r  from "\
                                    " (select a.v01000,a.c_bjtime,a.v11041,a.v11043,b.province "\
                                    " from observe.ele_awst_area a,observe.dict_station_area b     "\
                                    " where c_bjtime='%s' and v11041<>999999 and   v11041<65  and a.v01000=b.stationid order by v11041 desc) "\
                                    " where province='%s' and rownum<21",time,prov);     
    // printf("%s\n",select_statement2);
   // printf("fengyxuing2\n");
    status = OCIStmtPrepare(stmthp2, errhp,
                            select_statement2, (ub4) strlen(select_statement2),
                            (ub4) OCI_NTV_SYNTAX, (ub4) OCI_DEFAULT);
    
    status |= OCIDefineByPos(stmthp2, &def1hp, errhp, 1, 
                             (ub1 *)&station,(sword) sizeof(double), SQLT_FLT, 
                             (dvoid *) 0, (ub2 *) 0, (ub2 *) 0, (ub4) OCI_DEFAULT);
      status |= OCIDefineByPos(stmthp2, &def2hp, errhp, 2, 
                             (ub1 *)&ws, (sword) sizeof(double), SQLT_FLT, 
                             (dvoid *) 0, (ub2 *) 0, (ub2 *) 0, (ub4) OCI_DEFAULT);
        status |= OCIDefineByPos(stmthp2, &def3hp, errhp, 3, 
                             (ub1 *)&wd, (sword) sizeof(double), SQLT_FLT, 
                             (dvoid *) 0, (ub2 *) 0, (ub2 *) 0, (ub4) OCI_DEFAULT);
      status |= OCIDefineByPos(stmthp2, &def4hp, errhp, 4, 
                             (ub1 *)&rownum, (sword) sizeof(rownum), SQLT_STR, 
                             (dvoid *) 0, (ub2 *) 0, (ub2 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIStmtExecute(svchp, stmthp2, errhp, (ub4) 0, (ub4) 0,
                              NULL, NULL, (ub4) OCI_DEFAULT);
    if (status != OCI_SUCCESS)
    { 
       printf("fengyxuing3\n");
    	error_proc(errhp, status);
    	printf("%s\n",oci_err); 
    	return -1;
    }
    while ((status = OCIStmtFetch(stmthp2, errhp, 1, OCI_FETCH_NEXT, OCI_DEFAULT)) != OCI_NO_DATA)
    {  
       // printf("%f\n",station);
       // printf("%f\n",ws);
       //   printf("%f\n",wd);
       // printf("%s\n",rownum); 
         fprintf(file_list,"%f %s %f %f",station,time,ws,wd);
         fprintf(file_list,"\n");
    }

    }
    fclose(file_list);
   return 0;
}

/*********************************************************************
**
**    Function    :  write_awst_wd() 
**    Author      :  FengYuXing 
**
**********************************************************************/
int write_awst_wd(char *date,float value[])
{
    char insert_statement[255],select_statement[255],update_statement[500];
    int count=0,ret=0;
     char wdlevel[30];
     int  wslevel;
    sword  status;
    OCIDefine *def1hp,*def2hp,*def3hp;
    OCIBind   *bnd1hp, *bnd2hp, *bnd3hp, *bnd4hp,*bnd5hp, *bnd6hp, *bnd7hp, *bnd8hp,*bnd9hp, *bnd10hp, *bnd11hp, *bnd12hp;
    OCIBind   *bnd13hp, *bnd14hp, *bnd15hp, *bnd16hp,*bnd17hp;
    
    
    /*
    --- select
    */
    memset(select_statement, '\0', sizeof(select_statement));
   
    sprintf(select_statement,"select (select count(*) from observe.ele_awst_wind@RACDB where v01000 = :1 and c_bjtime = :2 ),"\
                                  " (select name as wd_level from base.observe_yaan_dict_wd where code=name_user.get_forecast_fine_wd_level('%f')), "\
                                  " (select name_user.get_forecast_fine_ws_level(%f) as ws_level from dual) from dual",value[2],value[1]);
   

    status = OCIStmtPrepare(stmthp1, errhp,
                            select_statement, (ub4) strlen(select_statement),
                            (ub4) OCI_NTV_SYNTAX, (ub4) OCI_DEFAULT);
    
    status |= OCIBindByPos(stmthp1, &bnd1hp, errhp, 1,
                           (dvoid *)&value[0], (sword)sizeof(value[0]), SQLT_FLT, 
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
     status |= OCIBindByPos(stmthp1, &bnd2hp, errhp, 2,
                           (dvoid *)date, strlen(date)+1, SQLT_STR,
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIDefineByPos(stmthp1, &def1hp, errhp, 1, 
                             (ub1 *)&count, (sword)sizeof(count), SQLT_INT, 
                             (dvoid *) 0, (ub2 *) 0, (ub2 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIDefineByPos(stmthp1, &def2hp, errhp, 2, 
                             (ub1 *)&wdlevel, (sword)sizeof(wdlevel), SQLT_STR, 
                             (dvoid *) 0, (ub2 *) 0, (ub2 *) 0, (ub4) OCI_DEFAULT);
     status |= OCIDefineByPos(stmthp1, &def3hp, errhp, 3, 
                             (ub1 *)&wslevel, (sword)sizeof(wslevel), SQLT_INT, 
                             (dvoid *) 0, (ub2 *) 0, (ub2 *) 0, (ub4) OCI_DEFAULT);

    status |= OCIStmtExecute(svchp, stmthp1, errhp, (ub4) 0, (ub4) 0,
                              NULL, NULL, (ub4) OCI_DEFAULT);
    if (status != OCI_SUCCESS)
    { 
    	error_proc(errhp, status);
    	printf("%s\n",oci_err); 
    	return -1;
    }
    while ((status = OCIStmtFetch(stmthp1, errhp, 1, OCI_FETCH_NEXT, OCI_DEFAULT)) != OCI_NO_DATA)
    {
    	ret = count;
        //printf("%s\n",wdlevel);
       // printf("%d\n",wslevel);
    }
    //printf("%d\n",ret);
    /*
    --- delect
    */
    if(ret>0)
    {    
       //printf("fengyuxing1\n");  
      memset(update_statement,'\0',sizeof(update_statement));
      sprintf(update_statement,"update  observe.ele_awst_wind@RACDB set wind_direct = :3 ,wind_direct_level = :4 , wind_speed = :5 ,wind_speed_level = :6 where v01000 = :1 and c_bjtime = :2");
     // printf("%s\n",update_statement);
       
      status = OCIStmtPrepare(stmthp2, errhp,
                              update_statement, (ub4) strlen(update_statement),
                              (ub4) OCI_NTV_SYNTAX, (ub4) OCI_DEFAULT);
     
      status |= OCIBindByPos(stmthp2, &bnd1hp, errhp, 1,
                           (dvoid *)&value[0], (sword)sizeof(value[0]), SQLT_FLT, 
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
        status |= OCIBindByPos(stmthp2, &bnd2hp, errhp, 2,
                           (dvoid *)date, strlen(date)+1, SQLT_STR,
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
       status |= OCIBindByPos(stmthp2, &bnd3hp, errhp, 3,
                           (dvoid *)&value[2], (sword)sizeof(value[2]), SQLT_FLT, 
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
       status |= OCIBindByPos(stmthp2, &bnd4hp, errhp, 4,
                           (dvoid *)wdlevel, strlen(wdlevel)+1, SQLT_STR,
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
       status |= OCIBindByPos(stmthp2, &bnd5hp, errhp, 5,
                           (dvoid *)&value[1], (sword)sizeof(value[1]), SQLT_FLT, 
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
       status |= OCIBindByPos(stmthp2, &bnd6hp, errhp, 6,
                           (dvoid *)&wslevel, (sword)sizeof(wslevel), SQLT_INT, 
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    
      status |= OCIStmtExecute(svchp, stmthp2, errhp, (ub4) 1, (ub4) 0,
                                NULL, NULL, (ub4) OCI_DEFAULT);
      if (status != OCI_SUCCESS)
      { 
      	error_proc(errhp, status);
      	printf("%s\n",oci_err); 
      	return -1;
      }
    }
    else{
    /*
    --- insert
    */
  
    memset(insert_statement, '\0', sizeof(insert_statement));
    sprintf(insert_statement, "insert into observe.ele_awst_wind@RACDB values(:1,:2,:3,:4,:5,:6)");
    

    status = OCIStmtPrepare(stmthp, errhp, 
                            insert_statement, (ub4) strlen(insert_statement),
                            (ub4) OCI_NTV_SYNTAX, (ub4) OCI_DEFAULT);
    
    
    status |= OCIBindByPos(stmthp, &bnd1hp, errhp, 1,
                           (dvoid *)&value[0], (sword)sizeof(value[0]), SQLT_FLT, 
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
        status |= OCIBindByPos(stmthp, &bnd2hp, errhp, 2,
                           (dvoid *)date, strlen(date)+1, SQLT_STR,
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
       status |= OCIBindByPos(stmthp, &bnd3hp, errhp, 3,
                           (dvoid *)&value[2], (sword)sizeof(value[2]), SQLT_FLT, 
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
       status |= OCIBindByPos(stmthp, &bnd4hp, errhp, 4,
                           (dvoid *)wdlevel, strlen(wdlevel)+1, SQLT_STR,
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
       status |= OCIBindByPos(stmthp, &bnd5hp, errhp, 5,
                           (dvoid *)&value[1], (sword)sizeof(value[1]), SQLT_FLT, 
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
       status |= OCIBindByPos(stmthp, &bnd6hp, errhp, 6,
                           (dvoid *)&wslevel, (sword)sizeof(wslevel), SQLT_INT, 
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
        
    status |= OCIStmtExecute(svchp, stmthp, errhp, (ub4) 1, (ub4) 0,
                             (CONST OCISnapshot *) 0,
                             (OCISnapshot *) 0, 
                             (ub4) OCI_DEFAULT);
    if (status != OCI_SUCCESS)
    { 
    	error_proc(errhp, status);
    	printf("%s\n",oci_err); 
    	return -1;
    }
    
   } 
    /*
    --- transaction commit
    */
    status = OCITransCommit(svchp, errhp, 0);
    if (status != OCI_SUCCESS)
    { 
      error_proc(errhp, status);
      printf("%s\n",oci_err); 
      return -1;
    }
    return 0;
}



