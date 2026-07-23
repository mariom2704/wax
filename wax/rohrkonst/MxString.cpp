// MxString.cpp : implementation file
//
// MxString Implementation
//
// Written by Paul E. Bible <pbible@littlefishsoftware.com>
// Copyright (c) 2000. All Rights Reserved.
//
// This code may be used in compiled form in any way you desire. This
// file may be redistributed unmodified by any means PROVIDING it is 
// not sold for profit without the authors written consent, and 
// providing that this notice and the authors name and all copyright 
// notices remains intact. If the source code in this file is used in 
// any  commercial application then a statement along the lines of 
// "Portions copyright (c) Paul E. Bible, 2000" must be included in
// the startup banner, "About" box -OR- printed documentation. An email 
// letting me know that you are using it would be nice as well. That's 
// not much to ask considering the amount of work that went into this.
// If even this small restriction is a problem send me an email.
//
// This file is provided "as is" with no expressed or implied warranty.
// The author accepts no liability for any damage/loss of business that
// this product may cause.
//
// Expect bugs!
// 
// Please use and enjoy, and let me know of any bugs/mods/improvements 
// that you have found/implemented and I will fix/incorporate them into 
// this file. 
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"




#include "MxString.h"
#include <TCHAR.H>
#include <stdio.h>


//////////////////////////////////////////////////////////////////////
//
// Constructors/Destructor
//
//////////////////////////////////////////////////////////////////////

MxString::MxString()
{
	m_pszString = NULL;
	allocString(0);
}

MxString::MxString(const MxString& str)
{
	int nLen = str.length();
	m_pszString = NULL;
	allocString(nLen);
	_tcsncpy(m_pszString, (LPCTSTR) str, nLen);
}

MxString::MxString(LPCTSTR pszString)
{
	int nLen = _tcslen(pszString);
	m_pszString = NULL;
	allocString(nLen);
	_tcsncpy(m_pszString, pszString, nLen);
}

MxString::MxString(BSTR bstrString)
{
	USES_CONVERSION;
	int nLen = _tcslen(OLE2T(bstrString));
	m_pszString = NULL;
	allocString(nLen);
	_tcsncpy(m_pszString, OLE2T(bstrString), nLen);
}

MxString::MxString(TCHAR ch, int nRepeat)
{
	m_pszString = NULL;
	if (nRepeat > 0)
	{
		allocString(nRepeat);
#ifdef _UNICODE
		for (int i=0; i < nRepeat; i++)
			m_pszString[i] = ch;
#else
		memset(m_pszString, ch, nRepeat);
#endif
	}
}

MxString::~MxString()
{
	if (m_pszString)
		free(m_pszString);
}


//////////////////////////////////////////////////////////////////////
//
// Memory Manipulation
//
//////////////////////////////////////////////////////////////////////

void MxString::allocString(int nLen)
{
	ATLASSERT(nLen >= 0);
	if (m_pszString)
		free(m_pszString);
	m_pszString = (TCHAR*) malloc((nLen+1) * sizeof(TCHAR));
	ATLASSERT(m_pszString != NULL);
	m_pszString[nLen] = '\0';
}

void MxString::reAllocString(int nLen)
{
	ATLASSERT(nLen >= 0);
	m_pszString = (TCHAR*) realloc(m_pszString, (nLen+1) * sizeof(TCHAR));
	ATLASSERT(m_pszString != NULL);
	m_pszString[nLen] = '\0';
}

void MxString::stringCopy(int nSrcLen, LPCTSTR lpszSrcData)
{
	allocString(nSrcLen);
	memcpy(m_pszString, lpszSrcData, nSrcLen * sizeof(TCHAR));
	m_pszString[nSrcLen] = '\0';
}

void MxString::stringCopy(MxString &str, int nLen, int nIndex, int nExtra) const
{
	int nNewLen = nLen + nExtra;
	if (nNewLen != 0)
	{
		str.allocString(nNewLen);
		memcpy((void*)str.getString(), m_pszString+nIndex, nLen * sizeof(TCHAR));
	}
}

