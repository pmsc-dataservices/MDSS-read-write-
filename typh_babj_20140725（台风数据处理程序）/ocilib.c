#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <oci.h>
#include "typh_babj.h"

OCIEnv         *envhp;
OCIError       *errhp;
OCIServer      *srvhp;
OCISvcCtx      *svchp;
OCISession     *authp;
OCIStmt        *stmthp;

char oci_err[1000];

/*******************************************************
**
**		Function: 	error_proc()
**
********************************************************/
void error_proc(dvoid *errhp, sword status)
{
	text	errbuf[512];
	sb4	  errcode;
	
	memset(oci_err, '\0', sizeof(oci_err));
	
	switch(status)
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
			OCIErrorGet((dvoid *)errhp,
									(ub4) 1,
									NULL,
									&errcode,
									errbuf, (ub4) sizeof(errbuf),
									OCI_HTYPE_ERROR);
			sprintf(oci_err, "OCI ERROR CODE: %d\n OCI ERROR INFO: %s\n", errcode, errbuf);
			break;
		case OCI_INVALID_HANDLE:
			strcpy(oci_err, "OCI ERROR CODE: OCI_INVALID_HANDLE\n");
			break;
		case OCI_STILL_EXECUTING:
			strcpy(oci_err, "OCI ERROR CODE: OCI_STILL_EXECUTING\n");
			break;
		default:
			break;									
	}
}

/*********************************************************************
**
**    Function    :  logon() 
**    Description :  begin session with database and allocate resource
**
**********************************************************************/
int logon(char *username, char *password, char *dbname)
{
  sword status;
  
  /* create OCI environment */
  OCIEnvCreate(&envhp,
               OCI_DEFAULT,
               (dvoid *) 0,
               (dvoid * (*)(dvoid *, size_t)) 0,
               (dvoid * (*)(dvoid *, dvoid *, size_t)) 0,
               (void (*)(dvoid *, dvoid *)) 0,
               0,
               (dvoid **) 0);

  /* allocating ERROR handle */
  OCIHandleAlloc((dvoid *) envhp,
                 (dvoid **) &errhp,
                 OCI_HTYPE_ERROR,
                 (size_t) 0,
                 (dvoid **) 0);

  /* allocating SERVER handle */
  OCIHandleAlloc((dvoid *) envhp,
                 (dvoid **) &srvhp,
                 OCI_HTYPE_SERVER,
                 (size_t) 0,
                 (dvoid **) 0);

  /* allocating SERVICE CONTEXT handle */
  OCIHandleAlloc((dvoid *) envhp,
                 (dvoid **) &svchp,
                 OCI_HTYPE_SVCCTX,
                 (size_t) 0,
                 (dvoid **) 0);

  /* allocating SESSION handle */
  OCIHandleAlloc((dvoid *) envhp,
                 (dvoid **) &authp,
                 OCI_HTYPE_SESSION,
                 (size_t) 0,
                 (dvoid **) 0);

  /* allocating STATMENT handle */
  OCIHandleAlloc((dvoid *) envhp,
                 (dvoid **) &stmthp,
                 OCI_HTYPE_STMT,
                 (size_t) 0,
                 (dvoid **) 0);

  /* connect database */
  status = OCIServerAttach(srvhp, errhp,
                           (text *)dbname, (ub4) strlen(dbname), 0);
  if (status != OCI_SUCCESS)
  {
     return -1;
  }

  /* set SESSION property */
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

  /* set SERVICE CONTEXT (SERVER) property */
  OCIAttrSet((dvoid *) svchp,
             OCI_HTYPE_SVCCTX,
             (dvoid *) srvhp,
             (ub4) 0,
             OCI_ATTR_SERVER,
             errhp);
             
  /* begin session */
  status = OCISessionBegin(svchp, errhp, authp, 
                           OCI_CRED_RDBMS, (ub4) OCI_DEFAULT);
  if (status != OCI_SUCCESS)
  {
     fprintf(fplog, "user = %s, password = %s, dbname = %s \n", username, password, dbname);
     fprintf(fplog, "Connect database Error\n");
     fflush(fplog);
     return -1;
  }
  
  /* set SERVICE CONTEXT (SESSION) property */
  OCIAttrSet((dvoid *) svchp,
             OCI_HTYPE_SVCCTX,
             (dvoid *) authp,
             (ub4) 0,
             OCI_ATTR_SESSION,
             errhp);
  return 0;
}


/*************************************************************
**
**		Function: 		logoff()
**		Description: 	end session with db and release resource
**
**************************************************************/
void logoff()
{
	OCISessionEnd(svchp, errhp, authp, (ub4) 0);
	OCIServerDetach(srvhp, errhp, (ub4) OCI_DEFAULT);
	OCIHandleFree((dvoid *) envhp, OCI_HTYPE_ENV);
}

