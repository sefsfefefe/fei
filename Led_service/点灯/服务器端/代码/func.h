


#define SERV_PORT          9000   //server¼àÌı¶Ë¿Ú
struct client_info{
	char cli_ip_addr[16];
	struct sockaddr_in cin;
	int newfd;

	char name[20];
	char ID[20];
	char phonenumber[15];
	char sex;//  v ´ú±íÅ®ĞÔ£¬o´ú±íÄĞĞÔ
	int flag; // 1 ´ú±íÉè±¸£¬0´ú±íÓÃ»§
	int info_type;	

	int logIn¿//logIn =0µÇÈëÊ§°Ü£¬1µÇÈë³É¹¦
	int logUp;//logUp=0 ×¢²áÊ§°Ü,1×¢²á³É¹¦	
	int link=0;//link=0 Á´½Ó¶Ï¿ª£¬1Á´½ÓÊ§°Ü
}
static enum{
	LOG_IN,
	LOG_UP,
	LINK,
	SET_INFO,
	DISCONNECT
};


