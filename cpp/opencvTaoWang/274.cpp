#include "../Common/Common.h"
#include "highgui.h"
#include <iostream>
using namespace std;

void main()
{
	IplImage* src;//源图像指针
	src = cvLoadImage("../Common/005.jpg",3);//加载源图像
	if(&src == NULL)
		cout<<"加载源图像失败!"<<endl;
	else
	{
		cvNamedWindow("src");//创建图像窗口
		cvShowImage("src",src);//显示图像函数
		cvWaitKey(-1);//等待用户响应
		cvDestroyWindow("src");//销毁窗口
	}
	cvReleaseImage(&src);//释放源图像指针
}