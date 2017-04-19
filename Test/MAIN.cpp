#include "stdafx.h"
#include "HSVHist.h"
#include "ProImage.h"
#include "CalcLoc.h"
#include <fstream>
#include <stdio.h>
#include <time.h>
#define TEST4

INITIALIZE_EASYLOGGINGPP

int main()
{
	Configurations conf("my-conf.conf");
	Loggers::reconfigureLogger("default", conf);
	LOG(TRACE) << "Begin";
#ifdef SINGLE
	string srcpath = "4.bmp";
	string backpath = "1.bmp";
	CalcLoc src, back;
	if (!src.getImage(srcpath) || !back.getImage(backpath))
		return 0;

	/////////根据直方图去除背景///////////
	HSVHist::removeBack(src, back);
	src.HSVHist::showImage();
	//////////////////////////////////////

	src.getContour();

	string winname = "UI";
	src.setImageWin(winname);

	src.showImage();

	setMouseCallback(winname, ProImage::onMouseHandle, (void*)&src);
	for (int endsign = 0; endsign != 1; )
	{
		int c = waitKey(0);
		switch ((char)c)
		{
		case 'f':
			endsign = 1;
			src.fitContour();
			src.writeResult();
			break;
		case 'r':
			src.reset();
		}
	}

	src.getHUMoment();
	src.getCenter();

	waitKey(0);

	return 0;
#elif defined MULTIPLE
	CalcLoc Temp, Dst;
	HSVHist Back;
	string strpBack = "back.bmp", strpTemp = "wp_000.bmp", strpDst = "wp_";
	int Dstindex = 5;

	if (!Back.getImage(strpBack) || !Temp.getImage(strpTemp))
		return 0;

	HSVHist::removeBack(Temp, Back);

	Temp.getContour();

	string winname = "UI";
	Temp.setImageWin(winname);
	Temp.showImage();
	setMouseCallback(winname, ProImage::onMouseHandle, (void*)&Temp);
	for (int endsign = 0; endsign != 1; )
	{
		int c = waitKey(0);
		switch ((char)c)
		{
		case 'f':
			endsign = 1;
			Temp.fitContour();
			Temp.writeResult();
			Temp.getCenter();
			break;
		}
	}
	time_t nowtime;
	nowtime = time(NULL);
	CalcLoc::printResult(nowtime);
	CalcLoc::printResult(Temp);
	Temp.writeResult("r"+strpTemp);

	while (1)
	{
		string strpath;
		char strindex[3];
		sprintf(strindex, "%03d", Dstindex);
		strpath = strpDst + strindex + ".bmp";
		if (Dst.getImage(strpath) != 1)
			break;

		HSVHist::removeBack(Dst, Back);

		Dst.Init();
		Dst.getContour();

		Dst.getData(Temp);
		Dst.fitContour();
		Dst.getCenter();

		CalcLoc::printResult(Dst);
		Dst.writeResult("r"+strpath);

		Dstindex += 5;
	}
	cout << "Over" << endl;
	waitKey(0);
	return 0;
#elif defined TEST1
	string strfore = "wp_000.bmp", strback = "1.bmp";
	Mat mask = imread("2syk.jpg", 0);
	threshold(mask, mask, 254, 255, THRESH_BINARY);
	imshow("mask", mask);
	Mat fmask = Mat(mask.size(), CV_8UC1, Scalar::all(255));

	ProImage src, back;
	src.getImage(strfore);
	back.getImage(strback);

	src.getHist();
	src.drawHist();

	back.getHist();
	//back.getHist();
	back.drawHist();

	src.removeSeg(back);
	//src.preproImage();
	src.showImage();
	waitKey(0);
	return 0;
#elif defined TEST2
	string strfore = "4.bmp", strback = "1.bmp";
	ProImage src, back;
	src.getImage(strfore);
	back.getImage(strback);
	HSVHist::removeBack(src, back);

	src.preproImage();
	src.getContour();

	src.ProImage::printContour();
	
	//string winname = "test";
	//src.setImageWin(winname);
	//src.showImage();



	waitKey(0);
	return 0;
#elif defined TEST3
	string strfore = "2.bmp", strback = "1.bmp";
	CalcLoc src, back;
	src.getImage(strfore);
	back.getImage(strback);
	//Mat mask = imread("2syk.jpg", 0);
	//threshold(mask, mask, 254, 255, THRESH_BINARY_INV);
	//src.getHist();
	//back.getHist();
	//src.drawHist();
	//back.drawHist();
	//src.removeSeg(back);
	HSVHist::removeBack(src, back);
	//src.HSVHist::showImage();

	src.getContour();
	string winname = "test";
	src.setImageWin(winname);
	src.initWin();
	setMouseCallback(winname, ProImage::onMouseHandle, (void*)&src);

	for (int endsign = 0; endsign != 1; )
	{
		int c = waitKey(0);
		switch ((char)c)
		{
		case 'f':
			endsign = 1;
			src.fitContour();
			src.writeResult();
			destroyAllWindows();			//去除所有窗口
			break;
		case 'r':
			src.reset();
		}
	}
	src.getCenter();
	src.getAngle();
	src.getFetchCenterAngle(winname);
	src.writeResult();
	src.affineNegTrans();
	//string winname = "test";
	//src.setImageWin(winname);
	//src.showImage();

	waitKey(0);
	return 0;
#elif defined TEST4
	CalcLoc Temp, Dst;
	HSVHist Back;
	string strpBack = "back.bmp", strpTemp = "wp_000.bmp", strpDst = "wp_";
	int Dstindex = 5;

	if (!Back.getImage(strpBack) || !Temp.getImage(strpTemp))
	return 0;

	HSVHist::removeBack(Temp, Back);

	Temp.getContour();

	string winname = "UI";
	Temp.setImageWin(winname);
	Temp.initWin();
	setMouseCallback(winname, ProImage::onMouseHandle, (void*)&Temp);
	for (int endsign = 0; endsign != 1; )
	{
		int c = waitKey(0);
		switch ((char)c)
		{
		case 'f':
			endsign = 1;
			Temp.fitContour();
			destroyAllWindows();
			//Temp.writeResult();
			break;
		case 'r':
			Temp.reset();
		}
	}

	Temp.getFitResult();
	Temp.getFetchCenterAngle(winname);
	Temp.affineNegTrans();

	time_t nowtime;
	nowtime = time(NULL);
	CalcLoc::printResult(nowtime);
	CalcLoc::printResult(Temp);
	Temp.writeResult("r" + strpTemp);

	while (1)
	{
		string strpath;
		char strindex[3];
		sprintf(strindex, "%03d", Dstindex);
		strpath = strpDst + strindex + ".bmp";
		if (Dst.getImage(strpath) != 1)
			break;

		HSVHist::removeBack(Dst, Back);

		Dst.reset();
		Dst.getContour();

		Dst.getData(Temp);
		Dst.fitContour();

		Dst.getFitResult();
		Dst.affinePosTrans();

		CalcLoc::printResult(Dst);
		Dst.writeResult("r" + strpath);

		Dstindex += 5;
	}
	cout << "Over" << endl;
	waitKey(0);
	return 0;
#endif
}
