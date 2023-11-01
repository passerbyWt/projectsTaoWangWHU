#include "Laser.h"
#include "Hero.h"

void Laser::drawLaser(Hero& h)
{
	//通过精灵机来定位镭射
	putimage(h.getCoordinate().x + 55, h.getCoordinate().y - 575, &laserPic);
}

void Laser::show(Hero& h)
{
	getTheTime();//获得游戏开始了多长时间

	//如果需要发射镭射
	if (getFlag())
	{
		//现在的时间比发射镭射的时间晚，设置为不发射镭射
		if (nowTime - laserTime > 1)
			setLaserFlag(false);
		//否则发射镭射
		else
		{
			drawLaser(h);
		}
	}
	//通过时间来判断是否有镭射
	//删掉镭射成员
	for (auto it = crash.begin(); it != crash.end(); )
	{
		if (it->t <= nowTime)
		{
			it = crash.erase(it);
		}
		else it++;
	}
}

//向镭射容器中添加成员
void Laser::add(double x, double y)
{
	node tmp;
	tmp.x = x, tmp.y = y;
	tmp.t = time(NULL) + 1;
	crash.push_back(tmp);
}

//初始化
void Laser::initLaser()
{
	//获取当前时间
	getTime = time(NULL);
	nowTime = time(NULL);
	//加载图片
	loadimage(&laserPic, _T("res\\images\\laser.png"));
}

time_t Laser::getTheTime()//获得游戏开始了多长时间
{
	nowTime = time(NULL);
	return nowTime - getTime; //现在的时间减去以前获取的时间
}


bool Laser::getFlag()
{
	return laserFlag;
}

//设置镭射的发射开关
void Laser::setLaserFlag(bool f)
{
	laserFlag = f;
}

void Laser::setLaserTime()
{
	laserTime = time(NULL);
}