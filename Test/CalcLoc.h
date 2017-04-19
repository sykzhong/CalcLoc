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
	EXIT = -1
	//SAVE_AUTO_MODE = 9,			//按顺序扫描
	//SAVE_MANUAL_MODE = 10,		//人工控制扫描
	//SAVE_AUTO2_MODE = 11			//乱序情况下的扫描
};

class CalcLoc : public ProImage
{
public:
	CalcLoc();
	~CalcLoc();
	void getCenter();				//叠加轮廓中心去平均值作为中心
	void getAngle();				//取角度基准轮廓计算角度
	void getFitResult();

	void writeResult(string _imgname = "");
	static void printResult(CalcLoc &dst);		
	static void printResult(time_t nowtime);

	int getFetchCenterAngle(const string &winname = "result");		//用户自定义抓取点与抓取角度
	void Init();													//对界面进行初始化
	void moveCam(int &flag);										//用户操作移动相机
	void showImage(const string &winname = "result");				//显示用户操作情况

	void affineNegTrans();				//逆变换，将绝对坐标系抓取中心与角度映射到相对坐标系中
	void affinePosTrans();				//正变换，将相对坐标系抓取中心与角度映射到绝对坐标系中

	void reset();					//重复使用同一对象时，进行参数重置

	//static void mapCenterAngle(CalcLoc _template, CalcLoc _target);	//将模板中的抓取点、抓取角度等信息映射至target中并求出其绝对抓取点与角度

private:
	Mat fitresult;					//记录拟合结果
	Mat m_showimage;				//用于实时显示的图像

	Point2f m_center;
	double m_angle;

	Point2f realcenter;				//绝对坐标系中，用户抓取点，默认与工件中心重叠
	double realangle;				//绝对坐标系中，用户抓取角度，默认与工件基准角度重叠
	double moveratio;				//坐标移动速率

	static Point2f compcenter;		//相对坐标系中，用户抓取点相对工件中心距离。工件基准角度作为x轴
	static double compangle;		//相对坐标系中，用户抓取角度相对工件基准角度的差值

	Point TopLeft, TopRight, BottomLeft, BottomRight;		//可观测区的四个角点(留作机械手移动极限位置接口)
};
#endif

