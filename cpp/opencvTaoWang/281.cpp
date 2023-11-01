#include "cv.h"
#include "highgui.h"
#include "../Common/Common.h"
#include <iostream>
using namespace std;

void main()
{
	cvNamedWindow("close");//窗口命名
	IplImage * src = cvLoadImage("../Common/twovalue.jpg", 0);//源图像
	IplImage * temp = cvCreateImage(cvGetSize(src), 8,1);//创建临时
	IplImage * dst  = cvCreateImage(cvGetSize(src), 8,1);//创建目标
	cvCopyImage(src,temp);//初始化临时
	cvCopyImage(src, dst);//初始化目标
	cvMorphologyEx(//闭运算
		src,//源
		dst,//目标
		temp,//临时
		NULL, //默认模板3*3
		CV_MOP_CLOSE,//闭运算
		1);//迭代次数
	cvShowImage("close", dst);//显示目标图像
	cvSaveImage("../Common/close.jpg", dst);//保存图像
	cvWaitKey(0);
	cvReleaseImage(&src);//释放
	cvReleaseImage(&temp);
	cvReleaseImage(&dst);
	cvDestroyAllWindows();//销毁窗口
}