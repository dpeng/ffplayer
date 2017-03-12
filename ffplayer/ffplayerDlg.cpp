#include "stdafx.h"
#include "ffplayer.h"
#include "ffplayerDlg.h"
#include "afxdialogex.h"
#include <crtdbg.h>
#define _CRTDBG_MAP_ALLOC 

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/***************************about dialog*************start********/
class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);
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
/***************************about dialog***************end******/

CffplayerDlg::CffplayerDlg(CWnd* pParent)
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
	DDX_Control(pDX, ID_BUTTON_CONSOLE, m_btnConsole);
	DDX_Control(pDX, ID_BUTTON_PLAYNEXT, m_btnPlayNext);
	DDX_Control(pDX, ID_BUTTON_PLAYPREVIOUS, m_btnPlayPreVious);
	DDX_Control(pDX, ID_BUTTON_PLAYTONEXTFRAME, m_btnPlayFrame);
}

BEGIN_MESSAGE_MAP(CffplayerDlg, CDialogEx)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDERPLAYPROGRESS, &CffplayerDlg::OnNMReleasedcaptureSliderplayprogress)
	ON_BN_CLICKED(ID_BUTTON_OPENFILE, &CffplayerDlg::OnBnClickedButtonOpenfile)
	ON_BN_CLICKED(ID_BUTTON_PLAY, &CffplayerDlg::OnBnClickedButtonPlay)
	ON_BN_CLICKED(ID_BUTTON_STOP, &CffplayerDlg::OnBnClickedButtonStop)
	ON_BN_CLICKED(ID_BUTTON_PAUSE, &CffplayerDlg::OnBnClickedButtonPause)
	ON_WM_QUERYDRAGICON()
	ON_WM_SYSCOMMAND()
	ON_WM_CTLCOLOR()
	ON_WM_PAINT()
	ON_WM_CLOSE()
	ON_WM_TIMER()
	ON_BN_CLICKED(ID_BUTTON_CONSOLE, &CffplayerDlg::OnBnClickedButtonConsole)
	ON_BN_CLICKED(ID_BUTTON_PLAYNEXT, &CffplayerDlg::OnBnClickedButtonPlaynext)
	ON_BN_CLICKED(ID_BUTTON_PLAYPREVIOUS, &CffplayerDlg::OnBnClickedButtonPlayprevious)
	ON_BN_CLICKED(ID_BUTTON_PLAYTONEXTFRAME, &CffplayerDlg::OnBnClickedButtonPlaytonextframe)
END_MESSAGE_MAP()

BOOL CffplayerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
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

	SetIcon(m_hIcon, TRUE);
	SetIcon(m_hIcon, FALSE);

	ShowWindow(SW_SHOWNORMAL);
	m_curPlayingIndex = 0;
	m_totalFileNameInList = 0;
	//m_fileNameList[m_curPlayingIndex] = _T("D:\\temp\\ShapeOfYou.mp4");
	m_sliderPlay.SetRangeMin(0);
	m_sliderPlay.SetRangeMax(1000);
	m_sliderPlay.SetPos(0);
	m_sliderPlay.SetLineSize(1);
	m_sliderPlay.SetPageSize(5);
	m_playHandler = NULL;
	m_playProcessHandler = NULL;
	m_consoleMonitorProcessHandler = NULL;
    m_screenWidth = 0;
    m_screenHeight = 0;
	m_bIsFullScreen = false;
	m_bIsPlaying = FALSE;
	CreateBtnSkin();
	m_brushBackground.CreateSolidBrush(SYSTEM_BACKCOLOR);
	m_brushPlayarea.CreateSolidBrush(RGB(26, 26, 26));
	m_hOutputConsole = NULL;
	m_hInputConsole = NULL;
	m_bIsConsoleDisplay = FALSE;
	//_CrtDumpMemoryLeaks();
	OnBnClickedButtonConsole();
	ffplay_toggle_set_init_volume(10);
	return TRUE;
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

void CffplayerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this);
		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
	this->UpdateWindow();
}

