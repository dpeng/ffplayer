
// ffplayerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ffplayer.h"
#include "ffplayerDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CffplayerDlg dialog



CffplayerDlg::CffplayerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_FFPLAYER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CffplayerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SLIDERPLAYPROGRESS, m_sliderPlay);
}

BEGIN_MESSAGE_MAP(CffplayerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(ID_BUTTON_OPENFILE, &CffplayerDlg::OnBnClickedButtonOpenfile)
	ON_BN_CLICKED(ID_BUTTON_PLAY, &CffplayerDlg::OnBnClickedButtonPlay)
	ON_WM_CLOSE()
	ON_BN_CLICKED(ID_BUTTON_STOP, &CffplayerDlg::OnBnClickedButtonStop)
	ON_BN_CLICKED(ID_BUTTON_PAUSE, &CffplayerDlg::OnBnClickedButtonPause)
	ON_WM_TIMER()
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDERPLAYPROGRESS, &CffplayerDlg::OnNMReleasedcaptureSliderplayprogress)
END_MESSAGE_MAP()


// CffplayerDlg message handlers

BOOL CffplayerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	ShowWindow(SW_SHOWNORMAL);

	// TODO: Add extra initialization here
	m_strFileName = _T("D:\\temp\\ShapeOfYou.mp4");
	//OnBnClickedButtonPlay();
	m_sliderPlay.SetRangeMin(0);
	m_sliderPlay.SetRangeMax(1000);
	m_sliderPlay.SetPos(0);
	m_sliderPlay.SetLineSize(1);
	m_sliderPlay.SetPageSize(5);
	m_playHandler = NULL;
    m_screenWidth = 0;
    m_screenHeight = 0;
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CffplayerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CffplayerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CffplayerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CffplayerDlg::OnBnClickedButtonOpenfile()
{
	// TODO: Add your control notification handler code here
	CString tempfilename = _T("");
	CFileDialog FileChooser(TRUE,
		NULL,
		NULL,
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		_T("all files(*.*)|*.*|mp4 files(*.mp4)|*.mp4|mp3 files(*.mp3)|*.mp3||"));

	if (FileChooser.DoModal() == IDOK)    tempfilename = FileChooser.GetPathName();
	else    return;

	m_strFileName = tempfilename;
	tempfilename = _T("");
}

static void av_log_encoder(void *ptr, int level, const char *fmt, va_list vargs)
{
	if(level >= 32)
		return;
	char logbuf[MAX_PATH];
	vsnprintf(logbuf, sizeof(logbuf), fmt, vargs);
	OutputDebugString(logbuf);
}
DWORD CffplayerDlg::playProcess(LPVOID pParam)
{
	CffplayerDlg* pThis = (CffplayerDlg*)pParam;
	char filename[MAX_PATH] = {};
	strcpy(filename, (LPCSTR)(CStringA)pThis->m_strFileName);
	int ret = ffplay_init(filename, pThis->m_playHandler, pThis->m_screenWidth, pThis->m_screenHeight);
	if (ret == 0)
	{
		pThis->KillTimer(1);
		ffplay_stop();
		CloseHandle(pThis->m_playProcessHandler);
		pThis->m_playProcessHandler = NULL;
		pThis->m_playHandler = NULL;
		pThis->m_sliderPlay.SetPos(0);
	}
	return ret;
}
void CffplayerDlg::OnBnClickedButtonPlay()
{
	// TODO: Add your control notification handler code here
	RECT rc = {0};
	GetDlgItem(IDC_STATIC_PLAY)->GetWindowRect(&rc);
	m_screenWidth = rc.right - rc.left;
	m_screenHeight = rc.bottom - rc.top;
	ffplay_av_log_set_callback(av_log_encoder);
	GetDlgItem(IDC_STATIC_PLAY)->ShowWindow(SW_SHOWNORMAL);
	m_playHandler = GetDlgItem(IDC_STATIC_PLAY)->GetSafeHwnd();
	SetTimer(1, 40, NULL);
	
	DWORD dw;
	m_playProcessHandler = CreateThread(NULL, 0, CffplayerDlg::playProcess, this, 0, &dw);
}


void CffplayerDlg::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	KillTimer(1);
	TerminateThread(m_playProcessHandler, 0);
	ffplay_exit();
	CloseHandle(m_playProcessHandler);
	m_playProcessHandler = NULL;
	m_playHandler = NULL;
	CDialogEx::OnClose();
}


void CffplayerDlg::OnBnClickedButtonStop()
{
	// TODO: Add your control notification handler code here
	KillTimer(1);
	TerminateThread(m_playProcessHandler, 0);
	ffplay_stop();
	CloseHandle(m_playProcessHandler);
	m_playProcessHandler = NULL;
	m_playHandler = NULL;
	m_sliderPlay.SetPos(0);
}


void CffplayerDlg::OnBnClickedButtonPause()
{
	// TODO: Add your control notification handler code here
	ffplay_pause();
}


void CffplayerDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	if (nIDEvent == 1)
	{
		double curTime; 
		int	totalTime;
		curTime = ffplay_get_stream_curtime();
		totalTime = ffplay_get_stream_totaltime();
		/*
		CString str = _T(" ");
		str.Format("Total: %d, Current: %7.2f\n", totalTime, curTime);
		OutputDebugString(str);
		*/
		if(totalTime >= 1)
			m_sliderPlay.SetPos(curTime*1000/totalTime);
	}
	CDialogEx::OnTimer(nIDEvent);
}


BOOL CffplayerDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if ( pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN) 
		return TRUE;
	if ( pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE) 
		return TRUE;
	if ( pMsg->message == WM_LBUTTONDOWN)
	{
		CRect rect;
        GetDlgItem(IDC_SLIDERPLAYPROGRESS)->GetWindowRect(&rect);
		
        CPoint  pt;
        GetCursorPos(&pt);

        if (rect.PtInRect(pt))
        {
			KillTimer(1);
        }
	}
	if ( pMsg->message == WM_LBUTTONUP)
		SetTimer(1, 40, NULL);
	return CDialogEx::PreTranslateMessage(pMsg);
}


void CffplayerDlg::OnNMReleasedcaptureSliderplayprogress(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	int sliderPos = m_sliderPlay.GetPos();
	double pos = (double)sliderPos / (double)1000;
	ffplay_seek(pos);
	*pResult = 0;
}
