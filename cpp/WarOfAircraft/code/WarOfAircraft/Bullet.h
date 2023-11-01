#pragma once
#ifndef BULLET_H
#define BULLET_H
#include"Object.h"
using namespace std;

//子弹类，继承物体类
class Bullet : public Object
{
public:
	//友元函数
	//检查
	friend bool check();
	//移动子弹
	friend void moveAllBullet(int dir);

	//默认构造函数
	Bullet() = default;
	//构造函数
	Bullet(double x, double y);
	//构造函数
	Bullet(Coordinate coor);
	//向子弹链表中添加子弹
	void addBullet();
	//用迭代器在子弹链表中遍历，并且调用函数画出子弹
	void drawAllBullet();

private:
	//私有成员变量， 一个声明为静态的子弹链表
	static list<Bullet> objectList;
};


#endif

