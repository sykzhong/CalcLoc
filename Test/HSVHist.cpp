#include "stdafx.h"
#include "HSVHist.h"
HSVHist::HSVHist()
{
	//histsize[0] = 8;		//H通道bin数
	//histsize[1] = 40;		//S通道
	//histsize[2] = 8;		//V通道

	histsize[0] = 8;		//H通道bin数
	histsize[1] = 24;		//S通道
	histsize[2] = 4;		//V通道

	hrange[0] = 0;			//神tmbug，不如此中规中矩地设为180会出错
	hrange[1] = 180;
	ranges[0] = hrange;

	svrange[0] = 0;
	svrange[1] = 255;

	for (int i = 1; i < 3; i++)
		ranges[i] = svrange;		//各通道取值范围
	
	bin_w = 3;						//单格宽度
	hist_h = 60;
	hist_w = histsize[0]*histsize[1] *bin_w;	//hs的bin数总和，单格宽为6
	//hist_img = Mat(hist_h, hist_w, CV_8UC3, Scalar::all(0));		//HS版本
	hist_img = Mat(hist_h*histsize[2], hist_w, CV_8UC3, Scalar::all(0));
}

HSVHist::~HSVHist()
{

}

int HSVHist::getImage(string path)
{
	srcimage = imread(path, 1);
	if (!srcimage.data)
		return 0;
	this->path = path;
	m_image = srcimage.clone();
	cvtColor(m_image, m_image, COLOR_BGR2HSV);
	return 1;
}

void HSVHist::Init()
{
	getHist();
	drawHist();
}

void HSVHist::getHist()
{
	split(m_image, hsvplane);			//通道分离
	int channels[3] = { 0, 1, 2};
	//calcHist(hsvplane, 3, channels, Mat(), hsvhist, 2, histsize, ranges);		//直方图计算，仅HS通道
	calcHist(hsvplane, 3, channels, Mat(), hsvhist, 3, histsize, ranges);		//直方图计算，HSV三通道

	//归一化处理，仅HS通道
	//minMaxLoc(hsvhist, 0, &maxval, 0, 0);										//直方图中最大统计值的提取
	//for (int i = 0; i < histsize[0]; i++)
	//	for (int j = 0; j < histsize[1]; j++)
	//	{
	//		float binval = hsvhist.at<float>(i, j);
	//		binval = round(binval*hist_h / maxval);
	//		hsvhist.at<float>(i, j) = binval;
	//	}
	for (int i = 0; i < histsize[0]; i++)
		for (int j = 0; j < histsize[1]; j++)
			for (int k = 0; k < histsize[2]; k++)
			{
				if (maxval < hsvhist.at<float>(i, j, k))
					maxval = hsvhist.at<float>(i, j, k);
			}
	for (int i = 0; i < histsize[0]; i++)
		for (int j = 0; j < histsize[1]; j++)
			for(int k = 0; k < histsize[2]; k++)
			{
				float binval = hsvhist.at<float>(i, j, k);
				binval = round(binval*hist_h / maxval);
				hsvhist.at<float>(i, j, k) = binval;
			}	
}

void HSVHist::drawHist()
{
	/************HS双通道版通道*************/
	//Mat hsv_color = Mat(1, 1, CV_8UC3);
	//Mat rgb_color = Mat(1, 1, CV_8UC3);
	//for(int i = 0; i < histsize[0]; i++)
	//	for (int j = 0; j < histsize[1]; j++)
	//	{
	//		int loc = i*histsize[1] + j;
	//		float binval = hsvhist.at<float>(i, j);
	//		//printf("i:%d j:%d binval:%.0f\n", i, j, binval);
	//		
	//		//颜色由hsv转换为rgb
	//		hsv_color.setTo(Scalar(i*179.f / histsize[0], j*255.f / histsize[1], 255, 0));
	//		cvtColor(hsv_color, rgb_color, COLOR_HSV2BGR);
	//		Scalar color = rgb_color.at<Vec3b>(0, 0);
	//		
	//		rectangle(hist_img, Point(loc*bin_w, hist_h), Point((loc + 1)*bin_w, hist_h - binval), color, -1);
	//	}
	Mat hsv_color = Mat(1, 1, CV_8UC3);
	Mat rgb_color = Mat(1, 1, CV_8UC3);
	for (int k = 0; k < histsize[2]; k++)
	{
		for (int i = 0; i < histsize[0]; i++)
			for (int j = 0; j < histsize[1]; j++)
			{
				int loc = i*histsize[1] + j;
				float binval = hsvhist.at<float>(i, j, k);
				//printf("i:%d j:%d binval:%.0f\n", i, j, binval);

				//颜色由hsv转换为rgb
				//hsv_color.setTo(Scalar(i*179.f / histsize[0], j*255.f / histsize[1], 255, 0));		//HS版本
				//hsv_color.setTo(Scalar(i*179.f / histsize[0], j*255.f / histsize[1], k*255.f / histsize[2], 0));		//HSV变亮版本
				hsv_color.setTo(Scalar(i*179.f / histsize[0], j*255.f / histsize[1], 255.f, 0));		//HSV变亮版本
				cvtColor(hsv_color, rgb_color, COLOR_HSV2BGR);
				Scalar color = rgb_color.at<Vec3b>(0, 0);
				//rectangle(hist_img, Point(loc*bin_w, hist_h), Point((loc + 1)*bin_w, hist_h - binval), color, -1);		//HS版本
				rectangle(hist_img, Point(loc*bin_w, hist_h*(k + 1)), Point((loc + 1)*bin_w, hist_h*(k + 1) - binval), color, -1);
			}
	}
	
	imshow(path, hist_img);
}

void HSVHist::removeSeg(HSVHist back)
{
	int nRows = m_image.rows;
	int nCols = m_image.cols;
	int nChannels = m_image.channels();
	if(m_image.isContinuous() == true)
	{
		nCols *= nRows;
		nRows = 1;
	}
	uchar *psrc;
	int Hval, Sval;
	int Vval;			//V通道
	for(int i = 0; i < nRows; i++)
		for (int j = 0; j < nCols; j++)
		{
			//提取原图中的色阶
			psrc = m_image.ptr<uchar>(i);
			Hval = psrc[j*nChannels]*histsize[0] / 180;
			Sval = psrc[j*nChannels + 1]*histsize[1] / 256;
			Vval = psrc[j*nChannels + 2] * histsize[2] / 256;
			//if (back.hsvhist.at<float>(Hval, Sval) > 0)		//HS版本
			//	for (int k = 0; k < 3; k++)
			//		psrc[j*nChannels + k] = 0;
			//if (back.hsvhist.at<float>(Hval, Sval, Vval) > 0 || psrc[j*nChannels + 2] * histsize[2] > 230)
			if (back.hsvhist.at<float>(Hval, Sval, Vval) > 0)
				for (int k = 0; k < 3; k++)
					psrc[j*nChannels + k] = 0;
		}
	cvtColor(m_image, m_image, CV_HSV2BGR);
}

void HSVHist::showImage(string strpath)
{
	if (strpath == "")
		strpath = this->path;
	strpath += "hist";
	imshow(strpath, m_image);
	imwrite("houghtest.jpg", m_image);
}

void HSVHist::removeBack(HSVHist dst, HSVHist back)
{
	dst.Init();
	back.Init();
	dst.removeSeg(back);
}
