#include "Enemy.h"

list<Enemy> Enemy::objectList;//声明敌机链表

void Enemy::addEnemy()
{
	Enemy::objectList.insert(objectList.begin(), *this);
}

void Enemy::move(int)
{
	//获取坐标
	Coordinate coor = getCoordinate();
	//获取移动数组中的目录，确定移动方式
	int dir = getDir();
	//获取速度
	double speed = getSpeed();

	//坐标的增减实现移动
	coor.x += direction[dir][0] * speed;
	coor.y += direction[dir][1] * speed;

	//敌机出界不作处理
	if (coor.x >= 550 || coor.x < 0 || coor.y < 0 || coor.y > 770)
	{

	}
	//敌机未出界，设置坐标AADA
	else
		setCoordinate(coor.x, coor.y);
}

//射击函数
void Enemy::shoot()
{
	//获取敌机坐标
	Coordinate tmp = Enemy::getCoordinate();
	//创建子弹类
	Bullet bullet(tmp.x + 63, tmp.y - 30);
	//加载子弹图片
	loadimage(&bullet.img2, _T("res\\images\\bullet2-1.png"));
	loadimage(&bullet.img1, _T("res\\images\\bullet2.png"));
	//添加子弹
	bullet.Bullet::addBullet();
	//画子弹
	bullet.draw();
}

