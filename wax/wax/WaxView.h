#pragma once

#include "WaxDoc.h"

class WaxSplitterWnd;
class CChildFrame;
class CWaxView : public CView
{
	DECLARE_DYNCREATE(CWaxView)
	DECLARE_MESSAGE_MAP()

	CChildFrame* m_pMainSplitterFrame;

public: // Nur aus Serialisierung erzeugen
	CWaxView() : CView() {};

	void SetMainSplitter(CChildFrame* pMainSplitterFrame)
	{
		 m_pMainSplitterFrame = pMainSplitterFrame;
	};

// Attribute
public:
	CWaxDoc* GetDocument();

// Operationen
public:
	

	virtual void OnDraw(CDC* pDC){UNREFERENCED_PARAMETER(pDC);}
	virtual ~CWaxView(){};

protected:
	
};

inline CWaxDoc* CWaxView::GetDocument()
   { return (CWaxDoc*)m_pDocument; }

