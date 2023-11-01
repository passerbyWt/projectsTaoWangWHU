#include "../Common/Common.h"
#include <cv.h>
#include <highgui.h>
#include <string>
#include <iostream>
using namespace std;

void main()
{
	IplImage* src=cvLoadImage("../Common/005.jpg",0);//加载图像
	int width=src->width;//图像宽度
	int height=src->height;//图像高度
	int step=src->widthStep;//图像字节
	uchar* data=(uchar *)src->imageData;//图像数据
	int hist[256]={0};//灰度级
	int max=0;//最大值
	IplImage* dst=cvCreateImage(cvSize(400,300),8,0);//目标图像
	cvSet(dst,cvScalarAll(255),0);//初始化
	double bin_width;//宽度间隔
	double bin_unith;//相对dst高度间隔
	for(int i=0;i<height;i++)//行
	{
		for(int j=0;j<width;j++)//列
		{
			hist[data[i*step+j]]++;//对应灰度级加1
		}
	}
	for(int i=0;i<256;i++)//级
	{
		if(hist[i]>max)//求最大值
		{
			max=hist[i];
		}
	}
	bin_width=(double)dst->width/256;
	bin_unith=(double)dst->height/max;
	for(int i=0;i<256;i++)
	{
		CvPoint p0=cvPoint(i*bin_width,dst->height);//起始点
		CvPoint p1=cvPoint((i+1)*bin_width,dst->height-hist[i]*bin_unith);//终止点
		cvRectangle(dst,p0,p1,cvScalar(0,0,0),-1,8,0);//黑色
	}
	cvNamedWindow("hist",1);
	cvShowImage("hist",dst);
	cvWaitKey(0);
	cvDestroyAllWindows();
	cvReleaseImage(&src);
	cvReleaseImage(&dst);
}