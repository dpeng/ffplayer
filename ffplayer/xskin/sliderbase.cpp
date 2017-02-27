#include "stdafx.h"

#include "SliderBase.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CSliderThumb::CSliderThumb()
{
	m_DrawMode			= 1;
	m_FocusRectMargin	= 0;
	hClipRgn			= NULL;
	m_TextColor			= GetSysColor(COLOR_BTNTEXT);
	m_buttonDown		= FALSE;
	m_hCursor			= NULL;
}

CSliderThumb::~CSliderThumb()
{
	if (hClipRgn) DeleteObject(hClipRgn);
}

BEGIN_MESSAGE_MAP(CSliderThumb, CButton)
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_KILLFOCUS()
	ON_WM_SETCURSOR()
END_MESSAGE_MAP()

BOOL CSliderThumb::OnEraseBkgnd(CDC* pDC) 
{
	return TRUE;
}

void CSliderThumb::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
    ASSERT (lpDrawItemStruct);
    POINT mouse_position;
    if ((m_buttonDown) && (::GetCapture() == m_hWnd) && (::GetCursorPos(&mouse_position)))
	{
		if (::WindowFromPoint(mouse_position) == m_hWnd)
		{
			if ((GetState() & BST_PUSHED) != BST_PUSHED) 
			{
				SetState(TRUE);
				return;
			}
		} 
		else 
		{
			if ((GetState() & BST_PUSHED) == BST_PUSHED) 
			{
				SetState(FALSE);
				return;
			}
		}
	}
	
	CString sCaption;
	TEXTMETRIC tm;

	CDC *pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
	RECT r=lpDrawItemStruct->rcItem;
	int cx = r.right  - r.left;
	int cy = r.bottom - r.top;

	GetWindowText(sCaption);
	pDC->SetBkMode(TRANSPARENT);
	pDC->SelectObject(GetStockObject(DEFAULT_GUI_FONT));
	pDC->GetTextMetrics(&tm);
	int tx=(cx-tm.tmAveCharWidth*sCaption.GetLength())/2;
	int ty=(cy-tm.tmHeight)/2;

	if (lpDrawItemStruct->itemState & ODS_DISABLED)
	{
		if(m_bmpDisabled.m_hObject==NULL)
			pDC->FillSolidRect(&r,GetSysColor(COLOR_BTNFACE));
		else
			DrawBitmap(pDC,(HBITMAP)m_bmpDisabled,r,m_DrawMode);
		if (m_bBorder) pDC->DrawEdge(&r,EDGE_RAISED,BF_RECT);
		pDC->SetTextColor(GetSysColor(COLOR_3DHILIGHT));
		pDC->ExtTextOut(tx+1,ty+1,ETO_CLIPPED,&r,sCaption,NULL);
		pDC->SetTextColor(GetSysColor(COLOR_GRAYTEXT));
		pDC->ExtTextOut(tx,ty,ETO_CLIPPED,&r,sCaption,NULL);
	} 
	else 
	{
		if (lpDrawItemStruct->itemState & ODS_SELECTED)
		{
			if(m_bmpDown.m_hObject==NULL)
				pDC->FillSolidRect(&r,GetSysColor(COLOR_BTNFACE));
            else 
				DrawBitmap(pDC,(HBITMAP)m_bmpDown,r,m_DrawMode);
			if (m_bBorder)
				pDC->DrawEdge(&r,EDGE_SUNKEN,BF_RECT);
		} 
		else 
		{
			if(m_bmpNormal.m_hObject==NULL)
				pDC->FillSolidRect(&r,GetSysColor(COLOR_BTNFACE));
			else
                if (m_bmpOver.m_hObject!=NULL)
					DrawBitmap(pDC,(HBITMAP)m_bmpOver,r,m_DrawMode);
				else
					DrawBitmap(pDC,(HBITMAP)m_bmpNormal,r,m_DrawMode);
			if (m_bBorder) 
				pDC->DrawEdge(&r,EDGE_RAISED,BF_RECT);
		}

		if ((lpDrawItemStruct->itemState & ODS_FOCUS)&&(m_FocusRectMargin>0))
		{
			r.left   += m_FocusRectMargin ;
			r.top    += m_FocusRectMargin ;
			r.right  -= m_FocusRectMargin ;
			r.bottom -= m_FocusRectMargin ;
			DrawFocusRect (lpDrawItemStruct->hDC, &r) ;
		}

		pDC->SetTextColor(m_TextColor);
		pDC->ExtTextOut(tx,ty,ETO_CLIPPED,&r,sCaption,NULL);
	}
}
int CSliderThumb::GetBitmapWidth (HBITMAP hBitmap)
{ BITMAP bm; GetObject(hBitmap,sizeof(BITMAP),(PSTR)&bm); return bm.bmWidth;}

