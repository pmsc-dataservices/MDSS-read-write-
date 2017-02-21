/************************************************************
 * Following definitions and declarations are used for common  
 * process programms which are written in rdb_c_dq_base.pc.     
 ************************************************************/
/*
#include <sqlcpr.h>
#include <sqlda.h>
#include <sqlca.h>
*/
/*
* define data query error code
*/

#define	RDB_DQ_ERROR_QUERY_DATA 2501
#define RDB_DQ_PROC_SQL_ERROR 2502
#define RDB_DQ_TOO_MANY_SELECT_ITEMS 2503
#define RDB_DQ_ALLOC_DP_ERROR 2504
#define	RDB_DQ_ARRAY_TOO_SMALL_TINFO 2505
#define RDB_DQ_ARRAY_TOO_SMALL_CDATA 2506
#define RDB_DQ_ARRAY_TOO_SMALL_FDATA 2507
#define RDB_DQ_ARRAY_TOO_SMALL_CIDEN 2508
#define RDB_DQ_ARRAY_TOO_SMALL_FIDEN 2509
/* add 2005-06-16 */
#define RDB_DQ_ARRAY_TOO_SMALL_LEVELSQL 2510
/* end */

/* Add 2005-05-18 */
#define RDB_DQ_ONESQL_TOO_LONG 2511
#define RDB_DQ_SQL_TOO_LONG 2512
#define RDB_DQ_GET_FIELD_ERROR 2513
#define RDB_DQ_ADDITIONAL_CONDITION_TOO_LONG 2514
#define RDB_DQ_OPEN_RSTFILE_ERROR 2515
#define RDB_DQ_SPLIT_STRING_ERROR 2516
/* end */

/* add 2005-06-02 */
#define RDB_DQ_CHECK_MODEL_ALIAS_ERROR 2517
/* end */

#define RDB_DQ_ARGUMENT_ERROR_COM 2600
#define RDB_DQ_ARGUMENT_ERROR_DATE 2601
#define RDB_DQ_ARGUMENT_ERROR_TIME 2602
#define RDB_DQ_ARGUMENT_ERROR_AREA 2603
#define RDB_DQ_ARGUMENT_ERROR_SQLSTR 2604
#define RDB_DQ_CRST_LENGTH_SMALL 2605
#define RDB_DQ_CRST_WIDTH_SMALL 2606
#define RDB_DQ_FRST_LENGTH_SMALL 2607
#define RDB_DQ_ARGUMENT_ERROR_SERFLAG 2608

/* add 2005-05-18 */
#define RDB_DQ_ARGUMENT_ERROR_CROP 2609
#define RDB_DQ_ARGUMENT_ERROR_MODEL 2610
#define RDB_DQ_ARGUMENT_ERROR_CPAR 2611
#define RDB_DQ_ARGUMENT_ERROR_LPAR 2612
/* end */

/* add 2005-05-23 */
#define RDB_DQ_ARGUMENT_ERROR_USRINPUTITEMC 2613
#define RDB_DQ_ARGUMENT_ERROR_USRINPUTITEMF 2614
#define RDB_DQ_ARGUMENT_ERROR_SHIP 2615
#define RDB_DQ_ARGUMENT_ERROR_PART 2616
/* end */

/* add 2005-06-02 */
#define RDB_DQ_ARGUMENT_ERROR_MODELALIAS 2617
/* end */

/* add 2006-06-14*/
#define RDB_DQ_ARGUMENT_ERROR_LEVEL 2618
#define RDB_DQ_ARGUMENT_ERROR_NORETRIEVELEVEL 2619
/* end */

/* add 2006-03-24 */
#define RDB_DQ_ERROR_BFILE_NOTOPEN 2620

