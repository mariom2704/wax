// HtmlJScriptCoreView.cpp: Implementierungsdatei
//

#include "stdafx.h"
#include "wax.h"
#include "resource.h"
#include "WaxDoc.h"

		#include "HtmlJScriptCoreView.h"





IMPLEMENT_DYNCREATE(CHtmlJScriptCoreView, CHtmlView)

BEGIN_MESSAGE_MAP(CHtmlJScriptCoreView, CHtmlView)
	ON_WM_CREATE()
	ON_WM_SHOWWINDOW()
	ON_WM_SIZE()
END_MESSAGE_MAP()


CHtmlJScriptCoreView::CHtmlJScriptCoreView():m_bDrawingOnly(false), m_bShowPrintDialog(false)
{}


int CHtmlJScriptCoreView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CHtmlView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	mehrfachdruckDlg.Create(IDD_MEHRFACHDRUCK, this);	
	CRect rectDruck;
	mehrfachdruckDlg.GetClientRect(rectDruck);
	iDialogHeight = rectDruck.Height();
	//SetSilent(TRUE);
	return 0;
}



void CHtmlJScriptCoreView::DocumentComplete(LPDISPATCH pDisp, VARIANT* URL)
{
	m_spDoc = NULL;
	LPDISPATCH spDisp = GetHtmlDocument();
	if(spDisp)
	{
		HRESULT hr = spDisp->QueryInterface(IID_IHTMLDocument2,(void**)&m_spDoc);
		if(FAILED(hr))
		{
			ASSERT(FALSE);
		}
	}
	return;
}

void CHtmlJScriptCoreView::onPrintpreview()
{
	CWaitCursor w;
	LPOLECOMMANDTARGET lpTarget = NULL;
	LPDISPATCH lpDisp = GetHtmlDocument();

	if (lpDisp != NULL)
	{
		// the control will handle all printing UI

		if (SUCCEEDED(lpDisp->QueryInterface(IID_IOleCommandTarget,
				(LPVOID*) &lpTarget)))
		{
			lpTarget->Exec(NULL, OLECMDID_PRINTPREVIEW, 0, NULL, NULL);
			lpTarget->Release();
		}

		lpDisp->Release();
	}
}


void CHtmlJScriptCoreView::onUpdatePrintpreview(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(true);
}

CHtmlJScriptCoreView::~CHtmlJScriptCoreView()
{}


void CHtmlJScriptCoreView::DoDataExchange(CDataExchange* pDX)
{
	CHtmlView::DoDataExchange(pDX);
}





const HANDLE CHtmlJScriptCoreView::printWord()
{
		
	//printWord(filelist.GetAt(0));
	return 0;
}



bool CHtmlJScriptCoreView::print(int iPrintWidthScroll)
{
	CStringArray aPath;
	aPath.Add(_T(""));
	if(iPrintWidthScroll == TRUE)
	{
		// For extending the grafic on the right size without that the size of the svg grafic will be narrow
		// simulate scrolling from the user
		ExecuteJavaScriptFunction("scrollForPrint", aPath);
	}

	// HTML-Document direkt drucken
	LPDISPATCH lpDispatch = NULL;
	
	lpDispatch = GetHtmlDocument(); 
	XASSERT(lpDispatch);

	if (lpDispatch == NULL)
		return false;

	// Print ueber IOleCommandTarget		
	LPOLECOMMANDTARGET lpOleCommandTarget = NULL;
	lpDispatch->QueryInterface(IID_IOleCommandTarget, (void**)&lpOleCommandTarget);
	XASSERT(lpOleCommandTarget);

	if (lpOleCommandTarget == NULL)
		return false;

	lpOleCommandTarget->Exec(NULL, OLECMDID_PRINT, OLECMDEXECOPT_DONTPROMPTUSER, NULL, NULL);

	lpOleCommandTarget->Release();
	lpDispatch->Release();
	return true;
}






