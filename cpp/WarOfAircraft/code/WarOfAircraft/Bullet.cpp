#include "Bullet.h"

//声明一个子弹链表
list<Bullet> Bullet::objectList;

//声明函数
void moveAll(int dir);

Bullet::Bullet(double x, double y)
{
	setCoordinate(x, y);
	setSpeed(1);
	setDir(0);                     //子弹设置为方向目录0
}

Bullet::Bullet(Coordinate coor)
{
	Bullet(coor.x, coor.y);
}



void Bullet::addBullet()
{
	this->objectList.insert(objectList.begin(), *this);
}

void Bullet::drawAllBullet()
{
	for (auto iter = Bullet::objectList.begin(); iter != Bullet::objectList.end(); iter++)
	{
		iter->draw();
	}
}