void MxString::concatCopy(LPCTSTR lpszData)
{
	ATLASSERT(lpszData != NULL);

	// Save the existing string
	int nLen = length();
	TCHAR* pszTemp = (TCHAR*) malloc((nLen+1) * sizeof(TCHAR));
	memcpy(pszTemp, m_pszString, nLen * sizeof(TCHAR));
	pszTemp[nLen] = '\0';

	// Calculate the new string length and realloc memory
	int nDataLen = _tcslen(lpszData);
	int nNewLen = nLen + nDataLen;
	reAllocString(nNewLen);

	// Copy the strings into the new buffer
	memcpy(m_pszString, pszTemp, nLen * sizeof(TCHAR));
	memcpy(m_pszString+nLen, lpszData, nDataLen * sizeof(TCHAR));

	// Cleanup
	free(pszTemp);
}

void MxString::concatCopy(TCHAR ch)
{
	// Save the existing string
	int nLen = length();
	TCHAR* pszTemp = (TCHAR*) malloc((nLen+1) * sizeof(TCHAR));
	memcpy(pszTemp, m_pszString, nLen * sizeof(TCHAR));
	pszTemp[nLen] = '\0';

	// Calculate the new string length and realloc memory
	int nNewLen = nLen + 1;
	reAllocString(nNewLen);

	// Copy the strings into the new buffer
	memcpy(m_pszString, pszTemp, nLen * sizeof(TCHAR));
	m_pszString[nNewLen-1] = ch;

	// Cleanup
	//free(pszTemp);
}

void MxString::concatCopy(LPCTSTR lpszData1, LPCTSTR lpszData2)
{
	ATLASSERT(lpszData1 != NULL);
	ATLASSERT(lpszData2 != NULL);
	int nLen1 = _tcslen(lpszData1);
	int nLen2 = _tcslen(lpszData2);
	int nLen = nLen1 + nLen2;
	allocString(nLen);
	memcpy(m_pszString, lpszData1, nLen1 * sizeof(TCHAR));
	memcpy(m_pszString+nLen1, lpszData2, nLen2 * sizeof(TCHAR)); 
}

BSTR MxString::allocSysString()
{
#ifdef _UNICODE
	BSTR bstr = ::SysAllocStringLen(m_pszString, length());
	if (bstr == NULL)
	{
		ATLASSERT(0);
		return NULL;
	}
#else
	int nLen = MultiByteToWideChar(CP_ACP, 0, m_pszString, length(), NULL, NULL);
	BSTR bstr = ::SysAllocStringLen(NULL, nLen);
	if (bstr == NULL)
	{
		ATLASSERT(0);
		return NULL;
	}
	MultiByteToWideChar(CP_ACP, 0, m_pszString, length(), bstr, nLen);
#endif

	return bstr;
}


//////////////////////////////////////////////////////////////////////
//
// Accessors for the String as an Array
//
//////////////////////////////////////////////////////////////////////

void MxString::empty()
{
	if (_tcslen(m_pszString) > 0)
		allocString(0);
}

TCHAR MxString::getAt(int nIndex)
{
	int nLen = _tcslen(m_pszString);
	ATLASSERT(nIndex >= 0);
	ATLASSERT(nIndex < nLen);
	return m_pszString[nIndex];
}

TCHAR MxString::operator[] (int nIndex)
{
	int nLen = _tcslen(m_pszString);
	ATLASSERT(nIndex >= 0);
	ATLASSERT(nIndex < nLen);
	return m_pszString[nIndex];
}

void MxString::setAt(int nIndex, TCHAR ch)
{
	int nLen = _tcslen(m_pszString);
	ATLASSERT(nIndex >= 0);
	ATLASSERT(nIndex < nLen);
	m_pszString[nIndex] = ch;	
}

int MxString::length() const
{ 
	return _tcslen(m_pszString); 
}

bool MxString::isEmpty() const
{ 
	return (length() > 0) ? false : true; 
}