HCURSOR CffplayerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CffplayerDlg::OnBnClickedButtonOpenfile()
{

	CString FileName = _T("");
	CFileDialog FileChooser(TRUE, 
		NULL,
		NULL, 
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_ALLOWMULTISELECT,
		_T("all files(*.*)|*.*|mp4 files(*.mp4)|*.mp4|mp3 files(*.mp3)|*.mp3||"));

	FileChooser.m_ofn.nMaxFile = MAX_PATH;

	if (IDOK == FileChooser.DoModal())
	{
		POSITION pos;
		pos=FileChooser.GetStartPosition();
		while (pos!=NULL)
		{
			FileName = FileChooser.GetNextPathName(pos);
			if (m_totalFileNameInList < MAX_PATH) m_fileNameList[m_totalFileNameInList++] = FileName;
			else MessageBox(_T("file name list too long, the max number is 260"), _T("confirm"), MB_ICONQUESTION | MB_YESNO);
		}
		m_totalFileNameInList--;
	}
	//automatic play after open file
	//OnBnClickedButtonPlay();
}

static void pushLogsToConsole(const char *fmt, va_list vargs)
{
	char logbuf[1024*8];
	DWORD len;
	memset(logbuf, 0, sizeof(logbuf));
	
	vsprintf_s(logbuf, fmt, vargs);
	WriteConsoleA(GetStdHandle(STD_OUTPUT_HANDLE), logbuf, (DWORD)strlen(logbuf), &len, NULL);	
}

static void consolePrint(const char *fmt, ...)
{
    va_list vargs;
    va_start(vargs, fmt);
	pushLogsToConsole(fmt, vargs);
    va_end(vargs);
}

static void av_log_encoder(void *ptr, int level, const char *fmt, va_list vargs)
{
	if(level > 32)    return;
	pushLogsToConsole(fmt, vargs);
}

DWORD CffplayerDlg::playProcess(LPVOID pParam)
{
	CffplayerDlg* pThis = (CffplayerDlg*)pParam;
	char filename[MAX_PATH] = {};
	strcpy_s(filename, (LPCSTR)(CStringA)pThis->m_fileNameList[pThis->m_curPlayingIndex]);
	int ret = ffplay_init(filename, pThis->m_playHandler, pThis->m_screenWidth, pThis->m_screenHeight);
	//0 means return successful
	if (ret == 0) {
		pThis->cleanupResource(FALSE);
		pThis->SetTimer(2, 100, NULL); //inform the timer that this file play comes to an end
	}
	else pThis->MessageBox(_T("playing error, will pass play this file."), _T("confirm"), MB_ICONQUESTION | MB_OK);
	return ret;
}
void CffplayerDlg::OnBnClickedButtonPlay()
{
	if (m_fileNameList[m_curPlayingIndex].GetLength() < 1)
		return;
	if (!PathFileExists(m_fileNameList[m_curPlayingIndex]))
		return;
	RECT rc = {0};
	DWORD threadID;
	//stop the playing before open an new play
	OnBnClickedButtonStop();

	GetDlgItem(IDC_STATIC_PLAY)->GetWindowRect(&rc);
	m_screenWidth = rc.right - rc.left;
	m_screenHeight = rc.bottom - rc.top;
	ffplay_av_log_set_callback(av_log_encoder);

	//need show the play area again because when we close the SDL, it will hide the play window
	GetDlgItem(IDC_STATIC_PLAY)->ShowWindow(SW_SHOWNORMAL);

	m_playHandler = GetDlgItem(IDC_STATIC_PLAY)->GetSafeHwnd();
	m_bIsPlaying = TRUE;
	SetTimer(1, 40, NULL);
	
	m_playProcessHandler = CreateThread(NULL, 0, CffplayerDlg::playProcess, this, 0, &threadID);
}

void CffplayerDlg::OnClose()
{
	cleanupResource(TRUE);
	stopConsole();
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
int g_waitingTime = 0;
void CffplayerDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == 1)
	{
		double curTime; 
		int	totalTime;
		curTime = ffplay_get_stream_curtime();
		totalTime = ffplay_get_stream_totaltime();
		
		//consolePrint("playing: %.2f%% Total Time: %d Current Time: %.2f\n", (curTime * 100 / totalTime), totalTime, curTime);
		//isnan can judge the current time is invalid or not, it can happened when play start and seek
		if((totalTime >= 1) && !isnan(curTime))
			m_sliderPlay.SetPos((int)(curTime*1000/totalTime));
	}
	if (nIDEvent == 2)
	{
		consolePrint("current playing index: %d, fileName: %s\n", m_curPlayingIndex, m_fileNameList[m_curPlayingIndex]);
		
		if (m_curPlayingIndex < m_totalFileNameInList) m_curPlayingIndex++;
		else m_curPlayingIndex = 0;
		OnBnClickedButtonPlay();
		KillTimer(2);
	}
	CDialogEx::OnTimer(nIDEvent);
}

