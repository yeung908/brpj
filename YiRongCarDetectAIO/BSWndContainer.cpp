// BSWndContainer.cpp : implementation file
//

#include "stdafx.h"
#include "yirongcardetectaio.h"
#include "BSWndContainer.h"
/////////////////////////////////
#include "YiRongCarDetectAIODlg.h"
extern CYiRongCarDetectAIODlg *DlgMain;

/*
#pragma   warning(disable:4706)   
#define   COMPILE_MULTIMON_STUBS   
#include   <multimon.h>   
#pragma   warning(default:4706)   
*/

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBSWndContainer

CBSWndContainer::CBSWndContainer()
{
	// init active page pointer
	m_pActivePage	= NULL;

	// init window state
	m_bFullScreen	= FALSE;	// 全屏标记
	m_bMultiScreen	= TRUE;		// 多屏标记
	m_bAutoAdjustPos= FALSE;	// 自动调节标记

	for(int i=0;i<MAX_DEVICE_NUM;i++)
	{
		m_nStartPlay[i] = FALSE;
		m_nEnableDetect[i] = FALSE;
		m_nStartRecord[i] = FALSE;
		m_nEnableAlarm[i] = FALSE;
	}

	SetDrawActivePage(TRUE);	//	启动边框

	m_nShowPortion=100;			//	显示比例
}

CBSWndContainer::~CBSWndContainer()
{
	// remove all pages
	while(!m_PageList.IsEmpty())
		m_PageList.RemoveHead();
}


BEGIN_MESSAGE_MAP(CBSWndContainer, CWnd)
	//{{AFX_MSG_MAP(CBSWndContainer)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CBSWndContainer member functions

///////////////////////////////////////////////////
// call this function to create container object.
// it is override from cwnd class
BOOL CBSWndContainer::Create( LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext )
{
	dwStyle|=WS_EX_TOOLWINDOW;
	return CWnd::Create(lpszClassName,lpszWindowName,dwStyle,rect,pParentWnd,nID,pContext );
}


///////////////////////////////////////////////////
// call this function to add a page wnd to 
// container. if success retrun TRUE,else return 
// FALSE.
BOOL CBSWndContainer::AddPage(CWnd *pWnd, BOOL bRepaint)
{
	// check parameter
	if(	!pWnd || !IsWindow(pWnd->m_hWnd) )	return FALSE;

	// check list 
	POSITION pos=m_PageList.Find(pWnd);
	if(pos!=NULL) 
	{
		TRACE("This Window has been added to container, the operation will terminate.\n");
		return TRUE;
	}

	// added page
	m_PageList.AddTail(pWnd);

	if( m_bDrawActive ) DrawActivePage(FALSE);

	// reset active page
	SetActivePage(pWnd, bRepaint);

	return TRUE;
}

///////////////////////////////////////////////////
// call this function to remove a page wnd from
// container. 
CWnd *CBSWndContainer::DelPage(CWnd *pWnd)
{
	// check list
	POSITION pos=m_PageList.Find(pWnd);
	if(pos==NULL)
	{
		TRACE("This Window is not a member of container, the operation will terminate.\n");
		return NULL;
	}
	if(pWnd==m_pActivePage)
		if(m_pActivePage==GetPrevPage(pWnd))//m_PageList.IsEmpty()?NULL:m_PageList.GetHead();
			m_pActivePage=NULL;
		else m_pActivePage=GetPrevPage(pWnd);
	
	m_PageList.RemoveAt(pos);

	if (pWnd)
	{
		pWnd->ShowWindow(SW_HIDE);
	}

//	Invalidate();

	return pWnd;
}

///////////////////////////////////////////////////
// call this function to remove active page from
// container.
CWnd *CBSWndContainer::DelPage()
{
	return DelPage(m_pActivePage);
}

