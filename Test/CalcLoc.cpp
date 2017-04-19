#include "stdafx.h"
#include "CalcLoc.h"
CalcLoc::CalcLoc()
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
	line(fitresult, Point2f(tmpcenter.x + fitresult.cols / 2, tmpcenter.y + fitresult.cols / 2 * tan(m_angle)),
		Point2f(tmpcenter.x - fitresult.cols / 2, tmpcenter.y - fitresult.cols / 2 * tan(m_angle)), GREEN, 2);

	circle(fitresult, m_center, 3, RED, -1);
	if (_imgname == "")
		_imgname = "result.jpg";
	imwrite(_imgname, fitresult);
}

void CalcLoc::showImage(string &winname)
{
	
}

void CalcLoc::moveCam(int &flag)
{

}

int CalcLoc::getFetchCenterAngle()
{
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
		//showImage();
	}
}