int CSliderThumb::GetBitmapHeight (HBITMAP hBitmap)
{ BITMAP bm; GetObject(hBitmap,sizeof(BITMAP),(PSTR)&bm); return bm.bmHeight;}

void CSliderThumb::DrawBitmap(CDC* dc, HBITMAP hbmp, RECT r, int DrawMode)
{
	if(DrawMode==2)
	{
		FillWithBitmap(dc,hbmp,r);
		return;
	}
	if(!hbmp) return;

	int cx=r.right  - r.left;
	int cy=r.bottom - r.top;
	CDC dcBmp,dcMask;
	dcBmp.CreateCompatibleDC(dc);
	dcBmp.SelectObject(hbmp);
	
	if (m_bmpMask.m_hObject!=NULL)
	{
		dcMask.CreateCompatibleDC(dc);
		dcMask.SelectObject(m_bmpMask);

		CDC hdcMem;
		hdcMem.CreateCompatibleDC(dc);
	    CBitmap hBitmap;
		hBitmap.CreateCompatibleBitmap(dc,cx,cy);
	    hdcMem.SelectObject(hBitmap);
		
        hdcMem.BitBlt(r.left,r.top,cx,cy,dc,0,0,SRCCOPY);
		if(!DrawMode)
		{
			hdcMem.BitBlt(r.left,r.top,cx,cy,&dcBmp,0,0,SRCINVERT);
			hdcMem.BitBlt(r.left,r.top,cx,cy,&dcMask,0,0,SRCAND);
			hdcMem.BitBlt(r.left,r.top,cx,cy,&dcBmp,0,0,SRCINVERT);
		} 
		else 
		{
			int bx=GetBitmapWidth(hbmp);
			int by=GetBitmapHeight(hbmp);
			hdcMem.StretchBlt(r.left,r.top,cx,cy,&dcBmp,0,0,bx,by,SRCINVERT);
			hdcMem.StretchBlt(r.left,r.top,cx,cy,&dcMask,0,0,bx,by,SRCAND);
			hdcMem.StretchBlt(r.left,r.top,cx,cy,&dcBmp,0,0,bx,by,SRCINVERT);
		}
        dc->BitBlt(r.left,r.top,cx,cy,&hdcMem,0,0,SRCCOPY);

		hdcMem.DeleteDC();
		hBitmap.DeleteObject();

		DeleteDC(dcMask);
	} 
	else 
	{
		if(!DrawMode)
		{
			dc->BitBlt(r.left,r.top,cx,cy,&dcBmp,0,0,SRCCOPY);
		} 
		else 
		{
			int bx=GetBitmapWidth(hbmp);
			int by=GetBitmapHeight(hbmp);
			dc->StretchBlt(r.left,r.top,cx,cy,&dcBmp,0,0,bx,by,SRCCOPY);
		}
	}
	DeleteDC(dcBmp);
}

void CSliderThumb::FillWithBitmap(CDC* dc, HBITMAP hbmp, RECT r)
{
	if(!hbmp) return;
	CDC memdc;
	memdc.CreateCompatibleDC(dc);
	memdc.SelectObject(hbmp);
	int w = r.right - r.left;
	int	h = r.bottom - r.top;
	int x,y,z;
	int	bx=GetBitmapWidth(hbmp);
	int	by=GetBitmapHeight(hbmp);
	for (y = r.top ; y < h ; y += by)
	{
		if ((y+by)>h) by=h-y;
		z=bx;
		for (x = r.left ; x < w ; x += z)
		{
			if ((x+z)>w) z=w-x;
			dc->BitBlt(x, y, z, by, &memdc, 0, 0, SRCCOPY);
		}
	}
	DeleteDC(memdc);
}