//////////////////////////////////////////////////////////////////////
//
// Conversions
//
//////////////////////////////////////////////////////////////////////

void MxString::toUpper()
{
	_tcsupr(m_pszString);
}

void MxString::toLower()
{
	_tcslwr(m_pszString);
}

void MxString::reverse()
{
	_tcsrev(m_pszString);
}

void MxString::trimLeft()
{
	LPTSTR lpsz = m_pszString;

	while (_istspace(*lpsz))
		lpsz = _tcsinc(lpsz);

	if (lpsz != m_pszString)
	{
		memmove(m_pszString, lpsz, (length()+1) * sizeof(TCHAR));
	}
}

void MxString::trimRight()
{
	LPTSTR lpsz = m_pszString;
	LPTSTR lpszLast = NULL;

	while (*lpsz != '\0')
	{
		if (_istspace(*lpsz))
		{
			if (lpszLast == NULL)
				lpszLast = lpsz;
		}
		else
			lpszLast = NULL;
		lpsz = _tcsinc(lpsz);
	}

	if (lpszLast != NULL)
		*lpszLast = '\0';
}


void  MxString::trim()
{
	trimLeft();
	trimRight();
}


int   MxString::toInt()
{
	//QT
	//return _wtoi((wchar_t*)m_pszString);

	//MFC
	return atoi((char*)m_pszString);
}

double MxString::toDouble()
{
	//QT
	//return _wtof((wchar_t*)m_pszString);

	//MFC
	return atof((char*)m_pszString);
}



//////////////////////////////////////////////////////////////////////
//
// Searching
//
//////////////////////////////////////////////////////////////////////

int MxString::find(TCHAR ch) const
{
	return find(ch, 0);
}

int MxString::find(TCHAR ch, int nStart) const
{
	if (nStart >= length())
		return -1;
	LPTSTR lpsz = _tcschr(m_pszString + nStart, (_TUCHAR) ch);
	return (lpsz == NULL) ? -1 : (int)(lpsz - m_pszString);
}

int MxString::find(LPCTSTR lpszSub)
{
	return find(lpszSub, 0);
}

int MxString::find(LPCTSTR lpszSub, int nStart)
{
	ATLASSERT(_tcslen(lpszSub) > 0);

	if (nStart > length())
		return -1;

	LPTSTR lpsz = _tcsstr(m_pszString + nStart, lpszSub);
	return (lpsz == NULL) ? -1 : (int)(lpsz - m_pszString);
}

int MxString::findOneOf(LPCTSTR lpszCharSet) const
{
	LPTSTR lpsz = _tcspbrk(m_pszString, lpszCharSet);
	return (lpsz == NULL) ? -1 : (int)(lpsz - m_pszString);
}


//////////////////////////////////////////////////////////////////////
//
// Assignment Operations
//
//////////////////////////////////////////////////////////////////////

const MxString& MxString::operator=(MxString& strSrc)
{
	if (m_pszString != strSrc.getString())
		stringCopy(strSrc.length(), strSrc.getString());
	return *this;
}

const MxString& MxString::operator=(LPCTSTR lpsz)
{
	ATLASSERT(lpsz != NULL);
	stringCopy(_tcslen(lpsz), lpsz);
	return *this;
}

const MxString& MxString::operator=(BSTR bstrStr)
{
	USES_CONVERSION;
	int nLen = _tcslen(OLE2T(bstrStr));
	stringCopy(nLen, OLE2T(bstrStr));
	return *this;
}


//////////////////////////////////////////////////////////////////////
//
// Concatenation
//
//////////////////////////////////////////////////////////////////////

MxString operator+(MxString& strSrc1, MxString& strSrc2)
{
	MxString s;
	s.concatCopy((LPCTSTR) strSrc1, (LPCTSTR) strSrc2);
	return s;
}

MxString operator+(MxString& strSrc, LPCTSTR lpsz)
{
	MxString s;
	s.concatCopy((LPCTSTR) strSrc, lpsz);
	return s;
}

