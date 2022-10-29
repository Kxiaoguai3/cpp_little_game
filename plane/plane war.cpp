#include <cstdio>
#include <easyx.h>
#include "tools.h"
#include <time.h>
#include<mmSystem.h>   //播放音乐
#pragma comment (lib,"winmm.lib")	//导入winmm.lib库

#define BULLET_MANY 10    //子弹的数量
#define ENEMY_MANY 20     //敌人的数量

//声明函数
void createBllet();
void cai();

int diffcutly = 1000;

bool enemy_tmp;

//图片
IMAGE bk;
IMAGE player[3];
IMAGE BULLET;
IMAGE ENEMY;
IMAGE Cai;



//自定义计时器
bool Timer(int ms, int id) {
	static int start[5];
	int end = clock();
	if (end - start[id] >= ms) {
		start[id] = end;
		return true;
	}
	return false;;
}

//加载所有图片
void lodeing() {
	//窗口
	initgraph(1320, 750);   // 439, 700

	//背景
	loadimage(&bk, "photo/sky1.jpg");

	//玩家
	loadimage(player + 0, "photo/plane1.png");
	loadimage(player + 1, "photo/plane2.png");
	loadimage(player + 2, "photo/plane3.png");

	//子弹
	loadimage(&BULLET, "photo/bullet.png");

	//敌人
	loadimage(&ENEMY, "photo/COVID-19.png");

	//菜
	loadimage(&Cai, "photo/cai.png");
}

//飞机属性
struct Plane
{
	int x;
	int y;
	bool isDie;
	int flg;
}gamer;

Plane enemy[ENEMY_MANY];

//子弹属性
struct Bullet
{
	int x;
	int y;
	bool isDie;
}bullet[BULLET_MANY];

//初始化飞机
void Plane_init(Plane* pthis, int x, int y) {
	pthis->x = x;
	pthis->y = y;
	pthis->isDie = false;
	pthis->flg = 0;
}

//所有数据初始化
void init() {
	//初始化玩家飞机
	Plane_init(&gamer, getwidth() / 2 - player->getwidth() / 2, getheight() - player->getheight());

	//初始化子弹
	for (int i = 0; i < BULLET_MANY; ++i) {  
		bullet[i].isDie = true;
	}  ////全是死亡状态

	//初始化敌人
	for (int i = 0; i < ENEMY_MANY; i++)
	{
		enemy[i].isDie = true;
	}
}

//创建子弹
void createBllet() {
	for (int i = 0; i < BULLET_MANY; i++)
	{
		if (bullet[i].isDie) {
			bullet[i].x = player->getwidth() / 2 + gamer.x - 10;
			bullet[i].y = gamer.y - 15;
			bullet[i].isDie = false;  //生成子弹
			break;
		}
	}
}

//子弹移动
void bullet_move() {
	for (int i = 0; i < BULLET_MANY; ++i) {
		//移动
		
		if (!bullet[i].isDie) {
			bullet[i].y -= 10;
		}
		//子弹遇墙，遇敌，死亡
		if (bullet[i].y < -2) {
			bullet[i].isDie = true;
		}
		if (!bullet[i].isDie) {
			for (int j = 0; j < ENEMY_MANY; j++)
			{
				if (!enemy[j].isDie) {

					if (bullet[i].x < enemy[j].x + 95 && bullet[i].x > enemy[j].x && bullet[i].y <= enemy[j].y + 56 - 10) {
						enemy[j].isDie = true;
						bullet[i].isDie = true;
						break;
					}
				}
			}
		}
	}
}

