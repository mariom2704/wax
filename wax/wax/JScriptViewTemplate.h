#pragma once

#include "HtmlJScriptCoreView.h"
#include "MehrfachdruckDlg.h"


class WaxSplitterWnd;
class CChildFrame;
class CJScriptViewTemplate : public CHtmlJScriptCoreView
{
	DECLARE_DYNCREATE(CJScriptViewTemplate)
	
	

	protected:
		
		virtual void OnUpdate(CView* pSender, LPARAM, CObject*);
		

		

	public:
		virtual ~CJScriptViewTemplate(){};
		
		virtual bool ExecuteJavaScriptFunction(const CString strFunc, CStringArray& paramArray,CComVariant* pVarResult = NULL);
	
		bool createScriptHeaderAndGrafic(CString& strHeader, CString& strGrafic, CString& strShowRows);
		bool createScriptHeaderAndGrafic(WaxPart* pt, CString& strHeader, CString& strGrafic, CString& strShowRows);

		DECLARE_MESSAGE_MAP()
		virtual BOOL PreTranslateMessage(MSG* pMsg);
	
};
