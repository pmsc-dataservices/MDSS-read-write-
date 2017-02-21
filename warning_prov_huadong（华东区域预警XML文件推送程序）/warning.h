
/******the structure of warning ***************/
typedef struct
{
  char  alertid[50];         /* 1. 预警信号标识     */
  char  province[60];        /* 2. 省份 */
  char  city[110];           /* 3. 地市 */
  char  stationname[40];     /* 4. 台站 */ 
  int   stationid;           /* 5. 站号 */ 
  char  signaltype[30];      /* 6. 信号类别 */ 
  char  signallevel[30];     /* 7. 信号等级 */ 
  char  issuecontent[4000];  /* 8. 发布内容 */ 
  char  recoverychannel[500];/* 9. 发布管道 */ 
  char  dstandardtune[30];   /* 10. 达到标准时间 */
  char  relivetime[30];      /* 11. 解除时间 */
  char  tnumber[50];         /* 12. 天气过程编号 */
  char  underwriter[30];     /* 13. 签发人 */
  char  change[2];           /* 14. 更改标记 */
}WARNING;

FILE *fplog;
char db_err_info[2000];
char oci_err[1000];