BOOL CffplayerDlg::PreTranslateMessage(MSG* pMsg)
{
	if ( pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN) return TRUE;
	if ( pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE)
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
        if (rect.PtInRect(pt)) KillTimer(1);
	}
	if ((pMsg->message == WM_LBUTTONUP) && (TRUE == m_bIsPlaying))
	{
		Sleep(500);// avoid the slider jump from the start and seek position
		SetTimer(1, 40, NULL);
	}			
	if (pMsg->message == WM_LBUTTONDBLCLK)
	{
		CRect rect;
		CPoint  pt;
		GetDlgItem(IDC_STATIC_PLAY)->GetWindowRect(&rect);
		GetCursorPos(&pt);
		if (rect.PtInRect(pt)) OnWndFullScreen();
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
		pButton = (CButton *)GetDlgItem(ID_BUTTON_CONSOLE);
		pButton->ModifyStyle(WS_VISIBLE, 0, 0);
		pButton = (CButton *)GetDlgItem(ID_BUTTON_PLAYNEXT);
		pButton->ModifyStyle(WS_VISIBLE, 0, 0);
		pButton = (CButton *)GetDlgItem(ID_BUTTON_PLAYPREVIOUS);
		pButton->ModifyStyle(WS_VISIBLE, 0, 0);
		pButton = (CButton *)GetDlgItem(ID_BUTTON_PLAYTONEXTFRAME);
		pButton->ModifyStyle(WS_VISIBLE, 0, 0);
		m_sliderPlay.ModifyStyle(WS_VISIBLE, 0, 0);
		GetWindowPlacement(&m_OldWndplacement);
		ModifyStyle(WS_SIZEBOX, 0, 0);
		CRect WindowRect, ClientRect;
		RECT fullScreenRc;
		GetWindowRect(&WindowRect);
		WindowRect.left += 1;
		WindowRect.right += 1;
		MoveWindow(CRect(0, 0, 352, 288), TRUE);
		GetWindowRect(&WindowRect);
		GetClientRect(&ClientRect);
		ClientToScreen(&ClientRect);
		int x = GetSystemMetrics(SM_CXSCREEN);
		int y = GetSystemMetrics(SM_CYSCREEN);
		fullScreenRc.left = WindowRect.left - ClientRect.left;
		fullScreenRc.top = WindowRect.top - ClientRect.top;
		fullScreenRc.right = WindowRect.right - ClientRect.right + x;
		fullScreenRc.bottom = WindowRect.bottom - ClientRect.bottom + y;
		WINDOWPLACEMENT wndpl;
		wndpl.length = sizeof(WINDOWPLACEMENT);
		wndpl.flags = 0;
		wndpl.showCmd = SW_SHOWNORMAL;
		wndpl.rcNormalPosition = fullScreenRc;
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
		pButton = (CButton *)GetDlgItem(ID_BUTTON_CONSOLE);
		pButton->ModifyStyle(0, WS_VISIBLE, 0);
		pButton = (CButton *)GetDlgItem(ID_BUTTON_PLAYNEXT);
		pButton->ModifyStyle(0, WS_VISIBLE, 0);
		pButton = (CButton *)GetDlgItem(ID_BUTTON_PLAYPREVIOUS);
		pButton->ModifyStyle(0, WS_VISIBLE, 0);
		pButton = (CButton *)GetDlgItem(ID_BUTTON_PLAYTONEXTFRAME);
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
	if( pWnd->GetDlgCtrlID() == IDC_STATIC_PLAY) return m_brushPlayarea;
	return m_brushBackground;
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
	m_btnConsole.SetSkin(IDB_COMONBTNNORMAL, IDB_COMONBTNDOWN, IDB_COMONBTNOVER, 0, 0, 0, 0, 0, 0);
	m_btnConsole.SetTextColor(SYSTEM_BTNCOLOR);
	m_btnConsole.SizeToContent();
	m_btnPlayNext.SetSkin(IDB_COMONBTNNORMAL, IDB_COMONBTNDOWN, IDB_COMONBTNOVER, 0, 0, 0, 0, 0, 0);
	m_btnPlayNext.SetTextColor(SYSTEM_BTNCOLOR);
	m_btnPlayNext.SizeToContent();
	m_btnPlayPreVious.SetSkin(IDB_COMONBTNNORMAL, IDB_COMONBTNDOWN, IDB_COMONBTNOVER, 0, 0, 0, 0, 0, 0);
	m_btnPlayPreVious.SetTextColor(SYSTEM_BTNCOLOR);
	m_btnPlayPreVious.SizeToContent();
	m_btnPlayFrame.SetSkin(IDB_COMONBTNNORMAL, IDB_COMONBTNDOWN, IDB_COMONBTNOVER, 0, 0, 0, 0, 0, 0);
	m_btnPlayFrame.SetTextColor(SYSTEM_BTNCOLOR);
	m_btnPlayFrame.SizeToContent();
}

