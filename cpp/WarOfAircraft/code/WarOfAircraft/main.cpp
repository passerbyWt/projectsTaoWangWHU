#include<iostream>
#include<list>
#include<time.h>
#include<easyx.h>
#include<graphics.h>
#include"stdio.h"
#include"math.h"
#include "dos.h" 
#include<Windows.h>
#include<cstdlib>
#include<conio.h>
#pragma comment(lib, "WINMM.LIB")
#include <mmsystem.h>
#include"Object.h"
#include"Enemy.h"
#include"Bullet.h"
#include"Laser.h"
#include"Hero.h"
#include"Boss.h"
#include"Game.h"

#define KEY_DOWN(vk_c) (GetAsyncKeyState(vk_c)&0x8000)

using namespace std;

Hero hero(185, 650);  //初始化精灵机的位置
IMAGE background[5], allEnemy[10]; //各种图片对象
Laser L;//实例化一个激光对象
Boss boss(120, -80);//初始化Boss位置

//全局变量
int score = 0;

//实现子弹的移动
void moveAllBullet(int dir)
{
	//遍历链表
	for (auto it = Bullet::objectList.begin(); it != Bullet::objectList.end();)
	{
		it->move(dir);
		//当子弹越界删除这一成员，销毁子弹
		if (it->getCoordinate().y <= 0 || it->getCoordinate().x <= 0)
			it = Bullet::objectList.erase(it);
		//否指迭代器后移
		else
			it++;
	}
}

//检查玩家是否输掉游戏，镭射是否发射
bool check()
{
	void GameOver();
	int d = 50;
	//敌机迭代
	for (auto it = Enemy::objectList.begin(); it != Enemy::objectList.end(); it++)
	{
		//获取坐标
		auto tbullet = it->getCoordinate();
		//获取精灵机的坐标
		auto tpos = hero.getCoordinate();
		//敌机与导弹相撞
		if ((tbullet.x - tpos.x<= 127 && tbullet.x - tpos.x >= 0) && (tbullet.y - tpos.y <= 58 && tbullet.y - tpos.y>= 0))
		{
			return false;
		}
	}

	//获取镭射发射的信号
	if (L.getFlag())
	{
		//迭代敌机
		for (auto it = Enemy::objectList.begin(); it != Enemy::objectList.end(); )
		{
			//获取敌机和精灵机的坐标
			Coordinate tp = it->getCoordinate(), hp = hero.getCoordinate();
			//abs()为取绝对值函数，当敌机与精灵机的距离小于20px时
			if (abs(tp.x - hp.x) < 20)
			{
				//添加镭射
				L.add(tp.x, tp.y);
				//删除敌机
				it = Enemy::objectList.erase(it);
				score += 2;
			}
			else
				it++;
		}
	}

	//子弹的判定
	for (auto it = Bullet::objectList.begin(); it != Bullet::objectList.end();)
	{
		//获取子弹的坐标
		auto tbullet = it->getCoordinate();
		bool f = false;//子弹是否击中的标志
		//敌机迭代
		for (auto k = Enemy::objectList.begin(); k != Enemy::objectList.end();)
		{
			//获取敌机坐标
			auto tpos = k->getCoordinate();

			//子弹与敌机的距离
			tpos.x += 24;
			tpos.y += 24;

			//指定距离内击中敌机
			if ((tbullet.x - tpos.x<d && tbullet.x - tpos.x>-d) && (tbullet.y - tpos.y<d && tbullet.y - tpos.y>-d))
			{
				f = true;						//
				L.add(tpos.x, tpos.y);          //添加镭射
				k = Enemy::objectList.erase(k);			//删除敌机
				break;
			}
			else k++;
		}

		//子弹击中在链表中删除子弹
		if (!f)
		{
			it++;
		}
		else
		{
			it = Bullet::objectList.erase(it);
			score += 1;
		}
	}
	return true;
}

//画出敌机
void drawAllPlane()
{
	for (auto it = Enemy::objectList.begin(); it != Enemy::objectList.end(); it++)
	{
		it->draw();
	}
}

