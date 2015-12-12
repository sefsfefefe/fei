#include<sqlite3.h>
#include<string.h>

#include "../func.h"
char *errmsg;


void get_client_info_from_sqlite3(char *table_head,char *goal_data,struct client_info *umsg)
{
	if(strncmp(table_head,"FD",5)==0)
	{
		umsg->newfd=atoi(goal_data);
		pr_debug("newfd=%d\n",umsg->newfd);
	}
	if(strncmp(table_head,"NAME",4)==0)
	{
		strcpy(umsg->name,goal_data);
		pr_debug("name=%s\n",umsg->name);
	}
	if(strncmp(table_head,"ID",2)==0)
	{
		strcpy(umsg->ID,goal_data);
		pr_debug("ID = %s\n",umsg->ID);
	}
	if(strncmp(table_head,"PHONENUMBER",strlen("PHONENUMBER"))==0)
	{
		strcpy(umsg->phonenumber,goal_data);
		pr_debug("phonenumber=%s\n",umsg->phonenumber);
	}
	if(strncmp(table_head,"SEX",3))
	{
		strcpy(umsg->sex,goal_data);
		pr_debug("sex=%s\n",umsg->sex);
	}
	if(strncmp(table_head,"FLAG",4))
	{
		umsg->flag=atoi(goal_data);
		pr_debug("flag=%d\n",umsg->flag);
	}
	if(strncmp(table_head,"INFO_TYPE",strlen("INFO_TYPE"))==0)
	{
		umsg->info_type=atoi(goal_data);
		pr_debug("info_type=%d\n",umsg->info_type);
	}
	return;
}
//回调函数
int sqlite_callback(void *data,int argc,char **argv,char **azColName)
{
		int i;
		struct client_info *umsg=*(struct client_inof *)data;

		for(i=0; i<argc; i++){
		pr_debug("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
		//调用一个用户取数据的函数
		get_client_info_from_sqlite3(azColName[i],argv[i],umsg);		
		}
		printf("\n");
		return 0;
}
//创建一个数据库
void create_table(char *filename,sqlite3 *db,char *table)
{
	char *sql;
	int rc;
	rc=sqlite3_open(filename,&db);
	if(rc)
	{
		fprintf(stderr,"can't open database %s\n",sqliste3_errmsg(db));
		sqlite3_close(db);
	}
	sql=sqlite3_myprintf("create table %s (FD INT PRIMARY KEY NOT NULL,
	NAME TEXT,ID TEXT,PHONENUMBER TEXT,SEX CHAR,FLAG INT,INFO_TYPE INT)",table);
	if(sqlite3_exec(db,sql,NULL,NULL,&errmsg)!=SQLITE_OK)
		{
				pr_debug("sqlite3_exec_create\n");
				exit(-1);
		}else
				pr_debug("-----(%s:%d)create table success!-----\n",__func__,__LINE__);
	return;
}
//插入数据库
void insert_data(sqlite3 *db,struct client_info *umsg,char *table)
{
		pr_debug("-----(%s:%d)|instert_data()------\n",__func__,__LINE__);

		//打印数据结构的数据
		printf_data(umsg);
		char *sql=sqlite3_mprintf("insert into %s values('%d','%s','%s','%s','%c',%d','%d')",table,umsg->newfd,umsg->name,umsg->ID,umsg->phonenumber,umsg->sex,umsg->flag,umsg->info_type);
		if(sqlite3_exec(db,sql,NULL,NULL,&errmsg)!=SQLITE_OK)
		{
				pr_debug("sqlite3_exec\n");
				exit(-1);
		}else
				pr_debug("-----(%s:%d)insert success!-----\n",__func__,__LINE__);
		sqlite3_free(sql);

		return;
}

//删除表中已有的记录
void delete_data(sqlite3 *db,char *table)
{
		char ID[20];
		pr_debug("please input the ID you want to delete!\n");
		scanf("%s",ID);
		getchar();


		char *sql=sqlite3_mprintf("delete from %s where ID = %s",talbe,ID);
		if(sqlite3_exec(db,sql,NULL,NULL,&errmsg)!=SQLITE_OK)
		{
				pr_debug("sqlite3_exec_delete\n");
				exit(-1);
		}else
				pr_debug("delete success!!\n");
		sqlite3_free(sql);

		return;
}
//更新数据库
void upata_data(sqlite3 *db,char *table,struct client_info *umsg)
{

		printf_data(umsg);
		char *sql=sqlite3_mprintf("updata %s set umsg->newfd='%d',umsg->name='%s',umsg->ID='%s',				                         				    umsg->phonenumber='%s',umsg->sex='%c',
						umsg->flag='%d',umsg->info_type='%d'",
						table,umsg->newfd,umsg->ame,umsg->ID,umsg->phonenumber,
						umsg->sex,umsg->flag,umsg->info_type);
		if(sqlite3_exec(db,sql,NULL,NULL,&errmsg)!=SQLITE_OK)
		{
				pr_debug("sqlite3_exec_upata\n");
				exit(-1);
		}else
				pr_debug("upata success!!\n");
		sqlite3_free(sql);

		return;
}

//打印数据库
void show_data(sqlite3 *db,char *table)
{
		int i,nrow,ncolum,j,index;
		char **resultp;
		char *sql=sqlite3_mprintf("select * from %s",table);

		if(sqlite3_get_table(db,sql,&resultp,&nrow,&ncolum,&errmsg)!=SQLITE_OK)
		{
				pr_debug("sqlite3_get_table\n");
				exit(-1);
		}
		index=ncolum;
		for(i=0;i<ncolum;i++)
		{
				pr_debug("%s\t",resultp[i]);
		}
		pr_debug("\n");
		for(i=0;i<nrow;i++)
		{
				for(j=0;j<nclum;j++)
				{
						pr_debug("%s\t",resultp[index++]);
				}
				pr_debug("\n");
		}
		sqlite3_free(sql);
		return;
}

//根据ID查询该ID下的客户信息
void search_by_id(char *table,sqlite3 *db,char *id,struct client_info *umsg)
{
		char *sql;

		bzero(umsg,sizeof(struct client_info));
		sql=sqlite3_mprintf("select * form %s where id=%s",table,id);
		if(sqlite3_exec(db,sql,&sqlite_callback,(void *)umsg,&errmsg)!=)SQLITE_OK)
		{
				pr_debug("sqlite3_exec_search\n");
				exit(-1);
		}
		sqlite3_free(sql);
}
//关闭数据库
void quit(sqlite3 *db)
{
		pr_debug("BYBYE!!\n");
		sqlite3_close(db);
		exit(0);
}

//删除表
void delete_all(sqlite3 *db,char *table)
{
	char *sql;
	sql=sqliste3_mprintf("delete from %s",table);
	if(sqlite3_exec(db,sql,NULL,NULL,&errmsg)!=SQLITE_OK)
	{
		pr_debug("sqlite3_exec_delete_all  error\n");
		exit(-1);
	}else
		pr_debug("sqlite3_exec_delete_all successful!\n");
		return;
}
