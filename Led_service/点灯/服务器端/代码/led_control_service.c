
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

	//解析数据
	unpackage(buf,&client);

	//判断信息和处理信息
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
	//解析数据
	unpackage(buf,&client);

	if(client.link==0)
		pthread_exit(NULL);

	//判断信息和处理信息
	judge_info(&client);

}


/*等待用户链接，如果有客户链接
 * 建立一个线程，处理数据
 */
void SocketLin()
{
	pr_debug("Server staring....OK!\n");
	pthread_t thread;
	pthread_t device_thread;

	int flag;//设备和用户的标记，flag==0,表示用户，flag==1，表示设备



	char cli_ip_addr[16];

	struct client_info client;

	bzero(client,sizeof(struct client_info));
	socklen_t cli_len =sizeof (client.cin);
	while(1)
	{
		//若有客户连接，则接受连接
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

		�/*遍历数据库，查询是否存在这个IP，如果存在，判断是否是设备IP，如果是
		   单独给设备创建一个线程，用于与设备链接
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
		//创建一个线程用于链接客户端
		if(pthread_create(&thread,NULL,client_pthread,(void *)&client)!=0)
		{
			pr_debug("pthread_create failed!\n");d
				break;
		}
		pthread_detach(thread); 
	}d
}
/*socket 初始化*/
int SocketInit()
{


	struct sockaddr_in sin;
	if ((fd = socket (AF_INET, SOCK_STREAM, 0)) < 0) {	/* IPV4 TCP的通信程序 */
		pr_debug("----socket() error-----\n");
		return -1;
	}
	/*允许绑定的地址快速重用 */
	int b_use = 1;
	setsockopt (fd, SOL_SOCKET, SO_REUSEADDR, &b_use, sizeof (int));

	/* 填充sin结构体 */
	bzero(&sin,sizeof(sin));
	sin.sin_family=AF_INET;
	sin.sin_port=htons(SERV_PORT);
	sin.sin_addr.s_addr=htonl(INADDR_ANY);

	/*绑定监听端口*/
	if(bind(fd,(struct sockaddr *)&sin,sizeof(sin))<0)
	{
		pr_debug("-----bind() error----\n");
		return -1;
	}

	/*开始监听*/
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
