#include <graphics.h>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <conio.h>
#include "database.h"
#include "boxman.h"

using namespace std;



IMAGE images[ALL];

struct _POS manPos; // С���ڶ�ά�����е�λ��

/*��Ϸ��ͼ*/
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

// �ж���Ϸ�Ƿ����
bool isGameOver() {
	for (int i = 0; i < LINE; i++) {
		for (int j = 0; j < COLUMN; j++)
		{
			if (map[i][j] == BOX_DES) return false;
		}
	}
	return true;
}

// ��Ϸ��������
void gameOverScene(IMAGE* bg)
{
	putimage(0, 0, bg);
	settextcolor(RED);
	RECT rec = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	settextstyle(40, 0, _T("����"));
	drawtext(_T("��ϲ��~\n�����ڳ�Ϊ��һ���ϸ����������˾����"), &rec, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
}

// �ı���Ϸ��ͼ��ͼ��һ���Ӧ���߲�������ʾ
// pos - �����ڵ�ͼ�����е������±�
// prop  -���ߵ�����
void changeMap(POS* pos, PROPS prop)
{
	map[pos->x][pos->y] = prop;
	putimage(START_X + pos->y * RATIO, START_Y + pos->x * RATIO, &images[prop]);
}

// ʵ����Ϸ�ĸ�����Ŀ���, λ����ʵ�������±�
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
	BOX_DES,   // 2 ����Ŀ�ĵ�
	MAN,       // 3
	BOX,       // 4
	HIT,       // 5 ��������Ŀ��
	ALL        // 6
};*/

// ��չ�� isValid(pos) pos.x >= 0 && pos.x < LINE && pos.y >= 0 && pos.y < COLUMN
	if (isValid(next_pos) && map[next_pos.x][next_pos.y] == FLOOR)//�˵�ǰ���ǵذ�
	{
		changeMap(&next_pos, MAN); // С��ǰ��һ��
		changeMap(&manPos, FLOOR);
		manPos = next_pos;
	}
	else if (isValid(next_pos) && map[next_pos.x][next_pos.y] == BOX)
	{
		if (map[next_next_pos.x][next_next_pos.y] == FLOOR)
		{
			changeMap(&next_next_pos, BOX);
			changeMap(&next_pos, MAN); // С��ǰ��һ��
			changeMap(&manPos, FLOOR);
			manPos = next_pos;
		}
		else if (map[next_next_pos.x][next_next_pos.y] == BOX_DES)
		{
			changeMap(&next_next_pos, HIT);
			changeMap(&next_pos, MAN); // С��ǰ��һ��
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
		cout << "�������û�����";
		cin >> user.username;

		cout << "���������룺";
		cin >> user.passwd;

		ret = fetch_user_info(user);
		times++;
		if (times >= MAX_RETRY_TIMES) break;

		if (ret == false) {
			cout << "��¼ʧ�ܣ����������룡" << endl;
		}

	} while (!ret);

	return ret;
}

int main(void) {
	// �û������֤
	userinfo user;
	levelinfo level;
	bool ret = false;

	if (login(user) == false) {
		cout << "��¼ʧ�ܣ������µ�¼��" << endl;
		::system("pause");
		exit(-1);
	}
	else {
		cout << "��¼�ɹ���" << endl;
		::system("pause");
	}

	// ��ȡ�ؿ���Ϣ
	ret = fetch_level_info(level, user.level_id);
	if (!ret) {
		cout << "��¼�ؿ�����ʧ�ܣ������ԣ�" << endl;
		::system("pause");
		exit(-1);
	}
	else {
		cout << "�ؿ���Ϣ��ȡ�ɹ����������ڹؿ���" << level.id << " �ؿ�����" << level.name << " �뿪ʼ���ı��ݣ�" << endl;
		::system("pause");
	}

	// �����ݿ��ͼ����ת�浽��ά������
	/*int rev = transform_map_data2array(level, map);
	if (!rev) {
		cout << "��ͼ�������������ԣ�" << endl;
		::system("pause");
		exit(-1);
	}
	else {
		cout << "��ͼ��Ϣ��ȡ�ɹ���" << endl;
		::system("pause");
	}*/

	IMAGE bg_img;

	//��̨��Ϸ
	initgraph(SCREEN_WIDTH, SCREEN_HEIGHT);
	loadimage(&bg_img, _T("blackground.bmp"), SCREEN_WIDTH, SCREEN_HEIGHT, true);
	putimage(0, 0, &bg_img);

	//���ص���ͼ��  RATIO - ͼƬ��С
	loadimage(&images[WALL], _T("wall_right.bmp"), RATIO, RATIO, true);
	loadimage(&images[FLOOR], _T("floor.bmp"), RATIO, RATIO, true);
	loadimage(&images[BOX_DES], _T("des.bmp"), RATIO, RATIO, true);
	loadimage(&images[MAN], _T("man.bmp"), RATIO, RATIO, true);
	loadimage(&images[BOX], _T("box.bmp"), RATIO, RATIO, true);
	loadimage(&images[HIT], _T("box.bmp"), RATIO, RATIO, true);

	for (int i = 0; i < LINE; i++) {
		for (int j = 0; j < COLUMN; j++)
		{
			if (map[i][j] == MAN)  // ��������ֵΪ3�����趨Ϊ�˵�λ��
			{
				manPos.x = i;
				manPos.y = j;
			}

			putimage(START_X + j * RATIO, START_Y + i * RATIO, &images[map[i][j]]);
		}
	}

	// ��Ϸ����
	bool quit = false;

	do {
		if (_kbhit()) { // ��Ұ���
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

	//��Ϸ�������ͷ���Դ
	closegraph();
	return 0;
}