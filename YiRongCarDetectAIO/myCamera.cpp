
#include "stdafx.h"
#include "myCamera.h"

CMyCamera::CMyCamera()
{
	for(int i=0;i<MAX_DEVICE_NUM;i++)
	{
		m_LoginHandle[i] = 0;
		m_RealHandle[i] = -1;
	}
}

CMyCamera::~CMyCamera()
{
}

void CMyCamera::SDKInit()
{
}

bool CMyCamera::StartPlay(int venderID,int screenNo,char *name,char *sip,int nPort,int channel,
						  char *user,char *psw,HWND hWnd,int subtype,
						  char *Rtspurl,int RTP,int DecodeTag)
{
	bool ret = false;
	switch(venderID)
	{
#if OPEN_HAIKANG_SDK
		case VENDER_TYPE_HAIKANG:
			ret = m_haikang.StartPlay(screenNo,name, sip, nPort, channel, user, psw, hWnd, subtype);
			if(ret)
			{
				m_LoginHandle[screenNo] = m_haikang.m_LoginHandle[screenNo];
				m_RealHandle[screenNo] = m_haikang.m_RealHandle[screenNo];
			}
			break;
#endif
#if	OPEN_DAHUA_SDK
		case VENDER_TYPE_DAHUA:
			ret = m_dahua.StartPlay(screenNo,name, sip, nPort, channel, user, psw, hWnd, subtype);
			if(ret)
			{
				m_LoginHandle[screenNo] = m_dahua.m_LoginHandle[screenNo];
				m_RealHandle[screenNo] = m_dahua.m_RealHandle[screenNo];
			}
			break;
#endif
#if OPEN_YAAN_SDK
		case VENDER_TYPE_YAAN:
			ret = m_yaAn.StartPlay(screenNo,name, sip, channel, nPort, user, psw, hWnd, subtype);
			if(ret)
			{
				m_LoginHandle[screenNo] = m_yaAn.m_LoginHandle[screenNo];
				m_RealHandle[screenNo] = m_yaAn.m_RealHandle[screenNo];
			}
			break;
#endif
#if OPEN_YAAN_NEW_SDK
		case VENDER_TYPE_YAAN_NEW:
			ret = m_yaAnNew.StartPlay(screenNo,name, sip, nPort, user, psw, hWnd, subtype);
			if(ret)
			{
				m_LoginHandle[screenNo] = m_yaAnNew.m_LoginHandle[screenNo];
				m_RealHandle[screenNo] = m_yaAnNew.m_RealHandle[screenNo];
			}
			break;
#endif
#if OPEN_STREAM_CLIENT_SDK
		case VENDER_TYPE_STREAM:
			ret = m_StreamClient.StartPlay(screenNo,name, sip, user, psw, hWnd, subtype,Rtspurl, RTP,DecodeTag);
			if(ret)
			{
				m_LoginHandle[screenNo] =screenNo;
				m_RealHandle[screenNo] = m_StreamClient.m_RealHandle[screenNo];
			}
			break;
#endif
		default:break;
	}
	return ret;
}

void CMyCamera::StopPlay(int venderID,int screenNo)
{
	switch(venderID)
	{
#if OPEN_HAIKANG_SDK
		case VENDER_TYPE_HAIKANG:
			m_haikang.StopPlay(screenNo);
			break;
#endif
#if	OPEN_DAHUA_SDK
		case VENDER_TYPE_DAHUA:
			m_dahua.StopPlay(screenNo);
			break;
#endif
#if OPEN_YAAN_SDK
		case VENDER_TYPE_YAAN:
			m_yaAn.StopPlay(screenNo);
			break;
#endif
#if OPEN_YAAN_NEW_SDK
		case VENDER_TYPE_YAAN_NEW:
			m_yaAnNew.StopPlay(screenNo);
			break;
#endif
#if OPEN_STREAM_CLIENT_SDK
		case VENDER_TYPE_STREAM:
			m_StreamClient.StopPlay(screenNo);
			break;
#endif
		default:
			break;
	}
	m_LoginHandle[screenNo] = -1;
	m_RealHandle[screenNo] = -1;
}