void CSliderThumb::SetSkin(UINT normal,UINT down,UINT over,UINT disabled,UINT mask,
				  short drawmode, short border, UINT cursorID,short margin)
{
	m_bmpNormal.DeleteObject();
	m_bmpDown.DeleteObject();
	m_bmpOver.DeleteObject();
	m_bmpDisabled.DeleteObject();
	m_bmpMask.DeleteObject();

	if (normal>0) m_bmpNormal.LoadBitmap(normal);
	if (down>0)	  m_bmpDown.LoadBitmap(down);
	if (over>0)	  m_bmpOver.LoadBitmap(over);

	if (disabled>0) m_bmpDisabled.LoadBitmap(disabled);
	else if (normal>0) m_bmpDisabled.LoadBitmap(normal);

	m_DrawMode=max(0,min(drawmode,2));
	m_bBorder=border;
	m_FocusRectMargin=max(0,margin);
	
	if (mask>0)
	{
		m_bmpMask.LoadBitmap(mask);
		if (hClipRgn) DeleteObject(hClipRgn);
		hClipRgn = CreateRgnFromBitmap(m_bmpMask,RGB(255,255,255));
		if (hClipRgn)
		{
			SetWindowRgn(hClipRgn, TRUE);
			SelectClipRgn((HDC)GetDC(),hClipRgn);
		}
		if (m_DrawMode==0)
		{
			SetWindowPos(NULL,0,0,GetBitmapWidth(m_bmpMask),
						GetBitmapHeight(m_bmpMask),SWP_NOZORDER|SWP_NOMOVE);
		}
	}

	if(cursorID!=0)
	{
		SetBtnCursor(cursorID);
	}
}

HRGN CSliderThumb::CreateRgnFromBitmap(HBITMAP hBmp, COLORREF color)
{
	if (!hBmp) return NULL;

	BITMAP bm;
	GetObject( hBmp, sizeof(BITMAP), &bm );

	CDC dcBmp;
	dcBmp.CreateCompatibleDC(GetDC());
	dcBmp.SelectObject(hBmp);

	const DWORD RDHDR = sizeof(RGNDATAHEADER);
	const DWORD MAXBUF = 40;
	LPRECT	pRects;								
	DWORD	cBlocks = 0;
	INT		i, j;
	INT		first = 0;
	BOOL	wasfirst = FALSE;
	BOOL	ismask;
	
	RGNDATAHEADER* pRgnData = (RGNDATAHEADER*)new BYTE[ RDHDR + ++cBlocks * MAXBUF * sizeof(RECT) ];
	memset( pRgnData, 0, RDHDR + cBlocks * MAXBUF * sizeof(RECT) );

	pRgnData->dwSize	= RDHDR;
	pRgnData->iType		= RDH_RECTANGLES;
	pRgnData->nCount	= 0;
	for ( i = 0; i < bm.bmHeight; i++ )
	for ( j = 0; j < bm.bmWidth; j++ )
	{
		ismask=(dcBmp.GetPixel(j,bm.bmHeight-i-1)!=color);

		if ( wasfirst && (ismask ^ (j < bm.bmWidth - 1)) )
		{
			pRects = (LPRECT)((LPBYTE)pRgnData + RDHDR);
			pRects[ pRgnData->nCount++ ] = CRect( first, bm.bmHeight - i - 1, j, bm.bmHeight - i );
			if ( pRgnData->nCount >= cBlocks * MAXBUF )
			{
				LPBYTE pRgnDataNew = new BYTE[ RDHDR + ++cBlocks * MAXBUF * sizeof(RECT) ];
				memcpy( pRgnDataNew, pRgnData, RDHDR + (cBlocks - 1) * MAXBUF * sizeof(RECT) );
				delete pRgnData;
				pRgnData = (RGNDATAHEADER*)pRgnDataNew;
			}
			wasfirst = FALSE;
		} 
		else if ( !wasfirst && ismask )
		{
			first = j;
			wasfirst = TRUE;
		}
	}
	DeleteObject(dcBmp);
	HRGN hRgn=CreateRectRgn(0, 0, 0, 0);
	ASSERT( hRgn!=NULL );
	pRects = (LPRECT)((LPBYTE)pRgnData + RDHDR);
	for(i=0;i<(int)pRgnData->nCount;i++)
	{
		HRGN hr=CreateRectRgn(pRects[i].left, pRects[i].top, pRects[i].right, pRects[i].bottom);
		VERIFY(CombineRgn(hRgn, hRgn, hr, RGN_OR)!=ERROR);
	}
	ASSERT( hRgn!=NULL );

	delete pRgnData;
	return hRgn;
}

