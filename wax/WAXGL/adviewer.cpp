#include "stdafx.h"



#pragma warning (push)
#pragma warning (disable:4100)
#include "adviewer.h"
#pragma warning (pop)

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(CAdViewer, CWnd)

CLSID const& CAdViewer::GetClsid()
{
	/**
	Parameter DWF_VIEWER_WAXGL=adview4 ist die neueste Version, Parameter adview geht auch mit 
	Autodesk Express Viewer 3.1
	*/
	static CLSID const clsid_ad3
		= { 0xa662da7e, 0xccb7, 0x4743, { 0xb7, 0x1a, 0xd8, 0x17, 0xf6, 0xd5, 0x75, 0xdf } };

	return clsid_ad3;
}
	
unsigned long CAdViewer::GetBackColor()
{
	unsigned long result;
	InvokeHelper(DISPID_BACKCOLOR, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void CAdViewer::SetBackColor(unsigned long newValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(DISPID_BACKCOLOR, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

BOOL CAdViewer::GetEmbedSourceDocument()
{
	BOOL result;
	InvokeHelper(0x3, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void CAdViewer::SetEmbedSourceDocument(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x3, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

CString CAdViewer::GetSourcePath()
{
	CString result;
	InvokeHelper(0x1, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void CAdViewer::SetSourcePath(LPCTSTR lpszNewValue)
{
	static int fswitch = 0;
	
	CString newfile = fswitch ? "c:\\temp\\adview_1.dwf" : "c:\\temp\\adview_0.dwf";
	CString oldfile = fswitch ? "c:\\temp\\adview_0.dwf" : "c:\\temp\\adview_1.dwf";
	::DeleteFile(newfile);
	::CopyFile(lpszNewValue, newfile, false);
	
	
	static BYTE parms[] = VTS_BSTR;
	InvokeHelper(0x1, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newfile);//lpszNewValue);
	fswitch = fswitch ? 0 : 1;
	::DeleteFile(oldfile);
}

LPDISPATCH CAdViewer::GetViewer()
{
	LPDISPATCH result;
	InvokeHelper(0x2, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

void CAdViewer::ShowPrintDialog()
{
	InvokeHelper(0x65, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void CAdViewer::NavigateToUrl(LPCTSTR bstrUrl)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x67, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 bstrUrl);
}

void CAdViewer::ExecuteCommand(LPCTSTR bstrCommand)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x68, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 bstrCommand);
}

void CAdViewer::DrawToDC(long nDc, long nLeft, long nTop, long nRight, long nBottom)
{
	static BYTE parms[] =
		VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_I4;
	InvokeHelper(0x69, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 nDc, nLeft, nTop, nRight, nBottom);
}

VARIANT CAdViewer::Get_ActiveWindow()
{
	VARIANT result;
	InvokeHelper(0xc8, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
	return result;
}

VARIANT CAdViewer::Get_ClientWindow()
{
	VARIANT result;
	InvokeHelper(0xca, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
	return result;
}

CString CAdViewer::Get_DocumentParams()
{
	CString result;
	InvokeHelper(0xcb, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

CString CAdViewer::Get_HistoryParams()
{
	CString result;
	InvokeHelper(0xcc, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

CString CAdViewer::Get_LocalFilePath()
{
	CString result;
	InvokeHelper(0xcd, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void CAdViewer::Set_SourceStream(LPUNKNOWN newValue)
{
	static BYTE parms[] =
		VTS_UNKNOWN;
	InvokeHelper(0xce, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

void CAdViewer::Set_ViewerParams(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0xc9, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString CAdViewer::Get_ViewerParams()
{
	CString result;
	InvokeHelper(0xc9, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void CAdViewer::_GoBack()
{
	InvokeHelper(0x12c, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void CAdViewer::_GoForward()
{
	InvokeHelper(0x12d, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void CAdViewer::_SaveHistory()
{
	InvokeHelper(0x12e, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void CAdViewer::_ShowHelp(LPCTSTR bstrTopic)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x12f, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 bstrTopic);
}