void CMyCamera::Capture(int venderID,int screenNo,char *filename)
{
	switch(venderID)
	{
#if OPEN_HAIKANG_SDK
		case VENDER_TYPE_HAIKANG:
			m_haikang.Capture(screenNo,filename);
			break;
#endif
#if	OPEN_DAHUA_SDK
		case VENDER_TYPE_DAHUA:
			m_dahua.Capture(screenNo,filename);
			break;
#endif
#if OPEN_YAAN_SDK
		case VENDER_TYPE_YAAN:
			m_yaAn.Capture(screenNo,filename);
			break;
#endif
#if OPEN_YAAN_NEW_SDK
		case VENDER_TYPE_YAAN_NEW:
			m_yaAnNew.Capture(screenNo,filename);
			break;
#endif
#if OPEN_STREAM_CLIENT_SDK
		case VENDER_TYPE_STREAM:
			m_StreamClient.Capture(screenNo,filename);
			break;
#endif
		default:
			break;
	}
}


void CMyCamera::PtzControl(int venderID, int screenNo, int type, BOOL dwStop, int param)
{
	switch(venderID)
	{
#if OPEN_HAIKANG_SDK
		case VENDER_TYPE_HAIKANG:
			m_haikang.PtzControl(m_RealHandle[screenNo],type,dwStop,param);
			break;
#endif
#if	OPEN_DAHUA_SDK
		case VENDER_TYPE_DAHUA:
			m_dahua.PtzControl(m_LoginHandle[screenNo],type,dwStop,param);
			break;
#endif
#if OPEN_YAAN_SDK
		case VENDER_TYPE_YAAN:
			m_yaAn.PtzControl(m_LoginHandle[screenNo],type,dwStop,param);
			break;
#endif
#if OPEN_YAAN_NEW_SDK
		case VENDER_TYPE_YAAN_NEW:
			m_yaAnNew.PtzControl(m_LoginHandle[screenNo],type,dwStop,param);
			break;
#endif
#if OPEN_STREAM_CLIENT_SDK
		case VENDER_TYPE_STREAM:
			m_StreamClient.PtzControl(m_RealHandle[screenNo],type,dwStop,param);
			break;
#endif
		default:
			break;
	}
}


int CMyCamera::StartRecord(int venderID,int screenNo,char *filename)
{
	int iRet = 0;
	switch(venderID)
	{
#if OPEN_HAIKANG_SDK
		case VENDER_TYPE_HAIKANG:
			iRet = m_haikang.StartRecord(screenNo,filename);
			break;
#endif
#if	OPEN_DAHUA_SDK
		case VENDER_TYPE_DAHUA:
			iRet = m_dahua.StartRecord(screenNo,filename);
			break;
#endif
#if OPEN_YAAN_SDK
		case VENDER_TYPE_YAAN:
			iRet = m_yaAn.StartRecord(screenNo,filename);
			break;
#endif
#if OPEN_YAAN_NEW_SDK
		case VENDER_TYPE_YAAN_NEW:
			iRet = m_yaAnNew.StartRecord(screenNo,filename);
			break;
#endif
#if OPEN_STREAM_CLIENT_SDK
		case VENDER_TYPE_STREAM:
			iRet = m_StreamClient.StartRecord(screenNo,filename);
			break;
#endif
		default:
			break;
	}
	return iRet;
}

void CMyCamera::StopRecord(int venderID,int screenNo)
{
	switch(venderID)
	{
#if OPEN_HAIKANG_SDK
		case VENDER_TYPE_HAIKANG:
			m_haikang.StopRecord(screenNo);
			break;
#endif
#if	OPEN_DAHUA_SDK
		case VENDER_TYPE_DAHUA:
			m_dahua.StopRecord(screenNo);
			break;
#endif
#if OPEN_YAAN_SDK
		case VENDER_TYPE_YAAN:
			m_yaAn.StopRecord(screenNo);
			break;
#endif
#if OPEN_YAAN_NEW_SDK
		case VENDER_TYPE_YAAN_NEW:
			m_yaAnNew.StopRecord(screenNo);
			break;
#endif
#if OPEN_STREAM_CLIENT_SDK
		case VENDER_TYPE_STREAM:
			m_StreamClient.StopRecord(screenNo);
			break;
#endif
		default:
			break;
	}
}