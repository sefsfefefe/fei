
#ifndef __FUNC_H_
#define __FUNC_H_
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
#define LOG_IN 1  //����
#define LOG_UP 2  //ע��
#define LINK   3  //����
#define DISCONNECT 4 //�Ͽ�����
#define SET_DATA 5 //���豸��������
#define OK "ok!"
#define FAILU "failu!"
#define LOGUP_CUN "1"  //�Ѿ�ע��
#define LOGUP_OK  "2"  //ע��ɹ�
#define LOGUP_FAI "3"  //ע��ʧ��
#define SET_OK    "4"  //���óɹ�
#define SET_FAL   "5"  //����ʧ��
#define FILNAME		"led.db"
#define TABLE     "LEDSQLITE"




struct client_info{
	char cli_ip_addr[16];
	struct sockaddr_in cin;
	int newfd;

	char name[20];
	char ID[20];
	char phonenumber[15];
	char *sex;//  v ����Ů�ԣ�o��������
	int flag; // 1 �����豸��0�����û�
	int info_type;
	int led;//1���,0���	

};
#endif




