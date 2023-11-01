#pragma once
#include "Enemy.h"

//我方飞机类，精灵机类,继承敌机类
class Hero :public Enemy
{
public:
	//构造函数
	Hero(double x, double y);
	//加载精灵机图片
	void loadPic();
	void shootLaser()//发射激光 ，声明函数
	{
	}
	//移动函数重写
	virtual void move(int dir) override;
private:

};
void moveAllEnemy(int dir);