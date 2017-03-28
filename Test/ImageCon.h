#ifndef _IMAGECON_H_
#define _IMAGECON_H_

#include "GlobalHeader.h"
using namespace cv;
using namespace std;

enum ConState
{
	CON_IGNORE = 0,
	CON_NORMAL = 1,
	CON_POLY = 2,
	CON_ELLIPSE = 3
};

class ImageCon
{
public:
	void inputCon(vector<Point> &_contour, const int &_value, const int &_flag, const int &_thicklinesize, const int &_thinlinesize);
	void cvtCon2Mat(const Mat &_template);		//按模板大小绘制相应轮廓
	void fitContour();
	
	vector<Point> contour;	//单个图像轮廓
	int thicklinesize;		//当轮廓高亮状态时的粗细
	int thinlinesize;		//轮廓非高亮状态的粗细
	Scalar color;			//记录轮廓当前的颜色

	int value;				//每个点的值，可作为轮廓的索引
	Mat value_image;		//以图的形式绘制并存储轮廓索引
	Mat show_image;			//以图的形式绘制并春初轮廓平时状态

	int flag;				//用于记录轮廓处理状态，0：忽略 1：不变 2：先多边形后Hu 3：先椭圆后Hu

	RotatedRect ellipsebox;			//记录椭圆拟合结果
	vector<Point> polycontour;		//记录多边形拟合结果

	const static Scalar g_color[4];			//记录不同状态下轮廓的颜色
};

#endif