#ifndef _HSVHIST_H_
#define _HSVHIST_H_
#include "GlobalHeader.h"

using namespace std;
using namespace cv;



class HSVHist
{
public:
	HSVHist();
	~HSVHist();
	int getImage(string path);
	void getHist();
	void drawHist();
	void removeSeg(HSVHist back);
	static void removeBack(HSVHist dst, HSVHist back);
	void showImage(string strpath = "");
	void Init();
private:
	Mat hsvplane[3];		//�洢HSV��ͨ��
	
	float svrange[2];
	float hrange[2];
	const float *ranges[3];	//hsv��ȡֵ��Χ
	double maxval;			//ԭʼ�����е����ֵ

	int bin_w;				//ֱ��ͼ������
	int hist_h, hist_w;		//ֱ��ͼ�����
	Mat hist_img;			//������ʾ������ֱ��ͼ

protected:
	string path;			//ԭͼ·��

	Mat srcimage;			//ԭͼ��,��ȡΪHSV��ʽ
	Mat m_image;
	Mat hsvhist;			//���ڴ洢ֱ��ͼ������
	int histsize[3];		//hsv��ֱ��ͼ�зֲ���
};


#endif

