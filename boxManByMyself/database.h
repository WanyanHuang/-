#pragma once
#include <string>
#include <mysql.h>

using namespace std;

#define LINE 48
#define COLUMN 48

typedef struct _userinfo {
	int id;
	string username;
	string passwd;
	int level_id;
}userinfo;

typedef struct _levelinfo {
	int id;
	string name;
	int row;
	int column;
	string map_data;
	int next_level_id;
}levelinfo;

//��ȡ�û���Ϣ
bool fetch_user_info(userinfo& user);

//��ȡ�ؿ���Ϣ
bool fetch_level_info(levelinfo& level, int level_id);

//������һ�ص���Ϣ
bool update_user_level(userinfo& user, int next_level_id);


//�������ݿ�
bool connect_db(MYSQL& mysql);

// �����ݿ��ͼ����ת�浽��ά������
bool transform_map_data2array(levelinfo& level, int map[LINE][COLUMN]);