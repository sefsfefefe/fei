#include "../core/debug.h"
#include </usr/local/include/json/json.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>

typedef struct json_object*  Json_object;
int json_obect_long=sizeof(struct json_object);
//Json_object ��ʼ��
void init_jison_object(Json_object fat,Json_object json_client_id,Json_object json_phone_num,Json_object json_sex,
									Json_object json_name,Json_object json_info_type,Json_object json_flag,Json_object json_led)
{
	fat=(Json_object)malloc(json_obect_long);
	fat=NULL;
	json_client_id=(Json_object)malloc(json_obect_long);
	json_client_id=NULL;
	json_phone_num=(Json_object)malloc(json_obect_long);
	json_phone_num=NULL;
	json_sex=(Json_object)malloc(json_obect_long);
	json_sex=NULL;
	json_name=(Json_object)malloc(json_obect_long);
	json_name=NULL;
	json_info_type=(Json_object)malloc(json_obect_long);
	json_info_type=NULL;
	json_flag=(Json_object)malloc(json_obect_long);
	json_flag=NULL;
	json_led=(Json_object)malloc(json_obect_long);
	json_led=NULL;
}

//Json ���
void insert_task(int fd,struct client_info *umsg,char *buf)
{
	pr_debug("send insert task message\n");
	printf("fd : %d,client:%s,ID: %s,phonenumber:%s,sex:%s,info_type:%d,flag:%d,led:%d\n",fd,umsg->name,umsg->ID,umsg->phonenumber,umsge->sex,umsg->info_type,umsg->flag,umsg->led);

	struct json_object *fat,*json_client_id,*json_phone_num,*json_name,*json_sex,*json_info_type,*json_flag,json_object *json_led;
	init_jison_object(fat, json_client_id, json_phone_num, json_sex,
					json_name,  json_info_type, json_flag, json_led)
	//����һ��JSON����
	fat=json_object_new_object();
	
	//ת����JSon����,����ӵ�json������
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
	json_sex=json_object_new_string(umsge->sex);
	json_object_object_add(fat,"sex",json_sex);
	}
	if(umsg->info_type!=-1){
	json_info_type=json_object_new_int(umsg->info_type);
	json_object_object_add(fat,"info_type",json_info_type);
	}
	if(umsg->flag!=-1){
	json_flag=json_object_new_int(umsg->flag);
	json_object_object_add(fat,"flag",json_flag);
	}
	if(umsg->led!=-1){
		json_led=json_object_new_int(umsg->flag);
		json_object_object_add(fata,"led",json_flag);
	}
	//��json����ת�����ַ���
	buf=json_object_to_json_string(fat);
	pr_debug("---------(%s:%d)--------\n");
	pr_debug("%s\n",buf);
	pr_debug("---------(%s:%d)--------\n");
	
}

void unpack_data( char *buf,struct client_info *umsg)
{
	int i;
	struct json_object *json_bao,*json_client_id,*json_phone_num,*json_name,*json_sex,*json_info_type,*json_flag,*json_led;
	init_jison_object(json_bao, json_client_id, json_phone_num, json_sex,
					json_name, json_sex, json_info_type, json_flag, json_led)
	//���ַ���ת����JSON����
	json_bao=json_tokener_parse(buf);
	//ͨ����ֵ��ȡjson��Ա
	json_client_id=json_object_object_get(json_bao,"ID");
	if(json_client_id!=NULL)
	{
		umsg->ID=json_object_get_string(json_client_id);
		pr_debug("----(%s:%d):%s------\n",__func__,__LINE__,umsg->ID);
	}
	json_phone_num=json_object_object_get(json_bao,"phonenumber");
	if(json_phone_num!=NULL)
	{
		umsg->phonenumber=json_object_get_string(json_phone_num);
		pr_debug("----(%s:%d):%s------\n",__func__,__LINE__,umsg->phonenumber);
	}
	json_name=json_object_object_get(json_bao,"name");
	if(json_name!=NULL)
	{
		umsg->name=json_object_get_string(json_name);
		pr_debug("----(%s:%d):%s------\n",__func__,__LINE__,umsg->name);
	}
	json_sex=json_object_object_get(json_bao,"sex");
	if(json_sex!=NULL)
	{
		umsg->sex=json_object_get_string(json_sex);
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
	json_led=json_object_object_get_int(json_bao,"led");
	if(json_flag!=NULL)
	{
		umsg->led=json_object_get_int(json_led);
		pr_debug("---(%s:%d):%d-----\n",__func__,__LINE__,umsg->led);
	}
}
