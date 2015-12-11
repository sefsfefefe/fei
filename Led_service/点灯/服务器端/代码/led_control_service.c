
#include "Debug.h"
#include "func.h"



int fd = -1;

void *client_pthread(void *arg)
{
	struct client_info client =*(struct client_info *)arg;
	char buf[BUFSIZE];
	int ret;

	bzero(buf,BUFSIZE);
	do{
		pr_debug("----read starting----\n");
		ret=read(client.newfd,buf,BUFSIZE-1);
	}while(ret<0&&EINTR==errno);
	if(ret<0)
	{
		pr_debug("----read error----\n");
		return;
	}
	if(ret==0)
	{
		pr_debug ("Client(%s:%d) is exited.", client.cli_ip_addr, ntohs (client.cin.sin_port));
	}

	//��������
	unpackage(buf,&client);

	//�ж���Ϣ�ʹ�����Ϣ
	judge_info(&client);

	pthread_exit(NULL);
}

void *device_pthread(void *arg)
{
	struct client_info client =*(struct client_info *)arg;
	char buf[BUFSIZE];
	int ret;

	bzero(buf,BUFSIZE);
	do{
		pr_debug("----read starting----\n");
		ret=read(client.newfd,buf,BUFSIZE-1);
	}while(ret<0&&EINTR==errno);
	if(ret<0)
	{
		pr_debug("----read error----\n");
		return;
	}
	if(ret==0)
	{
		pr_debug ("Client(%s:%d) is exited.", client.cli_ip_addr, ntohs (client.cin.sin_port));
	}
	//��������
	unpackage(buf,&client);

	if(client.link==0)
		pthread_exit(NULL);

	//�ж���Ϣ�ʹ�����Ϣ
	judge_info(&client);

}


/*�ȴ��û����ӣ�����пͻ�����
 * ����һ���̣߳���������
 */
void SocketLin()
{
	pr_debug("Server staring....OK!\n");
	pthread_t thread;
	pthread_t device_thread;

	int flag;//�豸���û��ı�ǣ�flag==0,��ʾ�û���flag==1����ʾ�豸



	char cli_ip_addr[16];

	struct client_info client;

	bzero(client,sizeof(struct client_info));
	socklen_t cli_len =sizeof (client.cin);
	while(1)
	{
		//���пͻ����ӣ����������
		client.newfd=accept (fd, (struct sockaddr *) &(client.cin), &cli_len);
		if((client.newfd>0)&&(inet_ntop(AF_INET,(void *)&(client.cin.sin_addr),client.cli_ip_addr,sizeof(client.cin))!=NULL))
		{
			pr_debug("A new (ip =%s,port =%d) client connected.\n",client.cli_ip_addr,ntohs(client.cin.sin_port));
		}else{
			pr_debug("inet_ntop() error");
		}
		if(client.newfd<0)
		{
			pr_debug("accept error\n");
			exit(1);
		}

		�/*�������ݿ⣬��ѯ�Ƿ�������IP��������ڣ��ж��Ƿ����豸IP�������
		   �������豸����һ���̣߳��������豸����
		  */
			if(sqlite_select(client.cin.sin_addr,&flag)==0)
			{
				pr_debug(" This IP exit!\n");
				if(flag==1)
				{
					pr_debug("This Ip is device!\n");
					if(pthread_create(&device_thread,NULL,device_pthread,(void *)&client)!=0)
					{
						pr_debug("pthread_create failed!\n");
						break;
					}
					pthread_detach(device_thread);
				}
			}
		//����һ���߳��������ӿͻ���
		if(pthread_create(&thread,NULL,client_pthread,(void *)&client)!=0)
		{
			pr_debug("pthread_create failed!\n");d
				break;
		}
		pthread_detach(thread); 
	}d
}
/*socket ��ʼ��*/
int SocketInit()
{


	struct sockaddr_in sin;
	if ((fd = socket (AF_INET, SOCK_STREAM, 0)) < 0) {	/* IPV4 TCP��ͨ�ų��� */
		pr_debug("----socket() error-----\n");
		return -1;
	}
	/*����󶨵ĵ�ַ�������� */
	int b_use = 1;
	setsockopt (fd, SOL_SOCKET, SO_REUSEADDR, &b_use, sizeof (int));

	/* ���sin�ṹ�� */
	bzero(&sin,sizeof(sin));
	sin.sin_family=AF_INET;
	sin.sin_port=htons(SERV_PORT);
	sin.sin_addr.s_addr=htonl(INADDR_ANY);

	/*�󶨼����˿�*/
	if(bind(fd,(struct sockaddr *)&sin,sizeof(sin))<0)
	{
		pr_debug("-----bind() error----\n");
		return -1;
	}

	/*��ʼ����*/
	if(listen(fd,5)==-1)
	{
		pr_debug("ERROR in listen Stream Socket: %s\n",strerror(errno));  
		return -1; 
	}
	SocketLin();

	return 0;
}
int main()
{
	if(SocketInit() == -1){ 
		pr_debug("----SocketInit error---\n");
		return -1;  
	}  
	while(1){  
		sleep(2);  
	}  

	/*notice to exit*/  
	isStreamQuit = -1;  
	sleep(1);  
	if(gStreamServerFd){  
		close(gStreamServerFd);  
	}  
	pr_debug("SUCCESS in ConnectManageExit\n");  
}
