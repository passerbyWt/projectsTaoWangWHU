#pragma once

#ifndef OBJECT_H
#define OBJECT_H

#include<easyx.h>
#include<graphics.h>
#include<queue>
#include<vector>
#include<time.h>
#include<list>
using namespace std;

extern double direction[6][2];

//坐标结构体
struct Coordinate
{
	double x, y;
};

//物体类，是飞机和子弹的基类
class Object
{
public:
	//一个图片的类对象
	IMAGE img1, img2;

	//无参构造函数
	Object() = default;
	//构造函数
	Object(double x, double y);
	//形参为结构体的构造函数
	Object(Coordinate coor);
	//获取一个坐标结构体
	Coordinate getCoordinate();
	//获取速度
	double getSpeed();
	//设置速度
	void setSpeed(double s);
	//设置结构体的坐标
	void setCoordinate(double x, double y);
	//获取在方向数组的目录
	int getDir();
	//设置在方向数组的目录
	void setDir(int d);
	//移动的虚函数
	virtual void move(int);
	//在窗口的指定位置加载飞行物体的图片
	void draw();
	//==重载，目的是判断坐标是否相同
	bool operator==(const Object& t);

private:
	//成员变量
	Coordinate coor;//坐标结构体
	double speed;	//速度
	int dir;		//目录
};
#endif