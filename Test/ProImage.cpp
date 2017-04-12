#include "stdafx.h"
#include "ProImage.h"
ProImage::ProImage()
{
	
}

ProImage::~ProImage()
{

}

void ProImage::Init()
{
	//veccon.clear();
	//chosen.clear();
	vecImageCon.clear();
	//reset();
}

void ProImage::reset()
{
	selectindex = -1;
	recoverflag = 0;
}

void ProImage::preproImage()
{
	/****对于外圈轮廓，先腐蚀去掉剔除不干净的轮廓，再使用膨胀将轮廓胀满整个工件，效果佳***/
	int morph_elem = 0;		//Element:\n 0: Rect - 1: Cross - 2: Ellipse
	int morph_size = 3;
	Mat element = getStructuringElement(morph_elem, Size(2 * morph_size + 1, 2 * morph_size + 1), Point(morph_size, morph_size));
	erode(m_image, m_image, element, Point(-1, -1), 1);
	dilate(m_image, m_image, element, Point(-1, -1), 2);
	
	medianBlur(m_image, m_image, 3);
	//GaussianBlur(m_image, m_image, Size(3, 3), 0, 0);
	//imwrite("houghtest.jpg", m_image);

	
	//morphologyEx(m_image, m_image, MORPH_OPEN, element);
	//imshow("Proimage", m_image);
}