MxString operator+(LPCTSTR lpsz, MxString& strSrc)
{
	MxString s;
	s.concatCopy(lpsz, (LPCTSTR) strSrc);
	return s;
}

MxString operator+(MxString& strSrc, BSTR bstrStr)
{
	USES_CONVERSION;
	MxString s;
	s.concatCopy((LPCTSTR) strSrc, OLE2T(bstrStr));
	return s;
}

MxString operator+(BSTR bstrStr, MxString& strSrc)
{
	USES_CONVERSION;
	MxString s;
	s.concatCopy(OLE2T(bstrStr), (LPCTSTR) strSrc);
	return s;
}

const MxString& MxString::operator+=(MxString& strSrc)
{
	concatCopy((LPCTSTR) strSrc);
	return *this;
}

const MxString& MxString::operator+=(LPCTSTR lpsz)
{
	concatCopy(lpsz);
	return *this;
}

const MxString& MxString::operator+=(BSTR bstrStr)
{
	USES_CONVERSION;
	concatCopy(OLE2T(bstrStr));
	return *this;
}

const MxString& MxString::operator+=(TCHAR ch)
{
	USES_CONVERSION;
	concatCopy(ch);
	return *this;
}


MxString& MxString::append(int val)
{
	MxString tmp;
	tmp.format(_T("%d"),val);
	*this += tmp;

	return *this;
}


MxString& MxString::append(double val,int nachkomma)
{
	MxString tmp1,tmp2;
	tmp1.format(_T("%%.%df"),nachkomma);
	tmp2.format(tmp1,val);

	*this += tmp2;

	return *this;
}



//////////////////////////////////////////////////////////////////////
//
// Extraction
//
//////////////////////////////////////////////////////////////////////

MxString MxString::mid(int nFirst) const
{
	return mid(nFirst, length() - nFirst);	
}

MxString MxString::mid(int nFirst, int nCount) const
{
	if (nFirst < 0)
		nFirst = 0;
	if (nCount < 0)
		nCount = 0;

	if (nFirst + nCount > length())
		nCount = length() - nFirst;
	if (nFirst > length())
		nCount = 0;

	ATLASSERT(nFirst >= 0);
	ATLASSERT(nFirst + nCount <= length());

	if (nFirst == 0 && nFirst + nCount == length())
		return *this;

	MxString newStr;
	stringCopy(newStr, nCount, nFirst, 0);
	return newStr;
}

MxString MxString::left(int nCount) const
{
	if (nCount < 0)
		nCount = 0;
	if (nCount >= length())
		return *this;

	MxString newStr;
	stringCopy(newStr, nCount, 0, 0);
	return newStr;
}

MxString MxString::right(int nCount) const
{
	if (nCount < 0)
		nCount = 0;
	if (nCount >= length())
		return *this;

	MxString newStr;
	stringCopy(newStr, nCount, length() - nCount, 0);
	return newStr;
}

MxString MxString::spanIncluding(LPCTSTR lpszCharSet) const
{
	ATLASSERT(lpszCharSet != NULL);
#ifdef _UNICODE
	ATLASSERT(::IsBadStringPtrW(lpszCharSet, -1) == 0);
#else
	ATLASSERT(::IsBadStringPtrA(lpszCharSet, -1) == 0);
#endif
	return left(_tcsspn(m_pszString, lpszCharSet));
}

MxString MxString::spanExcluding(LPCTSTR lpszCharSet) const
{
	ATLASSERT(lpszCharSet != NULL);
#ifdef _UNICODE
	ATLASSERT(::IsBadStringPtrW(lpszCharSet, -1) == 0);
#else
	ATLASSERT(::IsBadStringPtrA(lpszCharSet, -1) == 0);
#endif
	return left(_tcscspn(m_pszString, lpszCharSet));
}


//////////////////////////////////////////////////////////////////////
//
// Comparison
//
//////////////////////////////////////////////////////////////////////
	
