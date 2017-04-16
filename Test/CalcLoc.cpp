#include "stdafx.h"
#include "CalcLoc.h"
CalcLoc::CalcLoc()
{

}

CalcLoc::~CalcLoc()
{

}

void CalcLoc::getHUMoment()
{
	Moments tmpmoment;
	for (int i = 0; i < vecImageCon.size(); i++)
	{
		if (vecImageCon[i].flag == CON_POLY)
		{
			tmpmoment = moments(vecImageCon[i].polycontour, false);
			vecmu.push_back(tmpmoment);
		}
	}
	//vecmu.resize(vecpoly.size());
	//for (int i = 0; i < vecmu.size(); i++)
	//	vecmu[i] = moments(vecpoly[i], false);
}

void CalcLoc::getCenter()
{
	/*将被选中轮廓的中心进行累加求平均值作为工件中心*/

	for (int i = 0; i < vecImageCon.size(); i++)
		m_center += vecImageCon[i].concenter;
	m_center /= (double)vecImageCon.size();
	/*this->getHUMoment();
	m_center = Point2f(0, 0);
	m_Ecenter.clear();
	m_Pcenter.clear();
	for (int i = 0; i < vecImageCon.size(); i++)
		if (vecImageCon[i].flag == CON_ELLIPSE)
		{
			m_Ecenter.push_back(vecImageCon[i].ellipsebox.center);
			m_center += m_Ecenter.back();
		}
	for (int i = 0; i < vecmu.size(); i++)
	{
		m_Pcenter.push_back(Point2f(vecmu[i].m10 / vecmu[i].m00, vecmu[i].m01 / vecmu[i].m00));
		m_center += m_Pcenter.back();
	}
	m_center /= (float)(m_Pcenter.size() + m_Ecenter.size());*/

	/*this->getHUMoment();
	m_Pcenter.resize(vecpoly.size());
	m_Ecenter.resize(vececllipse.size());
	m_center = Point2f(0, 0);
	for (int i = 0; i < vecmu.size(); i++)
	{
		m_Pcenter[i] = Point2f(vecmu[i].m10 / vecmu[i].m00, vecmu[i].m01 / vecmu[i].m00);
		m_center += m_Pcenter[i];
	}
	for (int i = 0; i < vececllipse.size(); i++)
	{
		m_Ecenter[i] = vececllipse[i].center;
		m_center += m_Ecenter[i];
	}
	m_center /= (float)(m_Pcenter.size() + m_Ecenter.size());*/
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
	Mat result = srcimage.clone();
	for (int i = 0; i < vecImageCon.size(); i++)
	{
		ImageCon &src = vecImageCon[i];
		switch (src.flag)
		{
		case CON_IGNORE:
			break;
		case CON_NORMAL:
			drawContours(result, vector<vector<Point>>(1, src.contour), -1, src.color, src.thinlinesize);
			break;
		case CON_POLY:
			drawContours(result, vector<vector<Point>>(1, src.polycontour), -1, src.color, src.thinlinesize);
			break;
		case CON_ELLIPSE:
			ellipse(result, src.ellipsebox, src.color, 2, CV_AA);
			break;
		}
	}
	circle(result, m_center, 3, RED, -1);
	if (_imgname == "")
		_imgname = "result.jpg";
	//imshow(_imgname, result);
	imwrite(_imgname, result);
	//Mat result = srcimage.clone();
	//for (int i = 0; i < vecpoly.size(); i++)
	//	drawContours(result, vecpoly, i, RED, 2);
	//for (int i = 0; i < vececllipse.size(); i++)
	//	ellipse(result, vececllipse[i], RED, 2, CV_AA);
	//circle(result, m_center, 3, RED, -1);
	//if (_imgname == "")
	//	_imgname = "result.jpg";
	////imshow(_imgname, result);
	//imwrite(_imgname, result);
}