/* add liuyy 2006-04-10 */
#define RDB_DQ_ARGUMENT_ERROR_CCCC 2621
#define RDB_DQ_ARGUMENT_ERROR_FIELDTYPE 2622
#define RDB_DQ_ARGUMENT_ERROR_VALIDTIME 2623
#define RDB_DQ_ARGUMENT_ERROR_AREACODE 2624
#define RDB_DQ_ARGUMENT_ERROR_PATH_IGNORED 2625
#define RDB_DQ_ARGUMENT_ERROR_ELEMENT 2626

#define RDB_DQ_ARGUMENT_ERROR_TYNUMBER 2627 
/* liuyy add 2006-12-04 */
#define RDB_DQ_MALLOC_ERROR 2628
#define RDB_DQ_ARGUMENT_ERROR_IDBSERMODEL 2629
#define RDB_DQ_IDBSERMODEL_ERROR 2630
#define RDB_DQ_C_AUTH_ERROR 2631
#define RDB_DQ_NORIGHT_ERROR 2632

#define CF_LEN          2048    /*liuyy change Maximum length of char type element in array*/
#define STR_LEN         2048   /* Maximum length of string */
#define TINFO_LEN       1000   /* Maximum length of array in TABLEINFO*/ 
#define MAX_SQL_LEN     40000  /* Maximum length of sql statement */
/* Add 2005-05-18 */
#define MAX_ONESQL_LEN  1500   /* Maximum length of one sql statement */
/* end */

/* add liuyy 060410 */
#define MAX_FILE_LENGTH 100000000
/*
* Following definition used for AllocDescriptor()
*/
/*
* begin ---------------------------------------
*/
#define MAX_ITEMS       150    /* Maximum number of select-list items. */
#define MAX_VNAME_LEN   30     /* Maximum length of the _names_ of the select-list items. */
#define MAX_INAME_LEN   30     /* Maximum length of the _names_ of indicator variables. */
/*
* end -----------------------------------------
*/

/*
* Following definition used for ProcSelectList()
*/
/*
* begin ---------------------------------------
*/
#define MAX_REP_LEN     20000
#define MAX_FLOAT_LEN   10
/*
* end -----------------------------------------
*/

/*
* Following definition used for rdb_dq_c_model.c
*/
/*
* begin ---------------------------------------
*/
#define PARA_MAX_NUM 10
#define PARA_MAX_WIDE 10
#define PARA_SEP ","
/*
* end -----------------------------------------
*/

/* add on 2005-10-11 must be rdb_insert */
#define TABLE_OWNER "rdb_insert" 
/*
#define TABLE_OWNER "rdb_test" 
*/
/* end */

/*
* Struct defition
*/
#define USERINFO struct userinfo
#define TABLEINFO struct tableinfo

/*
* storing user action information
*/
USERINFO
{
   int	action_no;            /* action sequence */
   char username[21];         /* user name */
   int  userid;               /* user id */
   char user_ip[21];          /* user IP address */
   int  type;                 /* query mode, 1:B/S, 2:C/S, 3:TUXEDO */
   char dsid[41];             /* data name */
   char sqlstr[MAX_SQL_LEN];  /* sql statement */
   int  quantity;             /* query quantity, the unit is kb */
   char	starttime[STR_LEN];   /* query start time */
   char endtime[STR_LEN];     /* query end time, BS:null, CS:endtime */
   int 	db_sign;              /* database signal, 1:RDB, 2:IDB */
}; 

/*
* storing query table infomation 
*/
TABLEINFO
{
    char EleTable[TINFO_LEN][100];     /* the element tables information */
    char KeyTable[TINFO_LEN][100];     /* the key tables information */
    char dictionary[100];              /* the directory information */
    int EleNum;                       /* the number of array element */
};

/*
* storing authentication information
*/
typedef struct
{ 
    int   is_success;
    char  db_url[50];
    char  db_user[50];
    char  db_pass[50];
    char  role_type;
}auth_info;

/*
* Declare a pointer to select descriptor.
*/
/*
SQLDA *select_dp;
*/