void ProImage::getContour()
{
	this->preproImage();			//预处理
	Mat conimage;
	cvtColor(m_image, conimage, CV_BGR2GRAY);
	conindex = Mat(m_image.rows, m_image.cols, CV_8UC1, Scalar::all(0));
	findContours(conimage, veccon, hierachy,
		CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

	vector<int> index;

	for (int i = 0; i < veccon.size(); i++)		//去除细小轮廓
		if (contourArea(veccon[i]) < m_image.cols*m_image.rows / 50)
		{
			veccon.erase(veccon.begin() + i);
			i--;
		}
	sort(veccon.begin(), veccon.end(), ProImage::compArea);
	
	////////使用ImageCon存储轮廓信息/////////
	vecImageCon.resize(veccon.size());
	for (int i = 0; i < vecImageCon.size(); i++)
	{
		vecImageCon[i].inputCon(veccon[i], i + 1, 1, 8, 2);
		vecImageCon[i].cvtCon2Mat(srcimage);
	}
	////////////////////////////////////////

	//r_orgcon.resize(veccon.size());
	//b_orgcon.resize(veccon.size());
	////////在conindex中记录轮廓坐标及索引值
	//for (int i = 0; i < veccon.size(); i++)
	//{
	//	drawContours(conindex, veccon, i, Scalar(i + 1), 5);
	//	///////////////绘制保存红蓝轮廓
	//	r_orgcon[i] = Mat(m_image.size(), CV_8UC3, Scalar::all(0));
	//	b_orgcon[i] = Mat(m_image.size(), CV_8UC3, Scalar::all(0));
	//	drawContours(r_orgcon[i], veccon, i, RED, 2);
	//	drawContours(b_orgcon[i], veccon, i, BLUE, 2);
	//	//////////////////////////////

	//	///////////////////试验用存图
	//	//Mat tmpimg = Mat(m_image.size(), CV_8UC1, Scalar::all(0));;
	//	//drawContours(tmpimg, veccon, i, Scalar::all(255), 1);
	//	//string strtmpimg = "tmpimg";
	//	//strtmpimg += 'a' + i;
	//	//strtmpimg += ".jpg";
	//	//imwrite(strtmpimg, tmpimg);
	//	/////////////////////////////
	//}

}

void ProImage::setImageWin(const string &_winname)
{
	if (_winname.empty())
		return;
	winname = _winname;
	
	chosen.clear();
	reset();
	//////初始化界面图像，将所有轮廓标红////////
	//m_showimage = srcimage.clone();
	//for (int i = 0; i < veccon.size(); i++)
	//	coverImage(m_showimage, r_orgcon[i]);
	//reset();
	namedWindow(winname, WINDOW_AUTOSIZE);
	///////////////////////////////////////
}

void ProImage::initWin()
{
	m_showimage = srcimage.clone();
	for (int i = 0; i < vecImageCon.size(); i++)
		coverImage(m_showimage, vecImageCon[i].show_image);
	reset();
	//m_showimage = vecImageCon[0].show_image.clone();
	imshow(winname, m_showimage);
}

void ProImage::coverImage(Mat &dst, Mat &img)			//原图像dst上覆盖img
{
	CV_Assert(dst.size() == img.size() && dst.channels() == img.channels());
	int nChannels = img.channels();
	int nRows = img.rows;
	int nCols = img.cols;
	if (img.isContinuous())
	{
		nCols *= nRows;
		nRows = 1;
	}
	uchar* p;
	uchar* q;
	for(int i = 0; i < nRows; i++)
		for (int j = 0; j < nCols; j++)
		{
			p = img.ptr<uchar>(i);
			q = dst.ptr<uchar>(i);
			int k = 0;
			for (k = 0; k < nChannels; k++)
				if (p[j*nChannels+k] != 0)
					break;
			if (k < 3)
				for (k = 0; k < nChannels; k++)
					q[j*nChannels + k] = p[j*nChannels + k];					
		}
}

//void ProImage::showImage()
//{
//	vector<Mat>::iterator iter;						//指定特定轮廓容器的迭代器
//	if (recoverflag == 1)							//轮廓由蓝色（高亮）变为红色（普通）
//		iter = r_orgcon.begin();
//	else if(recoverflag == 0)
//		iter = b_orgcon.begin();
//	
//	if (recoverflag == 1 && 
//		chosen.find(selectindex) != chosen.end())	//防止鼠标滑动过程将已选定轮廓复原为红色
//		return;
//
//	for (int i = 0; i < veccon.size(); i++)
//		if (i == selectindex)
//			coverImage(m_showimage, *(iter+i));
//
//	imshow(winname, m_showimage);
//}

void ProImage::highlightImage(const int &_selectindex)
{
	ImageCon &src = vecImageCon[_selectindex];
	drawContours(m_showimage, vector<vector<Point>>(1, src.contour), -1, src.color, src.thicklinesize);
	imshow(winname, m_showimage);
	//Mat &img = vecImageCon[_selectindex].value_image;		//单通道
	//Mat &dst = m_showimage;									//三通道
	//CV_Assert(dst.size() == img.size() && dst.channels() == 3 && img.channels() == 1);
	//int pChannels = img.channels();
	//int qChannels = dst.channels();
	//int nRows = img.rows;
	//int nCols = img.cols;
	//if (img.isContinuous() && dst.isContinuous())
	//{
	//	nCols *= nRows;
	//	nRows = 1;
	//}
	//uchar* p;			//记录覆盖在上的图像行指针
	//uchar* q;			//记录被覆盖的图像行指针
	//for (int i = 0; i < nRows; i++)
	//	for (int j = 0; j < nCols; j++)
	//	{
	//		p = img.ptr<uchar>(i);
	//		q = dst.ptr<uchar>(i);
	//		if (p[j*pChannels] != 0)
	//		{
	//			for (int k = 0; k < qChannels; k++)
	//				q[j*qChannels + k] = vecImageCon[_selectindex].color[k];

	//		}
	//	}
	//imshow(winname, dst);
}

void ProImage::lowlightImage(const int &_selectindex)
{
	Mat &img1 = vecImageCon[_selectindex].show_image;
	Mat &img2 = srcimage;
	Mat &mask = vecImageCon[_selectindex].value_image;
	Mat &dst = m_showimage;
	CV_Assert(dst.size() == img1.size() && dst.channels() == 3 && 
		mask.channels() == 1 && img1.channels() == 3 && img2.channels() == 3);
	int pChannels = mask.channels();		//mask的通道数，单通道
	int qChannels = dst.channels();			//img1, img2与dst的通道数，三通道
	int nRows = img1.rows;
	int nCols = img1.cols;
	if (img1.isContinuous() && img2.isContinuous() && dst.isContinuous() && mask.isContinuous())
	{
		nCols *= nRows;
		nRows = 1;
	}
	uchar* p_img1;			//img1行指针
	uchar* p_img2;
	uchar* p_dst;
	uchar* p_mask;
	for (int i = 0; i < nRows; i++)
		for (int j = 0; j < nCols; j++)
		{
			p_img1 = img1.ptr<uchar>(i);
			p_img2 = img2.ptr<uchar>(i);
			p_dst = dst.ptr<uchar>(i);
			p_mask = mask.ptr<uchar>(i);
			if (p_mask[j*pChannels] != 0)
			{
				int k;
				for (k = 0; k < qChannels; k++)
				{
					if (p_img1[j*qChannels + k] != 0)
						break;
				}
				if (k >= qChannels)				//说明该点不在show image上，恢复为srcimage
				{
					for (k = 0; k < qChannels; k++)
						p_dst[j*qChannels + k] = p_img2[j*qChannels + k];
				}
				else							//在show image的范围内，恢复为color
				{
					for (k = 0; k < qChannels; k++)
						p_dst[j*qChannels + k] = vecImageCon[_selectindex].color[k];
				}
			}
		}
	imshow(winname, dst);
}

void ProImage::fitContour()
{
	//vecpoly.resize(chosen.size());					//chosen的轮廓数为多边形拟合数目，vecpoly用于存储多边形拟合结果
	//vececllipse.resize(veccon.size() - vecpoly.size());
	//int j = 0, k = 0;
	//for (int i = 0; i < veccon.size(); i++)
	//{
	//	if (chosen.find(i) != chosen.end())
	//	{
	//		approxPolyDP(veccon[i], vecpoly[j], 10, true);
	//		j++;
	//	}
	//	else
	//	{
	//		Mat pointsf;
	//		Mat(veccon[i]).convertTo(pointsf, CV_32F);
	//		vececllipse[k] = fitEllipse(pointsf);	//vecellipse用于存储椭圆拟合结果
	//		k++;
	//	}
	//}
	for (int i = 0; i < vecImageCon.size(); i++)
		vecImageCon[i].fitContour();
}

void ProImage::writeResult(string _imgname)		//记录完整的处理结果
{
	//Mat result = srcimage.clone();
	//for (int i = 0; i < vecpoly.size(); i++)
	//	drawContours(result, vecpoly, i, RED, 2);
	//for(int i = 0; i < vececllipse.size(); i++)
	//	ellipse(result, vececllipse[i], RED, 2, CV_AA);
	//if (_imgname == "")
	//	_imgname = "result.jpg";
	////imshow(_imgname, result);
	//imwrite(_imgname, result);
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
	if (_imgname == "")
		_imgname = "result.jpg";
	//imshow(_imgname, result);
	imwrite(_imgname, result);
}

int ProImage::compArea(vector<Point> first, vector<Point> second)
{
	return contourArea(first) > contourArea(second);
}


/////////////////旧的使用红蓝contour的mousehandle
//void ProImage::onMouseHandle(int event, int x, int y, int flags, void* param)
//{
//	ProImage& proimage = *(ProImage*)param;
//	int index = (int)proimage.conindex.at<uchar>(y, x) - 1;		//轮廓索引值，负值表示无轮廓
//	switch (event)
//	{
//	case CV_EVENT_LBUTTONDOWN:
//		if (index >= 0)
//		{
//			if (proimage.chosen.find(index) != proimage.chosen.end())
//				proimage.chosen.erase(index);
//			else
//				proimage.chosen.insert(index);
//			proimage.showImage();
//		}
//	case CV_EVENT_MOUSEMOVE:
//		if (index >= 0)
//		{
//			if (proimage.selectindex == -1)
//			{
//				proimage.selectindex = index;
//				proimage.recoverflag = 0;
//				proimage.showImage();
//			}
//		}
//		else
//		{
//			if (proimage.selectindex != -1)
//			{
//				proimage.recoverflag = 1;
//				proimage.showImage();
//				proimage.selectindex = -1;
//			}
//		}
//		break;
//	}
//}

void ProImage::onMouseHandle(int event, int x, int y, int flags, void* param)
{
	ProImage& proimage = *(ProImage*)param;
	int index = 0;							//轮廓索引值，正值表示存在轮廓
	if (y < 0 || y >= proimage.srcimage.rows || x < 0 || x >= proimage.srcimage.cols)
		return;
	for (int i = 0; i < proimage.vecImageCon.size(); i++)
	{
		if (proimage.vecImageCon[i].value_image.at<uchar>(y, x) != 0)
		{
			index = proimage.vecImageCon[i].value_image.at<uchar>(y, x);
			break;
		}
	}
	switch (event)
	{
	case CV_EVENT_LBUTTONDOWN:		//左键单击，则对所选轮廓的颜色、状态进行相应更改
		if (index > 0)
		{
			int &_flag = proimage.vecImageCon[index - 1].flag;
			_flag++;
			if (_flag >= sizeof(ImageCon::g_color) / sizeof(Scalar))
				_flag = 0;
			proimage.vecImageCon[index - 1].updateColor();
		}
	case CV_EVENT_MOUSEMOVE:
		if (index > 0)
		{
			proimage.highlightImage(index - 1);
			proimage.selectindex = index;
		}
		else
		{
			if (proimage.selectindex != -1)
			{
				proimage.lowlightImage(proimage.selectindex - 1);
				proimage.selectindex = -1;
			}
		}
		break;
	}
}

void ProImage::getData(ProImage Temp)		//ProImage类之间传递被选轮廓的index
{
	//set<int>::iterator iter;
	//for (iter = Temp.chosen.begin(); iter != Temp.chosen.end(); iter++)
	//	chosen.insert(*iter);
	CV_Assert(Temp.vecImageCon.size() == this->vecImageCon.size());
	for (int i = 0; i < this->vecImageCon.size(); i++)
	{
		this->vecImageCon[i].flag = Temp.vecImageCon[i].flag;
		this->vecImageCon[i].updateColor();
	}
}

void ProImage::printContour()			//提取实验用图的代码
{
	ofstream fresult;
	fresult.open("contour.txt", ios::in | ios::ate);
	if (fresult.is_open())
	{
		for (int i = 0; i < veccon.size(); i++)
		{
			if (i == veccon.size() - 1)
			{
				fresult << i << endl;
				for (size_t j = 0; j < veccon[i].size(); j++)
				{
					fresult << veccon[i][j].x << " " << veccon[i][j].y << endl;
				}
			}
		}
	}
}