/********************************************************************
**
**    Function    :  select_typh_file_hash()
**    Description :  select from forecast.ele_typh_babj with hash provided
**
********************************************************************/
int select_typh_file_hash(char *hash)
{
    sword  status;

    char	statement[200];
    int		NUM;
    OCIDefine *def1hp;
    OCIBind *bnd1hp;

    memset(statement, '\0', sizeof(statement));
    strcpy(statement, "select count(*) from forecast.ele_typh_babj_new where hash = :1");
    
    status = OCIStmtPrepare(stmthp, errhp, 
                            statement, (ub4) strlen(statement),
                            (ub4) OCI_NTV_SYNTAX, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp, &bnd1hp, errhp, 1,
                           (dvoid *) hash, 41, SQLT_AVC, 
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIDefineByPos(stmthp, &def1hp, errhp, 1, 
                             (ub1 *)&NUM, (sb4) sizeof(int), SQLT_INT, 
                             (dvoid *) 0, (ub2 *) 0, (ub2 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIStmtExecute(svchp, stmthp, errhp, (ub4) 0, (ub4) 0,
                              NULL, NULL, (ub4) OCI_DEFAULT);
    if (status != OCI_SUCCESS)
    {
        error_proc(errhp, status);
  		  fprintf(fplog, "SQL = %s\n", (char *)statement);
  		  fprintf(fplog, "error is: %s\n", oci_err);
  		  fflush(fplog);
        return -1;
    }
    status = OCIStmtFetch(stmthp, errhp, 1, OCI_FETCH_NEXT, OCI_DEFAULT);
    if(NUM < 0)
    {
    	 return -1;	
    }
    else if(NUM == 0) // not found
    {
    	 return 0;
    }
    else
    {
    	 return 1;
    }
}

/********************************************************************
**
**    Function    :  select_typh_time_status()
**    Description :  select from forecast.ele_typh_babj with hash provided
**
********************************************************************/
int select_typh_time_status(TYPH_KEY *key)
{
    sword  status;
    char	 statement[200];
    char   ftime[20], itime[20], typh_status[10];
    OCIDefine *def1hp, *def2hp, *def3hp;
    OCIBind *bnd1hp, *bnd2hp;
    int    record_num=0;

    memset(statement, '\0', sizeof(statement));
    strcpy(statement, "select ftime, itime, ty_status from forecast.ele_typh_babj_new where V55002=:1 and TY_BJTIME=:2 and rownum < 2 ");
    
    status = OCIStmtPrepare(stmthp, errhp, 
                            statement, (ub4) strlen(statement),
                            (ub4) OCI_NTV_SYNTAX, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp, &bnd1hp, errhp, 1,
                           (dvoid *)&key->code_cn, sizeof(int), SQLT_INT, 
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIBindByPos(stmthp, &bnd2hp, errhp, 2,
                           (dvoid *)key->ty_bjtime, 13, SQLT_AVC, 
                           (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                           (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIDefineByPos(stmthp, &def1hp, errhp, 1, 
                             (ub1 *)ftime, (sb4) 15, SQLT_AVC, 
                             (dvoid *) 0, (ub2 *) 0, (ub2 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIDefineByPos(stmthp, &def2hp, errhp, 2, 
                             (ub1 *)itime, (sb4) 15, SQLT_AVC, 
                             (dvoid *) 0, (ub2 *) 0, (ub2 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIDefineByPos(stmthp, &def3hp, errhp, 3, 
                             (ub1 *)typh_status, (sb4) 7, SQLT_AVC, 
                             (dvoid *) 0, (ub2 *) 0, (ub2 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIStmtExecute(svchp, stmthp, errhp, (ub4) 0, (ub4) 0,
                              NULL, NULL, (ub4) OCI_DEFAULT);
    if (status != OCI_SUCCESS)
    {
        error_proc(errhp, status);
  		  fprintf(fplog, "SQL = %s\n", (char *)statement);
  		  fprintf(fplog, "error is: %s\n", oci_err);
  		  fflush(fplog);
        return -1;
    }
    while ((status = OCIStmtFetch(stmthp, errhp, 1, OCI_FETCH_NEXT, OCI_DEFAULT)) != OCI_NO_DATA)
    {
    	  record_num = 1;
    	  stringtrim(ftime);
    	  stringtrim(itime);
    	  stringtrim(typh_status);
    	  strncpy(key->ftime, ftime, 14);
    	  strncpy(key->itime, itime, 14);
    	  strncpy(key->ty_status, typh_status, 6);
    }
    
    if (record_num==0)
    {
        memset(statement, '\0', sizeof(statement));
        strcpy(statement, "select ty_status from forecast.ele_typh_babj_new where V55002=:1 and rownum < 2 ");
    
        status = OCIStmtPrepare(stmthp, errhp, 
                                statement, (ub4) strlen(statement),
                                (ub4) OCI_NTV_SYNTAX, (ub4) OCI_DEFAULT);
        status |= OCIBindByPos(stmthp, &bnd1hp, errhp, 1,
                               (dvoid *)&key->code_cn, sizeof(int), SQLT_INT, 
                               (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                               (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
        status |= OCIDefineByPos(stmthp, &def1hp, errhp, 1, 
                                 (ub1 *)typh_status, (sb4) 7, SQLT_AVC, 
                                 (dvoid *) 0, (ub2 *) 0, (ub2 *) 0, (ub4) OCI_DEFAULT);
        status |= OCIStmtExecute(svchp, stmthp, errhp, (ub4) 0, (ub4) 0,
                                 NULL, NULL, (ub4) OCI_DEFAULT);
        if (status != OCI_SUCCESS)
        {
            error_proc(errhp, status);
  		      fprintf(fplog, "SQL = %s\n", (char *)statement);
  		      fprintf(fplog, "error is: %s\n", oci_err);
  		      fflush(fplog);
            return -1;
        }
        while ((status = OCIStmtFetch(stmthp, errhp, 1, OCI_FETCH_NEXT, OCI_DEFAULT)) != OCI_NO_DATA)
        {
    	      record_num = 1;
     	      stringtrim(typh_status);
    	      strncpy(key->ty_status, typh_status, 6);
    	  }
    }
    
    return 1;
}


/***************************************************************
**
**		Function:		insert_typh_babj()
**		Description:	insert typh msg to observe.ele_typh_babj
**
****************************************************************/
int insert_typh_babj(TYPH_KEY *key, TYPH_ELE *ele)
{
	sword status;
	OCIBind *bnd1hp, *bnd2hp, *bnd3hp, *bnd4hp, *bnd5hp, *bnd6hp, *bnd7hp, *bnd8hp, *bnd9hp, *bnd10hp, 
					*bnd11hp, *bnd12hp, *bnd13hp, *bnd14hp, *bnd15hp, *bnd16hp, *bnd17hp, *bnd18hp, *bnd19hp,
					*bnd20hp, *bnd21hp, *bnd22hp, *bnd23hp, *bnd24hp, *bnd25hp, *bnd26hp, *bnd27hp, *bnd28hp,*bnd29hp, *bnd30hp, *bnd31hp;
	char statement[300];

	memset(statement, '\0', sizeof(statement));
	strcpy(statement, "delete forecast.ele_typh_babj_new where V55002=:1 and TY_BJTIME=:2 and VTI=:3");
	status = OCIStmtPrepare(stmthp, errhp,
												statement , (ub4) strlen(statement),
												(ub4) OCI_NTV_SYNTAX, (ub4) OCI_DEFAULT);
	status |= OCIBindByPos(stmthp, &bnd1hp, errhp, 1,
											(dvoid *) &key->code_cn, sizeof(key->code_cn), SQLT_INT,
											(dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
											(ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
	status |= OCIBindByPos(stmthp, &bnd2hp, errhp, 2,
											(dvoid *) key->ty_bjtime, strlen(key->ty_bjtime)+1, SQLT_AVC,
											(dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
											(ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
	status |= OCIBindByPos(stmthp, &bnd3hp, errhp, 3,
											(dvoid *) &ele->vti, sizeof(ele->vti), SQLT_INT,
											(dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
											(ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
  status |= OCIStmtExecute(svchp, stmthp, errhp, (ub4) 1, (ub4) 0,
                           (CONST OCISnapshot *) 0, (OCISnapshot *) 0, (ub4) OCI_DEFAULT);
  if(status != OCI_SUCCESS)
  {
  		error_proc(errhp, status);
  		fprintf(fplog, "SQL = %s\n", (char *)statement);
  		fprintf(fplog, "error is: %s\n", oci_err);
  		fflush(fplog);
  }
	
	memset(statement, '\0', sizeof(statement));
	strcpy(statement, "insert into forecast.ele_typh_babj_new values(:1, :2, :3, :4, :5, :6, :7, :8, :9, :10, :11, :12, :13, :14, :15, :16, :17, :18, :19, :20, :21, :22, :23, :24, :25, :26, :27, :28, :29, :30, :31)");
	status = OCIStmtPrepare(stmthp, errhp,
												statement , (ub4) strlen(statement),
												(ub4) OCI_NTV_SYNTAX, (ub4) OCI_DEFAULT);
	status |= OCIBindByPos(stmthp, &bnd1hp, errhp, 1,
											(dvoid *) key->tyname, strlen(key->tyname)+1, SQLT_AVC,
											(dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
											(ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
	status |= OCIBindByPos(stmthp, &bnd2hp, errhp, 2,
											(dvoid *) key->hash, strlen(key->hash)+1, SQLT_AVC,
											(dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
											(ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
	status |= OCIBindByPos(stmthp, &bnd3hp, errhp, 3,
											(dvoid *) key->cccc, strlen(key->cccc)+1, SQLT_AVC,
											(dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
											(ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
	status |= OCIBindByPos(stmthp, &bnd4hp, errhp, 4,
											(dvoid *) key->fname, strlen(key->fname)+1, SQLT_AVC,
											(dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
											(ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
	status |= OCIBindByPos(stmthp, &bnd5hp, errhp, 5,
											(dvoid *) key->ftime, strlen(key->ftime)+1, SQLT_AVC,
											(dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
											(ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
	status |= OCIBindByPos(stmthp, &bnd6hp, errhp, 6,
											(dvoid *) key->itime, strlen(key->itime)+1, SQLT_AVC,
											(dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
											(ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
	status |= OCIBindByPos(stmthp, &bnd7hp, errhp, 7,
											(dvoid *) key->ty_type, strlen(key->ty_type)+1, SQLT_AVC,
											(dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
											(ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
	status |= OCIBindByPos(stmthp, &bnd8hp, errhp, 8,
											(dvoid *) key->ty_status, strlen(key->ty_status)+1, SQLT_AVC,
											(dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
											(ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
	status |= OCIBindByPos(stmthp, &bnd9hp, errhp, 9,
											(dvoid *) key->ty_bjtime, strlen(key->ty_bjtime)+1, SQLT_AVC,
											(dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
											(ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
	status |= OCIBindByPos(stmthp, &bnd10hp, errhp, 10,
											(dvoid *) key->ty_utime, strlen(key->ty_utime)+1, SQLT_AVC,
											(dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
											(ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
	status |= OCIBindByPos(stmthp, &bnd11hp, errhp, 11,
											(dvoid *) &key->code_cn, sizeof(key->code_cn), SQLT_INT,
											(dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
											(ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
	status |= OCIBindByPos(stmthp, &bnd12hp, errhp, 12,
											(dvoid *) &key->code_in, sizeof(key->code_in), SQLT_INT,
											(dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
											(ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
	status |= OCIBindByPos(stmthp, &bnd13hp, errhp, 13,
											(dvoid *) &ele->vti, sizeof(ele->vti), SQLT_INT,
											(dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
											(ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
	status |= OCIBindByPos(stmthp, &bnd14hp, errhp, 14,
											(dvoid *) &ele->v05001, sizeof(ele->v05001), SQLT_FLT,
											(dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
											(ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
	status |= OCIBindByPos(stmthp, &bnd15hp, errhp, 15,
											(dvoid *) &ele->v06001, sizeof(ele->v06001), SQLT_FLT,
											(dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
											(ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
	status |= OCIBindByPos(stmthp, &bnd16hp, errhp, 16,
											(dvoid *) &ele->v10004, sizeof(ele->v10004), SQLT_FLT,
											(dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
											(ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
	status |= OCIBindByPos(stmthp, &bnd17hp, errhp, 17,
											(dvoid *) &ele->v11041, sizeof(ele->v11041), SQLT_FLT,
											(dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
											(ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
	status |= OCIBindByPos(stmthp, &bnd18hp, errhp, 18,
											(dvoid *) &ele->v55010_1, sizeof(ele->v55010_1), SQLT_FLT,
											(dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
											(ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
 status |= OCIBindByPos(stmthp, &bnd19hp, errhp, 19,
											(dvoid *) &ele->v55010_2, sizeof(ele->v55010_2), SQLT_FLT,
											(dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
											(ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
 status |= OCIBindByPos(stmthp, &bnd20hp, errhp, 20,
											(dvoid *) &ele->v55010_3, sizeof(ele->v55010_3), SQLT_FLT,
											(dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
											(ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
 status |= OCIBindByPos(stmthp, &bnd21hp, errhp, 21,
											(dvoid *) &ele->v55010_4, sizeof(ele->v55010_4), SQLT_FLT,
											(dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
											(ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
	status |= OCIBindByPos(stmthp, &bnd22hp, errhp, 22,
											(dvoid *) &ele->v55011_1, sizeof(ele->v55011_1), SQLT_FLT,
											(dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
											(ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
	status |= OCIBindByPos(stmthp, &bnd23hp, errhp, 23,
											(dvoid *) &ele->v55011_2, sizeof(ele->v55011_2), SQLT_FLT,
											(dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
											(ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
	status |= OCIBindByPos(stmthp, &bnd24hp, errhp, 24,
											(dvoid *) &ele->v55011_3, sizeof(ele->v55011_3), SQLT_FLT,
											(dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
											(ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
	status |= OCIBindByPos(stmthp, &bnd25hp, errhp, 25,
											(dvoid *) &ele->v55011_4, sizeof(ele->v55011_4), SQLT_FLT,
											(dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
											(ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
	status |= OCIBindByPos(stmthp, &bnd26hp, errhp, 26,
											(dvoid *) &ele->v55012_1, sizeof(ele->v55012_1), SQLT_FLT,
											(dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
											(ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
	status |= OCIBindByPos(stmthp, &bnd27hp, errhp, 27,
											(dvoid *) &ele->v55012_2, sizeof(ele->v55012_2), SQLT_FLT,
											(dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
											(ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
	status |= OCIBindByPos(stmthp, &bnd28hp, errhp, 28,
											(dvoid *) &ele->v55012_3, sizeof(ele->v55012_3), SQLT_FLT,
											(dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
											(ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
	status |= OCIBindByPos(stmthp, &bnd29hp, errhp, 29,
											(dvoid *) &ele->v55012_4, sizeof(ele->v55012_4), SQLT_FLT,
											(dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
											(ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
	status |= OCIBindByPos(stmthp, &bnd30hp, errhp, 30,
											(dvoid *) &ele->v11224, sizeof(ele->v11224), SQLT_FLT,
											(dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
											(ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
	status |= OCIBindByPos(stmthp, &bnd31hp, errhp, 31,
											(dvoid *) &ele->v11225, sizeof(ele->v11225), SQLT_FLT,
											(dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
											(ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
  status |= OCIStmtExecute(svchp, stmthp, errhp, (ub4) 1, (ub4) 0,
                           (CONST OCISnapshot *) 0, (OCISnapshot *) 0, (ub4) OCI_DEFAULT);
  status |= OCITransCommit(svchp, errhp, 0);										
  if(status != OCI_SUCCESS)
  {
  		error_proc(errhp, status);
  		fprintf(fplog, "SQL = %s\n", (char *)statement);
  		fprintf(fplog, "error is: %s\n", oci_err);
  		
  		//printf("Tyname = %s\n,Hash = %s\n,CCCC = %s\n,Fname = %s\n,Ftime = %s\n,ITime = %s\n,TY_type = %s\n,TY_status = %s\n,TY_bjtime = %s\n,TY_utime = %s\n,v55002 = %d\n,v55003 = %d\n,,Vti = %d\n,v05001 = %d\n,v06001 = %d\n,V10004 = %d\n,,V11041 = %d\n,V55010 = %d\n,V55011 = %d\n,V11224 = %d\n,V11225 = %d\n",key->tyname,key->hash,key->cccc,key->fname, key->ftime, key->itime, key->ty_type,  key->ty_status, key->ty_bjtime,  key->ty_utime, &key->code_cn, &key->code_in,&ele->v05001, &ele->v06001, &ele->v10004, &ele->v11041, &ele->v55010, &ele->v55011, &ele->v11224, &ele->v11225);
  		fprintf(fplog, "Tyname = %s\n,Hash = %s\n,CCCC = %s\n,Fname = %s\n,Ftime = %s\n,ITime = %s\n,TY_type = %s\n,TY_status = %s\n,TY_bjtime = %s\n,TY_utime = %s\n,v55002 = %d\n,v55003 = %d\n,,Vti = %d\n,v05001 = %d\n,v06001 = %d\n,V10004 = %d\n,,V11041 = %d\n,V55010_1 = %d\n,V55010_2 = %d\n,V55010_3 = %d\n,V55010_4 = %d\n,V55011_1 = %d\n,V55011_2 = %d\n,V55011_3 = %d\n,V55011_4 = %d\n,V55012_1 = %d\n,V55012_2 = %d\n,V55012_3 = %d\n,V55012_4 = %d\n,V11224 = %d\n,V11225 = %d\n",key->tyname,key->hash,key->cccc,key->fname, key->ftime, key->itime, key->ty_type,  key->ty_status, key->ty_bjtime,  key->ty_utime, &key->code_cn, &key->code_in,&ele->v05001, &ele->v06001, &ele->v10004, &ele->v11041, &ele->v55010_1, &ele->v55010_2,&ele->v55010_3,&ele->v55010_4,&ele->v55011_1,&ele->v55011_2,&ele->v55011_3,&ele->v55011_4, &ele->v55012_1,&ele->v55012_2,&ele->v55012_3,&ele->v55012_4, &ele->v11224, &ele->v11225);
  		fflush(fplog);
  		
  		return -1;
  }
  
  return 0;
}

/***************************************************************
**
**		Function:		insert_ele_typh()
**		Description:	insert typh msg to observe.ele_typh_babj
**
****************************************************************/
int insert_ele_typh(TYPH_KEY *key, TYPH_ELE *ele)
{
	sword status;
	OCIBind *bnd1hp, *bnd2hp, *bnd3hp, *bnd4hp, *bnd5hp, *bnd6hp, *bnd7hp, *bnd8hp, *bnd9hp, *bnd10hp, 
					*bnd11hp, *bnd12hp, *bnd13hp, *bnd14hp, *bnd15hp, *bnd16hp, *bnd17hp, *bnd18hp, *bnd19hp;
	OCIBind *bndhp[30];
	int i=0, j=1;
	char statement[1000], fcdate[10], fctime[10];
	int  v_cccc = 138;
  
  memset(fcdate, '\0', sizeof(fcdate));
  memset(fctime, '\0', sizeof(fctime));
	strncpy(fcdate, key->ty_utime, 8);
	strncpy(fctime, key->ty_utime+8, 4);

	memset(statement, '\0', sizeof(statement));
	strcpy(statement, "delete forecast.ele_typh where C_CCCC=:1 and V01230=:2 and C_BJTIME=:3 and VTI=:4");
	status = OCIStmtPrepare(stmthp, errhp,
												statement , (ub4) strlen(statement),
												(ub4) OCI_NTV_SYNTAX, (ub4) OCI_DEFAULT);
	status |= OCIBindByPos(stmthp, &bnd1hp, errhp, 1,
											(dvoid *) key->cccc, strlen(key->cccc)+1, SQLT_AVC,
											(dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
											(ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
	status |= OCIBindByPos(stmthp, &bnd2hp, errhp, 2,
											(dvoid *) &key->code_cn, sizeof(key->code_cn), SQLT_INT,
											(dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
											(ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
	status |= OCIBindByPos(stmthp, &bnd3hp, errhp, 3,
											(dvoid *) key->ty_bjtime, strlen(key->ty_bjtime)+1, SQLT_AVC,
											(dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
											(ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
	status |= OCIBindByPos(stmthp, &bnd4hp, errhp, 4,
											(dvoid *) &ele->vti, sizeof(ele->vti), SQLT_INT,
											(dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
											(ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
  status |= OCIStmtExecute(svchp, stmthp, errhp, (ub4) 1, (ub4) 0,
                           (CONST OCISnapshot *) 0, (OCISnapshot *) 0, (ub4) OCI_DEFAULT);
  if(status != OCI_SUCCESS)
  {
  		error_proc(errhp, status);
  		fprintf(fplog, "SQL = %s\n", (char *)statement);
  		fprintf(fplog, "error is: %s\n", oci_err);
  		fflush(fplog);
  }

	memset(statement, '\0', sizeof(statement));
	strcpy(statement, "insert into forecast.ele_typh values(:1, :2, :3, :4, :5, :6, :7, :8, to_date(:9, 'yyyymmddhh24miss'), :10, :11, \
							:12, :13, :14, :15, 999999, :16, 45, :17, 135, :18, 225, :19, 315, :20, 45, :21, 135, :22, 225, :23, 315,\
							:24, 999999, :25, 999999, :26, 999999, :27, 999999, :28, :29, :30)");
	status = OCIStmtPrepare(stmthp, errhp,
												statement , (ub4) strlen(statement),
												(ub4) OCI_NTV_SYNTAX, (ub4) OCI_DEFAULT);
	status |= OCIBindByPos(stmthp, &bndhp[i++], errhp, j++,
                            (dvoid *)&v_cccc, (sword)sizeof(v_cccc), SQLT_INT, 
                            (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
                            (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT); 		
	status |= OCIBindByPos(stmthp, &bndhp[i++], errhp, j++,
											(dvoid *) key->cccc, strlen(key->cccc)+1, SQLT_AVC,
											(dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
											(ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
	status |= OCIBindByPos(stmthp, &bndhp[i++], errhp, j++,
											(dvoid *) key->tyname, strlen(key->tyname)+1, SQLT_AVC,
											(dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
											(ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
	status |= OCIBindByPos(stmthp, &bndhp[i++], errhp, j++,
											(dvoid *) &key->code_cn, sizeof(key->code_cn), SQLT_INT,
											(dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
											(ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
	status |= OCIBindByPos(stmthp, &bndhp[i++], errhp, j++,
											(dvoid *) &key->code_in, sizeof(key->code_in), SQLT_INT,
											(dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
											(ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
	status |= OCIBindByPos(stmthp, &bndhp[i++], errhp, j++,
											(dvoid *) key->ty_bjtime, strlen(key->ty_bjtime)+1, SQLT_AVC,
											(dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
											(ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
	status |= OCIBindByPos(stmthp, &bndhp[i++], errhp, j++,
											(dvoid *) key->ty_type, strlen(key->ty_type)+1, SQLT_AVC,
											(dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
											(ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
	status |= OCIBindByPos(stmthp, &bndhp[i++], errhp, j++,
											(dvoid *) key->ty_status, strlen(key->ty_status)+1, SQLT_AVC,
											(dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
											(ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
	status |= OCIBindByPos(stmthp, &bndhp[i++], errhp, j++,
											(dvoid *) key->itime, strlen(key->itime)+1, SQLT_AVC,
											(dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
											(ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
	status |= OCIBindByPos(stmthp, &bndhp[i++], errhp, j++,
											(dvoid *) fcdate, strlen(fcdate)+1, SQLT_AVC,
											(dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
											(ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
	status |= OCIBindByPos(stmthp, &bndhp[i++], errhp, j++,
											(dvoid *) fctime, strlen(fctime)+1, SQLT_AVC,
											(dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
											(ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
	status |= OCIBindByPos(stmthp, &bndhp[i++], errhp, j++,
											(dvoid *) &ele->vti, sizeof(ele->vti), SQLT_INT,
											(dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
											(ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
	status |= OCIBindByPos(stmthp, &bndhp[i++], errhp, j++,
											(dvoid *) &ele->v05001, sizeof(ele->v05001), SQLT_FLT,
											(dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
											(ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
	status |= OCIBindByPos(stmthp, &bndhp[i++], errhp, j++,
											(dvoid *) &ele->v06001, sizeof(ele->v06001), SQLT_FLT,
											(dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
											(ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
	status |= OCIBindByPos(stmthp, &bndhp[i++], errhp, j++,
											(dvoid *) &ele->v10004, sizeof(ele->v10004), SQLT_FLT,
											(dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
											(ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
	status |= OCIBindByPos(stmthp, &bndhp[i++], errhp, j++,
											(dvoid *) &ele->v11041, sizeof(ele->v11041), SQLT_FLT,
											(dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
											(ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
	status |= OCIBindByPos(stmthp, &bndhp[i++], errhp, j++,
											(dvoid *) &ele->v55010_1, sizeof(ele->v55010_1), SQLT_FLT,
											(dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
											(ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
	status |= OCIBindByPos(stmthp, &bndhp[i++], errhp, j++,
											(dvoid *) &ele->v55010_2, sizeof(ele->v55010_2), SQLT_FLT,
											(dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
											(ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
	status |= OCIBindByPos(stmthp, &bndhp[i++], errhp, j++,
											(dvoid *) &ele->v55010_3, sizeof(ele->v55010_3), SQLT_FLT,
											(dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
											(ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
	status |= OCIBindByPos(stmthp, &bndhp[i++], errhp, j++,
											(dvoid *) &ele->v55010_4, sizeof(ele->v55010_4), SQLT_FLT,
											(dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
											(ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
	status |= OCIBindByPos(stmthp, &bndhp[i++], errhp, j++,
											(dvoid *) &ele->v55011_1, sizeof(ele->v55011_1), SQLT_FLT,
											(dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
											(ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
	status |= OCIBindByPos(stmthp, &bndhp[i++], errhp, j++,
											(dvoid *) &ele->v55011_2, sizeof(ele->v55011_2), SQLT_FLT,
											(dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
											(ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
	status |= OCIBindByPos(stmthp, &bndhp[i++], errhp, j++,
											(dvoid *) &ele->v55011_3, sizeof(ele->v55011_3), SQLT_FLT,
											(dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
											(ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
	status |= OCIBindByPos(stmthp, &bndhp[i++], errhp, j++,
											(dvoid *) &ele->v55011_4, sizeof(ele->v55011_4), SQLT_FLT,
											(dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
											(ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
	status |= OCIBindByPos(stmthp, &bndhp[i++], errhp, j++,
											(dvoid *) &ele->v55012_1, sizeof(ele->v55011_1), SQLT_FLT,
											(dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
											(ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
	status |= OCIBindByPos(stmthp, &bndhp[i++], errhp, j++,
											(dvoid *) &ele->v55012_2, sizeof(ele->v55011_2), SQLT_FLT,
											(dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
											(ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
	status |= OCIBindByPos(stmthp, &bndhp[i++], errhp, j++,
											(dvoid *) &ele->v55012_3, sizeof(ele->v55011_3), SQLT_FLT,
											(dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
											(ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
	status |= OCIBindByPos(stmthp, &bndhp[i++], errhp, j++,
											(dvoid *) &ele->v55012_4, sizeof(ele->v55011_4), SQLT_FLT,
											(dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
											(ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
	status |= OCIBindByPos(stmthp, &bndhp[i++], errhp, j++,
											(dvoid *) &ele->v11224, sizeof(ele->v11224), SQLT_FLT,
											(dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
											(ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
	status |= OCIBindByPos(stmthp, &bndhp[i++], errhp, j++,
											(dvoid *) &ele->v11225, sizeof(ele->v11225), SQLT_FLT,
											(dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
											(ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
  status |= OCIStmtExecute(svchp, stmthp, errhp, (ub4) 1, (ub4) 0,
                           (CONST OCISnapshot *) 0, (OCISnapshot *) 0, (ub4) OCI_DEFAULT);
  status |= OCITransCommit(svchp, errhp, 0);										
  if(status != OCI_SUCCESS)
  {
  		error_proc(errhp, status);
  		fprintf(fplog, "SQL = %s\n", (char *)statement);
  		fprintf(fplog, "error is: %s\n", oci_err);
  		fflush(fplog);
  		return -1;
  }

  return 0;
}


/***************************************************************
**
**		Function:		update_typh_status()
**		Description:	update expire typh status to stop
**
****************************************************************/
int update_typh_status()
{
   	sword status;
	  int  i, typh_code, typh_list[10], typh_list_num;
	  char statement[500];
	  OCIDefine *def1hp;
	  OCIBind *bnd1hp;
	  
	  memset(statement, '\0', sizeof(statement));
	  strcpy(statement, "select v55002 from forecast.ele_typh_babj_new where ty_status = 'active' group by v55002 having max(ty_bjtime) < to_char(sysdate-13/24,'yyyymmddhh24mi')");
	  
    status = OCIStmtPrepare(stmthp, errhp, 
                            statement, (ub4) strlen(statement),
                            (ub4) OCI_NTV_SYNTAX, (ub4) OCI_DEFAULT);
    status |= OCIDefineByPos(stmthp, &def1hp, errhp, 1, 
                             (ub1 *)&typh_code, (sb4) sizeof(int), SQLT_INT, 
                             (dvoid *) 0, (ub2 *) 0, (ub2 *) 0, (ub4) OCI_DEFAULT);
    status |= OCIStmtExecute(svchp, stmthp, errhp, (ub4) 0, (ub4) 0,
                              NULL, NULL, (ub4) OCI_DEFAULT);
    if (status != OCI_SUCCESS)
    {
        error_proc(errhp, status);
  		  fprintf(fplog, "SQL = %s\n", (char *)statement);
  		  fprintf(fplog, "error is: %s\n", oci_err);
  		  fflush(fplog);
        return -1;
    }

    typh_list_num = 0;
    while ((status = OCIStmtFetch(stmthp, errhp, 1, OCI_FETCH_NEXT, OCI_DEFAULT)) != OCI_NO_DATA)
    {
           	  typh_list[typh_list_num++] = typh_code;
       }
	  
	  for (i=0; i<typh_list_num; i++)
	  {
        memset(statement, '\0', sizeof(statement));
	      strcpy(statement, "update forecast.ele_typh_babj_new set ty_status='stop' where v55002=:1 and ty_status='active'");
        

        status = OCIStmtPrepare(stmthp, errhp, 
                                statement, (ub4) strlen(statement),
                                (ub4) OCI_NTV_SYNTAX, (ub4) OCI_DEFAULT);
	      status |= OCIBindByPos(stmthp, &bnd1hp, errhp, 1,
				         							 (dvoid *) &typh_list[i], sizeof(int), SQLT_INT,
									        		 (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
											         (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
       

       status |= OCIStmtExecute(svchp, stmthp, errhp, (ub4) 1, (ub4) 0,
                                 (CONST OCISnapshot *) 0, (OCISnapshot *) 0, (ub4) OCI_DEFAULT);
        if(status != OCI_SUCCESS)
        {
  		      error_proc(errhp, status);
  		      fprintf(fplog, "SQL = %s\n", (char *)statement);
  		      fprintf(fplog, "error is: %s\n", oci_err);
  		      fflush(fplog);
        }
         
	        

	      memset(statement, '\0', sizeof(statement));
	      strcpy(statement, "update forecast.ele_typh set c_status='stop' where v01230=:1 and c_status='active'");
        
        status = OCIStmtPrepare(stmthp, errhp, 
                                statement, (ub4) strlen(statement),
                                (ub4) OCI_NTV_SYNTAX, (ub4) OCI_DEFAULT);
	      status |= OCIBindByPos(stmthp, &bnd1hp, errhp, 1,
				         							 (dvoid *) &typh_list[i], sizeof(int), SQLT_INT,
									        		 (dvoid *) 0, (ub2 *) 0, (ub2 *) 0,
											         (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT);
        status |= OCIStmtExecute(svchp, stmthp, errhp, (ub4) 1, (ub4) 0,
                                 (CONST OCISnapshot *) 0, (OCISnapshot *) 0, (ub4) OCI_DEFAULT);
        if(status != OCI_SUCCESS)
        {
  		      error_proc(errhp, status);
  		      fprintf(fplog, "SQL = %s\n", (char *)statement);
  		      fprintf(fplog, "error is: %s\n", oci_err);
  		      fflush(fplog);
        }
	  }
	  
	  return 1;
}
