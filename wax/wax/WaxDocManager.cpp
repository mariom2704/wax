#include "stdafx.h"
#include <afxpriv.h>
#include "WaxDocManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#ifndef _countof
	#define _countof(array) (sizeof(array)/sizeof(array[0]))
#endif
const TCHAR _afxShellOpenFmt[] = _T("%s\\shell\\open\\%s");
const TCHAR _afxShellPrintFmt[] = _T("%s\\shell\\print\\%s");
const TCHAR _afxShellPrintToFmt[] = _T("%s\\shell\\printto\\%s");
const TCHAR _afxDefaultIconFmt[] = _T("%s\\DefaultIcon");
const TCHAR _afxShellNewFmt[] = _T("%s\\ShellNew");

#define DEFAULT_ICON_INDEX 0

const TCHAR _afxIconIndexFmt[] = _T(",%d");
const TCHAR _afxCommand[] = _T("command");
const TCHAR _afxOpenArg[] = _T(" \"%1\"");
const TCHAR _afxPrintArg[] = _T(" /p \"%1\"");
const TCHAR _afxPrintToArg[] = _T(" /pt \"%1\" \"%2\" \"%3\" \"%4\"");
const TCHAR _afxDDEArg[] = _T(" /dde");

const TCHAR _afxDDEExec[] = _T("ddeexec");
const TCHAR _afxDDEOpen[] = _T("[open(\"%1\")]");
const TCHAR _afxDDEPrint[] = _T("[print(\"%1\")]");
const TCHAR _afxDDEPrintTo[] = _T("[printto(\"%1\",\"%2\",\"%3\",\"%4\")]");

const TCHAR _afxShellNewValueName[] = _T("NullFile");
const TCHAR _afxShellNewValue[] = _T("");

// recursively remove a registry key if and only if it has no subkeys

BOOL _AfxDeleteRegKey(LPCTSTR lpszKey)
{
	// copy the string
	LPTSTR lpszKeyCopy = _tcsdup(lpszKey);

	if(lpszKeyCopy == NULL)
		return FALSE;

	LPTSTR lpszLast = lpszKeyCopy + lstrlen(lpszKeyCopy);

	// work until the end of the string
	while (lpszLast != NULL)
	{
		*lpszLast = '\0';
		lpszLast = _tcsdec(lpszKeyCopy, lpszLast);

		// try to open that key
		HKEY hKey;
		if (::RegOpenKey(HKEY_CLASSES_ROOT, lpszKeyCopy, &hKey) != ERROR_SUCCESS)
			break;

		// enumerate the keys underneath
		TCHAR szScrap[_MAX_PATH+1];
		DWORD dwLen = _countof(szScrap);
		BOOL bItExists = FALSE;

		if (::RegEnumKey(hKey, 0, szScrap, dwLen) == ERROR_SUCCESS)
			bItExists = TRUE;
		::RegCloseKey(hKey);

		// found one?  quit looping
		if (bItExists)
			break;

		// otherwise, delete and find the previous backwhack
		::RegDeleteKey(HKEY_CLASSES_ROOT, lpszKeyCopy);
		lpszLast = _tcsrchr(lpszKeyCopy, '\\');
	}

	// release the string and return
	free(lpszKeyCopy);
	return TRUE;
}

BOOL _AfxSetRegKey(LPCTSTR lpszKey, LPCTSTR lpszValue, LPCTSTR lpszValueName = NULL)
{
	if (lpszValueName == NULL)
	{
		if (::RegSetValue(HKEY_CLASSES_ROOT, lpszKey, REG_SZ,
			  lpszValue, lstrlen(lpszValue) * sizeof(TCHAR)) != ERROR_SUCCESS)
		{
			TRACE(traceAppMsg, 0, _T("Warning: registration database update failed for key '%s'.\n"),
				lpszKey);
			return FALSE;
		}
		return TRUE;
	}
	else
	{
		HKEY hKey;

		if(::RegCreateKey(HKEY_CLASSES_ROOT, lpszKey, &hKey) == ERROR_SUCCESS)
		{
			LONG lResult = ::RegSetValueEx(hKey, lpszValueName, 0, REG_SZ,
				(CONST BYTE*)lpszValue, (lstrlen(lpszValue) + 1) * sizeof(TCHAR));

			if(::RegCloseKey(hKey) == ERROR_SUCCESS && lResult == ERROR_SUCCESS)
				return TRUE;
		}
		TRACE(traceAppMsg, 0, _T("Warning: registration database update failed for key '%s'.\n"), lpszKey);
		return FALSE;
	}
}

