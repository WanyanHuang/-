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

//获取用户信息
bool fetch_user_info(userinfo& user);

//获取关卡信息
bool fetch_level_info(levelinfo& level, int level_id);

//更新下一关的信息
bool update_user_level(userinfo& user, int next_level_id);


//连接数据库
bool connect_db(MYSQL& mysql);

// 将数据库地图数据转存到二维数组中
bool transform_map_data2array(levelinfo& level, int map[LINE][COLUMN]);