
#include "stdafx.h"
#include "wax.h"
#include "resource.h"
#include "..\waxprj\drucklistenverwaltung.h"
#include "WaxDoc.h"


			#include "JScriptViewTemplate.h"




IMPLEMENT_DYNCREATE(CJScriptViewTemplate, CHtmlJScriptCoreView)



BEGIN_MESSAGE_MAP(CJScriptViewTemplate, CHtmlJScriptCoreView)
END_MESSAGE_MAP()



void CJScriptViewTemplate::OnUpdate(CView* pSender, LPARAM /*lHint*/, CObject* /*pHint*/)
{
	if(!m_strFunc.IsEmpty())
	{
		CStringArray aPath;
		ExecuteJavaScriptFunction(CString(), aPath);
	}
}



bool CJScriptViewTemplate::ExecuteJavaScriptFunction(const CString strlocalFunc, CStringArray& paramArray,CComVariant* pVarResult)
{
	// Early disable Window of the browser
	// so no WM_BUTTON or WM_KEY messages will be 
	// forwarded for making possible problems with the DWF Viewer
	m_wndBrowser.EnableWindow(FALSE);
	return CHtmlJScriptCoreView::ExecuteJavaScriptFunction(strlocalFunc, paramArray, pVarResult);
}



bool  CJScriptViewTemplate::createScriptHeaderAndGrafic(CString& strHeader, CString& strGrafic, CString& strShowRows)
{
	WaxPart* pt = ((CWaxDoc*)GetDocument())->get_active_object();
	XASSERT(pt);
	if (!pt)
		return "";
	
	if (pt->IsKindOf(RUNTIME_CLASS(SammelAuftrag)))
	{
		if (!WaxSys::isAV())
			dynamic_cast<SammelAuftrag*>(pt)->onCreateAutoarbeit();
	}
	

	WaxDruck::DrucklistenVerwaltung druckliste(pt, true);
	druckliste.makeAusgabe();
	vector<CString> strHeaderArray;
	strHeaderArray = druckliste.JScriptVisualisationAndPrinting.GetHeadercontent();
	for(UINT i=0; i< strHeaderArray.size(); i++)
	{
		strHeader += strHeaderArray[i];
	}
	strGrafic   = druckliste.JScriptVisualisationAndPrinting.GetGraficLinks();
	strShowRows = druckliste.JScriptVisualisationAndPrinting.GetShowRows();

	return true;
}



bool  CJScriptViewTemplate::createScriptHeaderAndGrafic(WaxPart* pt, CString& strHeader, CString& strGrafic, CString& strShowRows)
{
	
	XASSERT(pt);
	if (!pt)
		return "";
	
	if (pt->IsKindOf(RUNTIME_CLASS(SammelAuftrag)))
	{
		if (!WaxSys::isAV())
			dynamic_cast<SammelAuftrag*>(pt)->onCreateAutoarbeit();
	}
	

	WaxDruck::DrucklistenVerwaltung druckliste(pt, true);
	druckliste.makeAusgabe();
	vector<CString> strHeaderArray;
	strHeaderArray = druckliste.JScriptVisualisationAndPrinting.GetHeadercontent();
	for(UINT i=0; i< strHeaderArray.size(); i++)
	{
		strHeader += strHeaderArray[i];
	}
	strGrafic   = druckliste.JScriptVisualisationAndPrinting.GetGraficLinks();
	strShowRows = druckliste.JScriptVisualisationAndPrinting.GetShowRows();

	return true;
}





BOOL CJScriptViewTemplate::PreTranslateMessage(MSG* pMsg)
{

	POINTS pts = MAKEPOINTS( pMsg->lParam );
	POINT  point;
	point.x = pts.x;
	point.y = pts.y;

	CRect rect;
	GetClientRect(rect);
	int nScrollerWidth = ::GetSystemMetrics(SM_CYVSCROLL);
	int iBrowserwidth = rect.right - nScrollerWidth;

	if(point.x < iBrowserwidth && (pMsg->message == WM_RBUTTONDOWN ||pMsg->message == WM_LBUTTONDOWN || pMsg->message == WM_MOUSEMOVE))
	{
		::EnableWindow(m_wndBrowser.GetControlSite()->m_hWnd, FALSE);
	}

	if(point.x > iBrowserwidth)
	{
		::EnableWindow(m_wndBrowser.GetControlSite()->m_hWnd, TRUE);
	}

	return CHtmlJScriptCoreView::PreTranslateMessage(pMsg);
}
