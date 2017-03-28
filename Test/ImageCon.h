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

	vector<Point> contour;	//����ͼ������
	int thicklinesize;		//����������״̬ʱ�Ĵ�ϸ
	int thinlinesize;		//�����Ǹ���״̬�Ĵ�ϸ
	Scalar color;			//��¼������ǰ����ɫ

	int value;				//ÿ�����ֵ������Ϊ����������
	Mat value_image;		//��ͼ����ʽ���Ʋ��洢��������
	Mat show_image;			//��ͼ����ʽ���Ʋ���������ƽʱ״̬
};

#endif