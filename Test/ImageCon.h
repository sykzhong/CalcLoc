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
	void cvtCon2Mat(const Mat &_template);		//��ģ���С������Ӧ����
	void fitContour();
	
	vector<Point> contour;	//����ͼ������
	int thicklinesize;		//����������״̬ʱ�Ĵ�ϸ
	int thinlinesize;		//�����Ǹ���״̬�Ĵ�ϸ
	Scalar color;			//��¼������ǰ����ɫ

	int value;				//ÿ�����ֵ������Ϊ����������
	Mat value_image;		//��ͼ����ʽ���Ʋ��洢��������
	Mat show_image;			//��ͼ����ʽ���Ʋ���������ƽʱ״̬

	int flag;				//���ڼ�¼��������״̬��0������ 1������ 2���ȶ���κ�Hu 3������Բ��Hu

	RotatedRect ellipsebox;			//��¼��Բ��Ͻ��
	vector<Point> polycontour;		//��¼�������Ͻ��

	const static Scalar g_color[4];			//��¼��ͬ״̬����������ɫ
};

#endif