///////////////////////////////////////////////////
// call this function to set a page to be active
// page.
void CBSWndContainer::SetActivePage(CWnd *pWnd, BOOL bRepaint)
{
	// check parameter
	if(	!pWnd || !IsWindow(pWnd->m_hWnd) )	return;

	// if pWnd is the Active Page, return 
	if( m_pActivePage==pWnd ) return;

	// check list
	POSITION pos=m_PageList.Find(pWnd);
	if(pos==NULL)
	{
		TRACE("__This Window is not a member of container, the operation will terminate.\n");
		return;
	}

	if(bRepaint) UpdateWnd();

	if( m_bDrawActive ) DrawActivePage(FALSE);

	m_pActivePage=pWnd;

	if( m_bDrawActive ) DrawActivePage(TRUE);
}

///////////////////////////////////////////////////
// call this function to get the active page's
// pointer. if no active page,return NULL;
CWnd *CBSWndContainer::GetActivePage()
{
	return m_pActivePage;
}

CWnd *CBSWndContainer::GetTailPage()
{
	return m_PageList.GetTail();
}

///////////////////////////////////////////////////
// call this function to get the next page by
// the page that user defined. if the defined 
// page is not find in container, return NULL.
CWnd *CBSWndContainer::GetNextPage(CWnd *pWnd)
{
	// check parameter
	if(	!pWnd || !IsWindow(pWnd->m_hWnd) ) return NULL;

	// check list
	POSITION pos=m_PageList.Find(pWnd);
	if(pos==NULL)
	{
		TRACE("This Window is not a member of container, the operation will terminate.\n");
		return NULL;
	}
	
	//
	m_PageList.GetNext(pos);
	if(pos==NULL)
		return m_PageList.GetHead();
	else 
		return m_PageList.GetNext(pos);
}

///////////////////////////////////////////////////
// call this function to get the prev page by
// the page that user defined. if the defined
// page is not find in container,return NULL.
CWnd *CBSWndContainer::GetPrevPage(CWnd *pWnd)
{
	// check parameter
	if(	!pWnd || !IsWindow(pWnd->m_hWnd) ) return NULL;

	// check list
	POSITION pos=m_PageList.Find(pWnd);
	if(pos==NULL)
	{
		TRACE("This Window is not a member of container, the operation will terminate.\n");
		return NULL;
	}
	
	//
	m_PageList.GetPrev(pos);
	if(pos==NULL)
		return m_PageList.GetTail();
	else 
		return m_PageList.GetPrev(pos);
}

CWnd *CBSWndContainer::GetPage(int nIndex)
{
	CWnd *pRet = NULL;
	POSITION pos = m_PageList.FindIndex(nIndex);
	if(pos == NULL) return pRet;

	return m_PageList.GetAt(pos);
}

int CBSWndContainer::GetCount() const
{
	return m_PageList.GetCount();
}

