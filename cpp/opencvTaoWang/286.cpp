#include "../Common/Common.h"
#include <cv.h>
#include <highgui.h>
#include <string>
#include <iostream>
using namespace std;

void main()
{ 
	IplImage* src;//源图像
	string nsrc;
	string ndst;//源目标图像名
	string csrc;
	string cdst;//源目标图像名的最后3个字节
	cout<<"本程序转换图像格式，只支持的格式为：JPG、BMP、TIF、PNG、PPM\n";
	cout<<"输入源图像名：";
	cin>>nsrc;
	csrc=nsrc.substr(nsrc.length()-3,3);//源图像的后缀
	if(csrc!="jpg"
		&& csrc!="bmp" 
		&& csrc!="tif" 
		&& csrc!="png" 
		&& csrc!="ppm")
	{
		cout<<"源图像格式不支持\n";
	}
	else
	{
		cout<<"输入目标图像名：";
		cin>>ndst;
		cdst=ndst.substr(ndst.length()-3,3);//目标图像的后缀
		if(cdst!="jpg"
			&& cdst!="bmp" 
			&& cdst!="tif" 
			&& cdst!="png" 
			&& cdst!="ppm")
		{
			cout<<"目标图像格式不支持\n";
		}
		else
		{
			src=cvLoadImage(nsrc.c_str(),-1);//加载源图像
			if(src!=0)//读取成功
			{
				if(cvSaveImage(ndst.c_str(),src)!=0)//保存图像
					cout<<"转换成功\n";
			}
			else
				cout<<"读取失败\n";
		}
	}
	cvReleaseImage(&src);//释放
}