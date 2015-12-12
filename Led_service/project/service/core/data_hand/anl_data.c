

#include "../debug.h"
#include "../func.h"
int ret=-1;
//�ж����ݿ��Ƿ���ڸ�����
int judge_sqlite(struct client_info *client)
{
		struct client_info *umsg;
		umsg = (struct client_info*)malloc(sizeof(struct client_info));
		*umsg=NULL;
		seartch_by_type(table,db,client->newfd,NULL,umsg);
		if(client->newfd==umsg->newfd)
		{
			return 1;
		}else
		{
			return 0;
		}
}

//�ظ��û�����
int call_back_usr(int fd��char *msg)
{
	while(1){
		do{
			ret=write(client->newfd,msg,strlen(msg));
		}while(ret < 0 && EINTR == errno);
		if(ret < 0) {
			pr_debug("write to socket error");
			continue;
		}
		break;		
	}
	return 0;
}
//�����������ͣ�������ز���
int anl_data(struct client_info *client)
{


	if(client->info_type==LOG_IN)
	{
		//���룬�������ݿ⣬���Ƿ���ڸ��û�
		int ret=judge_sqlite(client);
			if(ret)
		{
			pr_debug("%s:%s --����ɹ�\n",client->name,client->ID);

			//д�����ݣ������û�����ɹ�
			if(call_back_usr(client->newfd,OK)==0)
			{
				pr_debug("д�����ݳɹ�\n");
			}
		}else
		{
			pr_debug("%s:%s --����ʧ��\n",client->name,client->ID);
			if(call_back_usr(client->newfd,FAILU)==0)
			{
				pr_debug("д�����ݳɹ�\n");
			}
		}

		return 1;
	}

	if(client->info_type==LOG_UP)
	{
		//ע�ᣬ�������ݿ⣬���Ƿ��Ѿ�ע��
		ret=judge_sqlite(client);
		if(ret)
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
			ret=insert_data(db,client,table);
			if(ret==0)
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
	if(client->info_type==SET_DATA)
	{
		//�������ݿ⣬Ѱ��ID��Ӧ��fd
		struct client_info *umsg;
		umsg = (struct client_info*)malloc(sizeof(struct client_info));
		*umsg=NULL;
		//�����豸ID��ѯ
		seartch_by_type(table,db,NULL,client->ID,umsg);
		if(umsg->newfd==NULL)
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
}