//移动飞机
void Plane_move(Plane* pthis) {
	//获取键盘按键
	if (GetAsyncKeyState(VK_UP) && pthis->y + player->getheight() / 2 > 0) {
		pthis->y -= 12;
	}
	if (GetAsyncKeyState(VK_DOWN) && pthis->y + player->getheight() / 2 < getheight()) {
		pthis->y += 12;
	}
	if (GetAsyncKeyState(VK_LEFT) && pthis->x + player->getwidth() / 2 > 0) {
		pthis->x -= 12;
	}
	if (GetAsyncKeyState(VK_RIGHT) && pthis->x + player->getwidth() / 2 < getwidth()) {
		pthis->x += 12;
	}

	//发射子弹
	if (GetAsyncKeyState(VK_SPACE) && Timer(100, 0)) {
		//发射子弹
		createBllet();
	}

	//是否死亡
	for (int i = 0; i < ENEMY_MANY; ++i) {
		if (!enemy[i].isDie && !gamer.isDie) {
			if (gamer.x + player->getwidth() / 2 > enemy[i].x && gamer.x + player->getwidth() / 2 < enemy[i].x + ENEMY.getwidth() && (gamer.y < enemy[i].y + ENEMY.getheight() - 20 && gamer.y > enemy[i].y || gamer.y + player->getheight() > enemy[i].y && gamer.y + player->getheight() < enemy[i].y + ENEMY.getheight())) {
				gamer.isDie = true;
				cai();
			}
		}
	}
}

//自动创建敌人
void creacteEnemy() {
	for (int i = 0; i < ENEMY_MANY; ++i) {
		if (enemy[i].isDie) {
			//随机坐标
			enemy[i].x = rand() % getwidth();
			enemy[i].y = -50;

			enemy[i].isDie = false;
			break;
		}
	}
}

//移动敌人
void enemy_move() {
	for (int i = 0; i < ENEMY_MANY; ++i) {
		if (!enemy[i].isDie) {
			enemy[i].y += 4;
		}
		if(enemy[i].y > getheight()) {
			enemy[i].isDie = true;

		}
	}
}

//绘制飞机
void draw_plane(Plane* pthis) {
	//玩家
	drawImg(pthis->x, pthis->y, player + pthis->flg);
	pthis->flg = (pthis->flg + 1) % 3;
}

/*
//判断击中
void Die_or_Life() {
	for (int i = 0; i < BULLET_MANY; ++i) {
		if (!bullet[i].isDie)
		{
			for (int j = 0; j < ENEMY_MANY; ++j) {
				if (!enemy[i].isDie) {
					if(bullet[i].x < enemy[j].x + ENEMY.getwidth() && bullet[i].x >= enemy[j].x && bullet[i].y <= enemy[j].y && bullet[i].y >= enemy[j].y + ENEMY.getheight()){
						bullet[i].isDie = true;
						enemy[j].isDie = true;
						break;
					}

				}
			}
		}
	}
}
*/

//绘制
void draw() {
	//BeginBatchDraw();
	//背景
	putimage(0, 0, &bk);
	//玩家
	draw_plane(&gamer);
	//子弹
	for (int i = 0; i < BULLET_MANY; i++)
	{
		if (!bullet[i].isDie) {
			drawImg(bullet[i].x, bullet[i].y, &BULLET);
		}

	}
	//敌人
	for (int i = 0; i < ENEMY_MANY; ++i) {
		if (!enemy[i].isDie) {
			drawImg(enemy[i].x, enemy[i].y, &ENEMY);
		}
		
	}
	
	
	
}

//嘲讽
void cai() {
	drawImg(600, 300, &Cai);
}

int main() {
	mciSendString(("open Musec/backMusec.mp3 alias song"), NULL, 0, NULL);
	mciSendString(TEXT(" play song"), NULL, 0, NULL);

	lodeing(); //加载图片
	init();    //初始化所有数据

	while (true)
	{
		//初始时间
		int startTime = clock();

		//开始循环
		Plane_move(&gamer);
		if (gamer.isDie) {

			mciSendString(("closs song"), NULL, 0, NULL); // 停止播放
			Sleep(10000000);
		}
		draw();
		//子弹移动
		bullet_move();
		
		//敌人出现的时间
		if (Timer(510, 1)) {
		//创建敌人
		creacteEnemy();
		}


		//敌人移动
		enemy_move();

		//判断死活
		//Die_or_Life();

		//过程时间
		int Time = clock() - startTime;

		//一帧的时间（提前完成了）
		if (1000 / 40 - Time > 0) {
			Sleep(1000 / 40 - Time);
		}
	}
}