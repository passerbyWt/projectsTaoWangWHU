#include "../Common/Common.h"
#include "highgui.h"
#include "cv.h"
#include <iostream>
using namespace std;

IplImage * HistEqualization(IplImage *src)
{
	IplImage* dst=0;   //定义目标图像并初始化
	CvHistogram* hist=0;       //定义直方图
	int n=256;//256个阶度
	double nn[256];   //直方图数组
	uchar T[256];   //归一化后的直方图数组
	CvMat* T_mat;   //定义矩阵
	int x;       //计数器
	int sum=0;  //图像中像素点的总和
	double val=0;
	hist=cvCreateHist(1,&n,CV_HIST_ARRAY,0,1); //创建直方图
	cvCalcHist(&src,hist,0,0);  //计算直方图
	val=0;  //累加计数器
	for (x=0;x<n;x++)//计算源图像直方图
	{
		val=val+cvGetReal1D(hist->bins,x);
		nn[x]=val;
	}
	//归一化直方图
	sum=src->height*src->width;
	for (x=0;x<n;x++)
	{
		T[x]=(uchar)(255*nn[x]/sum);
	}
	//创建均衡化处理后的新图
	dst=cvCloneImage(src);//复制图像
	T_mat=cvCreateMatHeader(1,256,CV_8UC1);//创建矩阵头
	cvSetData(T_mat,T,0); //设置新的data数据
	cvLUT(src,dst,T_mat); //填充数据
	//释放内存
	cvReleaseImage(&src);
	cvReleaseHist(&hist);
	src=NULL ;
	return dst;
}
void main()
{
	IplImage* src;//源图像指针
	IplImage* gray;//灰度化后图像内存
	IplImage* hist;//均衡化图像
	src = cvLoadImage("../Common/005.jpg",3);//加载源图像
	if(&src == NULL)
		cout<<"加载源图像失败!"<<endl;
	else
	{
		cvNamedWindow("hist");//创建图像窗口
		gray = cvCreateImage(cvGetSize(src), src->depth, 1);//创建gray的内存大小并初始化
		cvCvtColor(src,gray,CV_BGR2GRAY);//灰度化
		hist = cvCreateImage(cvGetSize(src), src->depth, 1);//创建hist的内存大小并初始化
		hist = HistEqualization(gray);//均衡化
		cvShowImage("hist",hist);//显示图像
		cvWaitKey(-1);//等待用户响应
		cvDestroyWindow("hist");//销毁窗口
	}
	cvReleaseImage(&src);//释放源图像指针
	cvReleaseImage(&gray);//释放灰度图像指针
	cvReleaseImage(&hist);//释放均衡图像指针
}