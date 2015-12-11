#include<sqlite3.h>
#include<string.h>

#include "../func.h"
char *errmsg;

//回调函数
int sqlite_callback(void *data,int argc,char **argv,char **azColName)
{
		int i;

		fprintf(stderr, "%s: ", (const char*)data);
		for(i=0; i<argc; i++){
				printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
		}
		printf("\n");
		return 0;
}

//插入数据库
void insert_data(sqlite3 *db,struct client_info *umsg,char *table)
{
		pr_debug("-----(%s:%d)|instert_data()------\n",__func__,__LINE__);

		//打印数据结构的数据
		printf_data(umsg);
		char *sql=sqlite3_mprintf("insert into %s values('%s','%s','%s','%c',%d','%d')",table,umsg->name,umsg->ID,umsg->phonenumber,umsg->sex,umsg->flag,umsg->info_type);
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
void upata_data(sqlite3 *db,char *table,struct client_info *umsg)
{

		printf_data(umsg);
		char *sql=sqlite3_mprintf("updata %s set umsg->name'%s',umsg->ID='%s',				                         umsg->phonenumber='%s',umsg->sex='%c',
						umsg->flag='%d',umsg->info_type='%d'",
						table,umsg->ame,umsg->ID,umsg->phonenumber,
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

void search_by_id(char *table,sqlite3 *db,char *id)
{
		char *sql;
		const char *data="Callback function called";
		sql=sqlite3_mprintf("select * form %s where id=%s",table,id);
		if(sqlite3_exec(db,sql,&sqlite_callback,(void *)data,&errmsg)!=)SQLITE_OK)
		{
				pr_debug("sqlite3_exec_search\n");
				exit(-1);
		}
		sqlite3_free(sql);
}
void quit(sqlite3 *db)
{
		pr_debug("BYBYE!!\n");
		sqlite3_close(db);
		exit(0);
}
