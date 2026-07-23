#include "stdafx.h"


#include <mxcontrols/msgbox.h>
#include <mapi.h>

#include "GpMail.h"


using namespace MxGui;

GpMail::GpMail(void)
{
}

GpMail::~GpMail(void)
{
}


bool GpMail::sendMail(const string &email, const string &betreff, const string &text, const string &file)
{
	UNREFERENCED_PARAMETER(text);

	CWaitCursor wait;

	HINSTANCE m_hInstMail = NULL;
	if (m_hInstMail == NULL)
		m_hInstMail = ::LoadLibrary("MAPI32.DLL");

	if (m_hInstMail == NULL)
	{
		MsgBox::show(AFX_IDP_FAILED_MAPI_LOAD, MsgBox::Error);
		return false;
	}
	XASSERT(m_hInstMail != NULL);

	ULONG (PASCAL *lpfnSendMail)(ULONG, ULONG, MapiMessage*, FLAGS, ULONG);
	(FARPROC&)lpfnSendMail = GetProcAddress(m_hInstMail, "MAPISendMail");
	if (lpfnSendMail == NULL)
	{
		MsgBox::show(AFX_IDP_INVALID_MAPI_DLL, MsgBox::Error);
		::FreeLibrary(m_hInstMail);
		return false;
	}
	XASSERT(lpfnSendMail != NULL);

	

	// prepare the file description (for the attachment)
	CString fn = file.c_str();
	MapiFileDesc fileDesc;
	if (file.length() > 0)
	{
		memset(&fileDesc, 0, sizeof(fileDesc));
		fileDesc.nPosition = (ULONG)-1;
		fileDesc.lpszPathName = fn.GetBuffer(0);
		fileDesc.lpszFileName = fn.GetBuffer(0);
	}

	MapiRecipDesc recip;
	CString smtp;
	smtp.Format("SMTP:%s", email.c_str());
	memset(&recip, 0, sizeof(recip));
	recip.ulRecipClass = MAPI_TO;
	//recip.lpszName = (LPSTR)(LPCTSTR)displayname;
	recip.lpszAddress = (LPSTR)(LPCTSTR)smtp;
	
		
	// prepare the message (empty with 1 attachment)
	MapiMessage message;
	CString subject = betreff.c_str();
	memset(&message, 0, sizeof(message));
	
	if (file.length() > 0)
	{
		message.lpFiles = &fileDesc;
		message.nFileCount = 1;
	}
	
	message.lpRecips = &recip;
	message.nRecipCount = 1;
	message.lpszSubject = (LPSTR)(LPCSTR)subject;

	// prepare for modal dialog box
	//AfxGetApp()->EnableModeless(FALSE);
	HWND hWndTop;
	CWnd* pParentWnd = CWnd::GetSafeOwner(NULL, &hWndTop);

	// some extra precautions are required to use MAPISendMail as it
	// tends to enable the parent window in between dialogs (after
	// the login dialog, but before the send note dialog).
	pParentWnd->SetCapture();
	::SetFocus(NULL);
	pParentWnd->m_nFlags |= WF_STAYDISABLED;

	int nError = lpfnSendMail(0, (ULONG)pParentWnd->GetSafeHwnd(),
		&message, MAPI_LOGON_UI|MAPI_DIALOG, 0);

	// after returning from the MAPISendMail call, the window must
	// be re-enabled and focus returned to the frame to undo the workaround
	// done before the MAPI call.
	::ReleaseCapture();
	pParentWnd->m_nFlags &= ~WF_STAYDISABLED;

	pParentWnd->EnableWindow(TRUE);
	::SetActiveWindow(NULL);
	pParentWnd->SetActiveWindow();
	pParentWnd->SetFocus();
	if (hWndTop != NULL)
		::EnableWindow(hWndTop, TRUE);
//	AfxGetApp()->EnableModeless(TRUE);

	if (nError != SUCCESS_SUCCESS &&
		nError != MAPI_USER_ABORT && nError != MAPI_E_LOGIN_FAILURE)
	{
		MsgBox::show(AFX_IDP_FAILED_MAPI_SEND, MsgBox::Error);
	}

	::FreeLibrary(m_hInstMail);
	return true;
}