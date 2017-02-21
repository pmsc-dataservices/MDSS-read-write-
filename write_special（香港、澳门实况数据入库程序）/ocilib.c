#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
OCIStmt        *stmthp3;

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
  OCIHandleAlloc((dvoid *) envhp,
                 (dvoid **) &stmthp3,
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
**    Function    :  insert_special() 
**    Author      :  FengYuXing 
**
**********************************************************************/
int insert_special(int station,char *date,int tempe,int wd,int ws,int sd,float value[])
{
    char insert_statement[500],select_statement[255],delete_statement[255];
    int count=0,ret=0;
     char time[25];
     int   ct[10];
    sword  status;
    OCIDefine *def1hp;
    OCIBind   *bnd1hp, *bnd2hp, *bnd3hp, *bnd4hp, *bnd5hp, *bnd6hp, *bnd7hp, *bnd8hp,*bnd9hp, *bnd10hp, *bnd11hp, *bnd12hp;
        
    
     /*
    --- select
    */
    // printf("fengyxuing1\n");
    memset(select_statement, '\0', sizeof(select_statement));
    sprintf(select_statement,"select count(*) as num from base.observe_ele_awst_special where v01000 = '%d'  and  c_bjtime = '%s' ",station,date);
    //printf("fengyxuing2\n");
    status = OCIStmtPrepare(stmthp1, errhp,
                            select_statement, (ub4) strlen(select_statement),
                            (ub4) OCI_NTV_SYNTAX, (ub4) OCI_DEFAULT);

    status |= OCIDefineByPos(stmthp1, &def1hp, errhp, 1, 
                             (ub1 *)&count, (sword)sizeof(count), SQLT_INT, 
                             (dvoid *) 0, (ub2 *) 0, (ub2 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIStmtExecute(svchp, stmthp1, errhp, (ub4) 0, (ub4) 0,
                              NULL, NULL, (ub4) OCI_DEFAULT);
    if (status != OCI_SUCCESS)
    { 
        printf("fengyxuing3\n");
    	error_proc(errhp, status);
    	printf("%s\n",oci_err); 
    	return -1;
    }
    while ((status = OCIStmtFetch(stmthp1, errhp, 1, OCI_FETCH_NEXT, OCI_DEFAULT)) != OCI_NO_DATA)
    {
    	ret = count;
    }
    printf("%d\n",ret);
    /*
    --- delect
    */
    if(ret>0)
    {
     // printf("fengyxuing4\n");
      memset(delete_statement,'\0',sizeof(delete_statement));
      sprintf(delete_statement,"delete from base.observe_ele_awst_special where v01000 = '%d'  and  c_bjtime = '%s' ",station,date);
      //printf("fengyxuing5\n");
      status = OCIStmtPrepare(stmthp2, errhp,
                              delete_statement, (ub4) strlen(delete_statement),
                              (ub4) OCI_NTV_SYNTAX, (ub4) OCI_DEFAULT);
     
      status |= OCIStmtExecute(svchp, stmthp2, errhp, (ub4) 1, (ub4) 0,
                                NULL, NULL, (ub4) OCI_DEFAULT);
      if (status != OCI_SUCCESS)
      { 
      	error_proc(errhp, status);
      	printf("%s\n",oci_err); 
      	return -1;
      }
    }

    /*
    --- insert
    */
    //printf("fengyxuing6\n");
    memset(insert_statement, '\0', sizeof(insert_statement));
    sprintf(insert_statement, "insert into base.observe_ele_awst_special values(:1,:2,:3,:4,:5,:6,:7,:8,:9)");
    //printf("fengyxuing7\n");
    status = OCIStmtPrepare(stmthp, errhp, 
                            insert_statement, (ub4) strlen(insert_statement),
                            (ub4) OCI_NTV_SYNTAX, (ub4) OCI_DEFAULT);
     status |= OCIBindByPos(stmthp, &bnd1hp, errhp, 1,
                           (dvoid *)&station, (sword)sizeof(station), SQLT_INT,
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
     status |= OCIBindByPos(stmthp, &bnd2hp, errhp, 2,
                           (dvoid *)date, strlen(date)+1, SQLT_STR,
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp, &bnd3hp, errhp, 3,
                           (dvoid *)&tempe, (sword)sizeof(tempe), SQLT_INT,
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp, &bnd4hp, errhp, 4,
                           (dvoid *)&wd, (sword)sizeof(wd), SQLT_INT,
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp, &bnd5hp, errhp, 5,
                           (dvoid *)&ws, (sword)sizeof(ws), SQLT_INT,
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp, &bnd6hp, errhp, 6,
                           (dvoid *)&sd, (sword)sizeof(sd), SQLT_INT, 
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp, &bnd7hp, errhp, 7,
                           (dvoid *)&value[0], (sword)sizeof(value[0]), SQLT_FLT,
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp, &bnd8hp, errhp, 8,
                           (dvoid *)&value[1], (sword)sizeof(value[1]), SQLT_FLT, 
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp, &bnd9hp, errhp, 9,
                           (dvoid *)&value[2], (sword)sizeof(value[2]), SQLT_FLT, 
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


