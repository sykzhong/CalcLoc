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
	/****������Ȧ�������ȸ�ʴȥ���޳����ɾ�����������ʹ�����ͽ�������������������Ч����***/
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
	this->preproImage();			//Ԥ����
	Mat conimage;
	cvtColor(m_image, conimage, CV_BGR2GRAY);
	conindex = Mat(m_image.rows, m_image.cols, CV_8UC1, Scalar::all(0));
	findContours(conimage, veccon, hierachy,
		CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

	vector<int> index;

	for (int i = 0; i < veccon.size(); i++)		//ȥ��ϸС����
		if (contourArea(veccon[i]) < m_image.cols*m_image.rows / 50)
		{
			veccon.erase(veccon.begin() + i);
			i--;
		}
	sort(veccon.begin(), veccon.end(), ProImage::compArea);
	
	////////ʹ��ImageCon�洢������Ϣ/////////
	vecImageCon.resize(veccon.size());
	for (int i = 0; i < vecImageCon.size(); i++)
	{
		vecImageCon[i].inputCon(veccon[i], i + 1, 1, 8, 2);
		vecImageCon[i].cvtCon2Mat(srcimage);
	}
	////////////////////////////////////////

	//r_orgcon.resize(veccon.size());
	//b_orgcon.resize(veccon.size());
	////////��conindex�м�¼�������꼰����ֵ
	//for (int i = 0; i < veccon.size(); i++)
	//{
	//	drawContours(conindex, veccon, i, Scalar(i + 1), 5);
	//	///////////////���Ʊ����������
	//	r_orgcon[i] = Mat(m_image.size(), CV_8UC3, Scalar::all(0));
	//	b_orgcon[i] = Mat(m_image.size(), CV_8UC3, Scalar::all(0));
	//	drawContours(r_orgcon[i], veccon, i, RED, 2);
	//	drawContours(b_orgcon[i], veccon, i, BLUE, 2);
	//	//////////////////////////////

	//	///////////////////�����ô�ͼ
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
	//////��ʼ������ͼ�񣬽������������////////
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

void ProImage::coverImage(Mat &dst, Mat &img)			//ԭͼ��dst�ϸ���img
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
//	vector<Mat>::iterator iter;						//ָ���ض����������ĵ�����
//	if (recoverflag == 1)							//��������ɫ����������Ϊ��ɫ����ͨ��
//		iter = r_orgcon.begin();
//	else if(recoverflag == 0)
//		iter = b_orgcon.begin();
//	
//	if (recoverflag == 1 && 
//		chosen.find(selectindex) != chosen.end())	//��ֹ��껬�����̽���ѡ��������ԭΪ��ɫ
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
	//Mat &img = vecImageCon[_selectindex].value_image;		//��ͨ��
	//Mat &dst = m_showimage;									//��ͨ��
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
	//uchar* p;			//��¼�������ϵ�ͼ����ָ��
	//uchar* q;			//��¼�����ǵ�ͼ����ָ��
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
	int pChannels = mask.channels();		//mask��ͨ��������ͨ��
	int qChannels = dst.channels();			//img1, img2��dst��ͨ��������ͨ��
	int nRows = img1.rows;
	int nCols = img1.cols;
	if (img1.isContinuous() && img2.isContinuous() && dst.isContinuous() && mask.isContinuous())
	{
		nCols *= nRows;
		nRows = 1;
	}
	uchar* p_img1;			//img1��ָ��
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
				if (k >= qChannels)				//˵���õ㲻��show image�ϣ��ָ�Ϊsrcimage
				{
					for (k = 0; k < qChannels; k++)
						p_dst[j*qChannels + k] = p_img2[j*qChannels + k];
				}
				else							//��show image�ķ�Χ�ڣ��ָ�Ϊcolor
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
	//vecpoly.resize(chosen.size());					//chosen��������Ϊ����������Ŀ��vecpoly���ڴ洢�������Ͻ��
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
	//		vececllipse[k] = fitEllipse(pointsf);	//vecellipse���ڴ洢��Բ��Ͻ��
	//		k++;
	//	}
	//}
	for (int i = 0; i < vecImageCon.size(); i++)
		vecImageCon[i].fitContour();
}

void ProImage::writeResult(string _imgname)		//��¼�����Ĵ�����
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


/////////////////�ɵ�ʹ�ú���contour��mousehandle
//void ProImage::onMouseHandle(int event, int x, int y, int flags, void* param)
//{
//	ProImage& proimage = *(ProImage*)param;
//	int index = (int)proimage.conindex.at<uchar>(y, x) - 1;		//��������ֵ����ֵ��ʾ������
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
	int index = 0;							//��������ֵ����ֵ��ʾ��������
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
	case CV_EVENT_LBUTTONDOWN:		//��������������ѡ��������ɫ��״̬������Ӧ����
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

void ProImage::getData(ProImage Temp)		//ProImage��֮�䴫�ݱ�ѡ������index
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

void ProImage::printContour()			//��ȡʵ����ͼ�Ĵ���
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
