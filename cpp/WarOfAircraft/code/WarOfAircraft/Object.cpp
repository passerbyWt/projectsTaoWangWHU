#include "Object.h"

//方向数组
double direction[6][2] = { { 0,-1 },{ 0,1 } ,{ -1,0 },{ 1,0 },{0.5,0.5},{-0.5,0.5} };//上下左右

Object::Object(double x, double y)
{
	coor.x = x;
	coor.y = y;
	speed = 1;
	setSpeed(1);
	setDir(1);
}

Object::Object(Coordinate coor)
{
	Object(coor.x, coor.y);//调用上面的构造函数
}

Coordinate Object::getCoordinate()
{
	return coor;
}

double Object::getSpeed()
{
	return speed;
}

void Object::setSpeed(double s)
{
	speed = s;
}

void Object::setCoordinate(double x, double y)
{
	coor.x = x; coor.y = y;
}

int Object::getDir()
{
	return dir;
}

void Object::setDir(int d)
{
	dir = d;
}

void Object::move(int)
{
	//
	coor.x += direction[dir][0] * speed;
	coor.y += direction[dir][1] * speed;
	//设置界限，防止飞行物体越界，可以看出此图片宽度为40px
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
}

void Object::draw()
{
	putimage(coor.x, coor.y, &img2, SRCAND);//掩码图
	putimage(coor.x, coor.y, &img1, SRCPAINT);//精灵图
}

bool Object::operator==(const Object& t)
{
	if (((int)t.coor.x == (int)coor.x) && ((int)t.coor.y == (int)coor.y))
		return true;
	return false;
}
