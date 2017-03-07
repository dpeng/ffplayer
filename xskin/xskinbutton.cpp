#include "stdafx.h"
#include "xSkinButton.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BOOL CSkinButton::s_bIsMouseDown = FALSE;
int  CSkinButton::s_nGetBtnOrder = 0;

BOOL ExtractBitmap(CBitmap * pSource, CBitmap * pDest, CRect rcExtract);

CSkinButton::CSkinButton()
:m_bSkin(FALSE)
{
	m_DrawMode=1;
	m_FocusRectMargin=0;
	hClipRgn=NULL;
	m_TextColor=GetSysColor(COLOR_BTNTEXT);
	m_button_down = m_tracking = FALSE;
	m_hCursor=NULL;
	m_nSetBtnOrder = 0;
	m_bMouseDown = FALSE;

	m_bFlag = FALSE;
}

CSkinButton::~CSkinButton()
{
	if (hClipRgn) DeleteObject(hClipRgn);
}

BEGIN_MESSAGE_MAP(CSkinButton, CButton)
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_KILLFOCUS()
    ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
	ON_WM_SETCURSOR()
END_MESSAGE_MAP()

BOOL CSkinButton::OnEraseBkgnd(CDC* pDC) 
{
	return 1;
}


void CSkinButton::SetSkin(BOOL bType)
{
	m_bSkin = bType;
}

void CSkinButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
    ASSERT (lpDrawItemStruct);

	POINT mouse_position;
    if ((m_button_down) && (::GetCapture() == m_hWnd) && (::GetCursorPos(&mouse_position))){
		if (::WindowFromPoint(mouse_position) == m_hWnd){
			if ((GetState() & BST_PUSHED) != BST_PUSHED) {
				SetState(TRUE);
				return;
			}
		} else {
			if ((GetState() & BST_PUSHED) == BST_PUSHED) {
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

	if ( m_bSkin )
	{
		DrawBitmap(pDC,(HBITMAP)m_bmpDown,r,m_DrawMode);
	}
	else 
	{
		if (lpDrawItemStruct->itemState & ODS_DISABLED)
		{
			if(m_bmpDisabled.m_hObject==NULL)
			{
				pDC->FillSolidRect(&r,GetSysColor(COLOR_BTNFACE));
			}
			else
			{
				DrawBitmap(pDC,(HBITMAP)m_bmpDisabled,r,m_DrawMode);
			}
			if (m_bBorder) 
			{
				pDC->DrawEdge(&r,EDGE_RAISED,BF_RECT);
			}
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
				{
					pDC->FillSolidRect(&r,GetSysColor(COLOR_BTNFACE));
				}
				else 
				{
					DrawBitmap(pDC,(HBITMAP)m_bmpDown,r,m_DrawMode);
				}
				if (m_bBorder) 
				{
					pDC->DrawEdge(&r,EDGE_SUNKEN,BF_RECT);
				}
			}
			else 
			{
				if(m_bmpNormal.m_hObject==NULL)
				{
					pDC->FillSolidRect(&r,GetSysColor(COLOR_BTNFACE));
				}
				else 
				{
					if ((m_tracking || m_bMouseDown)&&(m_bmpOver.m_hObject!=NULL))
					{
						DrawBitmap(pDC,(HBITMAP)m_bmpOver,r,m_DrawMode);
					} 
					else
					{
						DrawBitmap(pDC,(HBITMAP)m_bmpNormal,r,m_DrawMode);
					}
				}
				if (m_bBorder)
				{
					pDC->DrawEdge(&r,EDGE_RAISED,BF_RECT);
				}
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
	DeleteObject(pDC);
}

int CSkinButton::GetBitmapWidth (HBITMAP hBitmap)
{ BITMAP bm; GetObject(hBitmap,sizeof(BITMAP),(PSTR)&bm); return bm.bmWidth;}

int CSkinButton::GetBitmapHeight (HBITMAP hBitmap)
{ BITMAP bm; GetObject(hBitmap,sizeof(BITMAP),(PSTR)&bm); return bm.bmHeight;}

void CSkinButton::DrawBitmap(CDC* dc, HBITMAP hbmp, RECT r, int DrawMode)
{

	if(DrawMode==2)
	{
		FillWithBitmap(dc,hbmp,r);
		return;
	}
	if(!hbmp) 
		return;

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
	if(dcBmp)
		DeleteDC(dcBmp);
}

void CSkinButton::FillWithBitmap(CDC* dc, HBITMAP hbmp, RECT r)
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

void CSkinButton::SetSkin(UINT normal,UINT down,UINT over,UINT disabled,UINT mask,
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

void CSkinButton::SetSkin(CBitmap* normal,CBitmap* down, CBitmap* over, 
		CBitmap* disabled,UINT mask,short drawmode,
		short border,UINT cursorID,short margin)
{
	m_mutex.Lock();

	m_bmpNormal.DeleteObject();
	m_bmpDown.DeleteObject();
	m_bmpOver.DeleteObject();
	m_bmpDisabled.DeleteObject();
	m_bmpMask.DeleteObject();	

	BITMAP bmp;
	if (normal) 
	{
		normal->GetBitmap(&bmp);
		ExtractBitmap(normal,&m_bmpNormal,CRect(0,0,bmp.bmWidth,bmp.bmHeight));
	}
	if (down)	  
	{
		down->GetBitmap(&bmp);
		ExtractBitmap(down,&m_bmpDown,CRect(0,0,bmp.bmWidth,bmp.bmHeight));
	}
	if (over)	  
	{
		over->GetBitmap(&bmp);
		ExtractBitmap(over,&m_bmpOver,CRect(0,0,bmp.bmWidth,bmp.bmHeight));
	}

	if (disabled) 
	{
		disabled->GetBitmap(&bmp);
		ExtractBitmap(disabled,&m_bmpDisabled,CRect(0,0,bmp.bmWidth,bmp.bmHeight));
	}
	else if (normal) 
	{
		normal->GetBitmap(&bmp);
		ExtractBitmap(normal,&m_bmpDisabled,CRect(0,0,bmp.bmWidth,bmp.bmHeight));
	}

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
	m_mutex.Unlock();

}


HRGN CSkinButton::CreateRgnFromBitmap(HBITMAP hBmp, COLORREF color)
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

COLORREF CSkinButton::SetTextColor(COLORREF new_color)
{
	COLORREF tmp_color=m_TextColor;
	m_TextColor=new_color;
	return tmp_color;
}

void CSkinButton::SetToolTipText(CString s)
{
	if(m_tooltip.m_hWnd==NULL)
	{
		if(m_tooltip.Create(this))
			if(m_tooltip.AddTool(this, (LPCTSTR)s))
				m_tooltip.Activate(1);
	} 
	else 
	{
		m_tooltip.UpdateTipText((LPCTSTR)s,this);
	}
}

void CSkinButton::RelayEvent(UINT message, WPARAM wParam, LPARAM lParam)
{
	if(NULL != m_tooltip.m_hWnd)
	{
		MSG msg;
		msg.hwnd = m_hWnd;
		msg.message = message;
		msg.wParam = wParam;
		msg.lParam = lParam;
		msg.time = 0;
		msg.pt.x = LOWORD(lParam);
		msg.pt.y = HIWORD(lParam);

		m_tooltip.RelayEvent(&msg);
	}
}


void CSkinButton::OnLButtonDblClk(UINT flags, CPoint point) 
{
    SendMessage(WM_LBUTTONDOWN, flags, MAKELPARAM(point.x, point.y));
}

void CSkinButton::OnLButtonDown(UINT nFlags, CPoint point)
{

	s_bIsMouseDown = TRUE;
	s_nGetBtnOrder = m_nSetBtnOrder;	

	RelayEvent(WM_LBUTTONDOWN,(WPARAM)nFlags,MAKELPARAM(LOWORD(point.x),LOWORD(point.y)));

    if (m_tracking) 
	{
        TRACKMOUSEEVENT t = 
		{
            sizeof(TRACKMOUSEEVENT),
            TME_CANCEL | TME_LEAVE,
            m_hWnd,
            0
        };
        if (::_TrackMouseEvent(&t)) 
		{
            m_tracking = FALSE;
        }
    }

    m_button_down = TRUE;
	CButton::OnLButtonDown(nFlags, point);
}


void CSkinButton::OnLButtonUp(UINT nFlags, CPoint point)
{
	s_bIsMouseDown = FALSE;

	RelayEvent(WM_LBUTTONUP,(WPARAM)nFlags,MAKELPARAM(LOWORD(point.x),LOWORD(point.y)));
    m_button_down = FALSE;
	CButton::OnLButtonUp(nFlags, point);
}


void CSkinButton::OnMouseMove(UINT nFlags, CPoint point)
{

	RelayEvent(WM_MOUSEMOVE,(WPARAM)nFlags,MAKELPARAM(LOWORD(point.x),LOWORD(point.y)));

    if ((m_button_down) && (::GetCapture() == m_hWnd)) 
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
    } 
	else 
	{
        if (!m_tracking) 
		{
            TRACKMOUSEEVENT t = 
			{
                sizeof(TRACKMOUSEEVENT),
                TME_LEAVE,
                m_hWnd,
                0
            };
            if (::_TrackMouseEvent(&t)) 
			{
                m_tracking = TRUE;
                Invalidate();
            }
        }
    }

    CButton::OnMouseMove(nFlags, point);
}

LRESULT CSkinButton::OnMouseLeave(WPARAM, LPARAM)
{
    ASSERT (m_tracking);
    m_tracking = FALSE;
    Invalidate();
	return 0;
}

void CSkinButton::OnKillFocus(CWnd *new_wnd)
{
    if (::GetCapture() == m_hWnd) 
	{
        ::ReleaseCapture();
        ASSERT (!m_tracking);
        m_button_down = FALSE;
    }
    CButton::OnKillFocus(new_wnd);
}


BOOL CSkinButton::OnClicked() 
{
    if (::GetCapture() == m_hWnd)
	{
        ::ReleaseCapture();
        ASSERT (!m_tracking);
    }
    m_button_down = FALSE;
    return FALSE;
}


BOOL CSkinButton::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	if (m_hCursor != NULL)
	{
		::SetCursor(m_hCursor);
		return TRUE;
	}
	return CButton::OnSetCursor(pWnd, nHitTest, message);
}

BOOL CSkinButton::SetBtnCursor(UINT cursorID)
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

BOOL ExtractBitmap(CBitmap * pSource, CBitmap * pDest, CRect rcExtract)
{
	HWND hWnd = ::GetDesktopWindow();
	if(!pSource || !pSource->m_hObject || !hWnd || !pDest)  
	{
		return FALSE;
	}

	HDC hDC = ::GetDC(hWnd);
	CDC dc;
	dc.Attach(hDC);

	CDC memDC,memDC2;
	memDC.CreateCompatibleDC(&dc);
	memDC2.CreateCompatibleDC(&dc);
	pDest->Detach();
	pDest->CreateCompatibleBitmap(&dc,rcExtract.Width(),rcExtract.Height());
	CBitmap * pOld=memDC.SelectObject(pDest);
	CBitmap * pOld2=memDC2.SelectObject(pSource);

	memDC.BitBlt(0,0,rcExtract.Width(),rcExtract.Height(),&memDC2,
		rcExtract.left,rcExtract.top,SRCCOPY);

	memDC.SelectObject(pOld);
	memDC2.SelectObject(pOld2);

	return TRUE;
}

void  CSkinButton::SizeToContent() 
{ 
    ASSERT(m_bmpNormal.m_hObject != NULL); 
    CSize bitmapSize; 
    BITMAP bmInfo; 
    VERIFY(m_bmpNormal.GetObject(sizeof (bmInfo), &bmInfo) == sizeof (bmInfo)); 
    VERIFY(SetWindowPos(NULL, -1 , -1 , bmInfo.bmWidth, bmInfo.bmHeight, 
        SWP_NOMOVE|SWP_NOZORDER|SWP_NOREDRAW|SWP_NOACTIVATE)); 
} 