COLORREF CSliderThumb::SetTextColor(COLORREF new_color)
{
	COLORREF tmp_color=m_TextColor;
	m_TextColor=new_color;
	return tmp_color;
}


void CSliderThumb::OnLButtonDblClk(UINT flags, CPoint point) 
{
    SendMessage(WM_LBUTTONDOWN, flags, MAKELPARAM(point.x, point.y));
}

void CSliderThumb::OnLButtonDown(UINT nFlags, CPoint point)
{
	m_point = point;
    m_buttonDown = TRUE;
	CButton::OnLButtonDown(nFlags, point);
}

void CSliderThumb::OnLButtonUp(UINT nFlags, CPoint point)
{
    m_buttonDown = FALSE;
	((CSliderBase*)GetParent())->SetFilePtrOffset();

	CButton::OnLButtonUp(nFlags, point);
}

void CSliderThumb::OnMouseMove(UINT nFlags, CPoint point)
{
    if ((m_buttonDown) && (::GetCapture() == m_hWnd)) 
	{
	    POINT p2 = point;
        ::ClientToScreen(m_hWnd, &p2);
        HWND mouse_wnd = ::WindowFromPoint(p2);

        BOOL pressed = ((GetState() & BST_PUSHED) == BST_PUSHED);
        BOOL need_pressed = (mouse_wnd == m_hWnd);
        if (pressed != need_pressed) 
		{
            SetState(need_pressed ? TRUE : FALSE);
            Invalidate();
        }
		MoveThumb( nFlags, point);
    } 

    CButton::OnMouseMove(nFlags, point);
}

void CSliderThumb::OnKillFocus(CWnd *new_wnd)
{
    if (::GetCapture() == m_hWnd) 
	{
        ::ReleaseCapture();
        m_buttonDown = FALSE;
    }
    CButton::OnKillFocus(new_wnd);
}


BOOL CSliderThumb::OnClicked() 
{
	if (::GetCapture() == m_hWnd) 
	{
        ::ReleaseCapture();
    }
    m_buttonDown = FALSE;
    return FALSE;
}


BOOL CSliderThumb::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	if (m_hCursor != NULL)
	{
		::SetCursor(m_hCursor);
		return TRUE;
	}
	return CButton::OnSetCursor(pWnd, nHitTest, message);
}

BOOL CSliderThumb::SetBtnCursor(UINT cursorID)
{
	HINSTANCE hInstResource;
	if (m_hCursor != NULL) ::DestroyCursor(m_hCursor);
	m_hCursor = NULL;

	if (cursorID != 0)
	{
		hInstResource = AfxFindResourceHandle(MAKEINTRESOURCE(cursorID),
											RT_GROUP_CURSOR);
		m_hCursor = (HCURSOR)::LoadImage(hInstResource/*AfxGetApp()->m_hInstance*/, MAKEINTRESOURCE(cursorID), IMAGE_CURSOR, 0, 0, 0);

		if (m_hCursor == NULL) return FALSE;
	}
	return TRUE;
}


CSliderBase::CSliderBase()
{	
	m_nSecOfRecLen=0;
	m_nFileTotal=0;
	m_nSecOfPass=0;
	m_nFileoffset=0;
}

CSliderBase::~CSliderBase()
{
}


BEGIN_MESSAGE_MAP(CSliderBase, CWnd)
	ON_WM_CREATE()
	ON_WM_LBUTTONDOWN()
	ON_WM_PAINT()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()

int CSliderBase::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	CBitmap bmpTb;
	bmpTb.LoadBitmap(m_nThumbBmpUp);
	BITMAP bmp;
	bmpTb.GetBitmap(&bmp);

	CRect rect;

	GetWindowRect(&rect);
	int nHeight=rect.bottom-rect.top;

	m_btnThumb.Create(NULL,BS_OWNERDRAW|WS_CHILD|WS_VISIBLE,
		CRect(CPoint(0, 8),CSize(18,8)),this,ID_THUMB);

	m_btnThumb.SetSkin(m_nThumbBmpUp, m_nThumbBmpDown, 
		m_nThumbBmpUp, m_nThumbBmpBtn,
		NULL,1,0,0,0); 
	m_bSetPlayPos = FALSE;
	m_fPlayPos = 0.0;
	return 0;
}