///////////////////////////////////////////////////
// call this function to  page wnds,when
// the window is resized.
void CBSWndContainer::UpdateWnd()
{
	if(!IsWindowVisible()||IsIconic()) return;
/////////////////////
//计算显示总区域

	//得到窗口的设备坐标
	CRect rtContainer;
	GetClientRect(&rtContainer);
	GetShowRect(&rtContainer);
	rtContainer.DeflateRect(1,1);

	//调整Container位置
	if(m_bAutoAdjustPos)		
		AdjustRect(&rtContainer);

/////////////////////
//
	if(m_bMultiScreen)
	{ //多屏状态
		CRect rt;
		int nCount=m_PageList.GetCount();
		int i=0;
		for(POSITION pos=m_PageList.GetHeadPosition();pos!=NULL;)
		{
			CWnd *p=m_PageList.GetNext(pos);

			rt=rtContainer;
			CalcPageRect(&rt,i,nCount);
			rt.DeflateRect(WINDOW_SPACE,WINDOW_SPACE,WINDOW_SPACE,WINDOW_SPACE); //窗口之间的间隔
			p->MoveWindow(&rt);
			p->ShowWindow(SW_SHOW);
			i++;

			WinSetRect(p,rt);

		}
		if( m_bDrawActive && m_PageList.GetCount()>1 ) 
			DrawActivePage(TRUE);
	}
	else
	{ //单屏状态
		for(POSITION pos=m_PageList.GetHeadPosition();pos!=NULL;)
		{
			CWnd *p=m_PageList.GetNext(pos);
			if(p==m_pActivePage)
			{
				p->MoveWindow(&rtContainer);
			
				WinSetRect(p,rtContainer);
			}
			else 
			{
				if(m_bFullScreen)
				{
					p->MoveWindow(0,0,1,1);
					WinSetRect(p,CRect(0,0,1,1));
				}
				else
				{
					p->MoveWindow(rtContainer.right+1,rtContainer.bottom+1,1,1);
					WinSetRect(p,CRect(rtContainer.right+1,rtContainer.bottom+1,1,1));
				}
			}

		}
	}
}
void CBSWndContainer::WinSetRect(CWnd *p,CRect rt)
{
	int j;
	for(j=1;j<MAX_DEVICE_NUM;j++)
	{
		if(p== &(DlgMain->DlgScreen.m_screenPannel.m_wndVideo[j]))
		{
			m_winrect[j]=rt;
			return ;
		}
	}

}
int GetCurrentScreenMonitorRect(HWND hWnd,RECT& ScreenRect)
{
	/*
	int nRet = 0;
	HMONITOR hMonitor = MonitorFromWindow(hWnd, MONITOR_DEFAULTTONEAREST);
	MONITORINFO mInfo;
	mInfo.cbSize = sizeof(MONITORINFO);
	if(GetMonitorInfo(hMonitor,&mInfo) == TRUE)
	{
		ScreenRect = mInfo.rcMonitor;
	}
	else
	{
		//取主窗口的信息
		HMONITOR hMonitor2 = MonitorFromWindow(hWnd, MONITOR_DEFAULTTOPRIMARY);
		MONITORINFO mInfo2;
		mInfo2.cbSize = sizeof(MONITORINFO);
		if(GetMonitorInfo(hMonitor2,&mInfo2) == TRUE)
		{
			ScreenRect = mInfo.rcMonitor;
		}
		else
		{
			ScreenRect.bottom = 0;
			ScreenRect.left = 0;
			ScreenRect.right = 0;
			ScreenRect.top = 0;
			nRet  = -1;
		}
	}

	return nRet;
		*/
	return 1;
}


