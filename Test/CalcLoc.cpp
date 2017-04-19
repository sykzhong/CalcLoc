#include "stdafx.h"
#include "CalcLoc.h"

Point2f CalcLoc::compcenter = Point2f(0, 0);
double CalcLoc::compangle = 0;

CalcLoc::CalcLoc():
	moveratio(0.01)
{

}

CalcLoc::~CalcLoc()
{

}

void CalcLoc::getCenter()
{
	/*����ѡ�����������Ľ����ۼ���ƽ��ֵ��Ϊ��������*/

	for (int i = 0; i < vecImageCon.size(); i++)
		m_center += vecImageCon[i].concenter;
	m_center /= (double)vecImageCon.size();
}

void CalcLoc::getAngle()
{
	int index = this->ProImage::angleindex;
	m_angle = vecImageCon[index].conth;
}

void CalcLoc::printResult(CalcLoc &dst)
{
	ofstream fresult;
	fresult.open("result.txt", ios::in | ios::ate);
	if (fresult.is_open())
	{
		fresult << dst.m_center.x << " " << dst.m_center.y << endl;
	}
}

void CalcLoc::printResult(time_t nowtime)
{
	ofstream fresult;
	fresult.open("result.txt", ios::in | ios::ate);
	if (fresult.is_open())
		fresult << ctime(&nowtime) << endl;
}

void CalcLoc::writeResult(string _imgname)
{
	fitresult = srcimage.clone();
	for (int i = 0; i < vecImageCon.size(); i++)
	{
		ImageCon &src = vecImageCon[i];
		switch (src.flag)
		{
		case CON_IGNORE:
			break;
		case CON_NORMAL:
			drawContours(fitresult, vector<vector<Point>>(1, src.contour), -1, src.color, src.thinlinesize);
			break;
		case CON_POLY:
			drawContours(fitresult, vector<vector<Point>>(1, src.polycontour), -1, src.color, src.thinlinesize);
			break;
		case CON_ELLIPSE:
			ellipse(fitresult, src.ellipsebox, src.color, 2, CV_AA);
			break;
		}
	}

	Point2f tmpcenter = vecImageCon[angleindex].concenter;		//�ǶȻ�׼��������
	if(abs(abs(m_angle) - PI / 2) > PI/360)
		line(fitresult, Point2f(tmpcenter.x + fitresult.cols, tmpcenter.y + fitresult.cols * tan(m_angle)),
			Point2f(tmpcenter.x - fitresult.cols, tmpcenter.y - fitresult.cols * tan(m_angle)), LIGHTBLUE, 2);
	else
		line(fitresult, Point2f(tmpcenter.x, 0), Point2f(tmpcenter.x, fitresult.rows), LIGHTBLUE, 2);
	circle(fitresult, m_center, 3, RED, -1);

	if (abs(abs(realangle) - PI / 2) > PI / 360)
		line(m_showimage, Point2f(realcenter.x + m_showimage.cols, realcenter.y + m_showimage.cols * tan(realangle)),
			Point2f(realcenter.x - m_showimage.cols, realcenter.y - m_showimage.cols * tan(realangle)), YELLOW, 2);
	else
		line(m_showimage, Point2f(realcenter.x, 0), Point2f(realcenter.x, m_showimage.rows), YELLOW, 2);
	circle(fitresult, realcenter, 3, ORCHID, -1);

	if (_imgname == "")
		_imgname = "result.jpg";
	imwrite(_imgname, fitresult);
}

void CalcLoc::showImage(const string &winname)
{
	m_showimage = fitresult.clone();
	if (abs(abs(realangle) - PI / 2) > PI / 360)
		line(m_showimage, Point2f(realcenter.x + m_showimage.cols, realcenter.y + m_showimage.cols * tan(realangle)),
			Point2f(realcenter.x - m_showimage.cols, realcenter.y - m_showimage.cols * tan(realangle)), YELLOW, 2);
	else
		line(m_showimage, Point2f(realcenter.x, 0),	Point2f(realcenter.x, m_showimage.rows), YELLOW, 2);

	circle(m_showimage, realcenter, 3, ORCHID, -1);
	imshow(winname, m_showimage);
}

