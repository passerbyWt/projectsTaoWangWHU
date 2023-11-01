#pragma once
#ifndef ENEMY_H
#define ENEMY_H
#include <easyx.h>
#include"Bullet.h"
using namespace std;

//
class Laser;

//敌机类，继承飞行物体类
class Enemy: public Object
{
public:
	//友元函数和友元类
	friend Laser;
	friend bool check();
	friend void moveAllEnemy(int dir);
	friend void init();
	friend void drawAllPlane();
	friend class Bullet;
	using Object::Object;

	//向链表中加入敌机
	void addEnemy();
	//移动函数重写
	virtual void move(int) override;
	//发射虚函数
	virtual void shoot();

private:
	static list<Enemy> objectList; //敌机链表，用来储存敌机
};


#endif
