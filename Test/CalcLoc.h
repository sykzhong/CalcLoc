#ifndef _CALCLOC_H_
#define _CALCLOC_H_
#include "ProImage.h"
#include <fstream>
#include <time.h>
enum CamCtrlFlags
{
	MOVE_RIGHT = 0,
	MOVE_LEFT = 1,
	MOVE_DOWN = 2,
	MOVE_UP = 3,
	ZOOM_IN = 4,					//�Ŵ���Ұ
	ZOOM_OUT = 5,					//��С��Ұ
	ROTATE_CLOCKWISE = 6,			//˳ʱ����ת
	ROTATE_ANTICLOCKWISE = 7,		//��ʱ����ת
	EXIT = 8,
	//SAVE_AUTO_MODE = 9,				//��˳��ɨ��
	//SAVE_MANUAL_MODE = 10,			//�˹�����ɨ��
	//SAVE_AUTO2_MODE = 11			//��������µ�ɨ��
};

class CalcLoc : public ProImage
{
public:
	CalcLoc();
	~CalcLoc();
	void getCenter();				//������������ȥƽ��ֵ��Ϊ����
	void getAngle();				//ȡ�ǶȻ�׼��������Ƕ�

	void writeResult(string _imgname = "");
	static void printResult(CalcLoc &dst);		
	static void printResult(time_t nowtime);

	int getFetchCenterAngle(const string &winname = "result");		//�û��Զ���ץȡ����ץȡ�Ƕ�
	void Init();													//�Խ�����г�ʼ��
	void moveCam(int &flag);										//�û������ƶ����
	void showImage(const string &winname = "result");				//��ʾ�û��������

private:
	Mat fitresult;					//��¼��Ͻ��
	Mat m_showimage;				//����ʵʱ��ʾ��ͼ��

	Point2f m_center;
	double m_angle;

	Point2f compcenter;				//��������ϵ�У��û�ץȡ�㣬Ĭ���빤�������ص�
	double compangle;				//��������ϵ�У��û�ץȡ�Ƕȣ�Ĭ���빤����׼�Ƕ��ص�
	double moveratio;			//�����ƶ�����

	Point TopLeft, TopRight, BottomLeft, BottomRight;		//�ɹ۲������ĸ��ǵ�(������е���ƶ�����λ�ýӿ�)
};
#endif

