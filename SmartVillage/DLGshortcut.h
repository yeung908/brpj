#include "afxwin.h"
#if !defined(AFX_DLGSHORTCUT_H__6B00BCDF_E99D_4623_A6DB_6CE96853BBF3__INCLUDED_)
#define AFX_DLGSHORTCUT_H__6B00BCDF_E99D_4623_A6DB_6CE96853BBF3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DLGshortcut.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDLGshortcut dialog

class CDLGshortcut : public CDialog
{
// Construction
public:
	CDLGshortcut(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDLGshortcut)
	enum { IDD = IDD_SHORTCUT };



	CBitmapButton	m_preview;
	CBitmapButton	m_9mode;
	CBitmapButton	m_4mode;
	CBitmapButton	m_16mode;
	CBitmapButton	m_1mode;
	CBitmapButton	m_Capbmp;
	CBitmapButton	m_Car;
	//}}AFX_DATA

	CToolTipCtrl m_ToolTip; 
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	int		screenmode;
	bool	CarEnable;

	bool	PreviewEnable;
	bool	RecordEnable;


	bool	faceEnable;
	bool	adjustEnable;
	void AdjustBMP();
	void FaceBMP() ;
	void CarBMP(); 
	void PreviewBMP();
	void ModeBMP(int oldmode,int newmode);


	BOOL	OnInitDialog();
	void AutoSize();

	//���β���
	void OnOK();
	void OnCancel();

	afx_msg void OnButtonMode1();
	afx_msg void OnButtonMode4();
	afx_msg void OnButtonMode9();
	afx_msg void OnButtonMode16();
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDLGshortcut)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDLGshortcut)
	afx_msg void OnButtonCar();
	afx_msg void OnButtonCapbmp();



	afx_msg void OnButtonPreview();
	afx_msg void OnPaint();

	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
		afx_msg void OnBnClickedButtonFace();
	afx_msg void OnBnClickedButtonAutoadjust();
	CBitmapButton m_adjust;
	CBitmapButton m_face;
	CBitmapButton m_btnCarClear;
	CBitmapButton m_btnStopAll;
	afx_msg void OnBnClickedButtonCarClear();
	afx_msg void OnBnClickedButtonStopAll();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSHORTCUT_H__6B00BCDF_E99D_4623_A6DB_6CE96853BBF3__INCLUDED_)
