

#include "debug.h"
#include <errno.h>
#include <sqlite3.h>
#include "func.h"
#include "func_declartion.h"
int res=-1;
static char *filname=FILNAME;
static char *table=TABLE;
//判断数据库是否存在该数据
int judge_sqlite(struct client_info *client,sqlite3 *db)
{
		pr_debug("(%s:%d)|judge_sqlite()\n",__func__,__LINE__);
		struct client_info *umsg;
		umsg = (struct client_info*)malloc(sizeof(struct client_info));
		if(umsg==NULL)
		{
			pr_debug("------分配空间出错-----(%s:%d)\n",__func__,__LINE__);
		}
		search_by_type(table,db,&(client->newfd),NULL,umsg,filname);
		if(client->newfd==umsg->newfd)
		{
			return 1;
		}else
		{
			return 0;
		}
}

//回复用户数据
int call_back_usr(int fd,char *msg)
{
	while(1){
		do{
			res=write(fd,msg,strlen(msg));
		}while(res < 0 && EINTR == errno);
		if(res < 0) {
			pr_debug("write to socket error");
			continue;
		}
		break;		
	}
	return 0;
}
//分析数据类型，进行相关操作
int anl_data(struct client_info *client,sqlite3 *db)
{

	pr_debug("(%s:%d)|anl_data()\n",__func__,__LINE__);
	if(client->info_type==LOG_IN)
	{
		//登入，遍历数据库，看是否存在该用户
		res=judge_sqlite(client,db);
			if(res)
		{
			pr_debug("%s:%s --登入成功\n",client->name,client->ID);

			//写回数据，告诉用户登入成功
			if(call_back_usr(client->newfd,OK)==0)
			{
				pr_debug("写回数据成功\n");
			}
		}else
		{
			pr_debug("%s:%s --log_up fail!\n",client->name,client->ID);
			if(call_back_usr(client->newfd,FAILU)==0)
			{
				pr_debug("write back to client fail!\n");
			}
		}

		return 1;
	}

	if(client->info_type==LOG_UP)
	{
		//注册，遍历数据库，看是否已经注册
		res=judge_sqlite(client,db);
		if(res)
		{
			//用户已经注册
				pr_debug("%s:%s --用户已经注册\n",client->name,client->ID);
			//写回数据，告诉用户注册信息
			if(call_back_usr(client->newfd,LOGUP_CUN)==0)
			{
				pr_debug("写回数据成功\n");
			}
		}else
		{
			pr_debug("%s:%s --没有注册\n",client->name,client->ID);
			//存储用户信息到数据库
			res=insert_data(db,client,table);
			if(res==0)
			{
				if(call_back_usr(client->newfd,LOGUP_OK)==0)
				{
					pr_debug("写回数据成功\n");
				}
			}else
				pr_debug("存入数据库失败！\n");
		}

		return 1;
	}
	if(client->info_type==SET_DATA,db)
	{
		//遍历数据库，寻找ID对应的fd
		struct client_info *umsg;
		umsg = (struct client_info*)malloc(sizeof(struct client_info));
		if(umsg==NULL)
		{
			pr_debug("------分配空间出错-----(%s:%d)\n",__func__,__LINE__);
		}

		//根据设备ID查询
		search_by_type(table,db,NULL,client->ID,umsg,filname);
		if(umsg->newfd==0)
		{
			pr_debug("没有要找ID=%s\n",client->ID);
			if(call_back_usr(client->newfd,SET_FAL)==0)
				{
					pr_debug("写回数据成功\n");
				}
		}else{
			pr_debug("存在ID=%s\n",client->ID);
			//整数转字符串
			char msg=client->led+'0';
			if(call_back_usr(umsg->newfd,&msg)==0)
			{
				pr_debug("写回数据成功\n");
			}
			if(call_back_usr(client->newfd,SET_OK)==0)
				{
					pr_debug("写回数据成功\n");
				}
		}
		return 1;
	}
	if(client->info_type==DISCONNECT)
	{
		//断开链链接
		return 0;
	}
	if(client->info_type==LINK)
	{
		return 1;
	}
	return 0;
}
