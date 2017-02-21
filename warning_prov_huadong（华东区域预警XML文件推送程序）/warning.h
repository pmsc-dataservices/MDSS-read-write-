
/******the structure of warning ***************/
typedef struct
{
  char  alertid[50];         /* 1. Ԥ���źű�ʶ     */
  char  province[60];        /* 2. ʡ�� */
  char  city[110];           /* 3. ���� */
  char  stationname[40];     /* 4. ̨վ */ 
  int   stationid;           /* 5. վ�� */ 
  char  signaltype[30];      /* 6. �ź���� */ 
  char  signallevel[30];     /* 7. �źŵȼ� */ 
  char  issuecontent[4000];  /* 8. �������� */ 
  char  recoverychannel[500];/* 9. �����ܵ� */ 
  char  dstandardtune[30];   /* 10. �ﵽ��׼ʱ�� */
  char  relivetime[30];      /* 11. ���ʱ�� */
  char  tnumber[50];         /* 12. �������̱�� */
  char  underwriter[30];     /* 13. ǩ���� */
  char  change[2];           /* 14. ���ı�� */
}WARNING;

FILE *fplog;
char db_err_info[2000];
char oci_err[1000];
