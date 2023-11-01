//Canny检测
#include "../Common/Common.h"
#include "highgui.h"
#include "cv.h"
#include <iostream>
using namespace std;

void on_trackbar(int value)//变量条
{}
void main()
{
	int threshold1=20;//阈值1
	int threshold2=20;//阈值2
	IplImage* src;//源图像指针
	IplImage* gray;//灰度化后图像内存
	IplImage* edge;//检测图像
	src = cvLoadImage("../Common/005.jpg",3);//加载源图像
	if(&src == NULL)
		cout<<"加载源图像失败!"<<endl;
	else
	{
		cvNamedWindow("canny");//创建图像窗口
		gray = cvCreateImage(cvGetSize(src), src->depth, 1);//创建gray的内存大小等初始化
		cvCvtColor(src,gray,CV_BGR2GRAY);//灰度化
		edge = cvCreateImage(cvGetSize(src), IPL_DEPTH_8U, 1);
		//创建变量范围条
		cvCreateTrackbar("阈值1", "canny", &threshold1, 500, on_trackbar);
		cvCreateTrackbar("阈值2", "canny", &threshold2, 500, on_trackbar);
		while(1)
		{
			cvCanny(gray, edge, threshold1, threshold2, 3);//Canny检测
			cvShowImage("canny", edge);//显示图像
			if(cvWaitKey(100)>0)//等待用户响应
				break;
		}
		cvSaveImage("../Common/canny.jpg",edge);
	}
	cvDestroyWindow("canny");//销毁窗口
	cvReleaseImage(&src);//释放源图像指针
	cvReleaseImage(&gray);//释放灰度图像指针
	cvReleaseImage(&edge);//释放均衡图像指针
}