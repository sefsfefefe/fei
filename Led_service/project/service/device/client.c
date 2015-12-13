
#include "../core/func.h"
#include <errno.h>
#include "../core/debug.h"
int main (void)
{
	int fd = -1;
	struct sockaddr_in sin;

	pr_debug("-------%s:%d------\n",__func__,__LINE__);

	struct client_info *client;
	client=(struct client_info*)malloc(sizeof(struct client_info));
	if(client==NULL)
		exit(1);
pr_debug("-------%s:%d------\n",__func__,__LINE__);

	strcpy(client->name,"taoi");
	pr_debug("-------%s:%d------\n",__func__,__LINE__);

	strcpy(client->ID,"seieei23se2");
	pr_debug("-------%s:%d------\n",__func__,__LINE__);
	client->info_type=DISCONNECT;
	pr_debug("-------%s:%d------\n",__func__,__LINE__);
	client->flag=1;
pr_debug("-------%s:%d------\n",__func__,__LINE__);


	/* 1.创建套接字fd */
	if ((fd = socket (AF_INET, SOCK_STREAM, 0)) < 0) {	/* IPV4 TCP的通信程序 */
		perror ("socket");
		exit (EXIT_FAILURE);
	}
	pr_debug("-------%s:%d------\n",__func__,__LINE__);
	/* 2. 填充sin结构体 */
	bzero (&sin, sizeof (sin));
	sin.sin_family = AF_INET;
	sin.sin_port = htons (9000);
	sin.sin_addr.s_addr = inet_addr ("192.168.3.2");
	pr_debug("-------%s:%d------\n",__func__,__LINE__);

	if (connect (fd, (struct sockaddr *) &sin, sizeof (sin)) < 0) {
		perror ("connect");
		exit (EXIT_FAILURE);
	}

	printf ("Client starting ....OK!\n");
	char buf[BUFSIZ];
	bzero(buf,BUFSIZ);
	insert_task(fd,client,buf);
	int ret=-1;
	/* 3. 写套接字 */
		while (1) {

		do {
			ret = write (fd, buf, strlen (buf));	/* 写fd */
		} while (ret < 0 && EINTR == errno);
	   bzero(buf,BUFSIZ);
		do{
		ret=read(fd,buf,BUFSIZ);
		}while(ret<0&&EINTR==errno);
		printf("%s\n",buf);
	break;	
	}

	close (fd);

	return 0;
}
