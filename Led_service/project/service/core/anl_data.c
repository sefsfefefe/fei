

#include "debug.h"
#include <errno.h>
#include <sqlite3.h>
#include "func.h"
#include "func_declartion.h"
int res=-1;
static char *filname=FILNAME;
static char *table=TABLE;
//�ж����ݿ��Ƿ���ڸ�����
int judge_sqlite(struct client_info *client,sqlite3 *db)
{
		pr_debug("(%s:%d)|judge_sqlite()\n",__func__,__LINE__);
		struct client_info *umsg;
		umsg = (struct client_info*)malloc(sizeof(struct client_info));
		if(umsg==NULL)
		{
			pr_debug("------����ռ����-----(%s:%d)\n",__func__,__LINE__);
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

//�ظ��û�����
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
//�����������ͣ�������ز���
int anl_data(struct client_info *client,sqlite3 *db)
{

	pr_debug("(%s:%d)|anl_data()\n",__func__,__LINE__);
	if(client->info_type==LOG_IN)
	{
		//���룬�������ݿ⣬���Ƿ���ڸ��û�
		res=judge_sqlite(client,db);
			if(res)
		{
			pr_debug("%s:%s --����ɹ�\n",client->name,client->ID);

			//д�����ݣ������û�����ɹ�
			if(call_back_usr(client->newfd,OK)==0)
			{
				pr_debug("д�����ݳɹ�\n");
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
		//ע�ᣬ�������ݿ⣬���Ƿ��Ѿ�ע��
		res=judge_sqlite(client,db);
		if(res)
		{
			//�û��Ѿ�ע��
				pr_debug("%s:%s --�û��Ѿ�ע��\n",client->name,client->ID);
			//д�����ݣ������û�ע����Ϣ
			if(call_back_usr(client->newfd,LOGUP_CUN)==0)
			{
				pr_debug("д�����ݳɹ�\n");
			}
		}else
		{
			pr_debug("%s:%s --û��ע��\n",client->name,client->ID);
			//�洢�û���Ϣ�����ݿ�
			res=insert_data(db,client,table,filname);
			if(res==0)
			{
				if(call_back_usr(client->newfd,LOGUP_OK)==0)
				{
					pr_debug("д�����ݳɹ�\n");
				}
			}else
				pr_debug("�������ݿ�ʧ�ܣ�\n");
		}

		return 1;
	}
	if(client->info_type==SET_DATA,db)
	{
		//�������ݿ⣬Ѱ��ID��Ӧ��fd
		struct client_info *umsg;
		umsg = (struct client_info*)malloc(sizeof(struct client_info));
		if(umsg==NULL)
		{
			pr_debug("------����ռ����-----(%s:%d)\n",__func__,__LINE__);
		}

		//�����豸ID��ѯ
		search_by_type(table,db,NULL,client->ID,umsg,filname);
		if(umsg->newfd==0)
		{
			pr_debug("û��Ҫ��ID=%s\n",client->ID);
			if(call_back_usr(client->newfd,SET_FAL)==0)
				{
					pr_debug("д�����ݳɹ�\n");
				}
		}else{
			pr_debug("����ID=%s\n",client->ID);
			//����ת�ַ���
			char msg=client->led+'0';
			if(call_back_usr(umsg->newfd,&msg)==0)
			{
				pr_debug("д�����ݳɹ�\n");
			}
			if(call_back_usr(client->newfd,SET_OK)==0)
				{
					pr_debug("д�����ݳɹ�\n");
				}
		}
		return 1;
	}
	if(client->info_type==DISCONNECT)
	{
		//�Ͽ�������
		return 0;
	}
	if(client->info_type==LINK)
	{
		return 1;
	}
	return 0;
}
