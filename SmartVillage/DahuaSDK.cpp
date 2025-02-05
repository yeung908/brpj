
#include "stdafx.h"

#if	OPEN_DAHUA_SDK

#include "DahuaSDK.h"
//////////////////////////////////
#include "CarDetect.h"
#include "FaceDetect.h"
#include "HYZJCarDetect.h"
//////////////////////////////////
#include "SmartVillage.h"
#include "SmartVillageDlg.h"
extern CSmartVillageDlg *DlgMain;

#include "DLGSetSystem.h"
extern CDLGSetSystem DlgSetSystem;






//分析成功=1 分析失败=0
int zogSize2WH(long int size,int *w,int *h)
{
	switch(size*2/3)
	{
	case 307200:
		(*w)=640;(*h)=480;return 1;
	case 405504:
		(*w)=704;(*h)=576;return 1;
	case 337920:
		(*w)=704;(*h)=480;return 1;
	case 230400:
		(*w)=480;(*h)=480;return 1;
	case 473600:
		(*w)=800;(*h)=592;return 1;
	case 479232:
		(*w)=1024;(*h)=468;return 1;
	case 1024000:
		(*w)=1280;(*h)=800;return 1;
	case 1638400:
		(*w)=1600;(*h)=1024;return 1;
	case 1920000:
		(*w)=1600;(*h)=1200;return 1;
	case 2304000:
		(*w)=1920;(*h)=1200;return 1;
	case 921600:
		(*w)=1280;(*h)=720;return 1;
	case 2073600:
		(*w)=1920;(*h)=1080;return 1;
	case 1228800:
		(*w)=1280;(*h)=960;return 1;
	case 2635776:
		(*w)=1872;(*h)=1408;return 1;
	case 5271552:
		(*w)=3744;(*h)=1408;return 1;
	case 3145728:
		(*w)=2048;(*h)=1536;return 1;
	case 4985600:
		(*w)=2432;(*h)=2050;return 1;
	case 1245184:
		(*w)=1216;(*h)=1024;return 1;
	case 1441792:
		(*w)=1408;(*h)=1024;return 1;
	case 8147712:
		(*w)=3296;(*h)=2472;return 1;
	case 4915200:
		(*w)=2560;(*h)=1920;return 1;
	case 552960:
		(*w)=960;(*h)=576;return 1;
	case 460800:
		(*w)=960;(*h)=480;return 1;
	case 691200:
		(*w)=960;(*h)=720;return 1;

	default:return 0;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////
void __stdcall DisConnectFunc(LONG lLoginID, char *pchDVRIP, LONG nDVRPort, DWORD dwUser)
{
	DlgMain->ShowCameraMessage(pchDVRIP,"掉线",0);
}
void __stdcall AutoConnectFunc(LONG lLoginID,char *pchDVRIP,LONG nDVRPort,DWORD dwUser)
{
	DlgMain->ShowCameraMessage(pchDVRIP,"重连成功",0);
}


///////////////////////////////////////////////////////////////////////////////////////////
void __stdcall SubDisConnectFunc(EM_INTERFACE_TYPE emInterfaceType, BOOL bOnline, LONG lOperateHandle, LONG lLoginID, DWORD dwUser)
{
	switch(emInterfaceType)
	{
	case DH_INTERFACE_REALPLAY:
		break;
	case DH_INTERFACE_PREVIEW:
		break;
	case DH_INTERFACE_PLAYBACK:
		break;
	case DH_INTERFACE_DOWNLOAD:
		break;
	default:
		break;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////
void __stdcall RealDataCallBackEx(LONG lRealHandle, DWORD dwDataType, BYTE *pBuffer,DWORD dwBufSize, LONG lParam, DWORD dwUser)
{
	BOOL bInput=FALSE;
	if(0 != lRealHandle)
	{
		/*
		FILE *fp=fopen("dahua.txt","a+");
		if(fp)
		{
			fprintf(fp,"input data type: %d\n",dwDataType);
			fclose(fp);
		}
		*/
		//printf("input data type: %d\n", dwDataType);
		switch(dwDataType) {
		case 0:
			//原始音视频混合数据
			break;
		case 1:
			//标准视频数据
			
			break;
		case 2:
			//yuv 数据
			int screenNo;
			screenNo = DlgMain->DlgTabVideo.DlgScreen.m_video.m_dahua.GetHandleWindID(lRealHandle);
			if((screenNo == -1)||(screenNo > 15))
			{
				return;
			}		
			//未启用识别
			if(! DlgMain->DlgTabVideo.DlgScreen.m_videoInfo[screenNo].enableCarDetect)
				return ;

			int w,h;
			//车牌识别
#if OPEN_LC_CARDETECT_CODE 	
	
			if(zogSize2WH(dwBufSize,&w,&h))
			{

				//拷贝数值
				DlgMain->DlgTabVideo.DlgScreen.CarDetect[screenNo].m_playhandle=screenNo;
				
				DlgMain->DlgTabVideo.DlgScreen.CarDetect[screenNo].camid=
					DlgMain->DlgTabVideo.DlgScreen.m_videoInfo[screenNo].camID;
				
				strcpy(DlgMain->DlgTabVideo.DlgScreen.CarDetect[screenNo].cam_name,
					DlgMain->DlgTabVideo.DlgScreen.m_videoInfo[screenNo].name.GetBuffer(0));
				
				strcpy(DlgMain->DlgTabVideo.DlgScreen.CarDetect[screenNo].l_ipaddr,
					DlgMain->DlgTabVideo.DlgScreen.m_videoInfo[screenNo].ip.GetBuffer(0));

				DlgMain->DlgTabVideo.DlgScreen.CarDetect[screenNo].cam_Direction=
					DlgMain->DlgTabVideo.DlgScreen.m_videoInfo[screenNo].Direction;

				//颜色LC_VIDEO_FORMAT_I420
				DlgMain->DlgTabVideo.DlgScreen.CarDetect[screenNo].Start(LC_VIDEO_FORMAT_I420,\
					pBuffer,w,h,dwBufSize);
				
				DlgMain->DlgTabVideo.DlgScreen.CarDetect[screenNo].Result();
			}

#endif

#if OPEN_HYZJ_CARDETECT_CODE 	

			if(zogSize2WH(dwBufSize,&w,&h))
			{
				//拷贝数值
				DlgMain->DlgTabVideo.DlgScreen.HYZJCarDetect[screenNo].m_playhandle=screenNo;

				DlgMain->DlgTabVideo.DlgScreen.HYZJCarDetect[screenNo].camid=
					DlgMain->DlgTabVideo.DlgScreen.m_videoInfo[screenNo].camID;

				strcpy(DlgMain->DlgTabVideo.DlgScreen.HYZJCarDetect[screenNo].cam_name,
					DlgMain->DlgTabVideo.DlgScreen.m_videoInfo[screenNo].name.GetBuffer(0));

				strcpy(DlgMain->DlgTabVideo.DlgScreen.HYZJCarDetect[screenNo].l_ipaddr,
					DlgMain->DlgTabVideo.DlgScreen.m_videoInfo[screenNo].ip.GetBuffer(0));

				DlgMain->DlgTabVideo.DlgScreen.HYZJCarDetect[screenNo].cam_Direction=
					DlgMain->DlgTabVideo.DlgScreen.m_videoInfo[screenNo].Direction;

				//颜色LC_VIDEO_FORMAT_I420
	
				DlgMain->DlgTabVideo.DlgScreen.HYZJCarDetect[screenNo].Start(ImageFormatYUV420,\
					pBuffer,w,h,dwBufSize);

				DlgMain->DlgTabVideo.DlgScreen.HYZJCarDetect[screenNo].Result();
			}

#endif
			

#if OPEN_FACEDETECT_CODE
			//启用识别
			if(DlgMain->DlgTabVideo.DlgScreen.m_videoInfo[screenNo].enableFaceDetect)
			{
				DlgMain->DlgTabVideo.DlgScreen.FaceAdd[screenNo]++;
				if(0==DlgMain->DlgTabVideo.DlgScreen.FaceAdd[screenNo]%FACE_JUMP_NUM)
					DlgMain->DlgTabVideo.DlgScreen.FaceAdd[screenNo]=0;
				else
					break ;

				long long ftime;
				CTime nowtime=CTime::GetTickCount();
				ftime=nowtime.GetTime();

				//	YUV2RGB((unsigned char *)pBuf, 	DlgMain->DlgTabVideo.DlgScreen.RGBdata[screenNo], 	nWidth,nHeight);
				if(zogSize2WH(dwBufSize,&w,&h))
				{
					//拷贝数值
					DlgMain->DlgTabVideo.DlgScreen.FaceDetect[screenNo].m_playhandle=screenNo;

					DlgMain->DlgTabVideo.DlgScreen.FaceDetect[screenNo].camid=
						DlgMain->DlgTabVideo.DlgScreen.m_videoInfo[screenNo].camID;

					strcpy(DlgMain->DlgTabVideo.DlgScreen.FaceDetect[screenNo].cam_name,
						DlgMain->DlgTabVideo.DlgScreen.m_videoInfo[screenNo].name.GetBuffer(0));

					strcpy(DlgMain->DlgTabVideo.DlgScreen.FaceDetect[screenNo].l_ipaddr,
						DlgMain->DlgTabVideo.DlgScreen.m_videoInfo[screenNo].ip.GetBuffer(0));

					DlgMain->DlgTabVideo.DlgScreen.FaceDetect[screenNo].cam_Direction=
						DlgMain->DlgTabVideo.DlgScreen.m_videoInfo[screenNo].Direction;

					DlgMain->DlgTabVideo.DlgScreen.FaceDetect[screenNo].Start(VIDEO_FORMAT_I420,
						pBuffer,w,h,w*3,dwBufSize);

					//ZOGDramBMP("d:\\112212121.bmp",DlgMain->DlgTabVideo.DlgScreen.RGBdata[screenNo], nWidth,nHeight);
				}
			}

#endif
			break;
		case 3:
			//pcm 音频数据
			
			break;
		case 4:
			//原始音频数据
			
			break;
		default:
			break;
		}	
	}
}


///////////////////////////////////////////////////////////////////////////////////////////
CDahuaSDK::CDahuaSDK()
{
	m_bPTZCtrl = FALSE;
	for(int i=0;i<MAX_DEVICE_NUM;i++)
	{
		m_LoginHandle[i] = 0;
		m_RealHandle[i] = -1;
	}
	SDKInit();
}

CDahuaSDK::~CDahuaSDK()
{
	for(int i=0;i<MAX_DEVICE_NUM;i++)
	{
		StopPlay(i);
	}

	PtzStopPlay();
	//释放SDK资源
	CLIENT_Cleanup();
}

void CDahuaSDK::SDKInit()
{
	CLIENT_Init(DisConnectFunc, 0);
	CLIENT_SetSubconnCallBack(SubDisConnectFunc, 0);
	CLIENT_SetAutoReconnect(AutoConnectFunc,0);
	CLIENT_SetConnectTime(10000, 10);
}

bool CDahuaSDK::StartPlay(int screenNo,char *name,char *sip,int nPort,int channel,char *user,char *psw,
						  HWND hWnd,int subtype,int Direction)
{
	StopPlay(screenNo);

	NET_DEVICEINFO stDevInfo = {0};
	int nError = 0;
	int nChannelID = channel; // 预览通道号

	m_direction[screenNo]=Direction;

	m_LoginHandle[screenNo] = CLIENT_Login(sip, nPort, user, psw, &stDevInfo, &nError);

	if (m_LoginHandle[screenNo] != 0)
	{
		BOOL ret = CLIENT_StartListenEx(m_LoginHandle[screenNo]);
		if(!ret)
		{
			char *msg = RuntimeMessage();
			DlgMain->ShowCameraMessage(name,msg,0);
		}

		if(DlgSetSystem.m_display_preview)
		{
			//开启预览
			m_RealHandle[screenNo] = CLIENT_RealPlayEx(m_LoginHandle[screenNo], nChannelID, hWnd, (DH_RealPlayType)(DH_RType_Realplay_0 + subtype));
		}
		else
		{	
			m_RealHandle[screenNo] = CLIENT_RealPlayEx(m_LoginHandle[screenNo], nChannelID,(HWND)-1, (DH_RealPlayType)(DH_RType_Realplay_0 + subtype));
		}

		if (m_RealHandle[screenNo] == -1)
		{
			return false;
		}

		// 设置回调函数处理数据
		CLIENT_SetRealDataCallBackEx(m_RealHandle[screenNo], RealDataCallBackEx, 0, 0x00000006);
		
		DlgMain->ShowCameraMessage(name,"连接成功",FALSE);
		return true;
	}
	else
	{
		//char *msg = RuntimeMessage();
		GetConnectError(name,nError,0);
	}
	return false;
}

void CDahuaSDK::StopPlay(int screenNo)
{
	if(m_RealHandle[screenNo] != -1)
	{
		//关闭预览
		CLIENT_StopRealPlayEx(m_RealHandle[screenNo]);
		m_RealHandle[screenNo] = -1;

#if OPEN_LC_CARDETECT_CODE 	
	//停止识别
	if(false == DlgMain->DlgTabVideo.DlgScreen.m_videoInfo[screenNo].enableCarDetect)
		DlgMain->DlgTabVideo.DlgScreen.CarDetect[screenNo].Stop();

		DlgMain->DlgTabVideo.DlgScreen.CarAdd[screenNo]=0;
#endif

#if OPEN_HYZJ_CARDETECT_CODE 	
		//停止识别
		if(false == DlgMain->DlgTabVideo.DlgScreen.m_videoInfo[screenNo].enableCarDetect)
			DlgMain->DlgTabVideo.DlgScreen.HYZJCarDetect[screenNo].Stop();

		DlgMain->DlgTabVideo.DlgScreen.CarAdd[screenNo]=0;
#endif

#if OPEN_FACEDETECT_CODE 	
	//停止识别
	if(false == DlgMain->DlgTabVideo.DlgScreen.m_videoInfo[screenNo].enableFaceDetect)
		DlgMain->DlgTabVideo.DlgScreen.FaceDetect[screenNo].Stop();

	DlgMain->DlgTabVideo.DlgScreen.FaceAdd[screenNo]=0;
#endif

	}
	if(m_LoginHandle[screenNo] != 0)
	{
		//注销用户
		CLIENT_Logout(m_LoginHandle[screenNo]);
		m_LoginHandle[screenNo] = 0;
	}
}

bool CDahuaSDK::PtzStartPlay(char *sip,int nPort,int channel,char *user,char *psw,HWND hWnd)
{
	NET_DEVICEINFO stDevInfo = {0};
	int nError = 0;
	int nChannelID = channel; // 预览通道号

	m_ptzLoginHandle = CLIENT_Login(sip, nPort, user, psw, &stDevInfo, &nError);

	if (m_ptzLoginHandle != 0)
	{
		//开启预览
		m_ptzRealHandle = CLIENT_RealPlayEx(m_ptzLoginHandle, nChannelID, hWnd, DH_RType_Realplay_0);
		if (m_ptzRealHandle == 0)
		{
			return false;
		}

		// 设置回调函数处理数据
		CLIENT_SetRealDataCallBackEx(m_ptzRealHandle, NULL, 0, 0x00000006);
		return true;
	}
	return false;
}

void CDahuaSDK::PtzStopPlay()
{
	if(m_ptzRealHandle != 0)
	{
		//关闭预览
		CLIENT_StopRealPlayEx(m_ptzRealHandle);
		m_ptzRealHandle = 0;
	}
	if(m_ptzLoginHandle != 0)
	{
		//注销用户
		CLIENT_Logout(m_ptzLoginHandle);
		m_ptzLoginHandle = 0;
	}
}

void CDahuaSDK::PtzControl(long lLoginID, int type, BOOL dwStop, int param)
{
	BOOL ret;
	if (dwStop)
	{
		if(!m_bPTZCtrl)
		{
			goto exitPTZCtrl;
		}
	}
	if(lLoginID == 0)
	{
		goto exitPTZCtrl;
	}

	switch(type)
	{
		case PTZ_CONTROL_UP:
		case PTZ_CONTROL_DOWN:
		case PTZ_CONTROL_LEFT:
		case PTZ_CONTROL_RIGHT:
		case PTZ_CONTROL_ZOOM_ADD:
		case PTZ_CONTROL_ZOOM_SUB:
		case PTZ_CONTROL_FOCUS_ADD:
		case PTZ_CONTROL_FOCUS_SUB:
		case PTZ_CONTROL_IRIS_ADD:
		case PTZ_CONTROL_IRIS_SUB:
		case PTZ_CONTROL_POINT_MOVE:
		case PTZ_CONTROL_POINT_SET:
			ret = CLIENT_DHPTZControl(lLoginID, 0, type, 0, (BYTE)param, 0, dwStop);
			m_bPTZCtrl = !dwStop;
			break;
		case PTZ_CONTROL_UPLEFT:
		case PTZ_CONTROL_UPRIGHT:
		case PTZ_CONTROL_DOWNLEFT:
		case PTZ_CONTROL_DOWNRIGHT:
			ret = CLIENT_DHPTZControl(lLoginID, 0, type+21, (BYTE)param, (BYTE)param, 0, dwStop);
			m_bPTZCtrl = !dwStop;
			break;
		case PTZ_CONTROL_SPEED_ADD:
			break;
		case PTZ_CONTROL_SPEED_SUB:
			break;
		case PTZ_CONTROL_AUTO:
			break;
	}
	if(!ret)
	{
		//MessageBox(ConvertString(MSG_DEMODLG_PTZCTRLFAILED));
		goto exitPTZCtrl;
	}
	return;

exitPTZCtrl:
	m_bPTZCtrl = FALSE;
	return;
}

void CDahuaSDK::Capture(long screenNo,char *filename)
{
	BOOL ret = CLIENT_CapturePicture(m_RealHandle[screenNo], filename);
	if(!ret)
	{
		//LastError();
		//MessageBox(ConvertString(MSG_DEMODLG_CAPTUREFAILED));
	}
}

int CDahuaSDK::StartRecord(int screenNo,char *filename)
{
	int iRet;
	iRet = CLIENT_SaveRealData(m_RealHandle[screenNo],filename);
	if(iRet == 0)
	{
		TRACE("StartRecord Error:%d\n",iRet);
		return 1;
	}
	else
	{
		return 0;
	}
}

int CDahuaSDK::StopRecord(int screenNo)
{
	int iRet;
	iRet = CLIENT_StopSaveRealData(m_RealHandle[screenNo]);
	if(iRet == 0)
	{
		TRACE("StopRecord Error:%d\n",iRet);
		return 1;
	}
	else
	{
		return 0;
	}
}

//根据播放句柄获取窗口ID
int CDahuaSDK::GetHandleWindID(int RealHandle)
{
	for(int i=0;i<MAX_DEVICE_NUM;i++)
	{
		if(m_RealHandle[i] == RealHandle)
		{
			return i;
		}
	}
	return -1;
}

char *CDahuaSDK::RuntimeMessage(void)
{
	DWORD dwError = CLIENT_GetLastError();
	switch(dwError)
	{
	case NET_NOERROR:
		return ERROR_NET_NOERROR;
		break;
	case NET_ERROR:
		return ERROR_NET_ERROR;
		break;
	case NET_SYSTEM_ERROR:
		return ERROR_NET_SYSTEM_ERROR;
		break;
	case NET_NETWORK_ERROR:
		return ERROR_NET_NETWORK_ERROR;
		break;
	case NET_DEV_VER_NOMATCH:
		return ERROR_NET_DEV_VER_NOMATCH;
		break;
	case NET_INVALID_HANDLE:
		return ERROR_NET_INVALID_HANDLE;
		break;
	case NET_OPEN_CHANNEL_ERROR:
		return ERROR_NET_OPEN_CHANNEL_ERROR;
		break;
	case NET_CLOSE_CHANNEL_ERROR:
		return ERROR_NET_CLOSE_CHANNEL_ERROR;
		break;
	case NET_ILLEGAL_PARAM:
		return ERROR_NET_ILLEGAL_PARAM;
		break;
	case NET_SDK_INIT_ERROR:
		return ERROR_NET_SDK_INIT_ERROR;
		break;
	case NET_SDK_UNINIT_ERROR:
		return ERROR_NET_SDK_UNINIT_ERROR;
		break;
	case NET_RENDER_OPEN_ERROR:
		return ERROR_NET_RENDER_OPEN_ERROR;
		break;
	case NET_DEC_OPEN_ERROR:
		return ERROR_NET_DEC_OPEN_ERROR;
		break;
	case NET_DEC_CLOSE_ERROR:
		return ERROR_NET_DEC_CLOSE_ERROR;
		break;
	case NET_MULTIPLAY_NOCHANNEL:
		return ERROR_NET_MULTIPLAY_NOCHANNEL;
		break;
	case NET_TALK_INIT_ERROR:
		return ERROR_NET_TALK_INIT_ERROR;
		break;
	case NET_TALK_NOT_INIT:
		return ERROR_NET_TALK_NOT_INIT;
		break;	
	case NET_TALK_SENDDATA_ERROR:
		return ERROR_NET_TALK_SENDDATA_ERROR;
		break;
	case NET_NO_TALK_CHANNEL:
		return ERROR_NET_NO_TALK_CHANNEL;
		break;
	case NET_NO_AUDIO:
		return ERROR_NET_NO_AUDIO;
		break;								
	case NET_REAL_ALREADY_SAVING:
		return ERROR_NET_REAL_ALREADY_SAVING;
		break;
	case NET_NOT_SAVING:
		return ERROR_NET_NOT_SAVING;
		break;
	case NET_OPEN_FILE_ERROR:
		return ERROR_NET_OPEN_FILE_ERROR;
		break;
	case NET_PTZ_SET_TIMER_ERROR:
		return ERROR_NET_PTZ_SET_TIMER_ERROR;
		break;
	case NET_RETURN_DATA_ERROR:
		return ERROR_NET_RETURN_DATA_ERROR;
		break;
	case NET_INSUFFICIENT_BUFFER:
		return ERROR_NET_INSUFFICIENT_BUFFER;
		break;
	case NET_NOT_SUPPORTED:
		return ERROR_NET_NOT_SUPPORTED;
		break;
	case NET_NO_RECORD_FOUND:
		return ERROR_NET_NO_RECORD_FOUND;
		break;	
	case NET_LOGIN_ERROR_PASSWORD:
		return ERROR_NET_LOGIN_ERROR_PASSWORD;
		break;
	case NET_LOGIN_ERROR_USER:
		return ERROR_NET_LOGIN_ERROR_USER;
		break;
	case NET_LOGIN_ERROR_TIMEOUT:
		return ERROR_NET_LOGIN_ERROR_TIMEOUT;
		break;
	case NET_LOGIN_ERROR_RELOGGIN:
		return ERROR_NET_LOGIN_ERROR_RELOGGIN;
		break;
	case NET_LOGIN_ERROR_LOCKED:
		return ERROR_NET_LOGIN_ERROR_LOCKED;
		break;
	case NET_LOGIN_ERROR_BLACKLIST:
		return ERROR_NET_LOGIN_ERROR_BLACKLIST;
		break;
	case NET_LOGIN_ERROR_BUSY:
		return ERROR_NET_LOGIN_ERROR_BUSY;
		break;
	case NET_LOGIN_ERROR_CONNECT:
		return ERROR_NET_LOGIN_ERROR_CONNECT;
		break;
	case NET_LOGIN_ERROR_NETWORK:
		return ERROR_NET_LOGIN_ERROR_NETWORK;
		break;								
	case NET_RENDER_SOUND_ON_ERROR:
		return ERROR_NET_RENDER_SOUND_ON_ERROR;
		break;
	case NET_RENDER_SOUND_OFF_ERROR:
		return ERROR_NET_RENDER_SOUND_OFF_ERROR;
		break;
	case NET_RENDER_SET_VOLUME_ERROR:
		return ERROR_NET_RENDER_SET_VOLUME_ERROR;
		break;
	case NET_RENDER_ADJUST_ERROR:
		return ERROR_NET_RENDER_ADJUST_ERROR;
		break;
	case NET_RENDER_PAUSE_ERROR:
		return ERROR_NET_RENDER_PAUSE_ERROR;
		break;
	case NET_RENDER_SNAP_ERROR:
		return ERROR_NET_RENDER_SNAP_ERROR;
		break;
	case NET_RENDER_STEP_ERROR:
		return ERROR_NET_RENDER_STEP_ERROR;
		break;
	case NET_RENDER_FRAMERATE_ERROR:
		return ERROR_NET_RENDER_FRAMERATE_ERROR;
		break;
	case NET_CONFIG_DEVBUSY:
		return ERROR_NET_CONFIG_DEVBUSY;
		break;
	case NET_CONFIG_DATAILLEGAL:
		return ERROR_NET_CONFIG_DATAILLEGAL;
		break;							
	case NET_NO_INIT:
		return ERROR_NET_NO_INIT;
		break;
	case NET_DOWNLOAD_END:
		return ERROR_NET_DOWNLOAD_END;
		break;
	default:
		return ERROR_NET_ERROR;
		break;				
	}
}

//连接失败的类型
//flag ---- 1:弹窗
//			0:打印消息
void CDahuaSDK::GetConnectError(char *name,int error,int flag)
{
	//根据返回值不同判断错误类型
	switch(error) {
	case 1://密码不正确
		DlgMain->ShowCameraMessage(name,MSG_ERROR_PASSWORD,flag);
		break;
	case 2://用户名不存在
		DlgMain->ShowCameraMessage(name,MSG_ERROR_USER,flag);
		break;
	case 3://登录超时
		DlgMain->ShowCameraMessage(name,MSG_ERROR_TIMEOUT,flag);
		break;
	case 4://重复登录
		DlgMain->ShowCameraMessage(name,MSG_ERROR_RELOGGIN,flag);
		break;
	case 5://帐号被锁定
		DlgMain->ShowCameraMessage(name,MSG_ERROR_LOCKED,flag);
		break;
	case 6://帐号被列入黑名单
		DlgMain->ShowCameraMessage(name,MSG_ERROR_BLACKLIST,flag);
		break;
	case 7://系统忙，资源不足
		DlgMain->ShowCameraMessage(name,MSG_ERROR_BUSY,flag);
		break;
	case 8://子连接失败
		DlgMain->ShowCameraMessage(name,MSG_ERROR_CONNECT,flag);
		break;
	case 9://找不到主机
		DlgMain->ShowCameraMessage(name,MSG_ERROR_CONNECT,flag);
		break;
	case 0:
	default:
		DlgMain->ShowCameraMessage(name,MSG_ERROR_NETWORK,flag);
		break;
	}
}

//#endif


#endif