void CSliderBase::OnLButtonDown(UINT nFlags, CPoint point) 
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

void CSliderBase::OnLButtonUp(UINT nFlags, CPoint point) 
{
	CRect rcBase,rcThumb; 
	GetWindowRect(&rcBase);
	m_btnThumb.GetWindowRect(&rcThumb);

	m_fPlayPos=float((rcThumb.left-rcBase.left))/
		float((rcBase.right-rcBase.left-(rcThumb.right-rcThumb.left)));
	m_bSetPlayPos = TRUE;
	CWnd::OnLButtonUp(nFlags, point);
}

void CSliderBase::OnPaint() 
{
	CPaintDC dc(this);
	
	CDC memDC;
	memDC.CreateCompatibleDC(&dc);
	CBitmap bk;
	bk.LoadBitmap(m_nBackBmp);
	BITMAP bmp;
	bk.GetBitmap( &bmp );
	CBitmap *pOldBmp=memDC.SelectObject( &bk );
	dc.BitBlt( 0 , 0 , bmp.bmWidth , bmp.bmHeight  , &memDC , 0 , 0 , SRCCOPY );
	memDC.SelectObject( pOldBmp );
}

BOOL CSliderBase::Create(const RECT& rect, CWnd* pParentWnd,UINT nBgBmp,UINT nTbBmpUp,UINT nTbBmpDown,UINT nWndID) 
{
	m_nBackBmp=nBgBmp;
	m_nThumbBmpUp=nTbBmpUp;
	m_nThumbBmpDown=nTbBmpDown;
	m_nThumbBmpBtn = nWndID;
	return CWnd::Create(NULL, NULL, WS_CHILD|WS_VISIBLE, rect, pParentWnd, nWndID);
}

void CSliderThumb::MoveThumb(UINT nFlags,CPoint point)
{
	CRect rcParent;
	GetParent()->GetWindowRect(rcParent);

	CRect rect;
	GetWindowRect(rect);
	int nHeight=rect.bottom-rect.top;
	int nWide=rect.right-rect.left;

	int oldx=rect.left-rcParent.left;
	int cenx=oldx+nWide/2;
	if(oldx<0 || oldx>(rcParent.right-rcParent.left))
	{
		return;
	}

	int newx=((point.x-m_point.x)>=0)?(oldx+(point.x-m_point.x)):(oldx-(m_point.x-point.x));
	if(newx<0)
	{
		newx=0;
	}
	else if(newx>(rcParent.right-rcParent.left-nWide))
	{
		newx=rcParent.right-rcParent.left-nWide;
	}

	MoveWindow(newx, 10,nWide,nHeight);
}

void CSliderBase::SetPos(int nNewPos)
{
	if(nNewPos<0 || nNewPos>1000)  { return; }
	CRect rcBase,rcThumb;
	GetWindowRect(&rcBase);
	m_btnThumb.GetWindowRect(&rcThumb);
	int nNewX=(rcBase.right-rcBase.left-(rcThumb.right-
		rcThumb.left))*nNewPos/1000;
	m_btnThumb.MoveWindow(nNewX,10, rcThumb.right-rcThumb.left,
		rcThumb.bottom-rcThumb.top);
}

int CSliderBase::GetPos()
{
	CRect rcBase,rcThumb;
	GetWindowRect(&rcBase);
	m_btnThumb.GetWindowRect(&rcThumb);
	float fPos=float((rcThumb.left-rcBase.left))/
		float((rcBase.right-rcBase.left-
		(rcThumb.right-rcThumb.left)))*1000; 

	return (int)fPos;
}


CTime CSliderBase::GetCurTime()
{
	return m_oCurTime;
}

void CSliderBase::SetFilePtrOffset()
{
	CRect rcBase,rcThumb; 
	GetWindowRect(&rcBase);
	m_btnThumb.GetWindowRect(&rcThumb);

	long double  fThumbOffset=float((rcThumb.left-rcBase.left))/
		float((rcBase.right-rcBase.left-(rcThumb.right-rcThumb.left)));

	m_fPlayPos=float((rcThumb.left-rcBase.left))/
		float((rcBase.right-rcBase.left-(rcThumb.right-rcThumb.left)));
	m_bSetPlayPos = TRUE;
}

void CSliderBase::ResetSlider()
{
	SetPos(0);
}
