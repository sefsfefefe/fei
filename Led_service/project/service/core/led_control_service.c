
#include "debug.h"
#include "func.h"
#include <errno.h>
#include <pthread.h>
#include <arpa/inet.h>

#include "func_declartion.h"
int fd = -1;
char buf[BUFSIZ];
static char *filname=FILNAME;
static char *table=TABLE;
static sqlite3 *db;
int ret=-1;

void *client_pthread(void *arg)
{
	struct client_info *client =(struct client_info *)arg;
	char buf[BUFSIZ];
		
	pr_debug("(%s:%d)  |client_pthread()\n",__func__,__LINE__);
	while(1){
		bzero(buf,BUFSIZ);
		do{
			pr_debug("----read starting----\n");
			ret=read(client->newfd,buf,BUFSIZ-1);
		}while(ret<0&&EINTR==errno);
		if(ret<0)
		{
			pr_debug("----read error----\n");
			break;
		}
		if(ret==0)
		{
			pr_debug ("Client(%s:%d) not value", client->cli_ip_addr, ntohs (client->cin.sin_port));
			continue;
		}

		//解析数据
		unpack_data(buf,client);
		//分析数据
		ret=anl_data(client,db);
		if(ret)
		{
		  pr_debug("continue to read from client！\n");
		  continue;
		}else{
			close(client->newfd);
			pthread_exit(NULL);
			break;
		}
		
	}
	return NULL;

}

void *device_pthread(void *arg)
{
	struct client_info *client =(struct client_info *)arg;

	while(1){
		bzero(buf,BUFSIZ);
		do{
			pr_debug("----read starting----\n");
			ret=read(client->newfd,buf,BUFSIZ-1);
		}while(ret<0&&EINTR==errno);
		if(ret<0)
		{
			pr_debug("----read error----\n");
			break;
		}
		if(ret==0)
		{
			pr_debug ("Client(%s:%d) is exited.", client->cli_ip_addr, ntohs (client->cin.sin_port));
		}
		//解包数据
		unpack_data(buf,client);
	
		ret=anl_data(client,db);
		if(ret)
		{
		  pr_debug("继续循环，等待用户数据！\n");
		  continue;
		}else{
			close(client->newfd);
			pthread_exit(NULL);
			break;
		}
		}
	return NULL;
}


/*等待用户链接，如果有客户链接
 * 建立一个线程，处理数据
 */
void SocketLin()
{
	pr_debug("Server staring....OK!\n");
	pthread_t thread;
	pthread_t device_thread;

	while(1)
	{

		struct client_info *client;
		client = (struct client_info*)malloc(sizeof(struct client_info));
		if(client==NULL)
		{
			pr_debug("malloc error-----(%s:%d)----\n",__func__,__LINE__);
		}
		socklen_t cli_len=sizeof(client->cin);
		//若有客户连接，则接受连接
		client->newfd=accept (fd, (struct sockaddr *) &(client->cin), &cli_len);
		if((client->newfd>0)&&(inet_ntop(AF_INET,(void *)&(client->cin.sin_addr),client->cli_ip_addr,sizeof(client->cin))!=NULL))
		{
			pr_debug("A new (ip =%s,port =%d) client connected.\n",client->cli_ip_addr,ntohs(client->cin.sin_port));
		}else{
			pr_debug("inet_ntop() error");
		}
		if(client->newfd<0)
		{
			pr_debug("accept error\n");
			exit(1);
		}

		/*遍历数据库，查询是否存在这个fd，如果存在，判断是否是设备fd，如果是
			单独给设备创建一个线程，用于与设备链接
		 */
		struct client_info *umsg;
		umsg = (struct client_info*)malloc(sizeof(struct client_info));
		if(umsg==NULL)
		{
			pr_debug("malloc error!----(%s:%d)--\n",__func__,__LINE__);
		}
    pr_debug("----(%s:%d)----\n",__func__,__LINE__);
		search_by_type(table,db,&(client->newfd),NULL,umsg,filname);
		if(umsg->newfd==client->newfd)
		{
			pr_debug(" This fd exit!\n");
			if(umsg->flag==1)
			{
				pr_debug("This fd is device!\n");
				if(pthread_create(&device_thread,NULL,device_pthread,(void *)client)!=0)
				{
					pr_debug("pthread_create failed!\n");
					continue;
				}
				pthread_detach(device_thread);
				continue;
			}
		}
		pr_debug("----(%s:%d)--client_pthread--\n",__func__,__LINE__);
		//创建一个线程用于链接客户端
		if(pthread_create(&thread,NULL,client_pthread,(void *)client)!=0)
		{
			pr_debug("pthread_create failed!\n");
				continue;
		}
		pthread_detach(thread); 
	}
}
/*socket 初始化*/
int SocketInit()
{
/*	char *sql;
	char *errmsg;*/
	//sql=sqlite3_mprintf("select count(*) form sqlite_master where type='%s'and name='%s'",filname,table);
//	if(sqlite3_exec(db,sql,NULL,NULL,&errmsg)!=SQLITE_OK){	
	//创建数据库
  	pr_debug("------(%s:%d)------\n",__func__,__LINE__);
		create_table(filname,db,table);
//	}
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

	close(fd);
	return 0;
}
int main()
{
	
	if(SocketInit() == -1){ 
		pr_debug("----SocketInit error---\n");
		return -1;  
	}  

	pr_debug("SUCCESS in ConnectManageExit\n");  
	return 0;
}
