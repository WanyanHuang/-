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
 *功能：连接数据库
 *输入：myslql句柄
 *返回值：成功 true  失败 false
 *********************************************************/
bool connect_db(MYSQL& mysql)
{
	//1.初始化数据库句柄
	mysql_init(&mysql);

	//2.设置字符编码
	mysql_options(&mysql, MYSQL_SET_CHARSET_NAME, "gbk");  // gbk

	//3.连接数据库
	if (mysql_real_connect(&mysql, DB_HOST, DB_USER, DB_USER_PASSWD, DB_NAME, DB_POST, NULL, 0) == NULL)
	{
		printf("数据库连接出错，错误原因： %s\n", mysql_error(&mysql));
		return false;
	}

	return true;
}

/**********************************************************
 *功能：通过用户名和密码从数据库获取用户信息
 *输入：user 用户信息结构体
 *返回值：成功 true  失败 false
 *********************************************************/
bool fetch_user_info(userinfo & user)
{
	MYSQL mysql;
	MYSQL_RES* res; // 查询结果集
	MYSQL_ROW row; // 记录结构体
	char sql[256];
	bool ret = false;

	// 1.连接到数据库
	if (connect_db(mysql) == false)
	{
		return false;
	}
	//printf("数据库连接成功！正在核对游戏身份信息...\n");

	// 2.获取用户信息
	snprintf(sql, 256, "select id, level_id from users where username = '%s' and password = md5('%s');", user.username.c_str(), user.passwd.c_str());
	ret = mysql_query(&mysql, sql); // 成功返回0

	if (ret)
	{
		printf("信息获取失败，%s 错误原因： %s\n", sql,mysql_error(&mysql));
		//关闭数据库
		mysql_close(&mysql);
		return false;
	}

	//printf("身份核对成功，正在获取数据...\n");

	// 3. 获取结果
	res = mysql_store_result(&mysql);
	row = mysql_fetch_row(res);

	if (row == NULL)
	{
		//释放结果集
		mysql_free_result(res);
		//关闭数据库
		mysql_close(&mysql);
		return false;
	}

	user.id = atoi(row[0]);
	user.level_id = atoi(row[1]);

	if(debug) printf("userid: %d  level_id: %d\n", user.id, user.level_id);

	// 4.返回结果
	//释放结果集
	mysql_free_result(res);
	//关闭数据库
	mysql_close(&mysql);
	return true;


}

/**********************************************************
 *功能：通过用户关卡id从数据库获取对应关卡信息
 *输入：level 关卡信息结构体   level_id 用户所在关卡信息
 *返回值：成功 true  失败 false
 *********************************************************/
bool fetch_level_info(levelinfo& level, int level_id)
{
	MYSQL mysql;
	MYSQL_RES* res;
	MYSQL_ROW row;
	char sql[256];
	bool ret = false;

	// 1.连接到数据库
	if (!connect_db(mysql))
	{
		printf("数据库连接失败!\n");
		return false;
	}

	// 2.获取关卡信息
	snprintf(sql, 256, "select name, map_row, map_column, map_data, next_level_id from levels where id = %d;", level_id);
	ret = mysql_query(&mysql, sql); // 成功返回0

	if (ret)
	{
		printf("信息获取失败，%s 错误原因： %s\n", sql, mysql_error(&mysql));
		//关闭数据库
		mysql_close(&mysql);
		return false;
	}

	// 3. 获取结果
	res = mysql_store_result(&mysql);
	row = mysql_fetch_row(res);  // 一个level_id只有一行，所以不用循环获取

	if (row == NULL)
	{
		//释放结果集
		mysql_free_result(res);
		//关闭数据库
		mysql_close(&mysql);
		return false;
	}

	level.id = level_id;
	level.name = row[0];
	level.row = atoi(row[1]);
	level.column = atoi(row[2]);
	level.map_data = row[3];
	level.next_level_id = atoi(row[4]);

	if (debug) printf("关卡id: %d  关卡name: %s  row: %d   column: %d   map_data: %s  next_level_id: %d\n",
		level.id, level.name.c_str(), level.row, level.column, level.map_data.c_str(), level.next_level_id);

	// 4.返回结果
	//释放结果集
	mysql_free_result(res);
	//关闭数据库
	mysql_close(&mysql);
	return true;
}


/**********************************************************
 *功能：更新当前用户下一关的地图信息
 *输入：user.next_level_id 用户下一关信息   next_level_id 地图数据
 *返回值：成功 true  失败 false
 *********************************************************/
bool update_user_level(userinfo& user, int next_level_id) {
	MYSQL mysql;
	char sql[256];
	bool ret = false;

	// 1.连接到数据库
	if (!connect_db(mysql))
	{
		printf("数据库连接失败!\n");
		return false;
	}

	// 2.获取关卡信息
	snprintf(sql, 256, "update users set level_id = %d where id = %d", next_level_id,user.id);
	ret = mysql_query(&mysql, sql); // 成功返回0

	if (ret)
	{
		printf("信息获取失败，%s 错误原因： %s\n", sql, mysql_error(&mysql));
		//关闭数据库
		mysql_close(&mysql);
		return false;
	}


	// 4.返回结果
	//关闭数据库
	mysql_close(&mysql);
	return true;
}


/**********************************************************
 *功能：将数据库获得的地图信息转存到数组中
 *输入：level.map_data  地图数据     map 数组
 *返回值：成功 true  失败 false
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

		// 对地图信息进行解析
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
		printf("地图信息获取失败，请重试!\n");
		return false;
	}
	return true;
}


// 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0 | 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0|0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0

