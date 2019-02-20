
// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently,
// but are changed infrequently

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // Exclude rarely-used stuff from Windows headers
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // some CString constructors will be explicit

// turns off MFC's hiding of some common and often safely ignored warning messages
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions





#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC support for Internet Explorer 4 Common Controls
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // MFC support for ribbons and control bars

static const long SYSTEM_BACKCOLOR = RGB(75, 75, 75);
static const long SYSTEM_BTNCOLOR = RGB(198, 206, 225);

#include <conio.h >
#include <vector>
#include <io.h>
#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <tuple>
#include <deque>
#include <algorithm>
//#ifndef COMPILE_IN_WIN_XP
//#include <VersionHelpers.h>
//#endif
#include <iomanip>
#include <map>
#include <cmath>
using std::ofstream;
using std::ifstream;
using std::string;
using std::wstring;
using std::vector;
using std::deque;
using std::map;



//����֧��ʹ��������ص���
#include <afxinet.h>
#include <afxwin.h>
#include <afxcontrolbars.h>
#include <afxwin.h>
#include <afxcmn.h>




//�Զ���ĺ�

#ifdef UNICODE
#define _tstring  std::wstring
#else
#define _tstring  std::string
#endif // !UNICODE

//����DPI���Ŵ�С
//#define theApp.DPI(x) (theApp.m_dpi*(x)/96)

//CPlayer�����õ��ĳ���
#define NEXT (-999)		//���塰��һ��������
#define PREVIOUS (-998)		//���塰��һ��������

#define MAX_NUM_LENGTH 5000		//�����ȡ��Ƶ�ļ���Ϣ���������ֵ
#define MAX_SONG_NUM 9999		//�����б����ļ����������ֵ

#define VERSION L"2.64"	//����汾
#define COMPILE_DATE L"2019/02/16"

//������ͼ��ť
#define IDT_PLAY_PAUSE 1321		//����������ͼ������/��ͣ����ť��ID
#define IDT_PREVIOUS 1322		//����������ͼ����һ������ť��ID
#define IDT_NEXT 1323			//����������ͼ����һ������ť��ID


//��ʱ��
#define TIMER_ID 1234			//���嶨ʱ����ID
#define TIMER_ELAPSE 80		//���嶨ʱ����Ӧ��ʱ���������룩
#define TIMER_ID_MINI 1235		//����ģʽ�еĶ�ʱ��ID
#define TIMER_ID_MINI2 1236		//����ģʽ�еĶ�ʱ��ID��������ʾƵ�׷���
#define TIMER_ELAPSE_MINI 200	//��������ģʽ�еĶ�ʱ����Ӧ��ʱ���������룩
//#define TIMER_ID_SPECTRAL 
//#define TIMER_ID_EXIT 1237

//Ƶ�׷���
#define FFT_SAMPLE 128			//Ƶ�׷�����������
//#define FFT_NUM 128			//Ҫʹ�õ�Ƶ�׵ĵ���
#define SPECTRUM_ROW 64			//Ƶ�׷������ε�����������Ϊ2�������η���С�ڻ����FFT_SAMPLE��

//��ȡ����������ǩʱ��Ĭ���ַ���
//#define CCommon::LoadText(IDS_DEFAULT_TITLE) L"<δ֪����>"
//#define CCommon::LoadText(IDS_DEFAULT_ARTIST) L"<δ֪������>"
//#define CCommon::LoadText(IDS_DEFAULT_ALBUM) L"<δ֪��Ƭ��>"
//#define CCommon::LoadText(IDS_DEFAULT_YEAR) L"<δ֪���>"
//#define CCommon::LoadText(IDS_DEFAULT_GENRE) L"<δ֪����>"

//һ����Ϊ�հ�ʱ��Ĭ����ʾ�ı�
//#define CCommon::LoadText(IDS_DEFAULT_LYRIC_TEXT) L"����"
//#define CCommon::LoadText(IDS_DEFAULT_LYRIC_TEXT_CORTANA) L"Music ��"

#define IDC_SAVE_COMBO_BOX 1990		//������ӵ������Ϊ���Ի����е���Ͽ��ID

//��ӵ�ϵͳ�˵��еġ�����ģʽ���˵����ID
#define IDM_MINIMODE 112

//������
#define EQU_CH_NUM 10		//������ͨ��������
const int EQU_STYLE_TABLE[9][EQU_CH_NUM]		//������Ԥ��
{
	{ 0,0,0,0,0,0,0,0,0,0 } ,			//��
	{ 4,3,3,2,2,1,0,-1,-2,-2 },		//�ŵ�
	{ 3,2,0,-1,-2,-2,-1,0,2,3 },		//����
	{ 2,1,0,-1,-1,1,3,5,3 },		//��ʿ
	{ -2,0,2,4,-1,-1,0,0,2,3 },		//ҡ��
	{ 1,0,0,1,2,1,-1,-2,-2,-2 },		//���
	{ 4,6,6,-2,-1,0,0,0,0,0 },		//�ص���
	{ -5,-5,-3,-2,-2,0,0,0,0,0 },		//��������
	{ 0,0,0,0,0,-1,-3,-5,-5,-4 }		//��������
};

#define GRAY(x) RGB(x,x,x)

#define ALBUM_COVER_NAME L"CurrentAlbumCover-MusicPlayer2-jMZB7TMf"
//#define DEFAULT_ALBUM_NAME L"cover"

//#define BACKGROUND_ALPHA 200	//���汳����͸����0~255
#define ALPHA_CHG(alpha) ((alpha)*255/100)		//��͸���ȴ�0~100ӳ�䵽0~255

#define LOCAL_DIR_STR L"%localdir%"

#define WM_OPEN_FILE_COMMAND_LINE (WM_USER+110)		//ͨ�������д��ļ�ʱ������ʵ�����ڷ��͵���Ϣ
#define MY_WM_NOTIFYICON (WM_USER+115)

#define WM_NEXT_USER_MSG (WM_USER+117)#define _tstring  std::wstring








