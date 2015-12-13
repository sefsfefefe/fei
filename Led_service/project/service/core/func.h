
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

#define SERV_PORT          9000   //server监听端口
#define LOG_IN 1  //登入
#define LOG_UP 2  //注册
#define LINK   3  //链接
#define DISCONNECT 4 //断开链接
#define SET_DATA 5 //给设备发送数据
#define OK "ok!"
#define FAILU "failu!"
#define LOGUP_CUN "1"  //已经注册
#define LOGUP_OK  "2"  //注册成功
#define LOGUP_FAI "3"  //注册失败
#define SET_OK    "4"  //设置成功
#define SET_FAL   "5"  //设置失败
#define FILNAME		"led.db"
#define TABLE     "LEDSQLITE"




struct client_info{
	char cli_ip_addr[16];
	struct sockaddr_in cin;
	int newfd;

	char name[20];
	char ID[20];
	char phonenumber[15];
	char *sex;//  v 代表女性，o代表男性
	int flag; // 1 代表设备，0代表用户
	int info_type;
	int led;//1点灯,0灭灯	

};
#endif




