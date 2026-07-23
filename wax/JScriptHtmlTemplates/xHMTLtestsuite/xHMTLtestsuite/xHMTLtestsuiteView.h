
// xHMTLtestsuiteView.h: Schnittstelle der Klasse CxHMTLtestsuiteView
//


#pragma once


class CxHMTLtestsuiteView : public CHtmlView
{
protected: // Nur aus Serialisierung erstellen
	CxHMTLtestsuiteView();
	DECLARE_DYNCREATE(CxHMTLtestsuiteView)

// Attribute
public:
	CxHMTLtestsuiteDoc* GetDocument() const;

// Vorgänge
public:

// Überschreibungen
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void OnInitialUpdate(); // Erster Aufruf nach Erstellung

// Implementierung
public:
	virtual ~CxHMTLtestsuiteView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generierte Funktionen für die Meldungstabellen
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // Debugversion in xHMTLtestsuiteView.cpp
inline CxHMTLtestsuiteDoc* CxHMTLtestsuiteView::GetDocument() const
   { return reinterpret_cast<CxHMTLtestsuiteDoc*>(m_pDocument); }
#endif

