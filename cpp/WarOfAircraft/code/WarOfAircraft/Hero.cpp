#include "Hero.h"

Hero::Hero(double x, double y)
{
	setCoordinate(x, y);
	setSpeed(0.3);
}

void Hero::loadPic()
{
	loadimage(&img1, _T("res\\images\\hero.png"), 150, 100, true);//加载精灵机图片
	loadimage(&img2, _T("res\\images\\hero1.png"), 150, 100, true);//加掩码图
}


void Hero::move(int dir) 
{
	//int dir = Getdir();
	//获取坐标
	Coordinate coor = getCoordinate();
	//获取速度
	double speed = getSpeed();
	//坐标变化
	coor.x += direction[dir][0] * speed;
	coor.y += direction[dir][1] * speed;
	//判断坐标，是否出界，向右出界在左边出现，其他以此类推
	if (coor.x >= 550)
	{
		coor.x = 0;
	}
	else if (coor.x < 0)
	{
		coor.x = 510;
	}
	if (coor.y < 0)
	{
		coor.y = 765;
	}
	else if (coor.y > 750)
	{
		coor.y = 0;
	}
	//设置坐标
	setCoordinate(coor.x, coor.y);
}