void CffplayerDlg::cleanupResource(bool isTerminaterPlayProcess)
{   
	KillTimer(1);
	if (isTerminaterPlayProcess)    TerminateThread(m_playProcessHandler, 0);
	ffplay_stop();
	CloseHandle(m_playProcessHandler);
	m_playProcessHandler = NULL;
	m_playHandler = NULL;
	m_bIsPlaying = FALSE;
	m_sliderPlay.SetPos(0);

	//this is for redraw play area
	GetDlgItem(IDC_STATIC_PLAY)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_PLAY)->ShowWindow(SW_SHOWNORMAL);
}

void CffplayerDlg::initConsole()
{
	DWORD threadID;
	AllocConsole();
	m_consoleWindowWidth = 0;
	m_hOutputConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	m_hInputConsole  = GetStdHandle(STD_INPUT_HANDLE);
	//SetConsoleMode(m_hInputConsole, ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT);
	SMALL_RECT rc = { 0, 0, 0, 0 };
	COORD tmpCoord = GetLargestConsoleWindowSize(m_hOutputConsole);
	if(tmpCoord.X >= 100) tmpCoord.X = 100;
	else if(tmpCoord.X - 1 >= 50) tmpCoord.X = 50;
	rc.Right = tmpCoord.X - 1;
	rc.Bottom = (tmpCoord.Y)*2/3;
	tmpCoord.Y = tmpCoord.Y * 5;
	SetConsoleScreenBufferSize(m_hOutputConsole, tmpCoord);
	BOOL ret = SetConsoleWindowInfo(m_hOutputConsole, TRUE, &rc);
	if(ret == TRUE) m_consoleWindowWidth = tmpCoord.X;
	m_consoleMonitorProcessHandler = CreateThread(NULL, 0, CffplayerDlg::consoleInputMonitor, this, 0, &threadID);
}

void CffplayerDlg::stopConsole()
{
	FreeConsole();	
	CloseHandle(m_consoleMonitorProcessHandler);
	m_consoleMonitorProcessHandler = NULL;
	CloseHandle(m_hOutputConsole);
	m_hOutputConsole = NULL;
	CloseHandle(m_hInputConsole);
	m_hInputConsole = NULL;
}

