char db_err_info[2000];
FILE *fplog;
FILE   *fp;
#define MAX_STATION 3000
#define MAX_INDEXFILE 200

char INDEXfile[MAX_STATION][1000];
int stringtrim(char *string);
int stringsplit(char *string, char split, int number, char newstr[][100]);
int numtime(int *ntime,time_t btime);
int IsProcessExist(char *ProcName);
int file_size(char *filename);
int date_add(char *old_date, char *new_date, int num);
int read_pm_record25(char *data_dir,char *data_file,char  *date,int count);
int read_pm_record10(char *data_dir,char *data_file,char  *date,int count);
int read_o3_record(char *data_dir,char *data_file,char  *date);
int read_mul_record(char *data_dir,char *data_file1,char *data_file2,char *data_file3,char  *date);
int read_o3(char *data_dir,char *data_file,float value[]);
int read_58448(char *data_dir,char *data_file1,char *data_file2,char *data_file3,char *data_file4,float value[]);
int read_54421(char *data_dir,char *data_file1,char *data_file2,char *data_file3,char *data_file4,float value[]);
int read_51058(char *data_dir,char *data_file1,char *data_file2,char *data_file3,char *data_file4,float value[]);
int read_54084(char *data_dir,char *data_file1,char *data_file2,char *data_file3,char *data_file4,float value[]);




//for temp Ì½¿Õ
