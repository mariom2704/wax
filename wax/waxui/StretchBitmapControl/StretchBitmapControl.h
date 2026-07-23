#pragma once


// CStretchBitmapControl

class CStretchBitmapControl : public CWnd
{
	DECLARE_DYNAMIC(CStretchBitmapControl)

	public:
		CStretchBitmapControl(UINT BitmapID );
		virtual ~CStretchBitmapControl();
		virtual void OnDraw(CDC* pDC);     

	protected:
		DECLARE_MESSAGE_MAP()
		afx_msg void OnPaint();

	private:
		UINT m_BitmapID;

};


