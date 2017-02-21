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
**    Function    :  read_pm25() 
**    Author      :  FengYuXing 
**
**********************************************************************/
int read_pm25(char *data_dir,char *data_file,float value[])
{
    char insert_statement[500],select_statement[255],select_statement2[800],delete_statement[255];
    int count=0,ret=0;
     FILE *file_list;
     double  longitude,latitude;
     char time[14];
     char  prov[30];
     int   ct[10];
    sword  status;
    OCIDefine *def1hp,*def2hp,*def3hp,*def4hp;
 
        
           /*
          --- select
           */
    // printf("fengyxuing1\n");
    memset(select_statement, '\0', sizeof(select_statement2));
    sprintf(select_statement, "select longitude,latitude from  base.observe_dict_station where stationid='%d'",(int)value[0]);
    //printf("%s\n",select_statement);
    // printf("%s\n",select_statement);
   // printf("fengyxuing2\n");
    status = OCIStmtPrepare(stmthp, errhp,
                            select_statement, (ub4) strlen(select_statement),
                            (ub4) OCI_NTV_SYNTAX, (ub4) OCI_DEFAULT);
    
    status |= OCIDefineByPos(stmthp, &def1hp, errhp, 1, 
                             (ub1 *)&longitude,(sword) sizeof(double), SQLT_FLT, 
                             (dvoid *) 0, (ub2 *) 0, (ub2 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIDefineByPos(stmthp, &def2hp, errhp, 2, 
                             (ub1 *)&latitude, (sword) sizeof(double), SQLT_FLT, 
                             (dvoid *) 0, (ub2 *) 0, (ub2 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIStmtExecute(svchp, stmthp, errhp, (ub4) 0, (ub4) 0,
                              NULL, NULL, (ub4) OCI_DEFAULT);
    if (status != OCI_SUCCESS)
    { 
       printf("fengyxuing3\n");
    	error_proc(errhp, status);
    	printf("%s\n",oci_err); 
    	return -1;
    }
     if ((file_list=fopen(data_file,"a")) == NULL) 
    {
         printf("cannot write file %s\n", data_file);
         return -1;
    }
      
        
        
     if ((status = OCIStmtFetch(stmthp, errhp, 1, OCI_FETCH_NEXT, OCI_DEFAULT)) != OCI_NO_DATA)
     { 
                //printf("%f\n",longitude);
               // printf("%f\n",latitude);
               // printf("%f\n",value[1]);
             	fprintf(file_list, "%d %.2f %.2f %.2f %.2f\n",(int)value[0],longitude,latitude,value[3],value[5]);
    }
     else
      {
               fprintf(file_list, "%d %.2f %.2f %.2f %.2f\n",(int)value[0],value[1],value[2],value[3],value[5]);

      }

    
    fclose(file_list);
   return 0;
}
/*********************************************************************
**
**    Function    :  read_pm25() 
**    Author      :  FengYuXing 
**
**********************************************************************/
int read_pm10(char *data_dir,char *data_file,float value[])
{
    char insert_statement[500],select_statement[255],select_statement2[800],delete_statement[255];
    int count=0,ret=0;
     FILE *file_list;
     double  longitude,latitude;
     char time[14];
     char  prov[30];
     int   ct[10];
    sword  status;
    OCIDefine *def1hp,*def2hp,*def3hp,*def4hp;
 
        
           /*
          --- select
           */
    // printf("fengyxuing1\n");
    memset(select_statement, '\0', sizeof(select_statement2));
    sprintf(select_statement, "select longitude,latitude from  base.observe_dict_station where stationid='%d'",(int)value[0]);
    //printf("%s\n",select_statement);
    // printf("%s\n",select_statement);
   // printf("fengyxuing2\n");
    status = OCIStmtPrepare(stmthp, errhp,
                            select_statement, (ub4) strlen(select_statement),
                            (ub4) OCI_NTV_SYNTAX, (ub4) OCI_DEFAULT);
    
    status |= OCIDefineByPos(stmthp, &def1hp, errhp, 1, 
                             (ub1 *)&longitude,(sword) sizeof(double), SQLT_FLT, 
                             (dvoid *) 0, (ub2 *) 0, (ub2 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIDefineByPos(stmthp, &def2hp, errhp, 2, 
                             (ub1 *)&latitude, (sword) sizeof(double), SQLT_FLT, 
                             (dvoid *) 0, (ub2 *) 0, (ub2 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIStmtExecute(svchp, stmthp, errhp, (ub4) 0, (ub4) 0,
                              NULL, NULL, (ub4) OCI_DEFAULT);
    if (status != OCI_SUCCESS)
    { 
       printf("fengyxuing3\n");
    	error_proc(errhp, status);
    	printf("%s\n",oci_err); 
    	return -1;
    }
     if ((file_list=fopen(data_file,"a")) == NULL) 
    {
         printf("cannot write file %s\n", data_file);
         return -1;
    }
      
        
        
     if ((status = OCIStmtFetch(stmthp, errhp, 1, OCI_FETCH_NEXT, OCI_DEFAULT)) != OCI_NO_DATA)
     { 
                //printf("%f\n",longitude);
                //printf("%f\n",latitude);
                //printf("%f\n",value[1]);
             	fprintf(file_list, "%d %.2f %.2f %.2f %.2f\n",(int)value[0],longitude,latitude,value[3],value[4]);
    }
     else
      {
               fprintf(file_list, "%d %.2f %.2f %.2f %.2f\n",(int)value[0],value[1],value[2],value[3],value[4]);

      }

    
    fclose(file_list);
   return 0;
}