//所有敌机的移动
void moveAllEnemy(int dir)
{
	for (auto it = Enemy::objectList.begin(); it != Enemy::objectList.end();)
	{
		it->move(dir);
		//敌机出界，在链表中删除敌机
		if (it->getCoordinate().y >= 768)
			it = Enemy::objectList.erase(it);
		else
			it++;
	}
}

void init()//初始化窗口
{
	InitGame();
	CheckInitSuccess();

	//加载音乐
	mciSendString(_T("open res\\music.mp3 alias mysong"), NULL, 0, NULL);	//打开背景音乐
	mciSendStringW(_T("play mysong repeat"), NULL, 0, NULL);			//循环播放音乐

	//先调用镭射对象的初始化函数，以便第一时间获取游戏开始时间
	L.initLaser();
	//加载各种飞机的图片
	loadimage(&allEnemy[0], _T("res\\images\\enemy1.png"), 100, 100);
	loadimage(&allEnemy[1], _T("res\\images\\enemy2.png"), 100, 100);
	loadimage(&allEnemy[2], _T("res\\images\\enemy3.png"), 100, 100);
	loadimage(&allEnemy[3], _T("res\\images\\enemy4.png"), 100, 100);
	loadimage(&allEnemy[4], _T("res\\images\\smallenemy.png"), 100, 100);
	loadimage(&background[0], _T("res\\images\\background1.jpg"), 512, 768, true);
	loadimage(&background[1], _T("res\\images\\background2.jpg"), 512, 768, true);
	loadimage(&background[2], _T("res\\images\\background3.jpg"), 512, 768, true);
	loadimage(&background[3], _T("res\\images\\background4.jpg"), 512, 768, true);
	loadimage(&background[4], _T("res\\images\\background5.jpg"), 512, 768, true);
	//设置随机数种子
	srand((unsigned)time(NULL));
	//初始化窗口
	initgraph(512, 768);
}

int difficulitySelection()
{

	//加载背景图片
	loadimage(NULL, _T("res\\images\\background.jpg"), 512, 768, true);

	setbkmode(TRANSPARENT);				// 字体透明
	settextcolor(BLACK);				//字体颜色
	settextstyle(60, 40, _T("黑体"));	//字体样式
	outtextxy(85, 20, _T("飞机大战"));	//输出字体于指定位置
	settextstyle(20, 10, _T("宋体"));
	outtextxy(300, 90, _T("by Tao Wang, Zhao Pu and Longlong"));


	settextstyle(20, 10, _T("宋体"));
	outtextxy(80, 300, _T("a. 简单(小敌机拥有较慢的移动速度)"));
	settextstyle(20, 10, _T("宋体"));
	outtextxy(80, 330, _T("d. 困难(小敌机拥有较快的移动速度)"));

	settextstyle(20, 10, _T("宋体"));
	outtextxy(20, 430, _T("Tips:"));
	settextstyle(20, 10, _T("宋体"));
	outtextxy(20, 460, _T("1. 按↑↓←→上下左右移动"));
	settextstyle(20, 10, _T("宋体"));
	outtextxy(20, 490, _T("2. space发射子弹, shift发射激光"));
	settextstyle(20, 10, _T("宋体"));
	outtextxy(20, 520, _T("3. 镭射击毁得2分，子弹击毁得1分"));
	settextstyle(20, 10, _T("宋体"));
	outtextxy(20, 550, _T("4. Boss不能被击败，导弹可以被击毁"));
	settextstyle(20, 10, _T("宋体"));
	outtextxy(20, 580, _T("5. 按下a, b键选择难度开始游戏"));

	
	int selection = _getch();

	while (1)
	{
		if (selection == 'a' || selection == 'A' || selection == 'd' || selection == 'D')
		{
			return selection;
		}
		else
		{
			selection = _getch();
		}
	}
}

