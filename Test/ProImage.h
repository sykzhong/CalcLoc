#ifndef _PROIMAGE_H_
#define _PROIMAGE_H_
#include "HSVHist.h"
#include "ImageCon.h"
#include <set>
#include <fstream>
using namespace std;
using namespace cv;
class ProImage:public HSVHist
{
public:
	ProImage();
	~ProImage();
	void preproImage();										//对图像进行预处理

	void Init();											//轮廓相关参数初始化
	void setImageWin(const string &_winname);				//设定操作界面显示窗口
	void initWin();											//初始化窗口（初始显示图形界面）

	void getContour();										//提取图像轮廓并去除不合理轮廓
	
	void showImage();										//根据用户操作情况显示界面
	void coverImage(Mat &dst, Mat &img);					//对dst使用img进行覆盖
	void highlightImage(const int &_selectindex);					//对用户选择的轮廓进行高亮显示
	void lowlightImage(const int &_selectindex);					//对用户脱离选择的轮廓恢复低亮显示
	void fitContour();										//根据用户操作情况对轮廓进行重新拟合

	

	void getData(ProImage Temp);							//用于将模板数据传递至dst
	void printContour();

	void writeResult(string _imgname = "");					//显示用户操作的最终结果

	static void onMouseHandle(int event, int x, int y, int flags, void* param);
	static int compArea(vector<Point> first, vector<Point> second);

	void reset();
private:
	Mat conindex;						//记录轮廓索引值
	vector<Vec4i> hierachy;				//轮廓等级

	string winname;						//用户操作窗口名称
	Mat m_showimage;					//用户操作界面图像

	vector<ImageCon> vecImageCon;
	vector<Mat> r_orgcon;				//分块存储原轮廓
	vector<Mat> b_orgcon;				//分块存储原轮廓（高亮显示）

	int selectindex;					//记录被选择的单个轮廓索引
	int recoverflag;					//是否复原的标记（显示用）
	
protected:
	vector<vector<Point>> veccon;		//记录原始轮廓
	set<int> chosen;					//记录被选择的所有轮廓索引
	vector<vector<Point>> vecpoly;		//记录进行多边形拟合的轮廓
	vector<RotatedRect> vececllipse;	//记录进行椭圆拟合的轮廓

};
#endif