#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <oci.h>
#include "warning.h"

OCIEnv         *envhp;
OCIError       *errhp;
OCIServer      *srvhp;
OCISvcCtx      *svchp;
OCISession     *authp;
OCIStmt        *stmthp;

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

int	read_warning_province(char *para_time)
{
	 sword status;
	 int ret = 0;
   OCIDefine *def1hp,*def2hp,*def3hp,*def4hp,*def5hp,*def6hp,*def7hp,*def8hp,*def9hp;
   OCIDefine *def10hp,*def11hp,*def12hp,*def13hp,*def14hp,*def15hp;
   char  v_date[20];
   float v_val=0;
   
   char  alertid[50];         /* 1. 预警信号标识     */
   char  province[60];        /* 2. 省份 */
   char  city[110];           /* 3. 地市 */
   char  stationname[40];     /* 4. 台站 */ 
   int   stationid;           /* 5. 站号 */ 
   char  signaltype[30];      /* 6. 信号类别 */ 
   char  signallevel[30];     /* 7. 信号等级 */ 
   char  issuetime[30];       /* 8. 发布时间 */ 
   char  issuecontent[4000];  /* 8. 发布内容 */ 
   char  recoverychannel[500];/* 9. 发布管道 */ 
   char  dstandardtune[30];   /* 10. 达到标准时间 */
   char  relievetime[30];      /* 11.解除时间 */
   char  tnumber[50];         /* 12. 天气过程编号 */
   char  underwriter[30];     /* 13. 签发人 */
   char  change[3];           /* 14. 更改标记 */   
   
   FILE *datafp;
   char *filename[255];
  
	 char select_statement[2000];
   memset(select_statement, '\0', sizeof(select_statement));

   sprintf(select_statement, "select alertid,province,city,stationname,stationid,signaltype,signallevel,trim(to_char(issuetime)),issuecontent,recoverychannel,trim(to_char(dstandardtune)),trim(to_char(relievetime)),tnumber, underwriter,trim(to_char(change)) from base.warning_province_data where issuetime between %s000000 and %s999999 and province in ('安徽省','江西省','河南省','湖北省','湖南省','重庆市','四川省')", para_time, para_time);
 
	 status = OCIStmtPrepare(stmthp, errhp, 
                            select_statement, (ub4) strlen(select_statement),
                            (ub4) OCI_NTV_SYNTAX, (ub4) OCI_DEFAULT);

	 status |= OCIDefineByPos(stmthp, &def1hp, errhp, 1, 
                             (ub1 *)alertid, 40, SQLT_CHR, 
                             (dvoid *) 0, (ub2 *) 0, (ub2 *) 0, (ub4) OCI_DEFAULT);
	 status |= OCIDefineByPos(stmthp, &def2hp, errhp, 2, 
                             (ub1 *)province, 50, SQLT_CHR, 
                             (dvoid *) 0, (ub2 *) 0, (ub2 *) 0, (ub4) OCI_DEFAULT);
	 status |= OCIDefineByPos(stmthp, &def3hp, errhp, 3, 
                             (ub1 *)city, 100, SQLT_CHR, 
                             (dvoid *) 0, (ub2 *) 0, (ub2 *) 0, (ub4) OCI_DEFAULT);
	 status |= OCIDefineByPos(stmthp, &def4hp, errhp, 4, 
                             (ub1 *)stationname, 30, SQLT_CHR, 
                             (dvoid *) 0, (ub2 *) 0, (ub2 *) 0, (ub4) OCI_DEFAULT);
	 status |= OCIDefineByPos(stmthp, &def5hp, errhp, 5, 
                             (ub1 *)&stationid, (sb4)sizeof(stationid), SQLT_INT, 
                             (dvoid *) 0, (ub2 *) 0, (ub2 *) 0, (ub4) OCI_DEFAULT);
	 status |= OCIDefineByPos(stmthp, &def6hp, errhp, 6, 
                             (ub1 *)signaltype, 20, SQLT_CHR, 
                             (dvoid *) 0, (ub2 *) 0, (ub2 *) 0, (ub4) OCI_DEFAULT);
	 status |= OCIDefineByPos(stmthp, &def7hp, errhp, 7, 
                             (ub1 *)signallevel, 20, SQLT_CHR, 
                             (dvoid *) 0, (ub2 *) 0, (ub2 *) 0, (ub4) OCI_DEFAULT);
	 status |= OCIDefineByPos(stmthp, &def8hp, errhp, 8, 
                             (ub1 *)issuetime, 20, SQLT_CHR, 
                             (dvoid *) 0, (ub2 *) 0, (ub2 *) 0, (ub4) OCI_DEFAULT);
	 status |= OCIDefineByPos(stmthp, &def9hp, errhp, 9, 
                             (ub1 *)issuecontent, 4000, SQLT_STR, 
                             (dvoid *) 0, (ub2 *) 0, (ub2 *) 0, (ub4) OCI_DEFAULT);
	 status |= OCIDefineByPos(stmthp, &def10hp, errhp, 10, 
                             (ub1 *)recoverychannel, 500, SQLT_CHR, 
                             (dvoid *) 0, (ub2 *) 0, (ub2 *) 0, (ub4) OCI_DEFAULT);
	 status |= OCIDefineByPos(stmthp, &def11hp, errhp, 11, 
                             (ub1 *)dstandardtune, 20, SQLT_CHR, 
                             (dvoid *) 0, (ub2 *) 0, (ub2 *) 0, (ub4) OCI_DEFAULT);
	 status |= OCIDefineByPos(stmthp, &def12hp, errhp, 12, 
                             (ub1 *)relievetime, 20, SQLT_CHR, 
                             (dvoid *) 0, (ub2 *) 0, (ub2 *) 0, (ub4) OCI_DEFAULT);
	 status |= OCIDefineByPos(stmthp, &def13hp, errhp, 13, 
                             (ub1 *)tnumber, 20, SQLT_CHR, 
                             (dvoid *) 0, (ub2 *) 0, (ub2 *) 0, (ub4) OCI_DEFAULT);
	 status |= OCIDefineByPos(stmthp, &def14hp, errhp, 14, 
                             (ub1 *)underwriter, 20, SQLT_CHR, 
                             (dvoid *) 0, (ub2 *) 0, (ub2 *) 0, (ub4) OCI_DEFAULT);
	 status |= OCIDefineByPos(stmthp, &def15hp, errhp, 15, 
                             (ub1 *)change, 2, SQLT_STR, 
                             (dvoid *) 0, (ub2 *) 0, (ub2 *) 0, (ub4) OCI_DEFAULT);

	 status |= OCIStmtExecute(svchp, stmthp, errhp, (ub4) 0, (ub4) 0,
                              NULL, NULL, (ub4) OCI_DEFAULT);

    if (status != OCI_SUCCESS)
    {
       error_proc(errhp, status);
       memset(db_err_info, '\0', sizeof(db_err_info));
       sprintf(db_err_info, "SQL = %s\n%s", (char *)select_statement, oci_err);
       printf("%s\n", db_err_info);
       fprintf(fplog, "%s\n", db_err_info);
       fflush(fplog);
       ret = 0;
    }
   printf("#####SQL = %s#####\n",select_statement);


   memset(alertid,'\0',sizeof(alertid));
   memset(province,'\0',sizeof(province));
   memset(city,'\0',sizeof(city));
   memset(stationname,'\0',sizeof(stationname));
   stationid = 0;
   memset(signaltype,'\0',sizeof(signaltype));
   memset(signallevel,'\0',sizeof(signallevel));
   memset(issuetime,'\0',sizeof(issuetime));
   memset(issuecontent,'\0',sizeof(issuecontent));
   memset(recoverychannel,'\0',sizeof(recoverychannel));
   memset(dstandardtune,'\0',sizeof(dstandardtune));
   memset(relievetime,'\0',sizeof(relievetime));
   memset(tnumber,'\0',sizeof(tnumber));
   memset(underwriter,'\0',sizeof(underwriter));
   memset(change,'\0',sizeof(change));

   /* create data file */
   memset(filename, '\0', sizeof(filename));
   sprintf(filename, "%s/data/warnging_prov_huadong/MSP2_PWSC_WFDWS_ME_L88_CCN_%s_00000-02400.xml", getenv("HOME"), para_time); 
   if((datafp=fopen(filename,"w+"))==NULL)
   {
        fprintf(fplog, "file %s can not open ... \n", filename);
        fflush(fplog);
        return -1;
   }
   
   fprintf(datafp, "<?xml version=%c%s%c encoding=%c%s%c?>\n", '"', "1.0", '"', '"', "gb2312",'"');
   fprintf(datafp, "<dzsponbusalerts type=%c%s%c>\n", '"', "array", '"');
    
    	 int i_count = 0;
   while ((status = OCIStmtFetch(stmthp, errhp, 1, OCI_FETCH_NEXT, OCI_DEFAULT)) != OCI_NO_DATA)
	 {
        //printf("fengyuxing111\n");
       stringrtrim(alertid);
       stringrtrim(province);
       stringrtrim(city);
       stringrtrim(stationname);
       stringrtrim(signaltype);
       stringrtrim(signallevel);
       stringrtrim(issuetime);
       stringrtrim(issuecontent);
       stringrtrim(recoverychannel);
       stringrtrim(dstandardtune);
       stringrtrim(relievetime);
       stringrtrim(tnumber);
       stringrtrim(underwriter);
       stringrtrim(change);
	 	    // printf("%s\n",alertid);
	 	   fprintf(datafp, "  <dzsponbusalert>\n");
       fprintf(datafp, "    <alertid>%s</alertid>\n", alertid);
       fprintf(datafp, "    <province>%s</province>\n", province);
       fprintf(datafp, "    <city>%s</city>\n", city);
       fprintf(datafp, "    <stationname>%s</stationname>\n", stationname);
       fprintf(datafp, "    <stationid>%d</stationid>\n", stationid);
       fprintf(datafp, "    <signaltype>%s</signaltype>\n", signaltype);
       fprintf(datafp, "    <signallevel>%s</signallevel>\n", signallevel);
       fprintf(datafp, "    <issuetime>%s</issuetime>\n", issuetime);
       fprintf(datafp, "    <issuecontent>%s</issuecontent>\n", issuecontent);
       fprintf(datafp, "    <recoverychannel>%s</recoverychannel>\n", recoverychannel);
       fprintf(datafp, "    <dstandardtune>%s</dstandardtune>\n", dstandardtune);
       fprintf(datafp, "    <relievetime>%s</relievetime>\n", relievetime);
       fprintf(datafp, "    <tnumber>%s</tnumber>\n", tnumber);
       fprintf(datafp, "    <underwriter>%s</underwriter>\n", underwriter);
       fprintf(datafp, "    <change>%s</change>\n", change);
       fprintf(datafp, "  </dzsponbusalert>\n");
       fflush(datafp);
		   
		   
       memset(alertid,'\0',sizeof(alertid));
       memset(province,'\0',sizeof(province));
       memset(city,'\0',sizeof(city));
       memset(stationname,'\0',sizeof(stationname));
       stationid = 0;
       memset(signaltype,'\0',sizeof(signaltype));
       memset(signallevel,'\0',sizeof(signallevel));
       memset(issuetime,'\0',sizeof(issuetime));
       memset(issuecontent,'\0',sizeof(issuecontent));
       memset(recoverychannel,'\0',sizeof(recoverychannel));
       memset(dstandardtune,'\0',sizeof(dstandardtune));
       memset(relievetime,'\0',sizeof(relievetime));
       memset(tnumber,'\0',sizeof(tnumber));
       memset(underwriter,'\0',sizeof(underwriter));
       memset(change,'\0',sizeof(change));
       
       i_count++;
	 }

   fprintf(datafp, "</dzsponbusalerts>\n");
	 

	ret = i_count;
  return ret;
}
