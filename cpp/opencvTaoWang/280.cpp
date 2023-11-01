#include "cv.h"
#include "highgui.h"
#include "../Common/Common.h"
#include <iostream>
using namespace std;


void main()
{
	cvNamedWindow("open");//命名窗口
	IplImage * src = cvLoadImage("../Common/twovalue.jpg", 0);
	IplImage * temp = cvCreateImage(cvGetSize(src), 8,1);//创建临时变量
	IplImage * dst  = cvCreateImage(cvGetSize(src), 8, 1);//创建目标变量
	cvCopyImage(src,temp);//初始化临时
	cvCopyImage(src,dst);//初始化目标
	cvMorphologyEx(//开运算
		src,//源变量
		dst,//目标变量
		temp,//临时变量
		NULL, //模板大小，默认 3*3
		CV_MOP_OPEN,//运算种类
		1);//迭代次数
	cvShowImage("open", dst);	
	cvSaveImage("../Common/open.jpg", dst);
	cvWaitKey(0);
	cvReleaseImage(&temp);//释放
	cvReleaseImage(&src);
	cvReleaseImage(&dst);
	cvDestroyAllWindows();//销毁窗口
}