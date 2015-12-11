
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

	//½âÎöÊý¾Ý
	unpackage(buf,&client);

	//ÅÐ¶ÏÐÅÏ¢ºÍ´¦ÀíÐÅÏ¢
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
	//½âÎöÊý¾Ý
	unpackage(buf,&client);

	if(client.link==0)
		pthread_exit(NULL);

	//ÅÐ¶ÏÐÅÏ¢ºÍ´¦ÀíÐÅÏ¢
	judge_info(&client);

}


/*µÈ´ýÓÃ»§Á´½Ó£¬Èç¹ûÓÐ¿Í»§Á´½Ó
 * ½¨Á¢Ò»¸öÏß³Ì£¬´¦ÀíÊý¾Ý
 */
void SocketLin()
{
	pr_debug("Server staring....OK!\n");
	pthread_t thread;
	pthread_t device_thread;

	int flag;//Éè±¸ºÍÓÃ»§µÄ±ê¼Ç£¬flag==0,±íÊ¾ÓÃ»§£¬flag==1£¬±íÊ¾Éè±¸



	char cli_ip_addr[16];

	struct client_info client;

	bzero(client,sizeof(struct client_info));
	socklen_t cli_len =sizeof (client.cin);
	while(1)
	{
		//ÈôÓÐ¿Í»§Á¬½Ó£¬Ôò½ÓÊÜÁ¬½Ó
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

		¡/*±éÀúÊý¾Ý¿â£¬²éÑ¯ÊÇ·ñ´æÔÚÕâ¸öIP£¬Èç¹û´æÔÚ£¬ÅÐ¶ÏÊÇ·ñÊÇÉè±¸IP£¬Èç¹ûÊÇ
		   µ¥¶À¸øÉè±¸´´½¨Ò»¸öÏß³Ì£¬ÓÃÓÚÓëÉè±¸Á´½Ó
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
		//´´½¨Ò»¸öÏß³ÌÓÃÓÚÁ´½Ó¿Í»§¶Ë
		if(pthread_create(&thread,NULL,client_pthread,(void *)&client)!=0)
		{
			pr_debug("pthread_create failed!\n");d
				break;
		}
		pthread_detach(thread); 
	}d
}
/*socket ³õÊ¼»¯*/
int SocketInit()
{


	struct sockaddr_in sin;
	if ((fd = socket (AF_INET, SOCK_STREAM, 0)) < 0) {	/* IPV4 TCPµÄÍ¨ÐÅ³ÌÐò */
		pr_debug("----socket() error-----\n");
		return -1;
	}
	/*ÔÊÐí°ó¶¨µÄµØÖ·¿ìËÙÖØÓÃ */
	int b_use = 1;
	setsockopt (fd, SOL_SOCKET, SO_REUSEADDR, &b_use, sizeof (int));

	/* Ìî³äsin½á¹¹Ìå */
	bzero(&sin,sizeof(sin));
	sin.sin_family=AF_INET;
	sin.sin_port=htons(SERV_PORT);
	sin.sin_addr.s_addr=htonl(INADDR_ANY);

	/*°ó¶¨¼àÌý¶Ë¿Ú*/
	if(bind(fd,(struct sockaddr *)&sin,sizeof(sin))<0)
	{
		pr_debug("-----bind() error----\n");
		return -1;
	}

	/*¿ªÊ¼¼àÌý*/
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
