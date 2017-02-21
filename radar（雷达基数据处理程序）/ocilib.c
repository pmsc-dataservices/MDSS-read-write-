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
**    Function    :  insert_surf_surx() 
**    Author      :  FengYuXing 
**
**********************************************************************/
int  insert_radr(char *itime,char *ptime,char *sdir,char *tdir,char *filename,char *filesize,char *pname,char *sip,char *sname,char *tip,char *tname)
{
    char insert_statement[500],select_statement[255],delete_statement[255];
    int count=0,ret=0;
     char time[25];
     int   ct[10];
    sword  status;
    OCIDefine *def1hp;
    OCIBind   *bnd1hp, *bnd2hp, *bnd3hp, *bnd4hp, *bnd5hp, *bnd6hp, *bnd7hp, *bnd8hp,*bnd9hp, *bnd10hp, *bnd11hp, *bnd12hp;
     OCIBind   *bnd13hp, *bnd14hp, *bnd15hp, *bnd16hp, *bnd17hp, *bnd18hp, *bnd19hp, *bnd20hp,*bnd21hp, *bnd22hp, *bnd23hp, *bnd24hp;
     OCIBind   *bnd25hp, *bnd26hp, *bnd27hp, *bnd28hp, *bnd29hp, *bnd30hp, *bnd31hp, *bnd32hp,*bnd33hp, *bnd34hp, *bnd35hp, *bnd36hp;
     OCIBind   *bnd37hp, *bnd38hp, *bnd39hp, *bnd40hp, *bnd41hp, *bnd42hp, *bnd43hp, *bnd44hp,*bnd45hp, *bnd46hp, *bnd47hp, *bnd48hp;

      numtime(ct, -1);
      memset(time,'\0',sizeof(time));
      sprintf(time,"%4d/%d/%d %02d:%02d:%02d",ct[0],ct[1],ct[2],ct[3],ct[4],ct[5]);
   
    
    /*
    --- insert
    */
    printf("%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s\n",itime,ptime,sdir,tdir,filename,filesize,pname,sip,sname,tip,tname);

    memset(insert_statement, '\0', sizeof(insert_statement));
    sprintf(insert_statement, "insert into observe.monitor_data_info values(:1,:2,:3,:4,:5,:6,:7,:8,:9,:10,:11)");

    status = OCIStmtPrepare(stmthp, errhp, 
                            insert_statement, (ub4) strlen(insert_statement),
                            (ub4) OCI_NTV_SYNTAX, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp, &bnd1hp, errhp, 1,
                           (dvoid *)itime, strlen(itime)+1, SQLT_STR,
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp, &bnd2hp, errhp, 2,
                           (dvoid *)ptime, strlen(ptime)+1, SQLT_STR,
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
     status |= OCIBindByPos(stmthp, &bnd3hp, errhp, 3,
                           (dvoid *)sdir, strlen(sdir)+1, SQLT_STR,
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp, &bnd4hp, errhp, 4,
                           (dvoid *)tdir, strlen(tdir)+1, SQLT_STR,
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp, &bnd5hp, errhp, 5,
                           (dvoid *)filename, strlen(filename)+1, SQLT_STR,
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp, &bnd6hp, errhp, 6,
                           (dvoid *)filesize, strlen(filesize)+1, SQLT_STR,
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp, &bnd7hp, errhp, 7,
                           (dvoid *)pname, strlen(pname)+1, SQLT_STR,
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp, &bnd8hp, errhp, 8,
                           (dvoid *)sip, strlen(sip)+1, SQLT_STR,
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
     status |= OCIBindByPos(stmthp, &bnd9hp, errhp, 9,
                           (dvoid *)sname, strlen(sname)+1, SQLT_STR,
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
     status |= OCIBindByPos(stmthp, &bnd10hp, errhp, 10,
                           (dvoid *)tip, strlen(tip)+1, SQLT_STR,
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
      status |= OCIBindByPos(stmthp, &bnd11hp, errhp, 11,
                           (dvoid *)tname, strlen(tname)+1, SQLT_STR,
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