int mapSelection()
{
	cleardevice();
	loadimage(NULL, _T("res\\images\\background.jpg"), 512, 768, true);//加载背景图片
	setbkmode(TRANSPARENT);				// 字体透明
	settextcolor(BLACK);				//字体颜色
	settextstyle(60, 40, _T("宋体"));	//字体样式，长50px, 宽30px
	outtextxy(85, 10, _T("  地图"));	//输出字体于指定位置
	outtextxy(85, 100, _T("a. 森林"));
	outtextxy(85, 190, _T("b. 云端"));
	outtextxy(85, 280, _T("c. 荒山"));
	outtextxy(85, 370, _T("d. 基地"));
	outtextxy(85, 460, _T("e. 火山"));

	settextstyle(30, 15, _T("宋体"));
	outtextxy(120, 650, _T("按下字母选择地图"));

	int selection = _getch();
	while (1)
	{
		switch (selection)
		{
		case 'a':
		case 'b':
		case 'c':
		case 'd':
		case 'e':
		{
			return (selection);
		}
		default:
		{
			selection = _getch();
		}
		}
	
	}
}

void getScore()
{
	TCHAR scoretext[50];
	setfillcolor(BLACK);
	solidrectangle(0, 0, 100, 20);
	setbkmode(TRANSPARENT);				// 字体透明
	settextcolor(LIGHTBLUE);
	settextstyle(20, 10, _T("宋体"));
	_stprintf_s(scoretext, _T("得分:%d"), score);
	outtextxy(0, 0, scoretext);
}

void show(int ms)//更新画面
{
	switch (ms)
	{
	case 'a': ms = 0; break;
	case 'b': ms = 1; break;
	case 'c': ms = 2; break;
	case 'd': ms = 3; break;
	case 'e': ms = 4; break;
	default:
	{
		ms = 0;
		break;
	}
	}
	BeginBatchDraw();//批量绘制函数，防止屏幕闪烁 
	putimage(0, 0, &background[ms]);//背景
	hero.draw();//精灵机
	boss.draw();//敌机
	drawAllPlane();//画敌机
	Bullet tmp;//子弹
	tmp.drawAllBullet();//画子弹
	L.show(hero);//画镭射
	getScore();
	FlushBatchDraw();//批量绘制函数，与上面的函数配合使用
}

void Key_scan()//扫描键盘
{
	//按下不同的按键来实现不同的移动
	if (KEY_DOWN(VK_UP))
	{
		hero.move(0);
	}
	else if (KEY_DOWN(VK_LEFT))
	{
		hero.move(2);
	}
	else if (KEY_DOWN(VK_RIGHT))
	{
		hero.move(3);
	}
	else if (KEY_DOWN(VK_DOWN))
	{
		hero.move(1);
	}
	//射击
	else if (KEY_DOWN(VK_RETURN) || KEY_DOWN(VK_SPACE))
	{
		while (KEY_DOWN(VK_RETURN) || KEY_DOWN(VK_SPACE))
		{
		}
		hero.shoot();
	}
	//shift发射镭射
	else if (KEY_DOWN(VK_SHIFT))
	{
		L.setLaserFlag(true);
		L.setLaserTime();
		hero.shootLaser();
	}
}

bool  Update(int ds)//更新游戏数据
{
	double speed;
	if (ds == 'a' or ds == 'A')
	{
		speed = 0.05;
	}
	else if (ds == 'd' or ds == 'D')
	{
		speed = 0.2;
	}
	//敌机指定方向移动
	moveAllEnemy(1);
	//子弹指定方向发射
	moveAllBullet(0);
	//检查各种条件
	if (!check())
	{
		return false;
	}

	//随机添加敌机
	if (rand() % 1000 == 0)
	{
		Enemy tmp (rand() % 450, 0);
		tmp.img1 = allEnemy[rand() % 4];
		tmp.img2 = allEnemy[4];
		tmp.setSpeed(speed);
		tmp.addEnemy();
	}

	if (rand() % 1973 == 0)
	{
		boss.Shoot();
	}
	return true;
}

void GameOver()
{
	system("color 0a");
	cout << "                 游戏结束" << endl;
	cout << "                         制作人：Tao Wang, Zhao Pu and Longlong" << endl;
	cout << "           你的最终得分是： " << score << endl;
}

int main()
{
	int speedLevel;
	int mapChose;
	bool flag = true;
	hero.loadPic();
	boss.loadPic();
	init();
	speedLevel = difficulitySelection();
	mapChose = mapSelection();
	cleardevice();
	while (flag == true)
	{
		show(mapChose);
		Key_scan();
		flag = Update(speedLevel);
	}
	closegraph();
	GameOver();
	getchar();
	return 0;
}