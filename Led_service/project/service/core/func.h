#include <unistd.h>
#include <stdio.h>
#include <sqlite3.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <stdlib.h>

#define SERV_PORT          9000   //server�����˿�
#define LOG_IN 0  //����
#define LOG_UP 1  //ע��
#define LINK   3  //����
#define DISCONNECT 4 //�Ͽ�����
#define SET_DATA 5 //���豸��������
#define OK "ok!"
#define FAILU "failu!"
#define LOGUP_CUN 0  //�Ѿ�ע��
#define LOGUP_OK  1  //ע��ɹ�
#define LOGUP_FAI 2  //ע��ʧ��
#define SET_OK    3  //���óɹ�
#define SET_FAL   4  //����ʧ��



struct client_info{
	char cli_ip_addr[16];
	struct sockaddr_in cin;
	int newfd;

	char name[20];
	char ID[20];
	char phonenumber[15];
	char sex;//  v ����Ů�ԣ�o��������
	int flag; // 1 �����豸��0�����û�
	int info_type;
	int led;//1���,0���	

};



extern void unpack_data( char *,struct client_info *);
extern void insert_task(int ,struct client_info *,char *);
extern int judge_sqlite(struct client_info *client);
extern int call_back_usr(int fd,char *msg);
extern int anl_data(struct client_info *client);
extern void get_client_info_from_sqlite3(char *table_head,char *goal_data,struct client_info *umsg);
extern int sqlite_callback(void *data,int argc,char **argv,char **azColName);
extern void create_table(char *filename,sqlite3 *db,char *table);
extern int insert_data(sqlite3 *db,struct client_info *umsg,char *table);
extern void delete_data(sqlite3 *db,char *table);
extern void upata_data(sqlite3 *db,char *table,struct client_info *umsg);
extern void show_data(sqlite3 *db,char *table);
extern void search_by_type(char *table,sqlite3 *db,int *fd,char *ID,struct client_info *umsg);
extern void quit(sqlite3 *db);
extern void delete_all(sqlite3 *db,char *table);
