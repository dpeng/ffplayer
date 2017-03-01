
// ffplayerDlg.h : header file
//

#pragma once


extern "C"
{
#include "./ffplay/ffplay.h"
}

// CffplayerDlg dialog
#include "afxcmn.h"
#include "afxwin.h"
#include "xskin/xskinbutton.h"
class CffplayerDlg : public CDialogEx
{
// Construction
public:
	CffplayerDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FFPLAYER_DIALOG };
#endif
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	afx_msg void OnBnClickedButtonOpenfile();
	afx_msg void OnBnClickedButtonPlay();
	afx_msg void OnClose();
	afx_msg void OnBnClickedButtonStop();
	afx_msg void OnBnClickedButtonPause();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnNMReleasedcaptureSliderplayprogress(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
private:
	static DWORD WINAPI playProcess(LPVOID pParam);
	void OnWndFullScreen();
	void CreateBtnSkin();
	void cleanupResource(bool isTerminaterPlayProcess);
	WINDOWPLACEMENT m_OldWndplacement;
    HANDLE m_playProcessHandler;
	BOOL m_bIsFullScreen;
	CString m_strFileName;
	CBrush m_brushBackground;
	CBrush m_brushPlayarea;
	void * m_playHandler;
	int m_screenWidth;
	int m_screenHeight;
	CSliderCtrl m_sliderPlay;
	CSkinButton m_btnOpenFile;
	CSkinButton m_btnPlay;
	CSkinButton m_btnPause;
	CSkinButton m_btnStop;
	HICON m_hIcon;
	RECT m_rc;
};
