#pragma once

#include "Enemy.h"

//Boss类，继承飞行物体
class Boss :public Object
{
	//友元类
	friend class Enemy;
public:
	//构造函数
	Boss(double x, double y);
	//加载Boss图片
	void loadPic();
	//boss射击
	void Shoot();
};