///////////////////////////////////////////////////
// full screen
void CBSWndContainer::SetFullScreen(BOOL bFlag)
{
	if(bFlag==m_bFullScreen) return;

	if( bFlag )
	{//全屏
// 		//得到显示器分辨率
// 		int cx=GetSystemMetrics(SM_CXSCREEN);
// 		int cy=GetSystemMetrics(SM_CYSCREEN);
// 
// 		//保存位置信息
// 		GetWindowPlacement(&_temppl);
// 		//修改风格
// 		ModifyStyle(WS_CHILD,WS_POPUP);
// 		//修改父窗口
// 		_tempparent=SetParent(NULL);
// 		_tempparent->ShowWindow(SW_HIDE);


		//移动窗口
/*		MoveWindow(0,0,cx,cy);*/

		ModifyStyle(WS_CHILD,WS_POPUP);
 		//修改父窗口
//  		_tempparent=SetParent(NULL);
//  		_tempparent->ShowWindow(SW_HIDE);
		GetWindowPlacement(&m_OldWndPlacement); 
		CRect WindowRect; 
		GetWindowRect(&WindowRect); 
		CRect ClientRect; 
		RepositionBars(0, 0xffff, AFX_IDW_PANE_FIRST, reposQuery, &ClientRect); 
		ClientToScreen(&ClientRect); 
		// 获取屏幕的分辨率 
		CRect screenRect;
		GetCurrentScreenMonitorRect(m_hWnd, screenRect);
		int nFullWidth = screenRect.Width(); //GetSystemMetrics(SM_CXSCREEN); 
		int nFullHeight = screenRect.Height(); //GetSystemMetrics(SM_CYSCREEN); 
		//将除控制条外的客户区全屏显示到从(0,0)到(nFullWidth, nFullHeight)区域, 将(0,0)和(nFullWidth, nFullHeight)两个点外扩充原窗口和除控制条之外的 客户区位置间的差值, 就得到全屏显示的窗口位置 
		m_FullScreenRect.left= WindowRect.left - ClientRect.left + screenRect.left;
		m_FullScreenRect.top = WindowRect.top - ClientRect.top + screenRect.top;
		m_FullScreenRect.right = m_FullScreenRect.left + WindowRect.right - ClientRect.right + nFullWidth;//::GetSystemMetrics(SM_CXSCREEN);
		m_FullScreenRect.bottom = m_FullScreenRect.top + WindowRect.bottom - ClientRect.bottom + nFullHeight;//::GetSystemMetrics(SM_CYSCREEN);
		
		m_bFullScreen=TRUE; // 设置全屏显示标志为 TRUE 
		// 进入全屏显示状态 
		WINDOWPLACEMENT wndpl; 
		wndpl.length=sizeof(WINDOWPLACEMENT); 
		wndpl.flags=0; 
		wndpl.showCmd=SW_SHOWNORMAL; 
		wndpl.rcNormalPosition= m_FullScreenRect; 
		_tempparent=SetParent(NULL);
 		_tempparent->ShowWindow(SW_HIDE);
		SetWindowPlacement(&wndpl);
	//	SetWindowPos(&wndTopMost,0,0,cx,cy,NULL);
	}
	else
	{//还原
		//还原父窗口
// 		_tempparent->ShowWindow(SW_SHOW);
// 		SetParent(_tempparent);
// 		//还原风格
// 		ModifyStyle(WS_POPUP,WS_CHILD);
// 		//还原位置
// 		SetWindowPlacement(&_temppl);
		_tempparent->ShowWindow(SW_SHOW);
		SetParent(_tempparent);
		//还原风格
		ModifyStyle(WS_POPUP,WS_CHILD);
		SetWindowPlacement(&m_OldWndPlacement);
	}

	m_bFullScreen=bFlag;
	Invalidate();
}
BOOL CBSWndContainer::GetFullScreen()
{
	return m_bFullScreen;
}

///////////////////////////////////////////////////
// multiscreen
void CBSWndContainer::SetMultiScreen(BOOL bFlag)
{
	if(m_bMultiScreen==bFlag) return;
	m_bMultiScreen=bFlag;
	Invalidate();
}
BOOL CBSWndContainer::GetMultiScreen()
{
	return m_bMultiScreen;
}

//////////////////////////////////////////////////
// autoadjustpos
void CBSWndContainer::SetAutoAdjustPos(BOOL bFlag)
{
	if(m_bAutoAdjustPos==bFlag) return;
	m_bAutoAdjustPos=bFlag;
	Invalidate();
}
BOOL CBSWndContainer::GetAutoAdjustPos()
{
	return m_bAutoAdjustPos;
}
/////////////////////////////////////////////////
//开启预览
void CBSWndContainer::SetWindPlayState(int screenNo,bool bFlag)
{
	if((DlgMain->DlgScreen.GetCurWindPlayState(screenNo)) == bFlag)
		return;
	if(bFlag)
	{
		DlgMain->DlgNormal.StartPlay(screenNo);
	}
	else
	{
		DlgMain->DlgNormal.StopPlay(screenNo);
	}
}
BOOL CBSWndContainer::GetPlayState(int screenNo)
{
	BOOL isPlay = DlgMain->DlgScreen.GetCurWindPlayState(screenNo);
	return isPlay;
}

