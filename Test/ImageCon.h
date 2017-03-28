#ifndef _IMAGECON_H_
#define _IMAGECON_H_

#include "GlobalHeader.h"
using namespace cv;
using namespace std;

class ImageCon
{
public:
	void inputCon(vector<Point> &_contour, const int &_value, const Scalar &_color, const int &_thicklinesize, const int &_thinlinesize);
	void cvtCon2Mat(const Mat &_template);

	vector<Point> contour;	//单个图像轮廓
	int thicklinesize;		//当轮廓高亮状态时的粗细
	int thinlinesize;		//轮廓非高亮状态的粗细
	Scalar color;			//记录轮廓当前的颜色

	int value;				//每个点的值，可作为轮廓的索引
	Mat value_image;		//以图的形式绘制并存储轮廓索引
	Mat show_image;			//以图的形式绘制并春初轮廓平时状态
};

#endif