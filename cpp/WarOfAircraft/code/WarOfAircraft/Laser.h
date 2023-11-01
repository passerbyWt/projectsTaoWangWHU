#pragma once
#ifndef LASER_H
#define LASER_H
#include "Enemy.h"

//节点
struct node
{
	double x, y;
	time_t t; //时间对象
};

//激光类
class Laser
{
public:
	//两个图片对象
	IMAGE  laserPic;

	//友元函数和友元类
	friend class Hero;
	friend bool check();

	//画出镭射
	void drawLaser(Hero& h);

	void show(Hero& h);

	//向镭射容器中添加成员
	void add(double x, double y);

	//初始化
	void initLaser();

	time_t getTheTime();//获得游戏开始了多长时间

	//是否发射镭射
	bool getFlag();

	//设置镭射的发射开关
	void setLaserFlag(bool f);

	//获取系统的当前时间
	void setLaserTime();

private:
	time_t getTime; //获取时间
	time_t nowTime; //现在的时间
	time_t laserTime; //镭射的时间
	vector<node> crash; //一个镭射的容器
	bool laserFlag; //是否发射镭射的标志
};
#endif