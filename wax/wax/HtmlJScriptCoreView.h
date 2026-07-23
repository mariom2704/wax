#pragma once

#include "MehrfachdruckDlg.h"


#ifdef _WIN32_WCE
#error "CHtmlView wird für Windows CE nicht unterstützt."
#endif 

// CHtmlJScriptCoreView HTML-Ansicht

class CHtmlJScriptCoreView : public CHtmlView
{
	DECLARE_DYNCREATE(CHtmlJScriptCoreView)

		

protected:
	CString m_strFunc, m_strArg;
	CHtmlJScriptCoreView(); 
	CChildFrame* m_pMainSplitterFrame;
	virtual ~CHtmlJScriptCoreView();
	MehrfachdruckDlg mehrfachdruckDlg;
	bool m_bShowPrintDialog;
	int iDialogHeight;
	bool m_bDrawingOnly;
	virtual void DocumentComplete(LPDISPATCH pDisp, VARIANT* URL);
	CComPtr<IHTMLDocument2> m_spDoc;

public:
	virtual bool ExecuteJavaScriptFunction(const CString strFunc, CStringArray& paramArray,CComVariant* pVarResult = NULL);
	virtual void onUpdatePrintpreview(CCmdUI* pCmdUI);
	virtual void onPrintpreview();
	void SetShowPrintDialog(bool bShowDialog){m_bShowPrintDialog = bShowDialog;};
	bool GetShowPrintDialog(){return m_bShowPrintDialog;};
	void SetDrawingOnly(bool bDrawingOnly){m_bDrawingOnly = bDrawingOnly;};
		/// print document
	virtual bool print(int anzahl = 1);
	const HANDLE printWord();
	void SetMainSplitter(CChildFrame* pMainSplitterFrame)
	{
		m_pMainSplitterFrame = pMainSplitterFrame;
	}

	CChildFrame* GetMainSplitter()
	{
		return m_pMainSplitterFrame;
	}

protected:
	virtual void DoDataExchange(CDataExchange* pDX);

	DECLARE_MESSAGE_MAP()
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnSize(UINT nType, int cx, int cy);

};


