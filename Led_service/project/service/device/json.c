#include "../core/debug.h"
#include </usr/local/include/json/json.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include "../core/func.h"

//Json 打包
void insert_task(int fd,struct client_info *umsg,char *buf)
{
	pr_debug("send insert task message\n");
	printf("fd : %d,client:%s,ID: %s,phonenumber:%s,sex:%s,info_type:%d,flag:%d,led:%d\n",fd,umsg->name,umsg->ID,umsg->phonenumber,umsg->sex,umsg->info_type,umsg->flag,umsg->led);

	struct json_object *fat,*json_client_id,*json_phone_num,*json_name,*json_sex,*json_info_type,*json_flag,*json_led;
		//创建一个JSON对象
	fat=json_object_new_object();
	
	//转换成JSon类型,并添加到json对象里
	if(umsg->ID!=NULL){
	json_client_id=json_object_new_string(umsg->ID);
	json_object_object_add(fat,"ID",json_client_id);
	}
	if(umsg->phonenumber!=NULL){
	json_phone_num=json_object_new_string(umsg->phonenumber);
	json_object_object_add(fat,"phonenumber",json_phone_num);
	}
	if(umsg->name!=NULL){
	json_name=json_object_new_string(umsg->name);
	json_object_object_add(fat,"name",json_name);
	}
	if(umsg->sex!=NULL){
	json_sex=json_object_new_string(umsg->sex);
	json_object_object_add(fat,"sex",json_sex);
	}
	if(umsg->info_type!=0){
	json_info_type=json_object_new_int(umsg->info_type);
	json_object_object_add(fat,"info_type",json_info_type);
	}
	if(umsg->flag!=0){
	json_flag=json_object_new_int(umsg->flag);
	json_object_object_add(fat,"flag",json_flag);
	}
	if(umsg->led!=0){
		json_led=json_object_new_int(umsg->led);
		json_object_object_add(fat,"led",json_led);
	}
	//把json对象转换成字符串
	strcpy(buf,json_object_to_json_string(fat));
	pr_debug("---------(%s:%d)--------\n",__func__,__LINE__);
	pr_debug("%s\n",buf);
	pr_debug("---------(%s:%d)--------\n",__func__,__LINE__);
	
}

void unpack_data( char *buf,struct client_info *umsg)
{
	int i;
	struct json_object *json_bao,*json_client_id,*json_phone_num,*json_name,*json_sex,*json_info_type,*json_flag,*json_led;

	//把字符串转换成JSON对象
	json_bao=json_tokener_parse(buf);
	//通过键值获取json成员
	json_client_id=json_object_object_get(json_bao,"ID");
	if(json_client_id!=NULL)
	{
		strcpy(umsg->ID,json_object_get_string(json_client_id));
		pr_debug("----(%s:%d):%s------\n",__func__,__LINE__,umsg->ID);
	}
	json_phone_num=json_object_object_get(json_bao,"phonenumber");
	if(json_phone_num!=NULL)
	{
		strcpy(umsg->phonenumber,json_object_get_string(json_phone_num));
		pr_debug("----(%s:%d):%s------\n",__func__,__LINE__,umsg->phonenumber);
	}
	json_name=json_object_object_get(json_bao,"name");
	if(json_name!=NULL)
	{
		strcpy(umsg->name,json_object_get_string(json_name));
		pr_debug("----(%s:%d):%s------\n",__func__,__LINE__,umsg->name);
	}
	json_sex=json_object_object_get(json_bao,"sex");
	if(json_sex!=NULL)
	{
		strcpy(umsg->sex,json_object_get_string(json_sex));
		pr_debug("----(%s:%d):%s------\n",__func__,__LINE__,umsg->sex);
	}
	json_info_type=json_object_object_get(json_bao,"info_type");
	if(json_info_type!=NULL)
	{
		umsg->info_type=json_object_get_int(json_info_type);
		pr_debug("----(%s:%d):%d------\n",__func__,__LINE__,umsg->info_type);
	}
	json_flag=json_object_object_get(json_bao,"flag");
	if(json_flag!=NULL)
	{
		umsg->flag=json_object_get_int(json_flag);
		pr_debug("----(%s:%d):%d------\n",__func__,__LINE__,umsg->flag);
	}
	json_led=json_object_object_get(json_bao,"led");
	if(json_led!=NULL)
	{
		umsg->led=json_object_get_int(json_led);
		pr_debug("---(%s:%d):%d-----\n",__func__,__LINE__,umsg->led);
	}
}
