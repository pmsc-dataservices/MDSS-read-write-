#define DEFAULT_VALUE 999999
#define MAX_FILE_NUM 200
#define MAX_REP_NUM 100
#define MAX_VTI_NUM 10

char  TYPH_FILE[MAX_FILE_NUM][100];

typedef struct
{
	 int   vti;      /* 预报时效         */
   float v05001;   /*	中心纬度         */
   float v06001;	 /* 中心经度         */
   float v11041;   /*	中心附近最大风速 */
   float v10004;   /* 中心气压         */
    float v55010_1;   /*	≥7 级风东北半径      */
   float v55010_2;    /*	≥7 级风东南半径      */
   float v55010_3;    /*	≥7 级风西北半径      */
   float v55010_4;    /*	≥7 级风西南半径      */
   float v55011_1;   /* ≥10 级风东北半径     */
   float v55011_2;   /*	≥10 级风东南半径      */
   float v55011_3;   /*	≥10 级风西北半径      */
   float v55011_4;   /*	≥10 级风西南半径      */
   float v55012_1;   /* ≥10 级风东北半径     */
   float v55012_2;   /*	≥10 级风东南半径      */
   float v55012_3;   /*	≥10 级风西北半径      */
   float v55012_4;   /*	≥10 级风西南半径      */

   float v11224;   /*	未来移向(12时效) */
   float v11225;   /* 未来移速(12时效) */
}TYPH_ELE;

typedef struct
{
	char tyname[20];    /* 台风名                     */
	char hash[41];      /* hash(用来表示文件唯一)     */
	char cccc[10];       /* 发报中心                   */
	char fname[100];    /* 台风数据文件               */
	char ftime[20];     /* 台风文件首次时间           */
	char itime[20];     /* 台风文件首次入库时间       */
	char ty_type[10];   /* 台风级别                   */
	char ty_status[10];  /* 台风状态                   */
	char ty_bjtime[20]; /* 台风北京时间               */
	char ty_utime[20];  /* 台风国际时间               */
	int  code_cn;       /* 国内编号                   */
	int  code_in;       /* 国际编号                   */
	int  vti_num;
	TYPH_ELE ele[MAX_VTI_NUM];
}TYPH_KEY;


FILE *fplog;
