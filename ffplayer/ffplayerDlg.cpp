#include "stdafx.h"
#include "ffplayer.h"
#include "ffplayerDlg.h"
#include "afxdialogex.h"
#include <crtdbg.h>
#define _CRTDBG_MAP_ALLOC 

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
	DDX_Control(pDX, ID_BUTTON_OPENFILE, m_btnOpenFile);
	DDX_Control(pDX, ID_BUTTON_PLAY, m_btnPlay);
	DDX_Control(pDX, ID_BUTTON_PAUSE, m_btnPause);
	DDX_Control(pDX, ID_BUTTON_STOP, m_btnStop);
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
	ON_WM_CTLCOLOR()
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

	m_strFileName = _T("D:\\temp\\ShapeOfYou.mp4");
	//OnBnClickedButtonPlay();
	m_sliderPlay.SetRangeMin(0);
	m_sliderPlay.SetRangeMax(1000);
	m_sliderPlay.SetPos(0);
	m_sliderPlay.SetLineSize(1);
	m_sliderPlay.SetPageSize(5);
	m_playHandler = NULL;
	m_playProcessHandler = NULL;
    m_screenWidth = 0;
    m_screenHeight = 0;
	m_bIsFullScreen = false;
	CreateBtnSkin();
	m_brushBackground.CreateSolidBrush(SYSTEM_BACKCOLOR);
	m_brushPlayarea.CreateSolidBrush(RGB(50, 50, 50));
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
	this->UpdateWindow();
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CffplayerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CffplayerDlg::OnBnClickedButtonOpenfile()
{
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
	OutputDebugStringA(logbuf);
}
DWORD CffplayerDlg::playProcess(LPVOID pParam)
{
	CffplayerDlg* pThis = (CffplayerDlg*)pParam;
	char filename[MAX_PATH] = {};
	strcpy_s(filename, (LPCSTR)(CStringA)pThis->m_strFileName);
	int ret = ffplay_init(filename, pThis->m_playHandler, pThis->m_screenWidth, pThis->m_screenHeight);
	_CrtDumpMemoryLeaks();
	//0 means return successful
	if (ret == 0)		pThis->cleanupResource(FALSE);
	return ret;
}
void CffplayerDlg::OnBnClickedButtonPlay()
{
	//stop the playing before open an new play
	OnBnClickedButtonStop();

	RECT rc = {0};
	GetDlgItem(IDC_STATIC_PLAY)->GetWindowRect(&rc);
	m_screenWidth = rc.right - rc.left;
	m_screenHeight = rc.bottom - rc.top;
	ffplay_av_log_set_callback(av_log_encoder);

	//need show the play area again because when we close the SDL, it will hide the play window
	GetDlgItem(IDC_STATIC_PLAY)->ShowWindow(SW_SHOWNORMAL);

	m_playHandler = GetDlgItem(IDC_STATIC_PLAY)->GetSafeHwnd();
	SetTimer(1, 40, NULL);
	
	DWORD dw;
	m_playProcessHandler = CreateThread(NULL, 0, CffplayerDlg::playProcess, this, 0, &dw);
}


void CffplayerDlg::OnClose()
{
	cleanupResource(TRUE);
	CDialogEx::OnClose();
}


void CffplayerDlg::OnBnClickedButtonStop()
{
	cleanupResource(TRUE);
}


void CffplayerDlg::OnBnClickedButtonPause()
{
	ffplay_pause();
}


void CffplayerDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == 1)
	{
		double curTime; 
		int	totalTime;
		curTime = ffplay_get_stream_curtime();
		totalTime = ffplay_get_stream_totaltime();
		
		/*
		char logbuf[MAX_PATH];
		sprintf(logbuf, "Total: %d, Current: %7.2f, pos: %d\n", totalTime, curTime, (int)(curTime * 1000 / totalTime));
		OutputDebugStringA(logbuf);
		*/
		//isnan can judge the current time is invalide or not, it can happened when play start and seek
		if((totalTime >= 1) && !isnan(curTime))
			m_sliderPlay.SetPos((int)(curTime*1000/totalTime));
	}
	CDialogEx::OnTimer(nIDEvent);
}


BOOL CffplayerDlg::PreTranslateMessage(MSG* pMsg)
{
	if ( pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN) 
		return TRUE;
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE)
	{
		OnWndFullScreen();
		return TRUE;
	}
	if ( pMsg->message == WM_LBUTTONDOWN)
	{
		CRect rect;
		CPoint  pt;
        GetDlgItem(IDC_SLIDERPLAYPROGRESS)->GetWindowRect(&rect);		
        GetCursorPos(&pt);
        if (rect.PtInRect(pt))
			KillTimer(1);
	}
	if (pMsg->message == WM_LBUTTONUP)
	{
		Sleep(500);// avoid the slider jump from the start and seek position
		SetTimer(1, 40, NULL);
	}			
	return CDialogEx::PreTranslateMessage(pMsg);
}


void CffplayerDlg::OnNMReleasedcaptureSliderplayprogress(NMHDR *pNMHDR, LRESULT *pResult)
{
	int sliderPos = m_sliderPlay.GetPos();
	double pos = (double)sliderPos / (double)1000;
	ffplay_seek(pos);
	*pResult = 0;
}

