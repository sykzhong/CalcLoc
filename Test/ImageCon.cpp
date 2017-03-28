#include "stdafx.h"
#include "ImageCon.h"

const Scalar ImageCon::g_color[4] = { WHITE, RED, BLUE, GREEN };

void ImageCon::inputCon(vector<Point> &_contour, const int &_value, const int &_flag, const int &_thicklinesize, const int &_thinlinesize)
{
	contour.resize(_contour.size());
	flag = _flag;
	color = g_color[flag];
	thicklinesize = _thicklinesize;
	value = _value;
	thinlinesize = _thinlinesize;
	copy(_contour.begin(), _contour.end(), contour.begin());
}

void ImageCon::cvtCon2Mat(const Mat &_template)
{
	value_image = Mat(_template.size(), CV_8UC1, Scalar::all(0));
	show_image = Mat(_template.size(), CV_8UC3, Scalar::all(0));
	drawContours(value_image, vector<vector<Point>>(1, contour), -1, Scalar(value), thicklinesize);		//������������ֵͼ������״̬��ϸ��
	drawContours(show_image, vector<vector<Point>>(1, contour), -1, color, thinlinesize);				//��������(�Ǹ���״̬��ϸ)
}

void ImageCon::fitContour()
{
	switch (flag)
	{
	case CON_POLY:
		approxPolyDP(contour, polycontour, 10, true);
		break;
	case CON_ELLIPSE:
		Mat pointsf;
		Mat(contour).convertTo(pointsf, CV_32F);
		ellipsebox = fitEllipse(pointsf);	
		break;
	}
}