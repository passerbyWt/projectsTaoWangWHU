#include "../Common/Common.h"
#include <cv.h>
#include <highgui.h>
#include <string>
#include <iostream>
using namespace std;

void main()
{
	double fps;										//视频帧率
	int vfps;											//每帧播放时间(毫秒)
	IplImage *frame;									//每帧图像数据
	float pos_ratio;								//当前帧在AVI中相对位置比例
	CvCapture *capture=cvCreateFileCapture("../Common/Megamind_bugy.avi");//读入AVI	文件
	if(capture==NULL)									//变量值为空
	{
		cout<<"没有视频文件"<<endl;						//输出提示语
		return;										//返回
	}
	fps=cvGetCaptureProperty(capture,CV_CAP_PROP_FPS);	//读取视频的帧率
	vfps=1000/fps;										//计算每帧播放的时间
	cvNamedWindow("AVI",1);							//创建窗口
	while(1)											//循环
	{
		frame=cvQueryFrame(capture);					//抓取当前帧
		//读取该帧在视频中的相对位置
		pos_ratio=cvGetCaptureProperty(capture,CV_CAP_PROP_POS_AVI_RATIO);
		if(!frame)										//没有帧
			break;									//退出循环
		cvShowImage("AVI",frame);						//显示当前帧
		cvWaitKey(vfps);								//等待当前帧播放完
	}
	cvReleaseImage(&frame);
	cvReleaseCapture(&capture);							//释放视频
	cvDestroyWindow("AVI");								//销毁窗口
}