void CffplayerDlg::OnWndFullScreen()
{
	m_bIsFullScreen = !m_bIsFullScreen;
	CButton *pButton;
	if (m_bIsFullScreen)
	{
		GetDlgItem(IDC_STATIC_PLAY)->GetWindowRect(&m_rc);
		pButton = (CButton *)GetDlgItem(ID_BUTTON_OPENFILE);
		pButton->ModifyStyle(WS_VISIBLE, 0, 0);
		pButton = (CButton *)GetDlgItem(ID_BUTTON_PLAY);
		pButton->ModifyStyle(WS_VISIBLE, 0, 0);
		pButton = (CButton *)GetDlgItem(ID_BUTTON_PAUSE);
		pButton->ModifyStyle(WS_VISIBLE, 0, 0);
		pButton = (CButton *)GetDlgItem(ID_BUTTON_STOP);
		pButton->ModifyStyle(WS_VISIBLE, 0, 0);
		m_sliderPlay.ModifyStyle(WS_VISIBLE, 0, 0);
		GetWindowPlacement(&m_OldWndplacement);
		ModifyStyle(WS_SIZEBOX, 0, 0);
		CRect WindowRect, ClientRect;
		RECT m_FullScreenRect;
		GetWindowRect(&WindowRect);
		WindowRect.left += 1;
		WindowRect.right += 1;
		MoveWindow(CRect(0, 0, 352, 288), TRUE);
		GetWindowRect(&WindowRect);
		GetClientRect(&ClientRect);
		ClientToScreen(&ClientRect);
		int x = GetSystemMetrics(SM_CXSCREEN);
		int y = GetSystemMetrics(SM_CYSCREEN);
		m_FullScreenRect.left = WindowRect.left - ClientRect.left;
		m_FullScreenRect.top = WindowRect.top - ClientRect.top;
		m_FullScreenRect.right = WindowRect.right - ClientRect.right + x;
		m_FullScreenRect.bottom = WindowRect.bottom - ClientRect.bottom + y;
		WINDOWPLACEMENT wndpl;
		wndpl.length = sizeof(WINDOWPLACEMENT);
		wndpl.flags = 0;
		wndpl.showCmd = SW_SHOWNORMAL;
		wndpl.rcNormalPosition = m_FullScreenRect;
		SetWindowPlacement(&wndpl);
		RECT rc;
		GetClientRect(&rc);
		GetDlgItem(IDC_STATIC_PLAY)->MoveWindow(&rc, TRUE);
	}
	else
	{
		pButton = (CButton *)GetDlgItem(ID_BUTTON_OPENFILE);
		pButton->ModifyStyle(0, WS_VISIBLE, 0);
		pButton = (CButton *)GetDlgItem(ID_BUTTON_PLAY);
		pButton->ModifyStyle(0, WS_VISIBLE, 0);
		pButton = (CButton *)GetDlgItem(ID_BUTTON_PAUSE);
		pButton->ModifyStyle(0, WS_VISIBLE, 0);
		pButton = (CButton *)GetDlgItem(ID_BUTTON_STOP);
		pButton->ModifyStyle(0, WS_VISIBLE, 0);
		m_sliderPlay.ModifyStyle(0, WS_VISIBLE, 0);
		SetWindowPlacement(&m_OldWndplacement);
		//left top right  bottom;
		RECT rc = { 0, 0, 0, 0 };
		rc.right = m_rc.right - m_rc.left;
		rc.bottom = m_rc.bottom - m_rc.top;
		GetDlgItem(IDC_STATIC_PLAY)->MoveWindow(&rc, TRUE);
	}
	this->RedrawWindow();
}

HBRUSH CffplayerDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);


	pDC->SetBkMode(TRANSPARENT);
	if( pWnd->GetDlgCtrlID() == IDC_STATIC_PLAY)    
		return m_brushPlayarea;
	return m_brushBackground;
	return hbr;
}

void CffplayerDlg::CreateBtnSkin()
{
	/* doesn't create title bar skin currently
	m_titleBar.Create(CRect(0, 0, 520, 25), this,
		IDB_COMONTITLE, IDB_COMONTITLE,
		IDB_COMONTITLE, IDB_COMONTITLE);
	m_titleBar.SetLabel("playing....");
	*/
	m_btnOpenFile.SetSkin(IDB_COMONBTNNORMAL, IDB_COMONBTNDOWN, IDB_COMONBTNOVER, 0, 0, 0, 0, 0, 0);
	m_btnOpenFile.SetTextColor(SYSTEM_BTNCOLOR);
	m_btnOpenFile.SizeToContent();
	m_btnPlay.SetSkin(IDB_COMONBTNNORMAL, IDB_COMONBTNDOWN, IDB_COMONBTNOVER, 0, 0, 0, 0, 0, 0);
	m_btnPlay.SetTextColor(SYSTEM_BTNCOLOR);
	m_btnPlay.SizeToContent();
	m_btnPause.SetSkin(IDB_COMONBTNNORMAL, IDB_COMONBTNDOWN, IDB_COMONBTNOVER, 0, 0, 0, 0, 0, 0);
	m_btnPause.SetTextColor(SYSTEM_BTNCOLOR);
	m_btnPause.SizeToContent();
	m_btnStop.SetSkin(IDB_COMONBTNNORMAL, IDB_COMONBTNDOWN, IDB_COMONBTNOVER, 0, 0, 0, 0, 0, 0);
	m_btnStop.SetTextColor(SYSTEM_BTNCOLOR);
	m_btnStop.SizeToContent();
}


void CffplayerDlg::cleanupResource(bool isTerminaterPlayProcess)
{   
	KillTimer(1);
	if (isTerminaterPlayProcess)    TerminateThread(m_playProcessHandler, 0);
	ffplay_stop();
	CloseHandle(m_playProcessHandler);
	m_playProcessHandler = NULL;
	m_playHandler = NULL;
	m_sliderPlay.SetPos(0);

	//this is for redraw play area
	GetDlgItem(IDC_STATIC_PLAY)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_PLAY)->ShowWindow(SW_SHOWNORMAL);
}