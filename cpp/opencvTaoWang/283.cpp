//hough变换
#include "../Common/Common.h"
#include "highgui.h"
#include "cv.h"
#include <iostream>
using namespace std;

IplImage* showLines(CvSeq *lines, IplImage *img)
{
	IplImage* dst;
	dst = cvCreateImage(cvGetSize(img), IPL_DEPTH_8U, img->nChannels);
	cvCopy(img, dst, NULL);
	for(int i = 0; i< lines->total; i++)//循环所有直线
	{
		CvPoint* line = (CvPoint*)cvGetSeqElem(lines, i);//获取直线
		cvLine(dst, line[0], line[1], CV_RGB(255, 0, 0), 1, CV_AA, 0);//直线用红色输出
	}
	return dst;
}

void on_trackbar(int value)//变量条
{}
void main()
{
	int threshold1=20;//阈值1
	int threshold2=20;//阈值2
	int param0 = 400, param1 = 216, threshold = 33, hp1 = 2, hp2 = 3;
	int rho = 1,theta = 2;//hough变换变量
	IplImage* src;//源图像指针
	IplImage* gray;//灰度化后图像内存
	IplImage* edge;//检测图像
	IplImage* hough;//hough变换
	src = cvLoadImage("../Common/005.jpg",3);//加载源图像
	if(&src == NULL)
		cout<<"加载源图像失败!"<<endl;
	else
	{
		cvNamedWindow("canny");//创建图像窗口
		cvNamedWindow("hough");//创建图像窗口
		gray = cvCreateImage(cvGetSize(src), src->depth, 1);//创建gray的内存大小等初始化
		cvCvtColor(src,gray,CV_BGR2GRAY);//灰度化
		edge = cvCreateImage(cvGetSize(src), IPL_DEPTH_8U, 1);//创建canny检测变量
		hough = cvCreateImage(cvGetSize(src), IPL_DEPTH_8U, 3);//创建hough变换变量
		CvSeq* line;
		CvMemStorage * storage = cvCreateMemStorage(0);
		//创建变量范围条
		cvCreateTrackbar("阈值1", "canny", &threshold1, 500, on_trackbar);
		cvCreateTrackbar("阈值2", "canny", &threshold2, 500, on_trackbar);
		while(1)
		{
			cvCanny(gray, edge, threshold1, threshold2, 3);//Canny检测
			line = cvHoughLines2(edge, storage, CV_HOUGH_PROBABILISTIC, 
				rho, CV_PI/180, threshold, hp1, hp2);//hough变换
			cvShowImage("canny", edge);//显示边缘检测图像
			cvShowImage("hough", showLines(line,hough));//显示直线
			if(cvWaitKey(100)>0)//等待用户响应
				break;
		}
		cvSaveImage("../Common/canny.jpg",edge);//保存边缘检测图像
		cvSaveImage("../Common/hough.jpg",showLines(line,hough));//保存hough直线
	}
	cvReleaseImage(&src);//释放源图像指针
	cvReleaseImage(&gray);//释放灰度图像指针
	cvReleaseImage(&edge);//释放检测图像指针
	cvReleaseImage(&hough);//释放hough变换图像指针
	cvDestroyAllWindows();//销毁窗口
}