bool CHtmlJScriptCoreView::ExecuteJavaScriptFunction(const CString strlocalFunc, CStringArray& paramArray,CComVariant* pVarResult)
{
	CComPtr<IDispatch> spScript;

	HRESULT hr = m_spDoc->get_Script(&spScript);
	
	if(hr!= S_OK)
	{
		return false;
	}

	CString strFunc;
	
	if(!strlocalFunc.IsEmpty())
	{
		m_strFunc = strFunc = strlocalFunc;
		m_strArg  = paramArray.GetAt(0);
	}
	else
	{
		strFunc = m_strFunc;
	}
	CComBSTR bstrMember(strFunc);
	DISPID dispid = NULL;
	hr = spScript->GetIDsOfNames(IID_NULL,&bstrMember,1,
											LOCALE_SYSTEM_DEFAULT,&dispid);
	if(FAILED(hr))
	{
		return false;
	}

	int arraySize = paramArray.GetSize();

	if(arraySize == 0)
	{
		paramArray.Add(m_strArg);
	}

	arraySize = paramArray.GetSize();

	DISPPARAMS dispparams;
	memset(&dispparams, 0, sizeof dispparams);
	dispparams.cArgs = arraySize;
	dispparams.rgvarg = new VARIANT[dispparams.cArgs];
	
	for( int i = 0; i < arraySize; i++)
	{
		CComBSTR bstr = paramArray.GetAt(arraySize - 1 - i); // back reading
		bstr.CopyTo(&dispparams.rgvarg[i].bstrVal);
		dispparams.rgvarg[i].vt = VT_BSTR;
	}
	dispparams.cNamedArgs = 0;

	EXCEPINFO excepInfo;
	memset(&excepInfo, 0, sizeof excepInfo);
   	CComVariant vaResult;
	UINT nArgErr = (UINT)-1;  // initialize to invalid arg
         
	hr = spScript->Invoke(dispid,IID_NULL,0,
							DISPATCH_METHOD,&dispparams,&vaResult,&excepInfo,&nArgErr);

	delete [] dispparams.rgvarg;
	if(FAILED(hr))
	{
		return false;
	}
	
	if(pVarResult)
	{
		*pVarResult = vaResult;
	}
	return true;
}



void CHtmlJScriptCoreView::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CHtmlView::OnShowWindow(bShow, nStatus);

	if (::IsWindow(m_wndBrowser.m_hWnd) && bShow)
	{
		CRect rectDruck;
		mehrfachdruckDlg.GetClientRect(rectDruck);
		CRect rect;
		GetClientRect(rect);
		if(m_bDrawingOnly || !WaxSys::isAV())
		{
			rectDruck.bottom = 0;
			rect.top = 0;
		}
		else if(m_bShowPrintDialog)
		{
			rect.top = iDialogHeight + 4;
			rectDruck.bottom = iDialogHeight;
		}
		else
		{
			rectDruck.bottom = 25;
			rect.top = 27;
			
		}
		

		mehrfachdruckDlg.SetWindowPos(NULL, rectDruck.left, rectDruck.top ,
			rectDruck.Width(), rectDruck.Height(), SWP_NOACTIVATE | SWP_NOZORDER);
		::AdjustWindowRectEx(rect,m_wndBrowser.GetStyle(), FALSE, WS_EX_CLIENTEDGE);
		m_wndBrowser.SetWindowPos(NULL, rect.left, rect.top,
			rect.Width(), rect.Height(), SWP_NOACTIVATE | SWP_NOZORDER);
	
		WaxPart* pt = ((CWaxDoc*)GetDocument())->get_active_object();
		XASSERT(pt);
		if (!pt)
			return;
		bool vk3 = pt ? pt->get_werkauftrag()->get_leitkarte().get_vk3() : false;
		mehrfachdruckDlg.SetVk3(vk3);
	}
}



void CHtmlJScriptCoreView::OnSize(UINT nType, int cx, int cy)
{
	CHtmlView::OnSize(nType, cx, cy);

	if (::IsWindow(m_wndBrowser.m_hWnd))
	{
		CRect rectDruck;
		mehrfachdruckDlg.GetClientRect(rectDruck);
		CRect rect;
		GetClientRect(rect);
		if(m_bDrawingOnly)
		{
			rectDruck.bottom = 0;
			rect.top = 0;
		}
		else if(m_bShowPrintDialog)
		{
			rect.top = iDialogHeight + 4;
			rectDruck.bottom = iDialogHeight;
		}
		else
		{
			rectDruck.bottom = 25;
			rect.top = 27;
		}
		mehrfachdruckDlg.SetWindowPos(NULL, rectDruck.left, rectDruck.top ,
			rectDruck.Width(), rectDruck.Height(), SWP_NOACTIVATE | SWP_NOZORDER);
		::AdjustWindowRectEx(rect,m_wndBrowser.GetStyle(), FALSE, WS_EX_CLIENTEDGE);

		::AdjustWindowRectEx(rect,m_wndBrowser.GetStyle(), FALSE, WS_EX_CLIENTEDGE);
		m_wndBrowser.SetWindowPos(NULL, rect.left, rect.top ,
			rect.Width(), rect.Height(), SWP_NOACTIVATE | SWP_NOZORDER);
	
		
	}
}



// CHtmlJScriptCoreView-Meldungshandler
