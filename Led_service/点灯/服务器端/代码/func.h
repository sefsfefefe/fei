


#define SERV_PORT          9000   //server监听端口
struct client_info{
	char cli_ip_addr[16];
	struct sockaddr_in cin;
	int newfd;

	char name[20];
	char ID[20];
	char phonenumber[15];
	char sex;//  v 代表女性，o代表男性
	int flag; // 1 代表设备，0代表用户
	int info_type;	

	int logIn�//logIn =0登入失败，1登入成功
	int logUp;//logUp=0 注册失败,1注册成功	
	int link=0;//link=0 链接断开，1链接失败
}
static enum{
	LOG_IN,
	LOG_UP,
	LINK,
	SET_INFO,
	DISCONNECT
};


