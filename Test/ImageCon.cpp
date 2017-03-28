#include "stdafx.h"
#include "ImageCon.h"

void ImageCon::inputCon(vector<Point> &_contour, const int &_value, const Scalar &_color, const int &_thicklinesize, const int &_thinlinesize)
{
	contour.resize(_contour.size());
	color = _color;
	thicklinesize = _thicklinesize;
	value = _value;
	thinlinesize = _thinlinesize;
	copy(_contour.begin(), _contour.end(), contour.begin());
}

void ImageCon::cvtCon2Mat(const Mat &_template)
{
	value_image = Mat(_template.size(), CV_8UC1, Scalar::all(0));
	show_image = Mat(_template.size(), CV_8UC3, Scalar::all(0));
	drawContours(value_image, vector<vector<Point>>(1, contour), -1, Scalar(value), thicklinesize);		//»æÖÆÂÖÀªË÷ÒýÖµÍ¼£¨¸ßÁÁ×´Ì¬´ÖÏ¸£©
	drawContours(show_image, vector<vector<Point>>(1, contour), -1, color, thinlinesize);				//»æÖÆÂÖÀª(·Ç¸ßÁÁ×´Ì¬´ÖÏ¸)
}