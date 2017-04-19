#include "stdafx.h"
#include "CalcLoc.h"
CalcLoc::CalcLoc():
	moveratio(0.01)
{

}

CalcLoc::~CalcLoc()
{

}

void CalcLoc::getCenter()
{
	/*将被选中轮廓的中心进行累加求平均值作为工件中心*/

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

	Point2f tmpcenter = vecImageCon[angleindex].concenter;		//角度基准轮廓中心
	line(fitresult, Point2f(tmpcenter.x + fitresult.cols, tmpcenter.y + fitresult.cols * tan(m_angle)),
		Point2f(tmpcenter.x - fitresult.cols, tmpcenter.y - fitresult.cols * tan(m_angle)), PINK, 2);

	circle(fitresult, m_center, 3, RED, -1);
	if (_imgname == "")
		_imgname = "result.jpg";
	imwrite(_imgname, fitresult);
}

void CalcLoc::showImage(const string &winname)
{
	m_showimage = fitresult.clone();
	
	line(m_showimage, Point2f(compcenter.x + m_showimage.cols, compcenter.y + m_showimage.cols * tan(compangle)),
		Point2f(compcenter.x - m_showimage.cols, compcenter.y - m_showimage.cols * tan(compangle)), YELLOW, 2);
	circle(m_showimage, compcenter, 3, ORCHID, -1);
	imshow(winname, m_showimage);
}

void CalcLoc::moveCam(int &flag)
{
	switch (flag)
	{
	case MOVE_RIGHT:
		compcenter.x += fitresult.cols*moveratio;
		if (compcenter.x >= TopRight.x)
			compcenter.x = TopRight.x;
		break;
	case MOVE_LEFT:
		compcenter.x -= fitresult.cols*moveratio;
		if (compcenter.x <= TopLeft.x)
			compcenter.x = TopLeft.x;
		break;
	case MOVE_DOWN:
		compcenter.y += fitresult.rows*moveratio;
		if (compcenter.y >= BottomLeft.y)
			compcenter.y = BottomLeft.y;
		break;
	case MOVE_UP:
		compcenter.y -= fitresult.rows*moveratio;
		if (compcenter.y <= TopLeft.y)
			compcenter.y = TopLeft.y;
		break;
	case ROTATE_CLOCKWISE:
		compangle += PI / 180;
		break;
	case ROTATE_ANTICLOCKWISE:
		compangle -= PI / 180;
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
	compcenter = m_center;
	compangle = m_angle;
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