//////////////////////////////////////////////////
//开启识别
void CBSWndContainer::SetWindDetectState(int screenNo,bool bFlag)
{
	if((DlgMain->DlgScreen.GetDetectState(screenNo)) == bFlag)
		return;
	if(bFlag)
	{
		DlgMain->DlgNormal.OpenDetect(screenNo);
	}
	else
	{
		DlgMain->DlgNormal.CloseDetect(screenNo);
		//关闭识别 也会关闭报警
		DlgMain->DlgNormal.CloseAlarm(screenNo);
	}
}
BOOL CBSWndContainer::GetDetectState(int screenNo)
{
	BOOL bEnable = DlgMain->DlgScreen.GetDetectState(screenNo);
	return bEnable;
}

//////////////////////////////////////////////////
//开启报警
void CBSWndContainer::SetWindAlarmState(int screenNo,bool bFlag)
{
	if((DlgMain->DlgScreen.GetAlarmState(screenNo)) == bFlag)
		return;
	if(bFlag)
	{
		//开启报警也会开启识别
		DlgMain->DlgNormal.OpenDetect(screenNo);
		DlgMain->DlgNormal.OpenAlarm(screenNo);
	}
	else
	{
		DlgMain->DlgNormal.CloseAlarm(screenNo);
	}
}
BOOL CBSWndContainer::GetAlarmState(int screenNo)
{
	BOOL bEnable = DlgMain->DlgScreen.GetAlarmState(screenNo);
	return bEnable;
}
//////////////////////////////////////////////////
//开启录制
void CBSWndContainer::SetWindRecordState(int screenNo,bool bFlag)
{
	if((DlgMain->DlgScreen.GetCurWindRecordState(screenNo)) == bFlag)
		return;
	if(bFlag)
	{
		DlgMain->DlgNormal.OpenRecord(screenNo);
	}
	else
	{
		DlgMain->DlgNormal.CloseRecord(screenNo);
	}
}

BOOL CBSWndContainer::GetRecordState(int screenNo)
{
	BOOL isRecord = DlgMain->DlgScreen.GetCurWindRecordState(screenNo);
	return isRecord;
}

//////////////////////////////////////////////////
//抓拍图像
void CBSWndContainer::Capture(int screenNo)
{
	DlgMain->DlgNormal.Capture(screenNo);
}

//////////////////////////////////////////////////
// draw active page
void CBSWndContainer::SetDrawActivePage(BOOL bFlag,COLORREF clrTopLeft,COLORREF clrBottomRight)
{
	if(m_bDrawActive==bFlag) return;
	if(bFlag)
	{
		m_clrTopLeft=clrTopLeft;
		m_clrBottomRight=clrBottomRight;
	}
	m_bDrawActive=bFlag;
	DrawActivePage(bFlag);
}
BOOL CBSWndContainer::GetDrawActivePage()
{
	return m_bDrawActive;
}

//////////////////////////////////////////////////
//	显示百分比
//	40 <= nPortion <=100
void CBSWndContainer::SetShowPortion(int nPortion)
{
	if(m_nShowPortion==nPortion) return;
	if(m_nShowPortion<40) m_nShowPortion=40;
	if(m_nShowPortion>100) m_nShowPortion=100;
	m_nShowPortion=nPortion;
	Invalidate();
}
int  CBSWndContainer::GetShowPortion()
{
	return m_nShowPortion;
}

///////////////////////////////////////////////////
// clean the no useful page in the container,
// return the page count.
int CBSWndContainer::UpdateList()
{
	POSITION posPrev;
	for(POSITION pos=m_PageList.GetHeadPosition();pos!=NULL;)
	{
		posPrev=pos;
		CWnd *p=m_PageList.GetNext(pos);
		if(!IsWindow(p->m_hWnd))
			m_PageList.RemoveAt(posPrev);
	}
	return m_PageList.GetCount();
}

