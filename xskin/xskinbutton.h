#if !defined(_X_SKIN_BUTTON_H_)
#define _X_SKIN_BUTTON_H_

#if _MSC_VER > 1000
#pragma once
#endif 

#include <afxmt.h>
class CSkinButton : public CButton
{
public:
	CSkinButton();
	virtual ~CSkinButton();	
	void			SetFlag( BOOL bFlag ){ m_bFlag = bFlag;}
	virtual void	DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	void			SetToolTipText(CString s);
	COLORREF		SetTextColor(COLORREF new_color);
	void			SetSkin(UINT normal,UINT down, UINT over=NULL, 
						UINT disabled=NULL,UINT mask=NULL,short drawmode=1,
						short border=1,UINT cursorID=0,short margin=4);
	void			SetSkin(CBitmap* normal,CBitmap* down, CBitmap* over=NULL, 
						CBitmap* disabled=NULL,UINT mask=NULL,short drawmode=1,
						short border=1,UINT cursorID=0,short margin=4);
	void			SetSkin(BOOL bType);
	void			SizeToContent();
	static BOOL	s_bIsMouseDown; 
	static int  s_nGetBtnOrder;
	int			m_nSetBtnOrder;
	BOOL		m_bMouseDown;

protected:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg LRESULT OnMouseLeave(WPARAM, LPARAM);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg BOOL OnClicked();
	DECLARE_MESSAGE_MAP()
	BOOL	SetBtnCursor(UINT cursorID);
	void	RelayEvent(UINT message, WPARAM wParam, LPARAM lParam);
	HRGN	CreateRgnFromBitmap(HBITMAP hBmp, COLORREF color);
	void	FillWithBitmap(CDC* dc, HBITMAP hbmp, RECT r);
	void	DrawBitmap(CDC* dc, HBITMAP hbmp, RECT r, int DrawMode);
	int		GetBitmapWidth (HBITMAP hBitmap);
	int		GetBitmapHeight (HBITMAP hBitmap);
	HCURSOR		m_hCursor;
	BOOL		m_bFlag;	
	CMutex		m_mutex;
	BOOL		m_bSkin;
    BOOL		m_tracking;
    BOOL		m_button_down;
	CToolTipCtrl m_tooltip;
	CBitmap		m_bmpNormal,m_bmpDown,m_bmpDisabled,m_bmpMask,m_bmpOver;
	short		m_FocusRectMargin;
	COLORREF	m_TextColor;
	HRGN		hClipRgn;
	BOOL		m_bBorder;
	short		m_DrawMode;
};
#endif