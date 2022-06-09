#pragma once

#include <graphics.h>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <conio.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 650

#define RATIO 50

// 控制键
#define KEY_UP		'w'
#define KEY_LEFT    'a'
#define KEY_RIGHT   'd'
#define KEY_DOWN    's'
#define KEY_QUIT    'q'



#define START_X 100
#define START_Y 100

#define MAX_RETRY_TIMES 4

typedef enum _PROPS        PROPS;
typedef enum _DIRECTION    DIRECTION;
typedef struct _POS        POS;

#define isValid(pos) pos.x >= 0 && pos.x < LINE && pos.y >= 0 && pos.y < COLUMN

// 道具的类型
enum _PROPS {
	WALL,      // 0
	FLOOR,     // 1
	BOX_DES,   // 2 箱子目的地
	MAN,       // 3
	BOX,       // 4
	HIT,       // 5 箱子命中目标
	ALL        // 6
};

// 游戏控制方向
enum _DIRECTION
{
	UP,
	DOWN,
	LEFT,
	RIGHT
};

struct _POS {
	int x;  // 小人所在的二维数组的行
	int y;  
};






