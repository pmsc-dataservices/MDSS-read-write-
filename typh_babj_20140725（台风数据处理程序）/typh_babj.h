#define DEFAULT_VALUE 999999
#define MAX_FILE_NUM 200
#define MAX_REP_NUM 100
#define MAX_VTI_NUM 10

char  TYPH_FILE[MAX_FILE_NUM][100];

typedef struct
{
	 int   vti;      /* Ԥ��ʱЧ         */
   float v05001;   /*	����γ��         */
   float v06001;	 /* ���ľ���         */
   float v11041;   /*	���ĸ��������� */
   float v10004;   /* ������ѹ         */
    float v55010_1;   /*	��7 ���綫���뾶      */
   float v55010_2;    /*	��7 ���綫�ϰ뾶      */
   float v55010_3;    /*	��7 ���������뾶      */
   float v55010_4;    /*	��7 �������ϰ뾶      */
   float v55011_1;   /* ��10 ���綫���뾶     */
   float v55011_2;   /*	��10 ���綫�ϰ뾶      */
   float v55011_3;   /*	��10 ���������뾶      */
   float v55011_4;   /*	��10 �������ϰ뾶      */
   float v55012_1;   /* ��10 ���綫���뾶     */
   float v55012_2;   /*	��10 ���綫�ϰ뾶      */
   float v55012_3;   /*	��10 ���������뾶      */
   float v55012_4;   /*	��10 �������ϰ뾶      */

   float v11224;   /*	δ������(12ʱЧ) */
   float v11225;   /* δ������(12ʱЧ) */
}TYPH_ELE;

typedef struct
{
	char tyname[20];    /* ̨����                     */
	char hash[41];      /* hash(������ʾ�ļ�Ψһ)     */
	char cccc[10];       /* ��������                   */
	char fname[100];    /* ̨�������ļ�               */
	char ftime[20];     /* ̨���ļ��״�ʱ��           */
	char itime[20];     /* ̨���ļ��״����ʱ��       */
	char ty_type[10];   /* ̨�缶��                   */
	char ty_status[10];  /* ̨��״̬                   */
	char ty_bjtime[20]; /* ̨�籱��ʱ��               */
	char ty_utime[20];  /* ̨�����ʱ��               */
	int  code_cn;       /* ���ڱ��                   */
	int  code_in;       /* ���ʱ��                   */
	int  vti_num;
	TYPH_ELE ele[MAX_VTI_NUM];
}TYPH_KEY;


FILE *fplog;
