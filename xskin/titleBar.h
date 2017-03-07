#if !defined(_X_TITLE_BAR_H_)
#define _X_TITLE_BAR_H_

#if _MSC_VER > 1000
#pragma once
#endif

class CTitleBase : public CWnd
{
public:
	CTitleBase();
	virtual ~CTitleBase();
	virtual BOOL Create(const RECT& rect, CWnd* pParentWnd,UINT nBgBmp,UINT nTbBmpUp,UINT nTbBmpDown,UINT nWndID);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	UINT m_nBackBmp;
	void SetLabel(CString label);

protected:
	//{{AFX_MSG(CTitleBase)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnPaint();

	COLORREF m_clfLabel;
	CString m_strLabel;
	int m_nLabelHeight;
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif // !defined(_X_TITLE_BAR_H_)
