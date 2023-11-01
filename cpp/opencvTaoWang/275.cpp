#include "../Common/Common.h"
#include "highgui.h"
#include "cv.h"
#include <iostream>
using namespace std;

void main()
{
	IplImage* src;//源图像指针
	IplImage* gray;//灰度化后图像内存
	src = cvLoadImage("../Common/005.jpg",3);//加载源图像
	if(&src == NULL)
		cout<<"加载源图像失败!"<<endl;
	else
	{
		cvNamedWindow("gray");//创建图像窗口
		gray = cvCreateImage(cvGetSize(src), src->depth, 1);//创建gray的内存大小等初始化
		cvCvtColor(src,gray,CV_BGR2GRAY);//灰度化
		cvShowImage("gray",gray);//显示图像函数
		cvWaitKey(-1);//等待用户响应
		cvDestroyWindow("gray");//销毁窗口
	}
	cvReleaseImage(&src);//释放源图像指针
	cvReleaseImage(&gray);//释放灰度图像指针
}