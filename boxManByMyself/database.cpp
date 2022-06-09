#include <mysql.h>
#include <stdio.h>
#include "database.h"


#define DB_NAME "BOX_MAN"
#define DB_HOST "127.0.0.1"
#define DB_POST 3306
#define DB_USER "root"
#define DB_USER_PASSWD "zx12345678LY"
#define debug 1




/**********************************************************
 *���ܣ��������ݿ�
 *���룺myslql���
 *����ֵ���ɹ� true  ʧ�� false
 *********************************************************/
bool connect_db(MYSQL& mysql)
{
	//1.��ʼ�����ݿ���
	mysql_init(&mysql);

	//2.�����ַ�����
	mysql_options(&mysql, MYSQL_SET_CHARSET_NAME, "gbk");  // gbk

	//3.�������ݿ�
	if (mysql_real_connect(&mysql, DB_HOST, DB_USER, DB_USER_PASSWD, DB_NAME, DB_POST, NULL, 0) == NULL)
	{
		printf("���ݿ����ӳ�������ԭ�� %s\n", mysql_error(&mysql));
		return false;
	}

	return true;
}

/**********************************************************
 *���ܣ�ͨ���û�������������ݿ��ȡ�û���Ϣ
 *���룺user �û���Ϣ�ṹ��
 *����ֵ���ɹ� true  ʧ�� false
 *********************************************************/
bool fetch_user_info(userinfo & user)
{
	MYSQL mysql;
	MYSQL_RES* res; // ��ѯ�����
	MYSQL_ROW row; // ��¼�ṹ��
	char sql[256];
	bool ret = false;

	// 1.���ӵ����ݿ�
	if (connect_db(mysql) == false)
	{
		return false;
	}
	//printf("���ݿ����ӳɹ������ں˶���Ϸ�����Ϣ...\n");

	// 2.��ȡ�û���Ϣ
	snprintf(sql, 256, "select id, level_id from users where username = '%s' and password = md5('%s');", user.username.c_str(), user.passwd.c_str());
	ret = mysql_query(&mysql, sql); // �ɹ�����0

	if (ret)
	{
		printf("��Ϣ��ȡʧ�ܣ�%s ����ԭ�� %s\n", sql,mysql_error(&mysql));
		//�ر����ݿ�
		mysql_close(&mysql);
		return false;
	}

	//printf("��ݺ˶Գɹ������ڻ�ȡ����...\n");

	// 3. ��ȡ���
	res = mysql_store_result(&mysql);
	row = mysql_fetch_row(res);

	if (row == NULL)
	{
		//�ͷŽ����
		mysql_free_result(res);
		//�ر����ݿ�
		mysql_close(&mysql);
		return false;
	}

	user.id = atoi(row[0]);
	user.level_id = atoi(row[1]);

	if(debug) printf("userid: %d  level_id: %d\n", user.id, user.level_id);

	// 4.���ؽ��
	//�ͷŽ����
	mysql_free_result(res);
	//�ر����ݿ�
	mysql_close(&mysql);
	return true;


}

/**********************************************************
 *���ܣ�ͨ���û��ؿ�id�����ݿ��ȡ��Ӧ�ؿ���Ϣ
 *���룺level �ؿ���Ϣ�ṹ��   level_id �û����ڹؿ���Ϣ
 *����ֵ���ɹ� true  ʧ�� false
 *********************************************************/
bool fetch_level_info(levelinfo& level, int level_id)
{
	MYSQL mysql;
	MYSQL_RES* res;
	MYSQL_ROW row;
	char sql[256];
	bool ret = false;

	// 1.���ӵ����ݿ�
	if (!connect_db(mysql))
	{
		printf("���ݿ�����ʧ��!\n");
		return false;
	}

	// 2.��ȡ�ؿ���Ϣ
	snprintf(sql, 256, "select name, map_row, map_column, map_data, next_level_id from levels where id = %d;", level_id);
	ret = mysql_query(&mysql, sql); // �ɹ�����0

	if (ret)
	{
		printf("��Ϣ��ȡʧ�ܣ�%s ����ԭ�� %s\n", sql, mysql_error(&mysql));
		//�ر����ݿ�
		mysql_close(&mysql);
		return false;
	}

	// 3. ��ȡ���
	res = mysql_store_result(&mysql);
	row = mysql_fetch_row(res);  // һ��level_idֻ��һ�У����Բ���ѭ����ȡ

	if (row == NULL)
	{
		//�ͷŽ����
		mysql_free_result(res);
		//�ر����ݿ�
		mysql_close(&mysql);
		return false;
	}

	level.id = level_id;
	level.name = row[0];
	level.row = atoi(row[1]);
	level.column = atoi(row[2]);
	level.map_data = row[3];
	level.next_level_id = atoi(row[4]);

	if (debug) printf("�ؿ�id: %d  �ؿ�name: %s  row: %d   column: %d   map_data: %s  next_level_id: %d\n",
		level.id, level.name.c_str(), level.row, level.column, level.map_data.c_str(), level.next_level_id);

	// 4.���ؽ��
	//�ͷŽ����
	mysql_free_result(res);
	//�ر����ݿ�
	mysql_close(&mysql);
	return true;
}


/**********************************************************
 *���ܣ����µ�ǰ�û���һ�صĵ�ͼ��Ϣ
 *���룺user.next_level_id �û���һ����Ϣ   next_level_id ��ͼ����
 *����ֵ���ɹ� true  ʧ�� false
 *********************************************************/
bool update_user_level(userinfo& user, int next_level_id) {
	MYSQL mysql;
	char sql[256];
	bool ret = false;

	// 1.���ӵ����ݿ�
	if (!connect_db(mysql))
	{
		printf("���ݿ�����ʧ��!\n");
		return false;
	}

	// 2.��ȡ�ؿ���Ϣ
	snprintf(sql, 256, "update users set level_id = %d where id = %d", next_level_id,user.id);
	ret = mysql_query(&mysql, sql); // �ɹ�����0

	if (ret)
	{
		printf("��Ϣ��ȡʧ�ܣ�%s ����ԭ�� %s\n", sql, mysql_error(&mysql));
		//�ر����ݿ�
		mysql_close(&mysql);
		return false;
	}


	// 4.���ؽ��
	//�ر����ݿ�
	mysql_close(&mysql);
	return true;
}


/**********************************************************
 *���ܣ������ݿ��õĵ�ͼ��Ϣת�浽������
 *���룺level.map_data  ��ͼ����     map ����
 *����ֵ���ɹ� true  ʧ�� false
 *********************************************************/
bool transform_map_data2array(levelinfo& level, int map[LINE][COLUMN])
{
	int start = 0;
	int end = 0;
	int row = 0, column = 0;;
	while (start < level.map_data.length()) {
		end = level.map_data.find("|", start);
		if ( end ==  std::string::npos) {
			end = level.map_data.length();
		}
		int n = end - start;
		string line = level.map_data.substr(start, n);
		if(debug) printf("get line: %s\n", line.c_str());

		// �Ե�ͼ��Ϣ���н���
		char* next_tok = NULL;
		char* pch = strtok_s((char *)line.c_str(),",", &next_tok);
		column = 0;
		while (pch)
		{
			if (debug) printf("%s ", pch);
			map[row][column] = atoi(pch);
			column++;
			pch = strtok_s(NULL, ",", &next_tok);
		}

		printf("\n");
		row++;
		start = end + 1;
	}
	if (row != level.row || column != level.column) {
		printf("��ͼ��Ϣ��ȡʧ�ܣ�������!\n");
		return false;
	}
	return true;
}


// 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0 | 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0|0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0

