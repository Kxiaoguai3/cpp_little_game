#include <cstdio>
#include <easyx.h>
#include "tools.h"
#include <time.h>
#include<mmSystem.h>   //��������
#pragma comment (lib,"winmm.lib")	//����winmm.lib��

#define BULLET_MANY 10    //�ӵ�������
#define ENEMY_MANY 20     //���˵�����

//��������
void createBllet();
void cai();

int diffcutly = 1000;

bool enemy_tmp;

//ͼƬ
IMAGE bk;
IMAGE player[3];
IMAGE BULLET;
IMAGE ENEMY;
IMAGE Cai;



//�Զ����ʱ��
bool Timer(int ms, int id) {
	static int start[5];
	int end = clock();
	if (end - start[id] >= ms) {
		start[id] = end;
		return true;
	}
	return false;;
}

//��������ͼƬ
void lodeing() {
	//����
	initgraph(1320, 750);   // 439, 700

	//����
	loadimage(&bk, "photo/sky1.jpg");

	//���
	loadimage(player + 0, "photo/plane1.png");
	loadimage(player + 1, "photo/plane2.png");
	loadimage(player + 2, "photo/plane3.png");

	//�ӵ�
	loadimage(&BULLET, "photo/bullet.png");

	//����
	loadimage(&ENEMY, "photo/COVID-19.png");

	//��
	loadimage(&Cai, "photo/cai.png");
}

//�ɻ�����
struct Plane
{
	int x;
	int y;
	bool isDie;
	int flg;
}gamer;

Plane enemy[ENEMY_MANY];

//�ӵ�����
struct Bullet
{
	int x;
	int y;
	bool isDie;
}bullet[BULLET_MANY];

//��ʼ���ɻ�
void Plane_init(Plane* pthis, int x, int y) {
	pthis->x = x;
	pthis->y = y;
	pthis->isDie = false;
	pthis->flg = 0;
}

//�������ݳ�ʼ��
void init() {
	//��ʼ����ҷɻ�
	Plane_init(&gamer, getwidth() / 2 - player->getwidth() / 2, getheight() - player->getheight());

	//��ʼ���ӵ�
	for (int i = 0; i < BULLET_MANY; ++i) {  
		bullet[i].isDie = true;
	}  ////ȫ������״̬

	//��ʼ������
	for (int i = 0; i < ENEMY_MANY; i++)
	{
		enemy[i].isDie = true;
	}
}

//�����ӵ�
void createBllet() {
	for (int i = 0; i < BULLET_MANY; i++)
	{
		if (bullet[i].isDie) {
			bullet[i].x = player->getwidth() / 2 + gamer.x - 10;
			bullet[i].y = gamer.y - 15;
			bullet[i].isDie = false;  //�����ӵ�
			break;
		}
	}
}

//�ӵ��ƶ�
void bullet_move() {
	for (int i = 0; i < BULLET_MANY; ++i) {
		//�ƶ�
		
		if (!bullet[i].isDie) {
			bullet[i].y -= 10;
		}
		//�ӵ���ǽ�����У�����
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

//�ƶ��ɻ�
void Plane_move(Plane* pthis) {
	//��ȡ���̰���
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

	//�����ӵ�
	if (GetAsyncKeyState(VK_SPACE) && Timer(100, 0)) {
		//�����ӵ�
		createBllet();
	}

	//�Ƿ�����
	for (int i = 0; i < ENEMY_MANY; ++i) {
		if (!enemy[i].isDie && !gamer.isDie) {
			if (gamer.x + player->getwidth() / 2 > enemy[i].x && gamer.x + player->getwidth() / 2 < enemy[i].x + ENEMY.getwidth() && (gamer.y < enemy[i].y + ENEMY.getheight() - 20 && gamer.y > enemy[i].y || gamer.y + player->getheight() > enemy[i].y && gamer.y + player->getheight() < enemy[i].y + ENEMY.getheight())) {
				gamer.isDie = true;
				cai();
			}
		}
	}
}

//�Զ���������
void creacteEnemy() {
	for (int i = 0; i < ENEMY_MANY; ++i) {
		if (enemy[i].isDie) {
			//�������
			enemy[i].x = rand() % getwidth();
			enemy[i].y = -50;

			enemy[i].isDie = false;
			break;
		}
	}
}

//�ƶ�����
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

//���Ʒɻ�
void draw_plane(Plane* pthis) {
	//���
	drawImg(pthis->x, pthis->y, player + pthis->flg);
	pthis->flg = (pthis->flg + 1) % 3;
}

/*
//�жϻ���
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

//����
void draw() {
	//BeginBatchDraw();
	//����
	putimage(0, 0, &bk);
	//���
	draw_plane(&gamer);
	//�ӵ�
	for (int i = 0; i < BULLET_MANY; i++)
	{
		if (!bullet[i].isDie) {
			drawImg(bullet[i].x, bullet[i].y, &BULLET);
		}

	}
	//����
	for (int i = 0; i < ENEMY_MANY; ++i) {
		if (!enemy[i].isDie) {
			drawImg(enemy[i].x, enemy[i].y, &ENEMY);
		}
		
	}
	
	
	
}

//����
void cai() {
	drawImg(600, 300, &Cai);
}

int main() {
	mciSendString(("open Musec/backMusec.mp3 alias song"), NULL, 0, NULL);
	mciSendString(TEXT(" play song"), NULL, 0, NULL);

	lodeing(); //����ͼƬ
	init();    //��ʼ����������

	while (true)
	{
		//��ʼʱ��
		int startTime = clock();

		//��ʼѭ��
		Plane_move(&gamer);
		if (gamer.isDie) {

			mciSendString(("closs song"), NULL, 0, NULL); // ֹͣ����
			Sleep(10000000);
		}
		draw();
		//�ӵ��ƶ�
		bullet_move();
		
		//���˳��ֵ�ʱ��
		if (Timer(510, 1)) {
		//��������
		creacteEnemy();
		}


		//�����ƶ�
		enemy_move();

		//�ж�����
		//Die_or_Life();

		//����ʱ��
		int Time = clock() - startTime;

		//һ֡��ʱ�䣨��ǰ����ˣ�
		if (1000 / 40 - Time > 0) {
			Sleep(1000 / 40 - Time);
		}
	}
}