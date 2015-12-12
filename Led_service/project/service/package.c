
#include "func.h"
char ID_DEV[][20];
char ID_USR[][20];

//设备注册函数
void register_device(struct client_info *client)
{
	pr_debug("-----（%s:%d）设备注册----\n",__func__,__LINE__);
	int i=0;
	for(i=0;*ID_DEV[i]!=NULL;i++)
	{
	}
 	strncpy(ID_DEV[i],client->ID,sizeof(client->ID));	
}


//用户登入
void log_in(struct client_info *client)
{
	int i=0;
	for(i=0;*ID_USR[i]!=NULL;i++)
	{
	if(strncmp(client->ID,ID_USR[i],sizeof(ID[I]))==0)
	{
		pr_debug("---(%s:%d) 信息匹配，登入成功-----\n",__func__,__LINE__);
		client->logIn=1;
		break;
	}
	}
	if(*ID_USR[i]==NULL)
	{
		pr_debug("---(%s:%d) 信息不匹配，登入失败-----\n",__func__,__LINE__);
		client->logIn=0;
	}
	
}

//用户注册
void register_user(struct *client)
{
	pr_debug("-----（%s:%d）设备注册----\n",__func__,__LINE__);
	int i=0;
	for(i=0;*ID_USR[i]!=NULL;i++)
	{
	}
 	strncpy(ID_USR[i],client->ID,sizeof(client->ID));	
}	
void unpackage(char buf[],struct client_info *client)
{
	if(buf[0])
	{
		if(buf[1]==0xfe)
		{
			if(buf[2]==1)
			{
				//表示是设备数据
				pr_debug("----(%s:%d)设备----\n",__func__,__LINE__);
				client->flag=buf[2];

				if(buf[3]==LOG_UP)
				{
					//设备注册
					pr_debug("----(%s:%d)这是设备注册---\n",__func__,__LINE__);
					register_device(client);
				}
				if(buf[3]==LINK)
				{
					//设备链接服务器
					pr_debug("----(%s:%d)设备链接正常---\n",__func__,__LINE__);
					client->link=1			  
				}
				if(buf[3]==DISCONNECT)
				{
					//设备断开链接
					pr_debug("----(%s:%d)设备断开链接----\n",__func__,__LINE__);
					client->link=0;
				}
			}
			if(buf[2]==0)
			{
				//表示用户
				pr_debug("----(%s:%d)用户----\n",__func__,__LINE__);
				client->flag=buf[2];

				if(buf[3]==LOG_IN)
				{
					//登入
					pr_debug("----(%s:%d)用户登入----\n",__func__,__LINE__);
					log_in(client);
				}
				if(buf[3]==LOG_UP)
				{
					//注册
					pr_debug("----(%s:%d)用户注册----\n",__func__,__LINE__);
					register_user(client);	
				}
				if(buf[3]==SET_INFO)
				{
					//给设备发送数据
					pr_debug("----(%s:%d)给设备发送数据----\n",__func__,__LINE__);
					set_info(client);
				}
			}
		}

	}	
}