///////////////////////////////////////////////////
// get a rect by the index of a child
void CBSWndContainer::CalcPageRect(LPRECT lpRect,int nIndex,int nPageCount)
{
	if((nPageCount<=0)||(nIndex>=nPageCount))
	{
		lpRect->left=lpRect->right=lpRect->top=lpRect->bottom=0;
		return;
	}
//get row count
	int nRow=0;
	while((nRow)*(nRow)<nPageCount) nRow++;

//get singledlg width and height
	int nWidth=(lpRect->right-lpRect->left)/nRow;
	int nHeight=(lpRect->bottom-lpRect->top)/nRow;

//get top-left point
	CPoint pt;
	pt.x=lpRect->left+nWidth*(nIndex%nRow);
	pt.y=lpRect->top+nHeight*(nIndex/nRow);

//set rect return back
	lpRect->left=pt.x;
	lpRect->top=pt.y;
	lpRect->right=lpRect->left+nWidth;
	lpRect->bottom=lpRect->top+nHeight;
}

///////////////////////////////////////////////////
// adjust a rect by defined proportion 
void CBSWndContainer::AdjustRect(LPRECT lpRect)
{
	int nWidth=lpRect->right-lpRect->left;
	int nHeight=lpRect->bottom-lpRect->top;
	CPoint pt((lpRect->left+lpRect->right)/2,(lpRect->top+lpRect->bottom)/2);

	//比例改16：9模式 ZOGNA
//	int nTemp=nWidth*8/11;
	int nTemp=nWidth*9.0/16.0;
	if(nTemp>nHeight)
	{
//		nWidth=nHeight*11/8;
		nWidth=nHeight*16.0/9.0;
	}
	else if(nTemp<nHeight)
	{
		nHeight=nTemp;
	}
	lpRect->left=pt.x-nWidth/2;
	lpRect->right=pt.x+nWidth/2;
	lpRect->top=pt.y-nHeight/2;
	lpRect->bottom=pt.y+nHeight/2;
}

///////////////////////////////////////////////////
//	按比例得到显示区域
void CBSWndContainer::GetShowRect(LPRECT lpRect)
{
	if(m_nShowPortion<40) m_nShowPortion=40;
	if(m_nShowPortion>100) m_nShowPortion=100;

	int nWidth	= lpRect->right-lpRect->left;
	int nHeight	= lpRect->bottom-lpRect->top;

	int nNewWidth	= (int)(nWidth*m_nShowPortion/100);
	int nNewHeight	= (int)(nHeight*m_nShowPortion/100);

	int ndx	= ( nWidth-nNewWidth )/2;
	int ndy = ( nHeight-nNewHeight )/2;

	lpRect->left	= lpRect->left	+ ndx;
	lpRect->top		= lpRect->top	+ ndy;
	lpRect->right	= lpRect->left	+ nNewWidth;
	lpRect->bottom	= lpRect->top	+ nNewHeight;	
}

///////////////////////////////////////////////////
// draw the frame of active page
void CBSWndContainer::DrawActivePage(BOOL bFlag)
{
	if( !m_bMultiScreen || 
		!m_pActivePage	|| 
		m_PageList.GetCount()<2 
		) return;

	CRect rt;
	m_pActivePage->GetWindowRect(&rt);
	ScreenToClient(&rt);


	rt.InflateRect(1,1);
	
	if(bFlag)
	{
		CDC *pDC=GetDC();
		if(!pDC) return;
//		pDC->Draw3dRect(&rt,m_clrTopLeft, m_clrBottomRight);

		pDC->Draw3dRect(&rt,RGB(255,128,0), RGB(255,128,0));		//16窗口选中色
		rt.InflateRect(1,1);
		pDC->Draw3dRect(&rt,RGB(255,128,0), RGB(255,128,0));

		ReleaseDC(pDC);
	}
	else
	{
		//	InvalidateRect(&rt);

		InvalidateRect(&rt);
		rt.InflateRect(1,1);
		InvalidateRect(&rt);

	}
	
		
}

