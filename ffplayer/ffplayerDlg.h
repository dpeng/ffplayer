
// ffplayerDlg.h : header file
//

#pragma once


extern "C"
{
#include "./ffplay/ffplay.h"
}

// CffplayerDlg dialog
#include "afxcmn.h"
class CffplayerDlg : public CDialogEx
{
// Construction
public:
	CffplayerDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FFPLAYER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonOpenfile();

public:
	afx_msg void OnBnClickedButtonPlay();
	afx_msg void OnClose();
	afx_msg void OnBnClickedButtonStop();
	afx_msg void OnBnClickedButtonPause();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	CSliderCtrl m_sliderPlay;
    HANDLE m_playProcessHandler;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
private:
	CString m_strFileName;
	void * m_playHandler;
	int m_screenWidth;
	int m_screenHeight;
	static DWORD WINAPI playProcess(LPVOID pParam);
public:
	afx_msg void OnNMReleasedcaptureSliderplayprogress(NMHDR *pNMHDR, LRESULT *pResult);
	//full screen related fuction(s)
	RECT m_rc;
	WINDOWPLACEMENT m_OldWndplacement;
	BOOL m_bIsFullScreen;
	void OnWndFullScreen();
};