int MxString::compare(MxString& str) const
{
	ATLASSERT((LPCTSTR) str != NULL);
#ifdef _UNICODE
	ATLASSERT(::IsBadStringPtrW((LPCTSTR) str, -1) == 0);
#else
	ATLASSERT(::IsBadStringPtrA((LPCTSTR) str, -1) == 0);
#endif
	return _tcscmp(m_pszString, (LPCTSTR) str);	
}

int MxString::compare(LPCTSTR lpsz) const
{
	ATLASSERT(lpsz != NULL);
#ifdef _UNICODE
	ATLASSERT(::IsBadStringPtrW(lpsz, -1) == 0);
#else
	ATLASSERT(::IsBadStringPtrA(lpsz, -1) == 0);
#endif
	return _tcscmp(m_pszString, lpsz);	
}

int MxString::compareNoCase(MxString& str) const
{
	ATLASSERT((LPCTSTR) str != NULL);
#ifdef _UNICODE
	ATLASSERT(::IsBadStringPtrW((LPCTSTR) str, -1) == 0);
#else
	ATLASSERT(::IsBadStringPtrA((LPCTSTR) str, -1) == 0);
#endif
	return _tcsicmp(m_pszString, (LPCTSTR) str);	
}

int MxString::compareNoCase(LPCTSTR lpsz) const
{
	ATLASSERT(lpsz != NULL);
#ifdef _UNICODE
	ATLASSERT(::IsBadStringPtrW(lpsz, -1) == 0);
#else
	ATLASSERT(::IsBadStringPtrA(lpsz, -1) == 0);
#endif
	return _tcsicmp(m_pszString, lpsz);	
}

int MxString::collate(LPCTSTR lpsz) const
{
	ATLASSERT(lpsz != NULL);
#ifdef _UNICODE
	ATLASSERT(::IsBadStringPtrW(lpsz, -1) == 0);
#else
	ATLASSERT(::IsBadStringPtrA(lpsz, -1) == 0);
#endif
	return _tcscoll(m_pszString, lpsz);	
}

int MxString::collate(MxString &str) const
{
	ATLASSERT((LPCTSTR) str != NULL);
#ifdef _UNICODE
	ATLASSERT(::IsBadStringPtrW((LPCTSTR) str, -1) == 0);
#else
	ATLASSERT(::IsBadStringPtrA((LPCTSTR) str, -1) == 0);
#endif
	return _tcscoll(m_pszString, (LPCTSTR) str);	
}


//////////////////////////////////////////////////////////////////////
//
// Formatting
//
//////////////////////////////////////////////////////////////////////


MxString MxString::arg(const MxString& val)
{
	MxString str = *this,
		     platzhalter,
			 steuerZeichenVorne = _T("{"),
			 steuerZeichenHinten = _T("}");

	int maxCount	= 20;

	
	for (int count=0; count<=maxCount; count++)
	{
		platzhalter = steuerZeichenVorne;
		platzhalter.append(count);
		platzhalter += steuerZeichenHinten;



		if (str.replace(platzhalter,val))
		{
			//Wenn Platzhalter ersetzt wurde braucht nicht weiter gesucht werden
			break;
		}
		else
		{
			//Wenn Platzhalter nicht ersetzt wurde, prüfen ob weitere Platzhalter vorhanden sind
			if (str.find(steuerZeichenVorne) < 0)
				break;
		}
	}

	return str;
}


MxString MxString::arg(int val)
{
	MxString tempStr;
	tempStr.append(val);

	return arg(tempStr);
}

MxString MxString::arg(double val,int nachkomma)
{
	MxString tempStr;
	tempStr.append(val,nachkomma);

	return arg(tempStr);
}



	
void MxString::format(LPCTSTR pszCharSet, ...)
{
	va_list vl;
	va_start(vl, pszCharSet);

	TCHAR* pszTemp = NULL;
	int nBufferSize = 0;
	int nRetVal = -1;

	do {
		nBufferSize += 100;
		delete [] pszTemp;
		pszTemp = new TCHAR[nBufferSize];
		nRetVal = _vstprintf(pszTemp, pszCharSet, vl);
	} while (nRetVal < 0);

	int nNewLen = _tcslen(pszTemp);
	allocString(nNewLen);
	_tcscpy(m_pszString, pszTemp);
	delete [] pszTemp;

	va_end(vl);
}