WaxDocManager::WaxDocManager()
{
}

WaxDocManager::~WaxDocManager()
{
}

CDocument* WaxDocManager::OpenDocumentFile(LPCTSTR lpszFileName)
{
	if (strlen(lpszFileName))
	{
		return CDocManager::OpenDocumentFile(lpszFileName);
	}
	POSITION pos = m_templateList.GetHeadPosition();
	XASSERT(pos);
	if (!pos)
		return NULL;
	CDocTemplate* pTemplate = (CDocTemplate*)m_templateList.GetNext(pos);
	XASSERT(pTemplate);
	if (!pTemplate)
		return NULL;
	return pTemplate->OpenDocumentFile(lpszFileName);
}


void WaxDocManager::RegisterShellFileTypes(BOOL bCompat)
{
	/**
	Die Defaultversion macht einen Eintrag mit DDE. Ein Doppelklick führt dann allerdings
	zu einer Fehlermeldung. Ohne DDE gehts. 
	*/
	UNREFERENCED_PARAMETER(bCompat);
	UnregisterShellFileTypes();
	ASSERT(!m_templateList.IsEmpty());  // must have some doc templates

	CString strPathName, strTemp;

	AfxGetModuleShortFileName(AfxGetInstanceHandle(), strPathName);

	POSITION pos = m_templateList.GetHeadPosition();
	for (int nTemplateIndex = 1; pos != NULL; nTemplateIndex++)
	{
		CDocTemplate* pTemplate = (CDocTemplate*)m_templateList.GetNext(pos);

		CString strOpenCommandLine = strPathName;
		CString strPrintCommandLine = strPathName;
		CString strPrintToCommandLine = strPathName;
		CString strDefaultIconCommandLine = strPathName;

		

		CString strFilterExt, strFileTypeId, strFileTypeName;
		if (pTemplate->GetDocString(strFileTypeId,
		   CDocTemplate::regFileTypeId) && !strFileTypeId.IsEmpty())
		{
			// enough info to register it
			if (!pTemplate->GetDocString(strFileTypeName,
			   CDocTemplate::regFileTypeName))
				strFileTypeName = strFileTypeId;    // use id name

			ASSERT(strFileTypeId.Find(' ') == -1);  // no spaces allowed

			// first register the type ID of our server
			if (!_AfxSetRegKey(strFileTypeId, strFileTypeName))
				continue;       // just skip it

			

			// path\shell\open\command = path filename
			// path\shell\print\command = path /p filename
			// path\shell\printto\command = path /pt filename printer driver port
			strOpenCommandLine += _afxOpenArg;
			

			// path\shell\open\command = path filename
			strTemp.Format(_afxShellOpenFmt, (LPCTSTR)strFileTypeId,
				(LPCTSTR)_afxCommand);
			if (!_AfxSetRegKey(strTemp, strOpenCommandLine))
				continue;       // just skip it

			

			pTemplate->GetDocString(strFilterExt, CDocTemplate::filterExt);
			if (!strFilterExt.IsEmpty())
			{
				ASSERT(strFilterExt[0] == '.');

				LONG lSize = _MAX_PATH * 2;
				LONG lResult = ::RegQueryValue(HKEY_CLASSES_ROOT, strFilterExt,
					strTemp.GetBuffer(lSize), &lSize);
				strTemp.ReleaseBuffer();

				if (lResult != ERROR_SUCCESS || strTemp.IsEmpty() ||
					strTemp != strFileTypeId)
				{
					// no association for that suffix
					if (!_AfxSetRegKey(strFilterExt, strFileTypeId))
						continue;

					
				}
			}
		}
	}
}
