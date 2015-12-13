

#include <sqlite3.h>
extern void unpack_data( char *,struct client_info *);
extern void insert_task(int ,struct client_info *,char *);
extern int judge_sqlite(struct client_info *client,sqlite3 *db);
extern int call_back_usr(int fd,char *msg);
extern int anl_data(struct client_info *client,sqlite3 *db);
extern void get_client_info_from_sqlite3(char *table_head,char *goal_data,struct client_info *umsg);
extern int sqlite_callback(void *data,int argc,char **argv,char **azColName);
extern void create_table(char *filename,sqlite3 *db,char *table);
extern int insert_data(sqlite3 *db,struct client_info *umsg,char *table,char *filname);
extern void delete_data(sqlite3 *db,char *table);
extern void upata_data(sqlite3 *db,char *table,struct client_info *umsg);
extern void show_data(sqlite3 *db,char *table,char *filname);
extern void search_by_type(char *table,sqlite3 *db,int *fd,char *ID,struct client_info *umsg,char *filname);
extern void quit(sqlite3 *db);
extern void delete_all(sqlite3 *db,char *table);
