#ifndef _VIDEO_CAR_DETECT_H_
#define _VIDEO_CAR_DETECT_H_

#define VIDEO_WIDTH     1920
#define VIDEO_HEIGHT    1080

#if ALLTAB_DETECT_CAR_MODE
//汽车
#include "carSDK\\portab.h"
#include "carSDK\\lc_plate_analysis.h"

#else
//电动车
#include "elecarSDK\\portab.h"
#include "elecarSDK\\lc_plate_analysis.h"

#endif

//一张图里最多多少个汽车
#define CAR_STRUCT_MAX 8
//每个车牌最多多少个字符
#define CAR_STR_MAX 32
//车的省份
#define CAR_MASK_MAX 31
	////////////////////////////////////////////////////////

//车辆信息
typedef struct  
{
	//车牌字符串
	char *Str;
	//车牌字符数目
	uint8_t CharTotal; 
	//车牌颜色
	char* PlateColor; 
	//总体置信度
	uint8_t Reliability;
	//车辆运动方向 车辆运动方向(0:未知方向 1:车牌从上到下运动 2:表示车牌从下到上运动)
	uint8_t Direction;
	//车牌位置矩形框
	Rects CarRect;
	//车牌类型
	char *PlateType;
	//车身颜色
	char *CarColor;

}VideoCarInfoStruct;
	////////////////////////////////////////////////////////

//基本设置
struct VideoCarSetStruct
{
	//截图车牌带红框 0=不带 1=带
	int RedRect;
	//车牌倾斜校正使能标志 0:不校正，1:校正
	int Deskew;
	//JPG图片质量 75
	int JPGquality;
	//视频图像宽
	int Width;
	//视频图像高
	int Hight;
	//车牌最小宽度 60
	int MinWidth;
	//车牌最大宽度 180
	int MaxWidth;
	//检测范围 比例尺 
	Rects Range;
	//检测范围 比例尺 
	Rects RangeRate;
	//默认省份字符  
	//省份汉字：云 京 冀 吉 宁 川 新 晋 桂 沪 津 浙 渝 湘 琼 甘 皖 粤 苏 蒙 藏 豫 贵 赣 辽 鄂 闽 陕 青 鲁 黑
	char DefaultChar[CAR_STR_MAX];
	//31个省 标志位
	//云 京 冀 吉 宁 川 新 晋 桂 沪 津 浙 渝 湘 琼 甘 皖 粤 苏 蒙 藏 豫 贵 赣 辽 鄂 闽 陕 青 鲁 黑 (省份)
	//1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1
	uint8_t Mask[CAR_MASK_MAX];
	//汉字置信度 75
	int Reliability;
	//车身颜色识别标志，0:不识别，1:识别
	int CarColor;
};

	////////////////////////////////////////////////////////
class CVideoCarDetect
{
public:
	CVideoCarDetect();
	virtual ~CVideoCarDetect();

	void SetRange();
	void SetRedRect();
	void SetDeskew();
	void SetMinMaxWidth();
	void SetMask();
	void SetDefaultChar();
	void SetReliability();
	void SetJPGquality();
	void SetCarColor();
	void SetAll();


	void Stop();
	void Start(int format,unsigned char *image,int w,int h,int size);
	int WriteDBResult();
	int NoWriteDBResult();

	char* CarColor(uint8_t i);
	char* CarType(uint8_t i);
	char* CarDirection(uint8_t i);
	//bool CarCheckCharLite(int i,char *src);
	void CarCheckChar(int i);
	////////////////////////////////////////////////////////
	void LimitMessageBox(void);
	bool ErrorPlateName(char *name);

	bool FtpUploadElecar(char *TimeStr,
						   char *IpAddr,
						   char *Plate,
						   unsigned int Reliability,	
						   char *Direction,
						   unsigned char *FileBuf,
						   unsigned long int FileSize);

	bool FtpUploadCar(char *TimeStr,
							  char *IpAddr,
							  char *Plate,
							  unsigned int Reliability,	
							  char *Direction,
							  char *PlateType,
							  char *PlateColor,
							  char *CarColor,
							  unsigned char *FileBuf,
							  unsigned long int FileSize);
public:
	Plate_handle CarHandle;
	//句柄申请是否成功 1=成功。 0为失败 -1为未申请
	int State;

	//本帧车辆信息
	int CarTotal;
	VideoCarInfoStruct CarInfo[CAR_STRUCT_MAX];
	//车截图 PicFlag=1 准备截图 PicFlag=0不截图 
	bool PicFlag;
	unsigned char *Jpg;
	int JpgSize;
	//基本设置
	struct VideoCarSetStruct CarSet;
	////////////////////////////////////////////////////////
	//数据库
	char	cam_name[260];	//摄像头名称
	char	l_ipaddr[64];	//IP地址
	unsigned long int	camid;			//摄像头NID

	bool writeDBflag;//true =写入数据库 false =不写入数据库
	
	CTime starttime;
	long int totalsec;


};


#endif