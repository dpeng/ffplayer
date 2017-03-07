#include "stdafx.h"
#include "titleBar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CTitleBase::CTitleBase()
{	
}

CTitleBase::~CTitleBase()
{
}


BEGIN_MESSAGE_MAP(CTitleBase, CWnd)
	ON_WM_CREATE()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_PAINT()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()


int CTitleBase::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	m_nLabelHeight = 18;
	return 0;
}

void CTitleBase::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CRect rect;
	GetClientRect(&rect);
	rect.DeflateRect(0,0,0,12);
	if(rect.PtInRect(point))
	{
		ReleaseCapture();
		GetParent()->SendMessage(WM_NCLBUTTONDOWN,HTCAPTION,0);
	}
	CWnd::OnLButtonDown(nFlags, point);
}

void CTitleBase::OnLButtonUp(UINT nFlags, CPoint point) 
{
	CWnd::OnLButtonUp(nFlags, point);
}



void CTitleBase::OnMouseMove(UINT nFlags, CPoint point) 
{
	CWnd::OnMouseMove(nFlags, point);
}

void CTitleBase::OnPaint() 
{
	CPaintDC dc(this);
	
	CDC memDC;
	memDC.CreateCompatibleDC(&dc);
	CBitmap bk;
	bk.LoadBitmap(m_nBackBmp);
	BITMAP bmp;
	bk.GetBitmap( &bmp );
	CBitmap *pOldBmp=memDC.SelectObject( &bk );
	dc.BitBlt( 0, 0, bmp.bmWidth, bmp.bmHeight, &memDC, 0, 0, SRCCOPY);
	memDC.SelectObject( pOldBmp );
	
	CRect rect;
	CFont font;
	LOGFONT lf;
	memset(&lf, 0, sizeof(LOGFONT));
	lf.lfHeight = 12;
	lf.lfWeight = 12;
#pragma warning (disable : 4996)
	strcpy((char*)lf.lfFaceName, "MS Sans Serif");
	lf.lfWeight=FW_BOLD;
	VERIFY(font.CreateFontIndirect(&lf));
	CFont *pOldFont = dc.SelectObject(&font);
	dc.SetBkMode(TRANSPARENT);
	GetClientRect(&rect);
	rect.bottom= 28;
	COLORREF OldColor = dc.SetTextColor(::GetSysColor( COLOR_3DHILIGHT));
	dc.SetTextAlign(TA_LEFT);
	dc.SetTextColor(RGB(222,229,234));
	dc.ExtTextOut(rect.left,rect.top + 5,  ETO_CLIPPED, &rect, m_strLabel,
		m_strLabel.GetLength(),NULL);
}

BOOL CTitleBase::Create(const RECT& rect, CWnd* pParentWnd,UINT nBgBmp,UINT nTbBmpUp,UINT nTbBmpDown,UINT nWndID) 
{
	m_nBackBmp=nBgBmp;
	
	return CWnd::Create(NULL, NULL, WS_CHILD|WS_VISIBLE, rect, pParentWnd, nWndID);
}


void CTitleBase::SetLabel(CString label)
{
	m_strLabel = label;

	CRect rect;
	GetClientRect(&rect);
	int nWide=(rect.left+rect.right);
	rect.left=nWide/3;
	rect.right=(nWide*2)/3 + 20;
	rect.bottom=m_nLabelHeight + 12;
	InvalidateRect(&rect,FALSE);
}
