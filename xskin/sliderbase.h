#if !defined(_X_SLIDER_BASE_H_)
#define _X_SLIDER_BASE_H_

#if _MSC_VER > 1000
#pragma once
#endif
#define ID_THUMB                WM_USER+2500

class CSliderThumb : public CButton
{
public:
	CSliderThumb();
	virtual ~CSliderThumb();
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	COLORREF SetTextColor(COLORREF new_color);
	void MoveThumb(UINT nFlags,CPoint point);
	void SetSkin(UINT normal,UINT down, UINT over=NULL, 
	UINT disabled=NULL,UINT mask=NULL,short drawmode=1,
	short border=1,UINT cursorID=0,short margin=4);

protected:
	HCURSOR m_hCursor;
	BOOL SetBtnCursor(UINT cursorID);

	CPoint m_point;
	BOOL m_buttonDown;
	CBitmap m_bmpNormal,m_bmpDown,m_bmpDisabled,m_bmpMask,m_bmpOver;
	short	m_FocusRectMargin;
	COLORREF m_TextColor;
	HRGN	hClipRgn;
	BOOL	m_bBorder;
	short	m_DrawMode;
	HRGN	CreateRgnFromBitmap(HBITMAP hBmp, COLORREF color);
	void	FillWithBitmap(CDC* dc, HBITMAP hbmp, RECT r);
	void	DrawBitmap(CDC* dc, HBITMAP hbmp, RECT r, int DrawMode);
	int		GetBitmapWidth (HBITMAP hBitmap);
	int		GetBitmapHeight (HBITMAP hBitmap);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg BOOL OnClicked();
	DECLARE_MESSAGE_MAP()
};

class CSliderBase : public CWnd
{
public:
	CSliderBase();
	BOOL m_bIsLocal;
	CString strCurTime;
	void SetFilePtrOffset();
	CTime GetCurTime();
	void SetPos(int nNewPos);
	int GetPos();
	void ResetSlider();
	virtual ~CSliderBase();	
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	CSliderThumb m_btnThumb;
	BOOL		 m_bSetPlayPos;
	float		 m_fPlayPos;	
	CTime        m_oCurTime;
	virtual BOOL Create(const RECT& rect, CWnd* pParentWnd,UINT nBgBmp,UINT nTbBmpUp,UINT nTbBmpDown,UINT nWndID);

private:
	UINT m_nBackBmp;
	UINT m_nThumbBmpUp;
	UINT m_nThumbBmpDown;
	UINT m_nThumbBmpBtn;
	int m_nSecOfRecLen;
	DWORD m_nFileTotal;
	int m_nSecOfPass;
	int m_nFileoffset;
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()
};

#endif