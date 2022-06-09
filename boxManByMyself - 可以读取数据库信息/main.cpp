#include <graphics.h>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <conio.h>
#include "database.h"
#include "boxman.h"

using namespace std;



IMAGE images[ALL];

struct _POS manPos; // 小人在二维数组中的位置

/*游戏地图*/
//int map[LINE][COLUMN];
int map[LINE][COLUMN] = {
		{ 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0 },
		{ 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0 },
		{ 0, 1, 4, 1, 0, 2, 1, 0, 2, 1, 0, 0 },
		{ 0, 1, 0, 1, 0, 1, 0, 0, 1, 1, 1, 0 },
		{ 0, 1, 0, 2, 0, 1, 1, 4, 1, 1, 1, 0 },
		{ 0, 1, 1, 1, 0, 3, 1, 1, 1, 4, 1, 0 },
		{ 0, 1, 2, 1, 1, 4, 1, 1, 1, 1, 1, 0 },
		{ 0, 1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 0 },
		{ 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0 }
};

// 判断游戏是否结束
bool isGameOver() {
	for (int i = 0; i < LINE; i++) {
		for (int j = 0; j < COLUMN; j++)
		{
			if (map[i][j] == BOX_DES) return false;
		}
	}
	return true;
}

// 游戏结束场景
void gameOverScene(IMAGE* bg)
{
	putimage(0, 0, bg);
	settextcolor(RED);
	RECT rec = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	settextstyle(40, 0, _T("宋体"));
	drawtext(_T("恭喜您~\n您终于成为了一个合格的推箱子老司机！"), &rec, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
}

// 改变游戏地图视图中一格对应道具并重新显示
// pos - 道具在地图数组中的行列下标
// prop  -道具的类型
void changeMap(POS* pos, PROPS prop)
{
	map[pos->x][pos->y] = prop;
	putimage(START_X + pos->y * RATIO, START_Y + pos->x * RATIO, &images[prop]);
}

// 实现游戏四个方向的控制, 位置其实是数组下标
void gameControl(DIRECTION direct)
{
	POS next_pos = manPos;
	POS next_next_pos = manPos;
	switch (direct)
	{
	case UP:
		next_pos.x--;
		next_next_pos.x -= 2;
		break;
	case DOWN:
		next_pos.x++;
		next_next_pos.x += 2;
		break;
	case LEFT:
		next_pos.y--;
		next_next_pos.y -= 2;
		break;
	case RIGHT:
		next_pos.y++;
		next_next_pos.y += 2;
		break;
	default:
		break;
	}

	/*enum _PROPS {
	WALL,      // 0
	FLOOR,     // 1
	BOX_DES,   // 2 箱子目的地
	MAN,       // 3
	BOX,       // 4
	HIT,       // 5 箱子命中目标
	ALL        // 6
};*/

// 宏展开 isValid(pos) pos.x >= 0 && pos.x < LINE && pos.y >= 0 && pos.y < COLUMN
	if (isValid(next_pos) && map[next_pos.x][next_pos.y] == FLOOR)//人的前面是地板
	{
		changeMap(&next_pos, MAN); // 小人前进一格
		changeMap(&manPos, FLOOR);
		manPos = next_pos;
	}
	else if (isValid(next_pos) && map[next_pos.x][next_pos.y] == BOX)
	{
		if (map[next_next_pos.x][next_next_pos.y] == FLOOR)
		{
			changeMap(&next_next_pos, BOX);
			changeMap(&next_pos, MAN); // 小人前进一格
			changeMap(&manPos, FLOOR);
			manPos = next_pos;
		}
		else if (map[next_next_pos.x][next_next_pos.y] == BOX_DES)
		{
			changeMap(&next_next_pos, HIT);
			changeMap(&next_pos, MAN); // 小人前进一格
			changeMap(&manPos, FLOOR);
			manPos = next_pos;
		}
	}
}


bool login(userinfo& user)
{
	int times = 0;
	bool ret = false;

	do {
		cout << "请输入用户名：";
		cin >> user.username;

		cout << "请输入密码：";
		cin >> user.passwd;

		ret = fetch_user_info(user);
		times++;
		if (times >= MAX_RETRY_TIMES) break;

		if (ret == false) {
			cout << "登录失败，请重新输入！" << endl;
		}

	} while (!ret);

	return ret;
}

int main(void) {
	// 用户身份验证
	userinfo user;
	levelinfo level;
	bool ret = false;

	if (login(user) == false) {
		cout << "登录失败，请重新登录！" << endl;
		::system("pause");
		exit(-1);
	}
	else {
		cout << "登录成功！" << endl;
		::system("pause");
	}

	// 获取关卡信息
	ret = fetch_level_info(level, user.level_id);
	if (!ret) {
		cout << "登录关卡数据失败，请重试！" << endl;
		::system("pause");
		exit(-1);
	}
	else {
		cout << "关卡信息获取成功，您现在在关卡：" << level.id << " 关卡名：" << level.name << " 请开始您的表演！" << endl;
		::system("pause");
	}

	// 将数据库地图数据转存到二维数组中
	/*int rev = transform_map_data2array(level, map);
	if (!rev) {
		cout << "地图数据有误，请重试！" << endl;
		::system("pause");
		exit(-1);
	}
	else {
		cout << "地图信息获取成功！" << endl;
		::system("pause");
	}*/

	IMAGE bg_img;

	//搭台唱戏
	initgraph(SCREEN_WIDTH, SCREEN_HEIGHT);
	loadimage(&bg_img, _T("blackground.bmp"), SCREEN_WIDTH, SCREEN_HEIGHT, true);
	putimage(0, 0, &bg_img);

	//加载道具图标  RATIO - 图片大小
	loadimage(&images[WALL], _T("wall_right.bmp"), RATIO, RATIO, true);
	loadimage(&images[FLOOR], _T("floor.bmp"), RATIO, RATIO, true);
	loadimage(&images[BOX_DES], _T("des.bmp"), RATIO, RATIO, true);
	loadimage(&images[MAN], _T("man.bmp"), RATIO, RATIO, true);
	loadimage(&images[BOX], _T("box.bmp"), RATIO, RATIO, true);
	loadimage(&images[HIT], _T("box.bmp"), RATIO, RATIO, true);

	for (int i = 0; i < LINE; i++) {
		for (int j = 0; j < COLUMN; j++)
		{
			if (map[i][j] == MAN)  // 如果矩阵的值为3，则设定为人的位置
			{
				manPos.x = i;
				manPos.y = j;
			}

			putimage(START_X + j * RATIO, START_Y + i * RATIO, &images[map[i][j]]);
		}
	}

	// 游戏环节
	bool quit = false;

	do {
		if (_kbhit()) { // 玩家按键
			char ch = _getch();

			if (ch == KEY_UP)
			{
				gameControl(UP);
			}
			else if (ch == KEY_DOWN)
			{
				gameControl(DOWN);
			}
			else if (ch == KEY_LEFT)
			{
				gameControl(LEFT);
			}
			else if (ch == KEY_RIGHT)
			{
				gameControl(RIGHT);
			}
			else if (ch == KEY_QUIT)
			{
				quit = true;
			}
			if (isGameOver())
			{
				gameOverScene(&bg_img);
				quit = true;
			}
		}
		Sleep(100);
	} while (quit == false);

	::system("pause");

	//游戏结束，释放资源
	closegraph();
	return 0;
}