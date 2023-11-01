#include "Boss.h"

Boss::Boss(double x, double y)
{
	setCoordinate(x, y);
	setSpeed(0.0);
}

void Boss::loadPic()
{
	loadimage(&img1, __T("res\\images\\Boss.png"), 300, 300, true);
	loadimage(&img2, __T("res\\images\\Boss1.png"), 300, 300, true);
}

void Boss::Shoot()
{
	//获取坐标
	Enemy enemy(getCoordinate().x + 10, getCoordinate().y + 200);
	//加载子弹
	loadimage(&enemy.img1, _T("res\\images\\bullet1.png"));
	loadimage(&enemy.img2, _T("res\\images\\bullet1-1.png"));
	//设置速度
	enemy.setSpeed(0.5);
	//获取移动方式
	enemy.setDir(1);
	//向Boss链表中加入Boss
	enemy.addEnemy();

	Enemy enemy1(getCoordinate().x + 220, getCoordinate().y + 200);
	//加载子弹
	loadimage(&enemy1.img1, _T("res\\images\\bullet1.png"));
	loadimage(&enemy1.img2, _T("res\\images\\bullet1-1.png"));
	//设置速度
	enemy1.setSpeed(0.5);
	//获取移动方式
	enemy1.setDir(1);
	//向Boss链表中加入Boss
	enemy1.addEnemy();
}