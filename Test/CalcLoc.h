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
	ZOOM_IN = 4,					//放大视野
	ZOOM_OUT = 5,					//缩小视野
	ROTATE_CLOCKWISE = 6,			//顺时针旋转
	ROTATE_ANTICLOCKWISE = 7,		//逆时针旋转
	EXIT = 8,
	//SAVE_AUTO_MODE = 9,				//按顺序扫描
	//SAVE_MANUAL_MODE = 10,			//人工控制扫描
	//SAVE_AUTO2_MODE = 11			//乱序情况下的扫描
};

class CalcLoc : public ProImage
{
public:
	CalcLoc();
	~CalcLoc();
	void getCenter();				//叠加轮廓中心去平均值作为中心
	void getAngle();				//取角度基准轮廓计算角度
	void writeResult(string _imgname = "");
	static void printResult(CalcLoc &dst);		
	static void printResult(time_t nowtime);
	int getFetchCenterAngle();		//用户自定义抓取点与抓取角度
	void moveCam(int &flag);		//用户操作移动相机
	void showImage(string &winname);				//显示用户操作情况

private:
	Mat fitresult;					//记录拟合结果

	Point2f m_center;
	double m_angle;

	Point2f comPoint;				//绝对坐标系中，用户抓取点与工件中心的相对距离
	double comAngle;				//绝对坐标系中，用户抓取角度与工件角度的相对差值（以工件角度轴作为基准）
};
#endif

