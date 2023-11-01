#include "../Common/Common.h"
#include <cv.h>
#include <highgui.h>
#include <string>
#include <iostream>
using namespace std;

void main()
{
	IplImage *src;//源图像指针
	IplImage *dst;//目标图像指针
	float scale=1.2;//缩放倍数
	int sW,sH;//源图像宽高
	int dW,dH;//目标图像宽高
	int d_step,s_step;//图像字节
	uchar *dst_data;//目标数据
	uchar *src_data;//源数据
	src=cvLoadImage("../Common/005.jpg",0);//载入文件
	sW=src->width;//图像宽度
	sH=src->height;//图像高度
	s_step=src->widthStep;//源图像字节
	src_data=(uchar*)src->imageData;//源数据获取
	dW=sW*scale;
	dH=sH*scale;//目标图像高度
	dst=cvCreateImage(cvSize(dW,dH),src->depth,src->nChannels);//构造目标图像
	d_step=dst->widthStep;//目标图像字节
	dst_data=(uchar*)dst->imageData;//指向目标数据
	for(int i=0;i<dH;i++)//行
	{
		for(int j=0;j<dW;j++)//列
		{
			int i0=i/scale;//坐标还原
			int j0=j/scale;
			if((j0>=0)&&(j0<sW)&&(i0>=0)&&(i0<sH))//在源图范围内
			{
				//指向源图像第i0行第j0列像素并赋值
				*(dst_data+(dH-1-i)*d_step+j)=*(src_data+s_step*(sH-1-i0)+j0);
			}
			else//在源图范围外
			{
				*(dst_data+(dH-1-i)*d_step+j)=255;//赋值255
			}
		}
	}
	cvNamedWindow("src");//创建显示源图像窗口
	cvNamedWindow("dst");//创建显示目标图像窗口
	cvShowImage("src",src);//显示目标图像
	cvShowImage("dst",dst);//显示目标图像
	cvWaitKey(-1);//等待用户响应
	cvReleaseImage(&src);//释放内存
	cvReleaseImage(&dst);
	cvDestroyAllWindows();
}