DWORD CffplayerDlg::ProcessConsoleInput(INPUT_RECORD* pInputRec,DWORD dwInputs)
{
	if(pInputRec == NULL)	return 1;
	switch (pInputRec->EventType)
	{
	case KEY_EVENT:
		if(pInputRec->Event.KeyEvent.bKeyDown)
		{
			switch (pInputRec->Event.KeyEvent.wVirtualKeyCode)
			{
			case 0x50: /*VK_P*/
				OnBnClickedButtonPause();
				break;
			case 0x4e: /*VK_N*/
				break;
			case 0x46: /*VK_F*/
				OnWndFullScreen();
				break;
			case 0x48: /*VK_H*/
				consolePrint("\n****************************************Help*****************************************\n"
				           "                                                                                   **\n"
				           "O                   open file                                                      **\n"
				           "Space               play file                                                      **\n"
				           "Q                   quit                                                           **\n"
				           "F                   toggle full screen                                             **\n"
				           "P                   Pause                                                          **\n"
				           "right mouse click   seek to percentage in file corresponding to fraction of width  **\n"
				           "W                   cycle video filters or show modes                              **\n"
				           "M                   toggle mute                                                    **\n"
				           "9, 0                decrease and increase volume respectively                      **\n"
				           "S                   activate frame-step mode                                       **\n"
				           "                                                                                   **\n"
				           "****************************************Help*****************************************\n"
				           );
				break;
			case 0x4f:/*VK_O*/
				OnBnClickedButtonOpenfile();
				break;
			case 0x51:/*VK_Q*/
				OnClose();
				break;
			case 0x53:/*VK_S*/
				OnBnClickedButtonPlaytonextframe();
				break;
			case 0x57:/*VK_W*/
				ffplay_toggle_display();
				break;
			case VK_SPACE:
				OnBnClickedButtonPlay();
				break;
			case VK_MEDIA_NEXT_TRACK:
				OnBnClickedButtonPlaynext();
				break;
			case VK_MEDIA_PREV_TRACK:
				OnBnClickedButtonPlayprevious();
				break;
			case VK_MEDIA_PLAY_PAUSE:
				OnBnClickedButtonPause();
				break;
			case VK_VOLUME_UP:
			case 0x30:/*  0  */
				ffplay_toggle_update_volume(1, 0.75);
				break;
			case VK_VOLUME_DOWN:
			case 0x39:/*  9  */
				ffplay_toggle_update_volume(-1, 0.75);
				break;
			case VK_VOLUME_MUTE:
			case 0x4d:/*  M  */
				ffplay_toggle_mute();
				consolePrint("current volume: %f\n", ffplay_toggle_get_volume());
				break;
			case VK_RETURN:
				break;
			default:
				break;
			}
		}
		break;
	case MOUSE_EVENT:
		if(pInputRec->Event.MouseEvent.dwButtonState == RIGHTMOST_BUTTON_PRESSED)
		{
			consolePrint("FROM_LEFT_1ST_BUTTON_PRESSED: %d pos: %d %d\n", 
				pInputRec->EventType,
				pInputRec->Event.MouseEvent.dwMousePosition.X,
				pInputRec->Event.MouseEvent.dwMousePosition.Y);
			if ((m_consoleWindowWidth > 0) && (pInputRec->Event.MouseEvent.dwMousePosition.X >= 0) &&
				(m_consoleWindowWidth >= pInputRec->Event.MouseEvent.dwMousePosition.X))
			{
				double pos = (double)pInputRec->Event.MouseEvent.dwMousePosition.X/(double)m_consoleWindowWidth;
				if(m_bIsPlaying) ffplay_seek(pos);
			}
		}

		break;
	case WINDOW_BUFFER_SIZE_EVENT:
		break;
	case MENU_EVENT:
		break;
	case FOCUS_EVENT:
		break;
	default:
		break;
	}
	return 0;
}
DWORD CffplayerDlg::consoleInputMonitor(LPVOID pParam)
{
	CffplayerDlg *pThis = (CffplayerDlg *)pParam;
	if(pThis == NULL) return -1;

	DWORD   dwInputs;
	INPUT_RECORD lpBuffer[MAX_PATH];
	FlushConsoleInputBuffer(pThis->m_hInputConsole);

	while(pThis->m_consoleMonitorProcessHandler)
	{
		// If there are input events in buffer, this object is signaled
		WaitForSingleObject(pThis->m_hInputConsole, INFINITE);
		GetNumberOfConsoleInputEvents(pThis->m_hInputConsole, &dwInputs);
		ReadConsoleInput(pThis->m_hInputConsole, lpBuffer, min(dwInputs,MAX_PATH), &dwInputs);

		pThis->ProcessConsoleInput(lpBuffer,0);
	}
	return 0;
}


void CffplayerDlg::OnBnClickedButtonConsole()
{
	// TODO: Add your control notification handler code here
	m_bIsConsoleDisplay = !m_bIsConsoleDisplay;
	if (m_bIsConsoleDisplay)
	{
		initConsole();
	} 
	else
	{
		stopConsole();
	}
}

void CffplayerDlg::OnBnClickedButtonPlaynext()
{
	if (m_curPlayingIndex < m_totalFileNameInList) m_curPlayingIndex++;
	else m_curPlayingIndex = 0;
	OnBnClickedButtonPlay();
}


void CffplayerDlg::OnBnClickedButtonPlayprevious()
{
	if((m_curPlayingIndex - 1) > 0) m_curPlayingIndex--;
	else m_curPlayingIndex = m_totalFileNameInList;
	
	OnBnClickedButtonPlay();
}

void CffplayerDlg::OnBnClickedButtonPlaytonextframe()
{
	ffplay_step_to_next_frame();
}