//////////////////////////////////////////////////////////////////////
//
// Replacing
//
//////////////////////////////////////////////////////////////////////

int MxString::replace(TCHAR chOld, TCHAR chNew)
{
	int nCount = 0;

	if (chOld != chNew)
	{
		LPTSTR psz = m_pszString;
		LPTSTR pszEnd = psz + length();
		while(psz < pszEnd)
		{
			if (*psz == chOld)
			{
				*psz = chNew;
				nCount++;
			}
			psz = _tcsinc(psz);
		}
	}

	return nCount;
}

int MxString::replace(LPCTSTR lpszOld, LPCTSTR lpszNew)
{
	int nSourceLen = _tcslen(lpszOld);
	if (nSourceLen == 0)
		return 0;
	int nReplaceLen = _tcslen(lpszNew);

	int nCount = 0;
	LPTSTR lpszStart = m_pszString;
	LPTSTR lpszEnd = lpszStart + length();
	LPTSTR lpszTarget;

	// Check to see if any changes need to be made
	while (lpszStart < lpszEnd)
	{
		while ((lpszTarget = _tcsstr(lpszStart, lpszOld)) != NULL)
		{
			lpszStart = lpszTarget + nSourceLen;
			nCount++;
		}
		lpszStart += _tcslen(lpszStart) + 1;
	}

	// Do the actual work
	if (nCount > 0)
	{
		int nOldLen = length();
		int nNewLen = nOldLen + (nReplaceLen - nSourceLen) * nCount;
		if (length() < nNewLen)
		{
			MxString szTemp = m_pszString;
			reAllocString(nNewLen);
			memcpy(m_pszString, (LPCTSTR) szTemp, nOldLen * sizeof(TCHAR));
		}

		lpszStart = m_pszString;
		lpszEnd = lpszStart + length();

		while (lpszStart < lpszEnd)
		{
			while ((lpszTarget = _tcsstr(lpszStart, lpszOld)) != NULL)
			{
				int nBalance = nOldLen - (lpszTarget - m_pszString + nSourceLen);
				memmove(lpszTarget + nReplaceLen, lpszTarget + nSourceLen, nBalance * sizeof(TCHAR));
				memcpy(lpszTarget, lpszNew, nReplaceLen * sizeof(TCHAR));
				lpszStart = lpszTarget + nReplaceLen;
				lpszStart[nBalance] = '\0';
				nOldLen += (nReplaceLen - nSourceLen);
			}
			lpszStart += _tcslen(lpszStart) + 1;
		}
		ATLASSERT(m_pszString[length()] == '\0');
	}
	return nCount;
}




void DumpHtmlFile(CString strFileName, CString strContent, BOOL bAppend)
{

	CFile FileStore;
	DWORD dwCreationDisposition  = CREATE_ALWAYS;
	if(bAppend)
	{
		dwCreationDisposition  = OPEN_ALWAYS ;
	}

	TCHAR path[_MAX_PATH];
	GetModuleFileName(0, path, _MAX_PATH);
	CString strPath = path;
	int iWhere = strPath.ReverseFind('\\');
	CString pfadDefault = strPath.Mid(0, iWhere);
	pfadDefault += "\\Test.txt";

	HANDLE hFile = 	CreateFile (pfadDefault, GENERIC_READ | GENERIC_WRITE , 0, NULL, dwCreationDisposition,	0, 	NULL);        
	FileStore.m_hFile = (HANDLE) hFile;

	if(bAppend)
	{
		FileStore.SeekToEnd();
		strContent += _T("\r\n");
	}
	FileStore.Write(strContent, strContent.GetLength());
	FileStore.Close();
}


