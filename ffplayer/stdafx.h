
// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently,
// but are changed infrequently

#include "targetver.h"
#include <fstream>
#include <algorithm>
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


#ifdef UNICODE
#define _tstring  std::wstring
#else
#define _tstring  std::string
#endif // !UNICODE
