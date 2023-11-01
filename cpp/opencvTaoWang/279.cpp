//二值化
#include "../Common/Common.h"
#include "highgui.h"
#include "cv.h"
#include <iostream>
using namespace std;

int Threshold(IplImage *src)//求取阈值
{
	int width = src->width;//源图像宽度
	int height = src->height;//源图像高度
	int pixelCount[256];//像素级
	float pixelPro[256];//每个像素值所占比例
	int pixelSum = width * height;//像素个数 
	int threshold = 0;//阈值
	float w0, w1, u0tmp, u1tmp, u0, u1, u, 
		deltaTmp, deltaMax = 0;//遍历灰度级所需变量
	uchar* data = (uchar*)src->imageData;//源图像数据

	for(int i = 0; i <256; i++)//初始化数组变量
	{
		pixelCount[i] = 0;
		pixelPro[i] = 0;
	}
	for(int i = 0; i < height; i++)//统计灰度级每个像素值在整幅图像中的个数
	{
		for(int j = 0;j < width;j++)
		{
			pixelCount[(int)data[i * src->widthStep+ j]]++;
		}
	}
	for(int i = 0; i < 256; i++)//计算每个像素在整幅图像中的比例
	{
		pixelPro[i] = (float)pixelCount[i] / pixelSum;
	}
	for(int i = 0; i < 256; i++)//遍历灰度级[0,255]
	{
		w0 = w1 = u0tmp = u1tmp = u0 = u1 = u = deltaTmp = 0;
		for(int j = 0; j < 256; j++)
		{
			if(j <= i)   //背景部分
			{
				w0 += pixelPro[j];
				u0tmp += j * pixelPro[j];
			}
			else   //前景部分
			{
				w1 += pixelPro[j];
				u1tmp += j * pixelPro[j];
			}
		}
		u0 = u0tmp / w0;
		u1 = u1tmp / w1;
		u = u0tmp + u1tmp;
		deltaTmp = w0 * pow((u0 - u), 2) + w1 * pow((u1 - u), 2);
		if(deltaTmp > deltaMax)
		{
			deltaMax = deltaTmp;
			threshold = i;
		}
	}
	return threshold;//返回阈值
}
IplImage* binary(IplImage *src)
{   
	IplImage* imgOtsu;//二值化图像
	imgOtsu= cvCreateImage(cvGetSize(src),IPL_DEPTH_8U,1);//创建
	cvCopy(src,imgOtsu);//复制数据
	int threNumber;//阈值
	threNumber=Threshold(imgOtsu);//调用Threshold函数求阀值
	CvScalar otsu;//像素
	int temp;//像素值
	for(int i=0;i<src->height;i++)//  二值化
	{
		for(int j=0;j<src->width;j++)
		{
			otsu=cvGet2D(imgOtsu,i,j);//获取源图像素
			temp=(int)otsu.val[0];//像素值
			if(temp>=threNumber)//不小于阈值
				temp=255;//白
			else//小于阈值
				temp=0;//黑
			otsu.val[0]=temp;
			cvSet2D(imgOtsu,i,j,otsu); //写入二值化图像 
		}
	}
	return imgOtsu;   
}
void main()
{
	IplImage* src;//源图像指针
	IplImage* gray;//灰度化后图像内存
	IplImage* imgOtsu;//二值化图像
	src = cvLoadImage("../Common/005.jpg",3);//加载源图像
	if(&src == NULL)
		cout<<"加载源图像失败!"<<endl;
	else
	{
		cvNamedWindow("twovalue");//创建图像窗口
		gray = cvCreateImage(cvGetSize(src), src->depth, 1);//创建gray的内存大小等初始化
		cvCvtColor(src,gray,CV_BGR2GRAY);//灰度化
		imgOtsu=binary(gray);//二值化
		cvShowImage("twovalue",imgOtsu);//显示
		if(cvSaveImage("../Common/twovalue.jpg",imgOtsu))//保存图像
			cout<<"二值化图像保存成功!"<<endl;
		cvWaitKey(-1);//等待用户响应
		cvDestroyWindow("twovalue");//销毁窗口
	}
	cvReleaseImage(&src);//释放源图像指针
	cvReleaseImage(&gray);//释放灰度图像指针
	cvReleaseImage(&imgOtsu);//释放二值化图像指针
}