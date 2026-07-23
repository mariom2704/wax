// StretchBitmapControl.cpp: Implementierungsdatei
//

#include "stdafx.h"
#include "StretchBitmapControl.h"


// CStretchBitmapControl

IMPLEMENT_DYNAMIC(CStretchBitmapControl, CWnd)

CStretchBitmapControl::CStretchBitmapControl(UINT BitmapID)
{
	m_BitmapID = BitmapID;
}

CStretchBitmapControl::~CStretchBitmapControl()
{
}


BEGIN_MESSAGE_MAP(CStretchBitmapControl, CWnd)
	ON_WM_PAINT()
END_MESSAGE_MAP()



void CStretchBitmapControl::OnDraw(CDC* pDC)
{
};

void CStretchBitmapControl::OnPaint()
{ 

	CPaintDC dc(this);

	CRect rect;

	GetClientRect(&rect);

	CBrush mm(RGB(255,255,255));
	dc.FillRect(&rect, &mm);

	CBitmap bmp;
	if (bmp.LoadBitmap(m_BitmapID))
	{
		BITMAP bmpInfo;
		bmp.GetBitmap(&bmpInfo);
	
		CDC dcMemory;
		dcMemory.CreateCompatibleDC(&dc);

		CBitmap* pOldBitmap = dcMemory.SelectObject(&bmp);

		CRect rect;
		GetClientRect(&rect);
		int nX = rect.Width();
		int nY = rect.Height();
		
		dc.StretchBlt(50, 0, nX-100, nY, &dcMemory, 0,0,bmpInfo.bmWidth, bmpInfo.bmHeight, 
			 SRCCOPY);
		dcMemory.SelectObject(pOldBitmap);
	}
}



