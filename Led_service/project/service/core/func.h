


#define SERV_PORT          9000   //server�����˿�
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

	int logIn�//logIn =0����ʧ�ܣ�1����ɹ�
	int logUp;//logUp=0 ע��ʧ��,1ע��ɹ�	
	int link=0;//link=0 ���ӶϿ���1����ʧ��
}
static enum{
	LOG_IN,
	LOG_UP,
	LINK,
	SET_INFO,
	DISCONNECT
};


