#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <oci.h>
#include "ocilib.h"

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
**    Function    :  insert_GR01() 
**    Author      :  FengYuXing 
**
**********************************************************************/
int insert_GRO1(char *date,char  *cymd,char  *chms,float value[])
{
    char insert_statement[255],select_statement[255],delete_statement[255];
    int count=0,ret=0;
    sword  status;
    OCIDefine *def1hp;
    OCIBind   *bnd1hp, *bnd2hp, *bnd3hp, *bnd4hp,*bnd5hp, *bnd6hp, *bnd7hp, *bnd8hp,*bnd9hp, *bnd10hp, *bnd11hp, *bnd12hp;
    OCIBind   *bnd13hp, *bnd14hp, *bnd15hp, *bnd16hp;
    
    
    /*
    --- select
    */
     printf("fengyxuing1\n");
    memset(select_statement, '\0', sizeof(select_statement));
    sprintf(select_statement,"select count(*) as num from OBSERVE.ELE_GR01 where c_date = :1 and v01000 = :8 and v71001 = :14 and v71002 = :15");
    printf("fengyxuing2\n");
    status = OCIStmtPrepare(stmthp1, errhp,
                            select_statement, (ub4) strlen(select_statement),
                            (ub4) OCI_NTV_SYNTAX, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp1, &bnd1hp, errhp, 1,
                           (dvoid *)date, strlen(date)+1, SQLT_STR,
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp1, &bnd2hp, errhp, 2,
                           (dvoid *)&value[4], (sword)sizeof(value[4]), SQLT_FLT, 
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp1, &bnd3hp, errhp, 3,
                           (dvoid *)&value[10], (sword)sizeof(value[10]), SQLT_FLT, 
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp1, &bnd4hp, errhp, 4,
                           (dvoid *)&value[11], (sword)sizeof(value[11]), SQLT_FLT, 
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);

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
    
    /*
    --- delect
    */
    if(ret>0)
    {
      printf("fengyxuing4\n");
       memset(delete_statement,'\0',sizeof(delete_statement));
      sprintf(delete_statement,"delete from OBSERVE.ELE_GR01 where c_date = :1 and v01000 = :8 and v71001 = :14 and v71002 = :15");
      printf("fengyxuing5\n");
      status = OCIStmtPrepare(stmthp2, errhp,
                              delete_statement, (ub4) strlen(delete_statement),
                              (ub4) OCI_NTV_SYNTAX, (ub4) OCI_DEFAULT);
      status |= OCIBindByPos(stmthp2, &bnd1hp, errhp, 1,
                           (dvoid *)date, strlen(date)+1, SQLT_STR,
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
      status |= OCIBindByPos(stmthp2, &bnd2hp, errhp, 2,
                           (dvoid *)&value[4], (sword)sizeof(value[4]), SQLT_FLT, 
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
      status |= OCIBindByPos(stmthp2, &bnd3hp, errhp, 3,
                           (dvoid *)&value[10], (sword)sizeof(value[10]), SQLT_FLT, 
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
       status |= OCIBindByPos(stmthp2, &bnd4hp, errhp, 4,
                           (dvoid *)&value[11], (sword)sizeof(value[11]), SQLT_FLT, 
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
    
    /*
    --- insert
    */
    printf("fengyxuing6\n");
    memset(insert_statement, '\0', sizeof(insert_statement));
    sprintf(insert_statement, "insert into OBSERVE.ELE_GR01 values(:1,:2,:3,:4,:5,:6,:7,:8,:9,:10,"
			":11,:12,:13,:14,:15,:16)");
    printf("fengyxuing7\n");
    status = OCIStmtPrepare(stmthp, errhp, 
                            insert_statement, (ub4) strlen(insert_statement),
                            (ub4) OCI_NTV_SYNTAX, (ub4) OCI_DEFAULT);
    
    status |= OCIBindByPos(stmthp, &bnd1hp, errhp, 1,
                           (dvoid *)date, strlen(date)+1, SQLT_STR,
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp, &bnd2hp, errhp, 2,
                           (dvoid *)cymd, strlen(cymd)+1, SQLT_STR,
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp, &bnd3hp, errhp, 3,
                           (dvoid *)chms, strlen(chms)+1, SQLT_STR,
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp, &bnd4hp, errhp, 4,
                           (dvoid *)&value[0], (sword)sizeof(value[0]), SQLT_FLT,
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp, &bnd5hp, errhp, 5,
                           (dvoid *)&value[1], (sword)sizeof(value[1]), SQLT_FLT,
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp, &bnd6hp, errhp, 6,
                           (dvoid *)&value[2], (sword)sizeof(value[2]), SQLT_FLT, 
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp, &bnd7hp, errhp, 7,
                           (dvoid *)&value[3], (sword)sizeof(value[3]), SQLT_FLT,
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp, &bnd8hp, errhp, 8,
                           (dvoid *)&value[4], (sword)sizeof(value[4]), SQLT_FLT, 
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp, &bnd9hp, errhp, 9,
                           (dvoid *)&value[5], (sword)sizeof(value[5]), SQLT_FLT, 
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp, &bnd10hp, errhp, 10,
                           (dvoid *)&value[6], (sword)sizeof(value[6]), SQLT_FLT,
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp, &bnd11hp, errhp, 11,
                           (dvoid *)&value[7], (sword)sizeof(value[7]), SQLT_FLT,
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp, &bnd12hp, errhp, 12,
                           (dvoid *)&value[8], (sword)sizeof(value[8]), SQLT_FLT, 
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp, &bnd13hp, errhp, 13,
                           (dvoid *)&value[9], (sword)sizeof(value[9]), SQLT_FLT,
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp, &bnd14hp, errhp, 14,
                           (dvoid *)&value[10], (sword)sizeof(value[10]), SQLT_FLT, 
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp, &bnd15hp, errhp, 15,
                           (dvoid *)&value[11], (sword)sizeof(value[11]), SQLT_FLT, 
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp, &bnd16hp, errhp, 16,
                           (dvoid *)&value[12], (sword)sizeof(value[12]), SQLT_FLT,
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
/*********************************************************************
**
**    Function    :  insert_GR02() 
**    Author      :  FengYuXing 
**
**********************************************************************/
int insert_GRO2(char *date,char  *cymd,char  *chms,float value[])
{
    char insert_statement[255],select_statement[255],delete_statement[255];
    int count=0,ret=0;
    sword  status;
    OCIDefine *def1hp;
    OCIBind   *bnd1hp, *bnd2hp, *bnd3hp, *bnd4hp,*bnd5hp, *bnd6hp, *bnd7hp, *bnd8hp,*bnd9hp, *bnd10hp, *bnd11hp, *bnd12hp;
    OCIBind   *bnd13hp, *bnd14hp, *bnd15hp;
    
    
    /*
    --- select
    */
    memset(select_statement, '\0', sizeof(select_statement));

    sprintf(select_statement,"select count(*) as num from OBSERVE.ELE_GR02 where c_date = :1 and v01000 = :8 and v71001 = :14 and  v71006 = :15");


    status = OCIStmtPrepare(stmthp1, errhp,
                            select_statement, (ub4) strlen(select_statement),
                            (ub4) OCI_NTV_SYNTAX, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp1, &bnd1hp, errhp, 1,
                           (dvoid *)date, strlen(date)+1, SQLT_STR,
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp1, &bnd2hp, errhp, 2,
                           (dvoid *)&value[4], (sword)sizeof(value[4]), SQLT_FLT, 
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp1, &bnd3hp, errhp, 3,
                           (dvoid *)&value[10], (sword)sizeof(value[10]), SQLT_FLT, 
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
     status |= OCIBindByPos(stmthp1, &bnd4hp, errhp, 4,
                           (dvoid *)&value[11], (sword)sizeof(value[11]), SQLT_FLT, 
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
    
    /*
    --- delect
    */
    if(ret>0)
    {
       
      memset(delete_statement,'\0',sizeof(delete_statement));
      sprintf(delete_statement,"delete from OBSERVE.ELE_GR02 where c_date = :1 and v01000 = :8 and v71001 = :14 and v71006 = :15");
     

      status = OCIStmtPrepare(stmthp2, errhp,
                              delete_statement, (ub4) strlen(delete_statement),
                              (ub4) OCI_NTV_SYNTAX, (ub4) OCI_DEFAULT);
      status |= OCIBindByPos(stmthp2, &bnd1hp, errhp, 1,
                           (dvoid *)date, strlen(date)+1, SQLT_STR,
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
      status |= OCIBindByPos(stmthp2, &bnd2hp, errhp, 2,
                           (dvoid *)&value[4], (sword)sizeof(value[4]), SQLT_FLT, 
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
      status |= OCIBindByPos(stmthp2, &bnd3hp, errhp, 3,
                           (dvoid *)&value[10], (sword)sizeof(value[10]), SQLT_FLT, 
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
       status |= OCIBindByPos(stmthp2, &bnd4hp, errhp, 4,
                           (dvoid *)&value[11], (sword)sizeof(value[11]), SQLT_FLT, 
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
    
    /*
    --- insert
    */
  

    memset(insert_statement, '\0', sizeof(insert_statement));
    sprintf(insert_statement, "insert into OBSERVE.ELE_GR02 values(:1,:2,:3,:4,:5,:6,:7,:8,:9,:10,"
			":11,:12,:13,:14,:15)");
    status = OCIStmtPrepare(stmthp, errhp, 
                            insert_statement, (ub4) strlen(insert_statement),
                            (ub4) OCI_NTV_SYNTAX, (ub4) OCI_DEFAULT);
    
    status |= OCIBindByPos(stmthp, &bnd1hp, errhp, 1,
                           (dvoid *)date, strlen(date)+1, SQLT_STR,
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp, &bnd2hp, errhp, 2,
                           (dvoid *)cymd, strlen(cymd)+1, SQLT_STR,
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp, &bnd3hp, errhp, 3,
                           (dvoid *)chms, strlen(chms)+1, SQLT_STR,
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp, &bnd4hp, errhp, 4,
                           (dvoid *)&value[0], (sword)sizeof(value[0]), SQLT_FLT,
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp, &bnd5hp, errhp, 5,
                           (dvoid *)&value[1], (sword)sizeof(value[1]), SQLT_FLT,
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp, &bnd6hp, errhp, 6,
                           (dvoid *)&value[2], (sword)sizeof(value[2]), SQLT_FLT, 
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp, &bnd7hp, errhp, 7,
                           (dvoid *)&value[3], (sword)sizeof(value[3]), SQLT_FLT,
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp, &bnd8hp, errhp, 8,
                           (dvoid *)&value[4], (sword)sizeof(value[4]), SQLT_FLT, 
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp, &bnd9hp, errhp, 9,
                           (dvoid *)&value[5], (sword)sizeof(value[5]), SQLT_FLT, 
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp, &bnd10hp, errhp, 10,
                           (dvoid *)&value[6], (sword)sizeof(value[6]), SQLT_FLT,
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp, &bnd11hp, errhp, 11,
                           (dvoid *)&value[7], (sword)sizeof(value[7]), SQLT_FLT,
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp, &bnd12hp, errhp, 12,
                           (dvoid *)&value[8], (sword)sizeof(value[8]), SQLT_FLT, 
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp, &bnd13hp, errhp, 13,
                           (dvoid *)&value[9], (sword)sizeof(value[9]), SQLT_FLT,
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp, &bnd14hp, errhp, 14,
                           (dvoid *)&value[10], (sword)sizeof(value[10]), SQLT_FLT, 
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp, &bnd15hp, errhp, 15,
                           (dvoid *)&value[11], (sword)sizeof(value[11]), SQLT_FLT, 
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
/*********************************************************************
**
**    Function    :  insert_GR03() 
**    Author      :  FengYuXing 
**
**********************************************************************/
int insert_GRO3(char *date,char  *cymd,char  *chms,float value[])
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
    
    sprintf(select_statement,"select count(*) as num from OBSERVE.ELE_GR03 where c_date = :1 and v01000 = :8 and v71001 = :14 and v71812 = :15");
    

    status = OCIStmtPrepare(stmthp1, errhp,
                            select_statement, (ub4) strlen(select_statement),
                            (ub4) OCI_NTV_SYNTAX, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp1, &bnd1hp, errhp, 1,
                           (dvoid *)date, strlen(date)+1, SQLT_STR,
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp1, &bnd2hp, errhp, 2,
                           (dvoid *)&value[4], (sword)sizeof(value[4]), SQLT_FLT, 
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp1, &bnd3hp, errhp, 3,
                           (dvoid *)&value[10], (sword)sizeof(value[10]), SQLT_FLT, 
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp1, &bnd4hp, errhp, 4,
                           (dvoid *)&value[11], (sword)sizeof(value[11]), SQLT_FLT, 
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
    
    /*
    --- delect
    */
    if(ret>0)
    {
      memset(delete_statement,'\0',sizeof(delete_statement));
      sprintf(delete_statement,"delete from OBSERVE.ELE_GR03 where c_date = :1 and v01000 = :8 and v71001 = :14 and v71812 = :15");
      

      status = OCIStmtPrepare(stmthp2, errhp,
                              delete_statement, (ub4) strlen(delete_statement),
                              (ub4) OCI_NTV_SYNTAX, (ub4) OCI_DEFAULT);
      status |= OCIBindByPos(stmthp2, &bnd1hp, errhp, 1,
                           (dvoid *)date, strlen(date)+1, SQLT_STR,
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
      status |= OCIBindByPos(stmthp2, &bnd2hp, errhp, 2,
                           (dvoid *)&value[4], (sword)sizeof(value[4]), SQLT_FLT, 
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
      status |= OCIBindByPos(stmthp2, &bnd3hp, errhp, 3,
                           (dvoid *)&value[10], (sword)sizeof(value[10]), SQLT_FLT, 
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
       status |= OCIBindByPos(stmthp2, &bnd4hp, errhp, 4,
                           (dvoid *)&value[11], (sword)sizeof(value[11]), SQLT_FLT, 
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
    
    /*
    --- insert
    */
   

    memset(insert_statement, '\0', sizeof(insert_statement));
    sprintf(insert_statement, "insert into OBSERVE.ELE_GR03 values(:1,:2,:3,:4,:5,:6,:7,:8,:9,:10,"
			":11,:12,:13,:14,:15,:16,:17)");
  

    status = OCIStmtPrepare(stmthp, errhp, 
                            insert_statement, (ub4) strlen(insert_statement),
                            (ub4) OCI_NTV_SYNTAX, (ub4) OCI_DEFAULT);
    
    status |= OCIBindByPos(stmthp, &bnd1hp, errhp, 1,
                           (dvoid *)date, strlen(date)+1, SQLT_STR,
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp, &bnd2hp, errhp, 2,
                           (dvoid *)cymd, strlen(cymd)+1, SQLT_STR,
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp, &bnd3hp, errhp, 3,
                           (dvoid *)chms, strlen(chms)+1, SQLT_STR,
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp, &bnd4hp, errhp, 4,
                           (dvoid *)&value[0], (sword)sizeof(value[0]), SQLT_FLT,
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp, &bnd5hp, errhp, 5,
                           (dvoid *)&value[1], (sword)sizeof(value[1]), SQLT_FLT,
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp, &bnd6hp, errhp, 6,
                           (dvoid *)&value[2], (sword)sizeof(value[2]), SQLT_FLT, 
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp, &bnd7hp, errhp, 7,
                           (dvoid *)&value[3], (sword)sizeof(value[3]), SQLT_FLT,
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp, &bnd8hp, errhp, 8,
                           (dvoid *)&value[4], (sword)sizeof(value[4]), SQLT_FLT, 
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp, &bnd9hp, errhp, 9,
                           (dvoid *)&value[5], (sword)sizeof(value[5]), SQLT_FLT, 
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp, &bnd10hp, errhp, 10,
                           (dvoid *)&value[6], (sword)sizeof(value[6]), SQLT_FLT,
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp, &bnd11hp, errhp, 11,
                           (dvoid *)&value[7], (sword)sizeof(value[7]), SQLT_FLT,
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp, &bnd12hp, errhp, 12,
                           (dvoid *)&value[8], (sword)sizeof(value[8]), SQLT_FLT, 
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp, &bnd13hp, errhp, 13,
                           (dvoid *)&value[9], (sword)sizeof(value[9]), SQLT_FLT,
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp, &bnd14hp, errhp, 14,
                           (dvoid *)&value[10], (sword)sizeof(value[10]), SQLT_FLT, 
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp, &bnd15hp, errhp, 15,
                           (dvoid *)&value[11], (sword)sizeof(value[11]), SQLT_FLT, 
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp, &bnd16hp, errhp, 16,
                           (dvoid *)&value[12], (sword)sizeof(value[12]), SQLT_FLT,
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp, &bnd17hp, errhp, 17,
                           (dvoid *)&value[13], (sword)sizeof(value[13]), SQLT_FLT, 
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


/*********************************************************************
**
**    Function    :  insert_GR04() 
**    Author      :  FengYuXing 
**
**********************************************************************/
int insert_GRO4(char *date,char  *cymd,char  *chms,float value[])
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
   
    sprintf(select_statement,"select count(*) as num from OBSERVE.ELE_GR04 where c_date = :1 and v01000 = :8 and v71815 = :14 and v71817 = :16");
   

    status = OCIStmtPrepare(stmthp1, errhp,
                            select_statement, (ub4) strlen(select_statement),
                            (ub4) OCI_NTV_SYNTAX, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp1, &bnd1hp, errhp, 1,
                           (dvoid *)date, strlen(date)+1, SQLT_STR,
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp1, &bnd2hp, errhp, 2,
                           (dvoid *)&value[4], (sword)sizeof(value[4]), SQLT_FLT, 
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp1, &bnd3hp, errhp, 3,
                           (dvoid *)&value[10], (sword)sizeof(value[10]), SQLT_FLT, 
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
     status |= OCIBindByPos(stmthp1, &bnd4hp, errhp, 4,
                           (dvoid *)&value[12], (sword)sizeof(value[12]), SQLT_FLT, 
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
    
    /*
    --- delect
    */
    if(ret>0)
    {      
      memset(delete_statement,'\0',sizeof(delete_statement));
      sprintf(delete_statement,"delete from OBSERVE.ELE_GR04 where c_date = :1 and v01000 = :8 and v71815 = :14 and v71817 = :16");
      

      status = OCIStmtPrepare(stmthp2, errhp,
                              delete_statement, (ub4) strlen(delete_statement),
                              (ub4) OCI_NTV_SYNTAX, (ub4) OCI_DEFAULT);
      status |= OCIBindByPos(stmthp2, &bnd1hp, errhp, 1,
                           (dvoid *)date, strlen(date)+1, SQLT_STR,
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
      status |= OCIBindByPos(stmthp2, &bnd2hp, errhp, 2,
                           (dvoid *)&value[4], (sword)sizeof(value[4]), SQLT_FLT, 
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
       status |= OCIBindByPos(stmthp2, &bnd3hp, errhp, 3,
                           (dvoid *)&value[10], (sword)sizeof(value[10]), SQLT_FLT, 
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
       status |= OCIBindByPos(stmthp2, &bnd4hp, errhp, 4,
                           (dvoid *)&value[12], (sword)sizeof(value[12]), SQLT_FLT, 
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
    
    /*
    --- insert
    */
   

    memset(insert_statement, '\0', sizeof(insert_statement));
    sprintf(insert_statement, "insert into OBSERVE.ELE_GR04 values(:1,:2,:3,:4,:5,:6,:7,:8,:9,:10,"
			":11,:12,:13,:14,:15,:16,:17)");
    

    status = OCIStmtPrepare(stmthp, errhp, 
                            insert_statement, (ub4) strlen(insert_statement),
                            (ub4) OCI_NTV_SYNTAX, (ub4) OCI_DEFAULT);
    
    status |= OCIBindByPos(stmthp, &bnd1hp, errhp, 1,
                           (dvoid *)date, strlen(date)+1, SQLT_STR,
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp, &bnd2hp, errhp, 2,
                           (dvoid *)cymd, strlen(cymd)+1, SQLT_STR,
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp, &bnd3hp, errhp, 3,
                           (dvoid *)chms, strlen(chms)+1, SQLT_STR,
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp, &bnd4hp, errhp, 4,
                           (dvoid *)&value[0], (sword)sizeof(value[0]), SQLT_FLT,
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp, &bnd5hp, errhp, 5,
                           (dvoid *)&value[1], (sword)sizeof(value[1]), SQLT_FLT,
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp, &bnd6hp, errhp, 6,
                           (dvoid *)&value[2], (sword)sizeof(value[2]), SQLT_FLT, 
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp, &bnd7hp, errhp, 7,
                           (dvoid *)&value[3], (sword)sizeof(value[3]), SQLT_FLT,
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp, &bnd8hp, errhp, 8,
                           (dvoid *)&value[4], (sword)sizeof(value[4]), SQLT_FLT, 
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp, &bnd9hp, errhp, 9,
                           (dvoid *)&value[5], (sword)sizeof(value[5]), SQLT_FLT, 
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp, &bnd10hp, errhp, 10,
                           (dvoid *)&value[6], (sword)sizeof(value[6]), SQLT_FLT,
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp, &bnd11hp, errhp, 11,
                           (dvoid *)&value[7], (sword)sizeof(value[7]), SQLT_FLT,
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp, &bnd12hp, errhp, 12,
                           (dvoid *)&value[8], (sword)sizeof(value[8]), SQLT_FLT, 
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp, &bnd13hp, errhp, 13,
                           (dvoid *)&value[9], (sword)sizeof(value[9]), SQLT_FLT,
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp, &bnd14hp, errhp, 14,
                           (dvoid *)&value[10], (sword)sizeof(value[10]), SQLT_FLT, 
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp, &bnd15hp, errhp, 15,
                           (dvoid *)&value[11], (sword)sizeof(value[11]), SQLT_FLT, 
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp, &bnd16hp, errhp, 16,
                           (dvoid *)&value[12], (sword)sizeof(value[12]), SQLT_FLT,
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp, &bnd17hp, errhp, 17,
                           (dvoid *)&value[13], (sword)sizeof(value[13]), SQLT_FLT, 
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


/*********************************************************************
**
**    Function    :  insert_GR05() 
**    Author      :  FengYuXing 
**
**********************************************************************/
int insert_GRO5(char *date,char  *cymd,char  *chms,float value[])
{
    char insert_statement[255],select_statement[255],delete_statement[255];
    int count=0,ret=0;
    sword  status;
    OCIDefine *def1hp;
    OCIBind   *bnd1hp, *bnd2hp, *bnd3hp, *bnd4hp,*bnd5hp, *bnd6hp, *bnd7hp, *bnd8hp,*bnd9hp, *bnd10hp, *bnd11hp, *bnd12hp;
    OCIBind   *bnd13hp, *bnd14hp, *bnd15hp, *bnd16hp;
    
    
    /*
    --- select
    */
    memset(select_statement, '\0', sizeof(select_statement));
    
    sprintf(select_statement,"select count(*) as num from OBSERVE.ELE_GR05 where c_date = :1 and v01000 = :8 and v71001 = :14  and v71819 = :15");
    

    status = OCIStmtPrepare(stmthp1, errhp,
                            select_statement, (ub4) strlen(select_statement),
                            (ub4) OCI_NTV_SYNTAX, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp1, &bnd1hp, errhp, 1,
                           (dvoid *)date, strlen(date)+1, SQLT_STR,
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp1, &bnd2hp, errhp, 2,
                           (dvoid *)&value[4], (sword)sizeof(value[4]), SQLT_FLT, 
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp1, &bnd3hp, errhp, 3,
                           (dvoid *)&value[10], (sword)sizeof(value[10]), SQLT_FLT, 
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp1, &bnd4hp, errhp, 4,
                           (dvoid *)&value[11], (sword)sizeof(value[11]), SQLT_FLT, 
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
    
    /*
    --- delect
    */
    if(ret>0)
    {  
      memset(delete_statement,'\0',sizeof(delete_statement));
      sprintf(delete_statement,"delete from OBSERVE.ELE_GR05 where c_date = :1 and v01000 = :8 and v71001 = :14 and v71819 = :15");
     

      status = OCIStmtPrepare(stmthp2, errhp,
                              delete_statement, (ub4) strlen(delete_statement),
                              (ub4) OCI_NTV_SYNTAX, (ub4) OCI_DEFAULT);
      status |= OCIBindByPos(stmthp2, &bnd1hp, errhp, 1,
                           (dvoid *)date, strlen(date)+1, SQLT_STR,
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
      status |= OCIBindByPos(stmthp2, &bnd2hp, errhp, 2,
                           (dvoid *)&value[4], (sword)sizeof(value[4]), SQLT_FLT, 
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
      status |= OCIBindByPos(stmthp2, &bnd3hp, errhp, 3,
                           (dvoid *)&value[10], (sword)sizeof(value[10]), SQLT_FLT, 
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
       status |= OCIBindByPos(stmthp2, &bnd4hp, errhp, 4,
                           (dvoid *)&value[11], (sword)sizeof(value[11]), SQLT_FLT, 
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
    
    /*
    --- insert
    */
   

    memset(insert_statement, '\0', sizeof(insert_statement));
    sprintf(insert_statement, "insert into OBSERVE.ELE_GR05 values(:1,:2,:3,:4,:5,:6,:7,:8,:9,:10,"
			":11,:12,:13,:14,:15,:16)");
  

    status = OCIStmtPrepare(stmthp, errhp, 
                            insert_statement, (ub4) strlen(insert_statement),
                            (ub4) OCI_NTV_SYNTAX, (ub4) OCI_DEFAULT);
    
    status |= OCIBindByPos(stmthp, &bnd1hp, errhp, 1,
                           (dvoid *)date, strlen(date)+1, SQLT_STR,
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp, &bnd2hp, errhp, 2,
                           (dvoid *)cymd, strlen(cymd)+1, SQLT_STR,
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp, &bnd3hp, errhp, 3,
                           (dvoid *)chms, strlen(chms)+1, SQLT_STR,
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp, &bnd4hp, errhp, 4,
                           (dvoid *)&value[0], (sword)sizeof(value[0]), SQLT_FLT,
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp, &bnd5hp, errhp, 5,
                           (dvoid *)&value[1], (sword)sizeof(value[1]), SQLT_FLT,
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp, &bnd6hp, errhp, 6,
                           (dvoid *)&value[2], (sword)sizeof(value[2]), SQLT_FLT, 
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp, &bnd7hp, errhp, 7,
                           (dvoid *)&value[3], (sword)sizeof(value[3]), SQLT_FLT,
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp, &bnd8hp, errhp, 8,
                           (dvoid *)&value[4], (sword)sizeof(value[4]), SQLT_FLT, 
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp, &bnd9hp, errhp, 9,
                           (dvoid *)&value[5], (sword)sizeof(value[5]), SQLT_FLT, 
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp, &bnd10hp, errhp, 10,
                           (dvoid *)&value[6], (sword)sizeof(value[6]), SQLT_FLT,
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp, &bnd11hp, errhp, 11,
                           (dvoid *)&value[7], (sword)sizeof(value[7]), SQLT_FLT,
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp, &bnd12hp, errhp, 12,
                           (dvoid *)&value[8], (sword)sizeof(value[8]), SQLT_FLT, 
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp, &bnd13hp, errhp, 13,
                           (dvoid *)&value[9], (sword)sizeof(value[9]), SQLT_FLT,
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp, &bnd14hp, errhp, 14,
                           (dvoid *)&value[10], (sword)sizeof(value[10]), SQLT_FLT, 
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp, &bnd15hp, errhp, 15,
                           (dvoid *)&value[11], (sword)sizeof(value[11]), SQLT_FLT, 
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp, &bnd16hp, errhp, 16,
                           (dvoid *)&value[12], (sword)sizeof(value[12]), SQLT_FLT,
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


/*********************************************************************
**
**    Function    :  insert_GR06() 
**    Author      :  FengYuXing 
**
**********************************************************************/
int insert_GRO6(char *date,char  *cymd,char  *chms,float value[])
{
    char insert_statement[255],select_statement[255],delete_statement[255];
    int count=0,ret=0;
    sword  status;
    OCIDefine *def1hp;
    OCIBind   *bnd1hp, *bnd2hp, *bnd3hp, *bnd4hp,*bnd5hp, *bnd6hp, *bnd7hp, *bnd8hp,*bnd9hp, *bnd10hp, *bnd11hp, *bnd12hp;
    OCIBind   *bnd13hp, *bnd14hp, *bnd15hp, *bnd16hp;
    
    
    /*
    --- select
    */
    memset(select_statement, '\0', sizeof(select_statement));
   
    sprintf(select_statement,"select count(*) as num from OBSERVE.ELE_GR06 where c_date = :1 and v01000 = :8 and v71827 = :14");


    status = OCIStmtPrepare(stmthp1, errhp,
                            select_statement, (ub4) strlen(select_statement),
                            (ub4) OCI_NTV_SYNTAX, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp1, &bnd1hp, errhp, 1,
                           (dvoid *)date, strlen(date)+1, SQLT_STR,
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp1, &bnd2hp, errhp, 2,
                           (dvoid *)&value[4], (sword)sizeof(value[4]), SQLT_FLT, 
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp1, &bnd3hp, errhp, 3,
                           (dvoid *)&value[10], (sword)sizeof(value[10]), SQLT_FLT, 
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
    
    /*
    --- delect
    */
    if(ret>0)
    {       
      memset(delete_statement,'\0',sizeof(delete_statement));
      sprintf(delete_statement,"delete from OBSERVE.ELE_GR06 where c_date = :1 and v01000 = :8 and v71827 = :14");
      

      status = OCIStmtPrepare(stmthp2, errhp,
                              delete_statement, (ub4) strlen(delete_statement),
                              (ub4) OCI_NTV_SYNTAX, (ub4) OCI_DEFAULT);
      status |= OCIBindByPos(stmthp2, &bnd1hp, errhp, 1,
                           (dvoid *)date, strlen(date)+1, SQLT_STR,
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
      status |= OCIBindByPos(stmthp2, &bnd2hp, errhp, 2,
                           (dvoid *)&value[4], (sword)sizeof(value[4]), SQLT_FLT, 
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp2, &bnd3hp, errhp, 3,
                           (dvoid *)&value[10], (sword)sizeof(value[10]), SQLT_FLT, 
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
    
    /*
    --- insert
    */
   

    memset(insert_statement, '\0', sizeof(insert_statement));
    sprintf(insert_statement, "insert into OBSERVE.ELE_GR06 values(:1,:2,:3,:4,:5,:6,:7,:8,:9,:10,"
			":11,:12,:13,:14,:15,:16)");
   

    status = OCIStmtPrepare(stmthp, errhp, 
                            insert_statement, (ub4) strlen(insert_statement),
                            (ub4) OCI_NTV_SYNTAX, (ub4) OCI_DEFAULT);
    
    status |= OCIBindByPos(stmthp, &bnd1hp, errhp, 1,
                           (dvoid *)date, strlen(date)+1, SQLT_STR,
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp, &bnd2hp, errhp, 2,
                           (dvoid *)cymd, strlen(cymd)+1, SQLT_STR,
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp, &bnd3hp, errhp, 3,
                           (dvoid *)chms, strlen(chms)+1, SQLT_STR,
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp, &bnd4hp, errhp, 4,
                           (dvoid *)&value[0], (sword)sizeof(value[0]), SQLT_FLT,
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp, &bnd5hp, errhp, 5,
                           (dvoid *)&value[1], (sword)sizeof(value[1]), SQLT_FLT, 
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp, &bnd6hp, errhp, 6,
                           (dvoid *)&value[2], (sword)sizeof(value[2]), SQLT_FLT,
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp, &bnd7hp, errhp, 7,
                           (dvoid *)&value[3], (sword)sizeof(value[3]), SQLT_FLT, 
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp, &bnd8hp, errhp, 8,
                           (dvoid *)&value[4], (sword)sizeof(value[4]), SQLT_FLT, 
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp, &bnd9hp, errhp, 9,
                           (dvoid *)&value[5], (sword)sizeof(value[5]), SQLT_FLT,
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp, &bnd10hp, errhp, 10,
                           (dvoid *)&value[6], (sword)sizeof(value[6]), SQLT_FLT,
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp, &bnd11hp, errhp, 11,
                           (dvoid *)&value[7], (sword)sizeof(value[7]), SQLT_FLT, 
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp, &bnd12hp, errhp, 12,
                           (dvoid *)&value[8], (sword)sizeof(value[8]), SQLT_FLT,
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp, &bnd13hp, errhp, 13,
                           (dvoid *)&value[9], (sword)sizeof(value[9]), SQLT_FLT, 
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp, &bnd14hp, errhp, 14,
                           (dvoid *)&value[10], (sword)sizeof(value[10]), SQLT_FLT, 
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp, &bnd15hp, errhp, 15,
                           (dvoid *)&value[11], (sword)sizeof(value[11]), SQLT_FLT,
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp, &bnd16hp, errhp, 16,
                           (dvoid *)&value[12], (sword)sizeof(value[12]), SQLT_FLT, 
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



/*********************************************************************
**
**    Function    :  insert_DI01() 
**    Author      :  FengYuXing 
**
**********************************************************************/
int insert_DI01(char *date,char  *cymd,char  *chms,char  *des, float value[])
{
    char insert_statement[255],select_statement[255],delete_statement[255];
    int count=0,ret=0;
    sword  status;
    OCIDefine *def1hp;
    OCIBind   *bnd1hp, *bnd2hp, *bnd3hp, *bnd4hp,*bnd5hp, *bnd6hp, *bnd7hp, *bnd8hp,*bnd9hp, *bnd10hp, *bnd11hp, *bnd12hp;
    OCIBind   *bnd13hp, *bnd14hp, *bnd15hp, *bnd16hp,*bnd17hp, *bnd18hp, *bnd19hp;
    
    
    /*
    --- select
    */
   
    memset(select_statement, '\0', sizeof(select_statement));
    sprintf(select_statement,"select count(*) as num from OBSERVE.ELE_DI01 where c_date = :1 and v01000 = :9 and v71040 = :15 and v71821 = :16");
  
    status = OCIStmtPrepare(stmthp1, errhp,
                            select_statement, (ub4) strlen(select_statement),
                            (ub4) OCI_NTV_SYNTAX, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp1, &bnd1hp, errhp, 1,
                           (dvoid *)date, strlen(date)+1, SQLT_STR,
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp1, &bnd2hp, errhp, 2,
                           (dvoid *)&value[4], (sword)sizeof(value[4]), SQLT_FLT, 
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
     status |= OCIBindByPos(stmthp1, &bnd3hp, errhp, 3,
                           (dvoid *)&value[10], (sword)sizeof(value[10]), SQLT_FLT, 
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
     status |= OCIBindByPos(stmthp1, &bnd4hp, errhp, 4,
                           (dvoid *)&value[11], (sword)sizeof(value[11]), SQLT_FLT, 
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
    
    /*
    --- delect
    */
    if(ret>0)
    {  
       memset(delete_statement,'\0',sizeof(delete_statement));
      sprintf(delete_statement,"delete from OBSERVE.ELE_DI01 where c_date = :1 and v01000 = :9 and v71040 = :15 and v71821 = :16");
     
      status = OCIStmtPrepare(stmthp2, errhp,
                              delete_statement, (ub4) strlen(delete_statement),
                              (ub4) OCI_NTV_SYNTAX, (ub4) OCI_DEFAULT);
      status |= OCIBindByPos(stmthp2, &bnd1hp, errhp, 1,
                           (dvoid *)date, strlen(date)+1, SQLT_STR,
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
      status |= OCIBindByPos(stmthp2, &bnd2hp, errhp, 2,
                           (dvoid *)&value[4], (sword)sizeof(value[4]), SQLT_FLT, 
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
       status |= OCIBindByPos(stmthp2, &bnd3hp, errhp, 3,
                           (dvoid *)&value[10], (sword)sizeof(value[10]), SQLT_FLT, 
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
       status |= OCIBindByPos(stmthp2, &bnd4hp, errhp, 4,
                           (dvoid *)&value[11], (sword)sizeof(value[11]), SQLT_FLT, 
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
    
    /*
    --- insert
    */
    
    memset(insert_statement, '\0', sizeof(insert_statement));
    sprintf(insert_statement, "insert into OBSERVE.ELE_DI01 values(:1,:2,:3,:4,:5,:6,:7,:8,:9,:10,"
			":11,:12,:13,:14,:15,:16,:17,:18,:19)");
    
    status = OCIStmtPrepare(stmthp, errhp, 
                            insert_statement, (ub4) strlen(insert_statement),
                            (ub4) OCI_NTV_SYNTAX, (ub4) OCI_DEFAULT);
    
    status |= OCIBindByPos(stmthp, &bnd1hp, errhp, 1,
                           (dvoid *)date, strlen(date)+1, SQLT_STR,
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp, &bnd2hp, errhp, 2,
                           (dvoid *)cymd, strlen(cymd)+1, SQLT_STR,
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp, &bnd3hp, errhp, 3,
                           (dvoid *)chms, strlen(chms)+1, SQLT_STR,
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
     status |= OCIBindByPos(stmthp, &bnd4hp, errhp, 4,
                           (dvoid *)des, strlen(des)+1, SQLT_STR,
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp, &bnd5hp, errhp, 5,
                           (dvoid *)&value[0], (sword)sizeof(value[0]), SQLT_FLT,
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp, &bnd6hp, errhp, 6,
                           (dvoid *)&value[1], (sword)sizeof(value[1]), SQLT_FLT,
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp, &bnd7hp, errhp, 7,
                           (dvoid *)&value[2], (sword)sizeof(value[2]), SQLT_FLT, 
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp, &bnd8hp, errhp, 8,
                           (dvoid *)&value[3], (sword)sizeof(value[3]), SQLT_FLT,
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp, &bnd9hp, errhp, 9,
                           (dvoid *)&value[4], (sword)sizeof(value[4]), SQLT_FLT, 
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp, &bnd10hp, errhp, 10,
                           (dvoid *)&value[5], (sword)sizeof(value[5]), SQLT_FLT, 
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp, &bnd11hp, errhp, 11,
                           (dvoid *)&value[6], (sword)sizeof(value[6]), SQLT_FLT,
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp, &bnd12hp, errhp, 12,
                           (dvoid *)&value[7], (sword)sizeof(value[7]), SQLT_FLT,
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp, &bnd13hp, errhp, 13,
                           (dvoid *)&value[8], (sword)sizeof(value[8]), SQLT_FLT, 
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp, &bnd14hp, errhp, 14,
                           (dvoid *)&value[9], (sword)sizeof(value[9]), SQLT_FLT,
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp, &bnd15hp, errhp, 15,
                           (dvoid *)&value[10], (sword)sizeof(value[10]), SQLT_FLT, 
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp, &bnd16hp, errhp, 16,
                           (dvoid *)&value[11], (sword)sizeof(value[11]), SQLT_FLT, 
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp, &bnd17hp, errhp, 17,
                           (dvoid *)&value[12], (sword)sizeof(value[12]), SQLT_FLT,
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp, &bnd18hp, errhp, 18,
                           (dvoid *)&value[13], (sword)sizeof(value[13]), SQLT_FLT, 
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp, &bnd19hp, errhp, 19,
                           (dvoid *)&value[14], (sword)sizeof(value[14]), SQLT_FLT, 
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


/*********************************************************************
**
**    Function    :  insert_DI02() 
**    Author      :  FengYuXing 
**
**********************************************************************/
int insert_DI02(char *date,char  *cymd,char  *chms,char  *des, float value[])
{
    char insert_statement[255],select_statement[255],delete_statement[255];
    int count=0,ret=0;
    sword  status;
    OCIDefine *def1hp;
    OCIBind   *bnd1hp, *bnd2hp, *bnd3hp, *bnd4hp,*bnd5hp, *bnd6hp, *bnd7hp, *bnd8hp,*bnd9hp, *bnd10hp, *bnd11hp, *bnd12hp;
    OCIBind   *bnd13hp, *bnd14hp, *bnd15hp, *bnd16hp,*bnd17hp, *bnd18hp, *bnd19hp, *bnd20hp;
    
    
    /*
    --- select
    */
   
    memset(select_statement, '\0', sizeof(select_statement));
    sprintf(select_statement,"select count(*) as num from OBSERVE.ELE_DI02 where c_date = :1 and v01000 = :9 and v71040 = :15 and v71821 = :16");
  
    status = OCIStmtPrepare(stmthp1, errhp,
                            select_statement, (ub4) strlen(select_statement),
                            (ub4) OCI_NTV_SYNTAX, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp1, &bnd1hp, errhp, 1,
                           (dvoid *)date, strlen(date)+1, SQLT_STR,
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp1, &bnd2hp, errhp, 2,
                           (dvoid *)&value[4], (sword)sizeof(value[4]), SQLT_FLT, 
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
     status |= OCIBindByPos(stmthp1, &bnd3hp, errhp, 3,
                           (dvoid *)&value[10], (sword)sizeof(value[10]), SQLT_FLT, 
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
     status |= OCIBindByPos(stmthp1, &bnd4hp, errhp, 4,
                           (dvoid *)&value[11], (sword)sizeof(value[11]), SQLT_FLT, 
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
    
    /*
    --- delect
    */
    if(ret>0)
    {  
       memset(delete_statement,'\0',sizeof(delete_statement));
      sprintf(delete_statement,"delete from OBSERVE.ELE_DI02 where c_date = :1 and v01000 = :9 and v71040 = :15 and v71821 = :16");
     
      status = OCIStmtPrepare(stmthp2, errhp,
                              delete_statement, (ub4) strlen(delete_statement),
                              (ub4) OCI_NTV_SYNTAX, (ub4) OCI_DEFAULT);
      status |= OCIBindByPos(stmthp2, &bnd1hp, errhp, 1,
                           (dvoid *)date, strlen(date)+1, SQLT_STR,
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
      status |= OCIBindByPos(stmthp2, &bnd2hp, errhp, 2,
                           (dvoid *)&value[4], (sword)sizeof(value[4]), SQLT_FLT, 
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
       status |= OCIBindByPos(stmthp2, &bnd3hp, errhp, 3,
                           (dvoid *)&value[10], (sword)sizeof(value[10]), SQLT_FLT, 
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
       status |= OCIBindByPos(stmthp2, &bnd4hp, errhp, 4,
                           (dvoid *)&value[11], (sword)sizeof(value[11]), SQLT_FLT, 
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
    
    /*
    --- insert
    */
    
    memset(insert_statement, '\0', sizeof(insert_statement));
    sprintf(insert_statement, "insert into OBSERVE.ELE_DI02 values(:1,:2,:3,:4,:5,:6,:7,:8,:9,:10,"
			":11,:12,:13,:14,:15,:16,:17,:18,:19,:20)");
    
    status = OCIStmtPrepare(stmthp, errhp, 
                            insert_statement, (ub4) strlen(insert_statement),
                            (ub4) OCI_NTV_SYNTAX, (ub4) OCI_DEFAULT);
    
    status |= OCIBindByPos(stmthp, &bnd1hp, errhp, 1,
                           (dvoid *)date, strlen(date)+1, SQLT_STR,
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp, &bnd2hp, errhp, 2,
                           (dvoid *)cymd, strlen(cymd)+1, SQLT_STR,
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp, &bnd3hp, errhp, 3,
                           (dvoid *)chms, strlen(chms)+1, SQLT_STR,
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
     status |= OCIBindByPos(stmthp, &bnd4hp, errhp, 4,
                           (dvoid *)des, strlen(des)+1, SQLT_STR,
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp, &bnd5hp, errhp, 5,
                           (dvoid *)&value[0], (sword)sizeof(value[0]), SQLT_FLT,
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp, &bnd6hp, errhp, 6,
                           (dvoid *)&value[1], (sword)sizeof(value[1]), SQLT_FLT,
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp, &bnd7hp, errhp, 7,
                           (dvoid *)&value[2], (sword)sizeof(value[2]), SQLT_FLT, 
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp, &bnd8hp, errhp, 8,
                           (dvoid *)&value[3], (sword)sizeof(value[3]), SQLT_FLT,
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp, &bnd9hp, errhp, 9,
                           (dvoid *)&value[4], (sword)sizeof(value[4]), SQLT_FLT, 
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp, &bnd10hp, errhp, 10,
                           (dvoid *)&value[5], (sword)sizeof(value[5]), SQLT_FLT, 
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp, &bnd11hp, errhp, 11,
                           (dvoid *)&value[6], (sword)sizeof(value[6]), SQLT_FLT,
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp, &bnd12hp, errhp, 12,
                           (dvoid *)&value[7], (sword)sizeof(value[7]), SQLT_FLT,
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp, &bnd13hp, errhp, 13,
                           (dvoid *)&value[8], (sword)sizeof(value[8]), SQLT_FLT, 
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp, &bnd14hp, errhp, 14,
                           (dvoid *)&value[9], (sword)sizeof(value[9]), SQLT_FLT,
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp, &bnd15hp, errhp, 15,
                           (dvoid *)&value[10], (sword)sizeof(value[10]), SQLT_FLT, 
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp, &bnd16hp, errhp, 16,
                           (dvoid *)&value[11], (sword)sizeof(value[11]), SQLT_FLT, 
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp, &bnd17hp, errhp, 17,
                           (dvoid *)&value[12], (sword)sizeof(value[12]), SQLT_FLT,
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp, &bnd18hp, errhp, 18,
                           (dvoid *)&value[13], (sword)sizeof(value[13]), SQLT_FLT, 
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp, &bnd19hp, errhp, 19,
                           (dvoid *)&value[14], (sword)sizeof(value[14]), SQLT_FLT, 
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
     status |= OCIBindByPos(stmthp, &bnd20hp, errhp, 20,
                           (dvoid *)&value[15], (sword)sizeof(value[15]), SQLT_FLT, 
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

/*********************************************************************
**
**    Function    :  insert_DI03() 
**    Author      :  FengYuXing 
**
**********************************************************************/
int insert_DI03(char *date,char  *cymd,char  *chms,char  *des, float value[])
{
    char insert_statement[255],select_statement[255],delete_statement[255];
    int count=0,ret=0;
    sword  status;
    OCIDefine *def1hp;
    OCIBind   *bnd1hp, *bnd2hp, *bnd3hp, *bnd4hp,*bnd5hp, *bnd6hp, *bnd7hp, *bnd8hp,*bnd9hp, *bnd10hp, *bnd11hp, *bnd12hp;
    OCIBind   *bnd13hp, *bnd14hp, *bnd15hp, *bnd16hp,*bnd17hp, *bnd18hp, *bnd19hp, *bnd20hp, *bnd21hp, *bnd22hp, *bnd23hp;
    OCIBind   *bnd24hp;
    
    /*
    --- select
    */
   
    memset(select_statement, '\0', sizeof(select_statement));
    sprintf(select_statement,"select count(*) as num from OBSERVE.ELE_DI03 where c_date = :1 and v01000 = :9 and v71040 = :23 and v71826 = :24");
  
    status = OCIStmtPrepare(stmthp1, errhp,
                            select_statement, (ub4) strlen(select_statement),
                            (ub4) OCI_NTV_SYNTAX, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp1, &bnd1hp, errhp, 1,
                           (dvoid *)date, strlen(date)+1, SQLT_STR,
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp1, &bnd2hp, errhp, 2,
                           (dvoid *)&value[4], (sword)sizeof(value[4]), SQLT_FLT, 
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
     status |= OCIBindByPos(stmthp1, &bnd3hp, errhp, 3,
                           (dvoid *)&value[18], (sword)sizeof(value[18]), SQLT_FLT, 
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
     status |= OCIBindByPos(stmthp1, &bnd4hp, errhp, 4,
                           (dvoid *)&value[19], (sword)sizeof(value[19]), SQLT_FLT, 
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
    
    /*
    --- delect
    */
    if(ret>0)
    {  
       memset(delete_statement,'\0',sizeof(delete_statement));
      sprintf(delete_statement,"delete from OBSERVE.ELE_DI03 where c_date = :1 and v01000 = :9 and v71040 = :23 and v71826 = :24");
     
      status = OCIStmtPrepare(stmthp2, errhp,
                              delete_statement, (ub4) strlen(delete_statement),
                              (ub4) OCI_NTV_SYNTAX, (ub4) OCI_DEFAULT);
      status |= OCIBindByPos(stmthp2, &bnd1hp, errhp, 1,
                           (dvoid *)date, strlen(date)+1, SQLT_STR,
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
      status |= OCIBindByPos(stmthp2, &bnd2hp, errhp, 2,
                           (dvoid *)&value[4], (sword)sizeof(value[4]), SQLT_FLT, 
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
       status |= OCIBindByPos(stmthp2, &bnd3hp, errhp, 3,
                           (dvoid *)&value[18], (sword)sizeof(value[18]), SQLT_FLT, 
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
       status |= OCIBindByPos(stmthp2, &bnd4hp, errhp, 4,
                           (dvoid *)&value[19], (sword)sizeof(value[19]), SQLT_FLT, 
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
    
    /*
    --- insert
    */
    
    memset(insert_statement, '\0', sizeof(insert_statement));
    sprintf(insert_statement, "insert into OBSERVE.ELE_DI03 values(:1,:2,:3,:4,:5,:6,:7,:8,:9,:10,"
			":11,:12,:13,:14,:15,:16,:17,:18,:19,:20,:21,:22,:23,:24)");
    
    status = OCIStmtPrepare(stmthp, errhp, 
                            insert_statement, (ub4) strlen(insert_statement),
                            (ub4) OCI_NTV_SYNTAX, (ub4) OCI_DEFAULT);
    
    status |= OCIBindByPos(stmthp, &bnd1hp, errhp, 1,
                           (dvoid *)date, strlen(date)+1, SQLT_STR,
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp, &bnd2hp, errhp, 2,
                           (dvoid *)cymd, strlen(cymd)+1, SQLT_STR,
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp, &bnd3hp, errhp, 3,
                           (dvoid *)chms, strlen(chms)+1, SQLT_STR,
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
     status |= OCIBindByPos(stmthp, &bnd4hp, errhp, 4,
                           (dvoid *)des, strlen(des)+1, SQLT_STR,
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp, &bnd5hp, errhp, 5,
                           (dvoid *)&value[0], (sword)sizeof(value[0]), SQLT_FLT,
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp, &bnd6hp, errhp, 6,
                           (dvoid *)&value[1], (sword)sizeof(value[1]), SQLT_FLT,
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp, &bnd7hp, errhp, 7,
                           (dvoid *)&value[2], (sword)sizeof(value[2]), SQLT_FLT, 
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp, &bnd8hp, errhp, 8,
                           (dvoid *)&value[3], (sword)sizeof(value[3]), SQLT_FLT,
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp, &bnd9hp, errhp, 9,
                           (dvoid *)&value[4], (sword)sizeof(value[4]), SQLT_FLT, 
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp, &bnd10hp, errhp, 10,
                           (dvoid *)&value[5], (sword)sizeof(value[5]), SQLT_FLT, 
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp, &bnd11hp, errhp, 11,
                           (dvoid *)&value[6], (sword)sizeof(value[6]), SQLT_FLT,
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp, &bnd12hp, errhp, 12,
                           (dvoid *)&value[7], (sword)sizeof(value[7]), SQLT_FLT,
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp, &bnd13hp, errhp, 13,
                           (dvoid *)&value[8], (sword)sizeof(value[8]), SQLT_FLT, 
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp, &bnd14hp, errhp, 14,
                           (dvoid *)&value[9], (sword)sizeof(value[9]), SQLT_FLT,
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp, &bnd15hp, errhp, 15,
                           (dvoid *)&value[10], (sword)sizeof(value[10]), SQLT_FLT, 
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp, &bnd16hp, errhp, 16,
                           (dvoid *)&value[11], (sword)sizeof(value[11]), SQLT_FLT, 
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp, &bnd17hp, errhp, 17,
                           (dvoid *)&value[12], (sword)sizeof(value[12]), SQLT_FLT,
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp, &bnd18hp, errhp, 18,
                           (dvoid *)&value[13], (sword)sizeof(value[13]), SQLT_FLT, 
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp, &bnd19hp, errhp, 19,
                           (dvoid *)&value[14], (sword)sizeof(value[14]), SQLT_FLT, 
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
     status |= OCIBindByPos(stmthp, &bnd20hp, errhp, 20,
                           (dvoid *)&value[15], (sword)sizeof(value[15]), SQLT_FLT, 
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);    
     status |= OCIBindByPos(stmthp, &bnd21hp, errhp, 21,
                           (dvoid *)&value[16], (sword)sizeof(value[16]), SQLT_FLT, 
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);    
     status |= OCIBindByPos(stmthp, &bnd22hp, errhp, 22,
                           (dvoid *)&value[17], (sword)sizeof(value[17]), SQLT_FLT, 
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);    
     status |= OCIBindByPos(stmthp, &bnd23hp, errhp, 23,
                           (dvoid *)&value[18], (sword)sizeof(value[18]), SQLT_FLT, 
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);    
      status |= OCIBindByPos(stmthp, &bnd24hp, errhp, 24,
                           (dvoid *)&value[19], (sword)sizeof(value[19]), SQLT_FLT, 
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

/*********************************************************************
**
**    Function    :  insert_DI04() 
**    Author      :  FengYuXing 
**
**********************************************************************/
int insert_DI04(char *date,char  *cymd,char  *chms,char  *des, float value[])
{
    char insert_statement[255],select_statement[255],delete_statement[255];
    int count=0,ret=0;
    sword  status;
    OCIDefine *def1hp;
    OCIBind   *bnd1hp, *bnd2hp, *bnd3hp, *bnd4hp,*bnd5hp, *bnd6hp, *bnd7hp, *bnd8hp,*bnd9hp, *bnd10hp, *bnd11hp, *bnd12hp;
    OCIBind   *bnd13hp, *bnd14hp, *bnd15hp, *bnd16hp,*bnd17hp, *bnd18hp, *bnd19hp, *bnd20hp, *bnd21hp, *bnd22hp, *bnd23hp;
    OCIBind   *bnd24hp;
    
    /*
    --- select
    */
   
    memset(select_statement, '\0', sizeof(select_statement));
    sprintf(select_statement,"select count(*) as num from OBSERVE.ELE_DI04 where c_date = :1 and v01000 = :9 and v71040 = :23 and v71826 = :24");
  
    status = OCIStmtPrepare(stmthp1, errhp,
                            select_statement, (ub4) strlen(select_statement),
                            (ub4) OCI_NTV_SYNTAX, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp1, &bnd1hp, errhp, 1,
                           (dvoid *)date, strlen(date)+1, SQLT_STR,
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp1, &bnd2hp, errhp, 2,
                           (dvoid *)&value[4], (sword)sizeof(value[4]), SQLT_FLT, 
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
     status |= OCIBindByPos(stmthp1, &bnd3hp, errhp, 3,
                           (dvoid *)&value[18], (sword)sizeof(value[18]), SQLT_FLT, 
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
     status |= OCIBindByPos(stmthp1, &bnd4hp, errhp, 4,
                           (dvoid *)&value[19], (sword)sizeof(value[19]), SQLT_FLT, 
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
    
    /*
    --- delect
    */
    if(ret>0)
    {  
       memset(delete_statement,'\0',sizeof(delete_statement));
      sprintf(delete_statement,"delete from OBSERVE.ELE_DI04 where c_date = :1 and v01000 = :9 and v71040 = :23 and v71826 = :24");
     
      status = OCIStmtPrepare(stmthp2, errhp,
                              delete_statement, (ub4) strlen(delete_statement),
                              (ub4) OCI_NTV_SYNTAX, (ub4) OCI_DEFAULT);
      status |= OCIBindByPos(stmthp2, &bnd1hp, errhp, 1,
                           (dvoid *)date, strlen(date)+1, SQLT_STR,
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
      status |= OCIBindByPos(stmthp2, &bnd2hp, errhp, 2,
                           (dvoid *)&value[4], (sword)sizeof(value[4]), SQLT_FLT, 
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
       status |= OCIBindByPos(stmthp2, &bnd3hp, errhp, 3,
                           (dvoid *)&value[18], (sword)sizeof(value[18]), SQLT_FLT, 
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
       status |= OCIBindByPos(stmthp2, &bnd4hp, errhp, 4,
                           (dvoid *)&value[19], (sword)sizeof(value[19]), SQLT_FLT, 
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
    
    /*
    --- insert
    */
    
    memset(insert_statement, '\0', sizeof(insert_statement));
    sprintf(insert_statement, "insert into OBSERVE.ELE_DI04 values(:1,:2,:3,:4,:5,:6,:7,:8,:9,:10,"
			":11,:12,:13,:14,:15,:16,:17,:18,:19,:20,:21,:22,:23,:24)");
    
    status = OCIStmtPrepare(stmthp, errhp, 
                            insert_statement, (ub4) strlen(insert_statement),
                            (ub4) OCI_NTV_SYNTAX, (ub4) OCI_DEFAULT);
    
    status |= OCIBindByPos(stmthp, &bnd1hp, errhp, 1,
                           (dvoid *)date, strlen(date)+1, SQLT_STR,
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp, &bnd2hp, errhp, 2,
                           (dvoid *)cymd, strlen(cymd)+1, SQLT_STR,
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp, &bnd3hp, errhp, 3,
                           (dvoid *)chms, strlen(chms)+1, SQLT_STR,
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
     status |= OCIBindByPos(stmthp, &bnd4hp, errhp, 4,
                           (dvoid *)des, strlen(des)+1, SQLT_STR,
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp, &bnd5hp, errhp, 5,
                           (dvoid *)&value[0], (sword)sizeof(value[0]), SQLT_FLT,
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp, &bnd6hp, errhp, 6,
                           (dvoid *)&value[1], (sword)sizeof(value[1]), SQLT_FLT,
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp, &bnd7hp, errhp, 7,
                           (dvoid *)&value[2], (sword)sizeof(value[2]), SQLT_FLT, 
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp, &bnd8hp, errhp, 8,
                           (dvoid *)&value[3], (sword)sizeof(value[3]), SQLT_FLT,
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp, &bnd9hp, errhp, 9,
                           (dvoid *)&value[4], (sword)sizeof(value[4]), SQLT_FLT, 
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp, &bnd10hp, errhp, 10,
                           (dvoid *)&value[5], (sword)sizeof(value[5]), SQLT_FLT, 
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp, &bnd11hp, errhp, 11,
                           (dvoid *)&value[6], (sword)sizeof(value[6]), SQLT_FLT,
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp, &bnd12hp, errhp, 12,
                           (dvoid *)&value[7], (sword)sizeof(value[7]), SQLT_FLT,
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp, &bnd13hp, errhp, 13,
                           (dvoid *)&value[8], (sword)sizeof(value[8]), SQLT_FLT, 
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp, &bnd14hp, errhp, 14,
                           (dvoid *)&value[9], (sword)sizeof(value[9]), SQLT_FLT,
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp, &bnd15hp, errhp, 15,
                           (dvoid *)&value[10], (sword)sizeof(value[10]), SQLT_FLT, 
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp, &bnd16hp, errhp, 16,
                           (dvoid *)&value[11], (sword)sizeof(value[11]), SQLT_FLT, 
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp, &bnd17hp, errhp, 17,
                           (dvoid *)&value[12], (sword)sizeof(value[12]), SQLT_FLT,
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp, &bnd18hp, errhp, 18,
                           (dvoid *)&value[13], (sword)sizeof(value[13]), SQLT_FLT, 
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp, &bnd19hp, errhp, 19,
                           (dvoid *)&value[14], (sword)sizeof(value[14]), SQLT_FLT, 
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
     status |= OCIBindByPos(stmthp, &bnd20hp, errhp, 20,
                           (dvoid *)&value[15], (sword)sizeof(value[15]), SQLT_FLT, 
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);    
     status |= OCIBindByPos(stmthp, &bnd21hp, errhp, 21,
                           (dvoid *)&value[16], (sword)sizeof(value[16]), SQLT_FLT, 
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);    
     status |= OCIBindByPos(stmthp, &bnd22hp, errhp, 22,
                           (dvoid *)&value[17], (sword)sizeof(value[17]), SQLT_FLT, 
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);    
     status |= OCIBindByPos(stmthp, &bnd23hp, errhp, 23,
                           (dvoid *)&value[18], (sword)sizeof(value[18]), SQLT_FLT, 
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);    
      status |= OCIBindByPos(stmthp, &bnd24hp, errhp, 24,
                           (dvoid *)&value[19], (sword)sizeof(value[19]), SQLT_FLT, 
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




/*********************************************************************
**
**    Function    :  insert_TENS() 
**    Author      :  FengYuXing 
**
**********************************************************************/
int insert_TENS(char *date,float value[])
{
    char insert_statement[255],select_statement[255],delete_statement[255];
    int count=0,ret=0;
    sword  status;
    OCIDefine *def1hp;
    OCIBind   *bnd1hp, *bnd2hp, *bnd3hp, *bnd4hp,*bnd5hp, *bnd6hp, *bnd7hp, *bnd8hp,*bnd9hp, *bnd10hp, *bnd11hp, *bnd12hp;
    OCIBind   *bnd13hp, *bnd14hp, *bnd15hp, *bnd16hp, *bnd17hp, *bnd18hp, *bnd19hp, *bnd20hp, *bnd21hp, *bnd22hp, *bnd23hp;
    OCIBind   *bnd24hp, *bnd25hp, *bnd26hp, *bnd27hp, *bnd28hp, *bnd29hp, *bnd30hp, *bnd31hp;
    
    /*
    --- select
    */
   
    memset(select_statement, '\0', sizeof(select_statement));
    sprintf(select_statement,"select count(*) as num from OBSERVE.ELE_TENS where c_date = :1 and v01000 = :2");
  
    status = OCIStmtPrepare(stmthp1, errhp,
                            select_statement, (ub4) strlen(select_statement),
                            (ub4) OCI_NTV_SYNTAX, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp1, &bnd1hp, errhp, 1,
                           (dvoid *)date, strlen(date)+1, SQLT_STR,
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp1, &bnd2hp, errhp, 2,
                           (dvoid *)&value[0], (sword)sizeof(value[0]), SQLT_FLT, 
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
    
    /*
    --- delect
    */
    if(ret>0)
    {  
       memset(delete_statement,'\0',sizeof(delete_statement));
      sprintf(delete_statement,"delete from OBSERVE.ELE_TENS where c_date = :1 and v01000 = :2");
     
      status = OCIStmtPrepare(stmthp2, errhp,
                              delete_statement, (ub4) strlen(delete_statement),
                              (ub4) OCI_NTV_SYNTAX, (ub4) OCI_DEFAULT);
      status |= OCIBindByPos(stmthp2, &bnd1hp, errhp, 1,
                           (dvoid *)date, strlen(date)+1, SQLT_STR,
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
      status |= OCIBindByPos(stmthp2, &bnd2hp, errhp, 2,
                           (dvoid *)&value[0], (sword)sizeof(value[0]), SQLT_FLT, 
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
    
    /*
    --- insert
    */
    
    memset(insert_statement, '\0', sizeof(insert_statement));
    sprintf(insert_statement, "insert into OBSERVE.ELE_TENS values(:1,:2,:3,:4,:5,:6,:7,:8,:9,:10,"
			":11,:12,:13,:14,:15,:16,:17,:18,:19,:20,:21,:22,:23,:24,:25,:26,:27,:28,:29,:30,:31)");
    
    status = OCIStmtPrepare(stmthp, errhp, 
                            insert_statement, (ub4) strlen(insert_statement),
                            (ub4) OCI_NTV_SYNTAX, (ub4) OCI_DEFAULT);
    
    status |= OCIBindByPos(stmthp, &bnd1hp, errhp, 1,
                           (dvoid *)date, strlen(date)+1, SQLT_STR,
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp, &bnd2hp, errhp, 2,
                           (dvoid *)&value[0], (sword)sizeof(value[0]), SQLT_FLT,
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp, &bnd3hp, errhp, 3,
                           (dvoid *)&value[1], (sword)sizeof(value[1]), SQLT_FLT,
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp, &bnd4hp, errhp, 4,
                           (dvoid *)&value[2], (sword)sizeof(value[2]), SQLT_FLT, 
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp, &bnd5hp, errhp, 5,
                           (dvoid *)&value[3], (sword)sizeof(value[3]), SQLT_FLT,
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp, &bnd6hp, errhp, 6,
                           (dvoid *)&value[4], (sword)sizeof(value[4]), SQLT_FLT, 
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp, &bnd7hp, errhp, 7,
                           (dvoid *)&value[5], (sword)sizeof(value[5]), SQLT_FLT, 
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp, &bnd8hp, errhp, 8,
                           (dvoid *)&value[6], (sword)sizeof(value[6]), SQLT_FLT,
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp, &bnd9hp, errhp, 9,
                           (dvoid *)&value[7], (sword)sizeof(value[7]), SQLT_FLT,
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp, &bnd10hp, errhp, 10,
                           (dvoid *)&value[8], (sword)sizeof(value[8]), SQLT_FLT, 
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp, &bnd11hp, errhp, 11,
                           (dvoid *)&value[9], (sword)sizeof(value[9]), SQLT_FLT,
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp, &bnd12hp, errhp, 12,
                           (dvoid *)&value[10], (sword)sizeof(value[10]), SQLT_FLT, 
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp, &bnd13hp, errhp, 13,
                           (dvoid *)&value[11], (sword)sizeof(value[11]), SQLT_FLT, 
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT); 
     status |= OCIBindByPos(stmthp, &bnd14hp, errhp, 14,
                           (dvoid *)&value[12], (sword)sizeof(value[12]), SQLT_FLT, 
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT); 
    status |= OCIBindByPos(stmthp, &bnd15hp, errhp, 15,
                           (dvoid *)&value[13], (sword)sizeof(value[13]), SQLT_FLT, 
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT); 
    status |= OCIBindByPos(stmthp, &bnd16hp, errhp, 16,
                           (dvoid *)&value[14], (sword)sizeof(value[14]), SQLT_FLT, 
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT); 
     status |= OCIBindByPos(stmthp, &bnd17hp, errhp, 17,
                           (dvoid *)&value[15], (sword)sizeof(value[15]), SQLT_FLT, 
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT); 
    status |= OCIBindByPos(stmthp, &bnd18hp, errhp, 18,
                           (dvoid *)&value[16], (sword)sizeof(value[16]), SQLT_FLT, 
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT); 
    status |= OCIBindByPos(stmthp, &bnd19hp, errhp, 19,
                           (dvoid *)&value[17], (sword)sizeof(value[17]), SQLT_FLT, 
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT); 
   status |= OCIBindByPos(stmthp, &bnd20hp, errhp, 20,
                           (dvoid *)&value[18], (sword)sizeof(value[18]), SQLT_FLT, 
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT); 
    status |= OCIBindByPos(stmthp, &bnd21hp, errhp, 21,
                           (dvoid *)&value[19], (sword)sizeof(value[19]), SQLT_FLT, 
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT); 
    status |= OCIBindByPos(stmthp, &bnd22hp, errhp, 22,
                           (dvoid *)&value[20], (sword)sizeof(value[20]), SQLT_FLT, 
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT); 
    status |= OCIBindByPos(stmthp, &bnd23hp, errhp, 23,
                           (dvoid *)&value[21], (sword)sizeof(value[21]), SQLT_FLT, 
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT); 
    status |= OCIBindByPos(stmthp, &bnd24hp, errhp, 24,
                           (dvoid *)&value[22], (sword)sizeof(value[22]), SQLT_FLT, 
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT); 
    status |= OCIBindByPos(stmthp, &bnd25hp, errhp, 25,
                           (dvoid *)&value[23], (sword)sizeof(value[23]), SQLT_FLT, 
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT); 
    status |= OCIBindByPos(stmthp, &bnd26hp, errhp, 26,
                           (dvoid *)&value[24], (sword)sizeof(value[24]), SQLT_FLT, 
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT); 
    status |= OCIBindByPos(stmthp, &bnd27hp, errhp, 27,
                           (dvoid *)&value[25], (sword)sizeof(value[25]), SQLT_FLT, 
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT); 
    status |= OCIBindByPos(stmthp, &bnd28hp, errhp, 28,
                           (dvoid *)&value[26], (sword)sizeof(value[26]), SQLT_FLT, 
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT); 
    status |= OCIBindByPos(stmthp, &bnd29hp, errhp, 29,
                           (dvoid *)&value[27], (sword)sizeof(value[27]), SQLT_FLT, 
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT); 
    status |= OCIBindByPos(stmthp, &bnd30hp, errhp, 30,
                           (dvoid *)&value[28], (sword)sizeof(value[28]), SQLT_FLT, 
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT); 
    status |= OCIBindByPos(stmthp, &bnd31hp, errhp, 31,
                           (dvoid *)&value[29], (sword)sizeof(value[29]), SQLT_FLT, 
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


/*********************************************************************
**
**    Function    :  insert_ MONT() 
**    Author      :  FengYuXing 
**
**********************************************************************/
int insert_MONT(char *date,float value[])
{
    char insert_statement[255],select_statement[255],delete_statement[255];
    int count=0,ret=0;
    sword  status;
    OCIDefine *def1hp;
    OCIBind   *bnd1hp, *bnd2hp, *bnd3hp, *bnd4hp,*bnd5hp, *bnd6hp, *bnd7hp, *bnd8hp,*bnd9hp, *bnd10hp, *bnd11hp, *bnd12hp;
    OCIBind   *bnd13hp, *bnd14hp, *bnd15hp, *bnd16hp, *bnd17hp, *bnd18hp, *bnd19hp, *bnd20hp, *bnd21hp;
        
    /*
    --- select
    */
   
    memset(select_statement, '\0', sizeof(select_statement));
    sprintf(select_statement,"select count(*) as num from OBSERVE.ELE_MONT where c_date = :1 and v01000 = :2");
  
    status = OCIStmtPrepare(stmthp1, errhp,
                            select_statement, (ub4) strlen(select_statement),
                            (ub4) OCI_NTV_SYNTAX, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp1, &bnd1hp, errhp, 1,
                           (dvoid *)date, strlen(date)+1, SQLT_STR,
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp1, &bnd2hp, errhp, 2,
                           (dvoid *)&value[0], (sword)sizeof(value[0]), SQLT_FLT, 
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
    
    /*
    --- delect
    */
    if(ret>0)
    {  
       memset(delete_statement,'\0',sizeof(delete_statement));
      sprintf(delete_statement,"delete from OBSERVE.ELE_MONT where c_date = :1 and v01000 = :2");
     
      status = OCIStmtPrepare(stmthp2, errhp,
                              delete_statement, (ub4) strlen(delete_statement),
                              (ub4) OCI_NTV_SYNTAX, (ub4) OCI_DEFAULT);
      status |= OCIBindByPos(stmthp2, &bnd1hp, errhp, 1,
                           (dvoid *)date, strlen(date)+1, SQLT_STR,
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
      status |= OCIBindByPos(stmthp2, &bnd2hp, errhp, 2,
                           (dvoid *)&value[0], (sword)sizeof(value[0]), SQLT_FLT, 
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
    
    /*
    --- insert
    */
    
    memset(insert_statement, '\0', sizeof(insert_statement));
    sprintf(insert_statement, "insert into OBSERVE.ELE_MONT values(:1,:2,:3,:4,:5,:6,:7,:8,:9,:10,"
			":11,:12,:13,:14,:15,:16,:17,:18,:19,:20,:21)");
    
    status = OCIStmtPrepare(stmthp, errhp, 
                            insert_statement, (ub4) strlen(insert_statement),
                            (ub4) OCI_NTV_SYNTAX, (ub4) OCI_DEFAULT);
    
    status |= OCIBindByPos(stmthp, &bnd1hp, errhp, 1,
                           (dvoid *)date, strlen(date)+1, SQLT_STR,
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp, &bnd2hp, errhp, 2,
                           (dvoid *)&value[0], (sword)sizeof(value[0]), SQLT_FLT,
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp, &bnd3hp, errhp, 3,
                           (dvoid *)&value[1], (sword)sizeof(value[1]), SQLT_FLT,
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp, &bnd4hp, errhp, 4,
                           (dvoid *)&value[2], (sword)sizeof(value[2]), SQLT_FLT, 
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp, &bnd5hp, errhp, 5,
                           (dvoid *)&value[3], (sword)sizeof(value[3]), SQLT_FLT,
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp, &bnd6hp, errhp, 6,
                           (dvoid *)&value[4], (sword)sizeof(value[4]), SQLT_FLT, 
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp, &bnd7hp, errhp, 7,
                           (dvoid *)&value[5], (sword)sizeof(value[5]), SQLT_FLT, 
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp, &bnd8hp, errhp, 8,
                           (dvoid *)&value[6], (sword)sizeof(value[6]), SQLT_FLT,
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp, &bnd9hp, errhp, 9,
                           (dvoid *)&value[7], (sword)sizeof(value[7]), SQLT_FLT,
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp, &bnd10hp, errhp, 10,
                           (dvoid *)&value[8], (sword)sizeof(value[8]), SQLT_FLT, 
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp, &bnd11hp, errhp, 11,
                           (dvoid *)&value[9], (sword)sizeof(value[9]), SQLT_FLT,
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp, &bnd12hp, errhp, 12,
                           (dvoid *)&value[10], (sword)sizeof(value[10]), SQLT_FLT, 
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp, &bnd13hp, errhp, 13,
                           (dvoid *)&value[11], (sword)sizeof(value[11]), SQLT_FLT, 
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT); 
     status |= OCIBindByPos(stmthp, &bnd14hp, errhp, 14,
                           (dvoid *)&value[12], (sword)sizeof(value[12]), SQLT_FLT, 
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT); 
    status |= OCIBindByPos(stmthp, &bnd15hp, errhp, 15,
                           (dvoid *)&value[13], (sword)sizeof(value[13]), SQLT_FLT, 
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT); 
    status |= OCIBindByPos(stmthp, &bnd16hp, errhp, 16,
                           (dvoid *)&value[14], (sword)sizeof(value[14]), SQLT_FLT, 
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT); 
     status |= OCIBindByPos(stmthp, &bnd17hp, errhp, 17,
                           (dvoid *)&value[15], (sword)sizeof(value[15]), SQLT_FLT, 
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT); 
    status |= OCIBindByPos(stmthp, &bnd18hp, errhp, 18,
                           (dvoid *)&value[16], (sword)sizeof(value[16]), SQLT_FLT, 
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT); 
    status |= OCIBindByPos(stmthp, &bnd19hp, errhp, 19,
                           (dvoid *)&value[17], (sword)sizeof(value[17]), SQLT_FLT, 
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT); 
   status |= OCIBindByPos(stmthp, &bnd20hp, errhp, 20,
                           (dvoid *)&value[18], (sword)sizeof(value[18]), SQLT_FLT, 
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT); 
    status |= OCIBindByPos(stmthp, &bnd21hp, errhp, 21,
                           (dvoid *)&value[19], (sword)sizeof(value[19]), SQLT_FLT, 
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