void CalcLoc::moveCam(int &flag)
{
	switch (flag)
	{
	case MOVE_RIGHT:
		realcenter.x += fitresult.cols*moveratio;
		if (realcenter.x >= TopRight.x)
			realcenter.x = TopRight.x;
		break;
	case MOVE_LEFT:
		realcenter.x -= fitresult.cols*moveratio;
		if (realcenter.x <= TopLeft.x)
			realcenter.x = TopLeft.x;
		break;
	case MOVE_DOWN:
		realcenter.y += fitresult.rows*moveratio;
		if (realcenter.y >= BottomLeft.y)
			realcenter.y = BottomLeft.y;
		break;
	case MOVE_UP:
		realcenter.y -= fitresult.rows*moveratio;
		if (realcenter.y <= TopLeft.y)
			realcenter.y = TopLeft.y;
		break;
		/***realangle�ĽǶȷ�Χ��[-PI/2, PI/2]***/
	case ROTATE_CLOCKWISE:
		realangle += PI / 180;
		if (realangle > PI / 2)
			realangle = PI - realangle;		
		break;
	case ROTATE_ANTICLOCKWISE:
		realangle -= PI / 180;
		break;
	default:
		break;
	}
}

void CalcLoc::Init()
{
	TopRight = Point(fitresult.cols, 0);
	TopLeft = Point(0, 0);
	BottomLeft = Point(0, fitresult.rows);
	BottomRight = Point(fitresult.cols, fitresult.rows);
	realcenter = m_center;
	realangle = m_angle;
	moveratio = 0.005;
}

int CalcLoc::getFetchCenterAngle(const string &winname)
{
	Init();
	for (;;)
	{
		int c = waitKey(0);
		int flag = -1;
		switch ((char)c)
		{
		case 'd':
			flag = MOVE_RIGHT;
			break;
		case 'a':
			flag = MOVE_LEFT;
			break;
		case 'w':
			flag = MOVE_UP;
			break;
		case 's':
			flag = MOVE_DOWN;
			break;
		case 'o':
			flag = ROTATE_CLOCKWISE;
			break;
		case 'p':
			flag = ROTATE_ANTICLOCKWISE;
			break;
		case 27:
			return EXIT;
		default:
			cout << "Wrong button ..." << endl;
			break;
		}
		moveCam(flag);
		showImage();
	}
}

void CalcLoc::affineNegTrans()
{
	/***��֪��������ϵץȡ�㡢ץȡ�Ƕȣ����������ϵ������ڹ������ġ�������׼�Ƕȵ�ֵ***/
	Point2f diffcenter = realcenter - m_center;			//��������ϵ��ץȡ���빤�����ĵĲ�ֵ

	compcenter.x = diffcenter.x*cos(m_angle) + diffcenter.y*sin(m_angle);	//����任
	compcenter.y = diffcenter.x*-sin(m_angle) + diffcenter.y*cos(m_angle);

	compangle = realangle - m_angle;
}

void CalcLoc::affinePosTrans()
{
	/***��֪�������ϵץȡ�㡢ץȡ�Ƕȣ����������ϵ������ڹ������ġ�������׼�Ƕȵ�ֵ***/
	Point2f diffcenter;			//��������ϵ��ץȡ���빤�����ĵĲ�ֵ

	diffcenter.x = compcenter.x*cos(m_angle) + compcenter.y*-sin(m_angle);
	diffcenter.y = compcenter.x*sin(m_angle) + compcenter.y*cos(m_angle);

	realcenter = diffcenter + m_center;

	realangle = compangle + m_angle;
}

//void mapCenterAngle(CalcLoc _template, CalcLoc _target)
//{
//	
//}