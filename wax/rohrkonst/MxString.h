// MxString.h : header file
//
// MxString Header
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

#if !defined(AFX_COMSTRING_H__C514A391_FB4C_11D3_AABC_0000E215F0C2__INCLUDED_)
#define AFX_COMSTRING_H__C514A391_FB4C_11D3_AABC_0000E215F0C2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma warning (disable : 4244)
#pragma warning (disable : 4267)
#pragma warning (disable : 4245)

#include <atlbase.h>

ROHRKONSTDLL void DumpHtmlFile(CString strFileName, CString strContent, BOOL bAppend);


class ROHRKONSTDLL MxString  
{
public: 
	MxString();
	MxString(const MxString& szString);
	MxString(LPCTSTR pszString);
	MxString(BSTR bstrString);
	MxString(TCHAR ch, int nRepeat);
	virtual ~MxString();
	
	// Assignment Operations
	const MxString& operator=(MxString& strSrc);
	const MxString& operator=(LPCTSTR lpsz);
	const MxString& operator=(BSTR bstrStr);
	operator LPCTSTR() const	{ return m_pszString; }
	TCHAR* getString()const	{ return m_pszString; }
	BSTR	allocSysString();
	
	// Concatenation
	const MxString& operator+=(MxString& strSrc);
	const MxString& operator+=(LPCTSTR lpsz);
	const MxString& operator+=(BSTR bstrStr);
	const MxString& operator+=(TCHAR ch);
	friend MxString operator+(MxString& strSrc1, MxString& strSrc2);
	friend MxString operator+(MxString& strSrc, LPCTSTR lpsz);
	friend MxString operator+(LPCTSTR lpsz, MxString& strSrc);
	friend MxString operator+(MxString& strSrc, BSTR bstrStr);
	friend MxString operator+(BSTR bstrStr, MxString& strSrc);

	MxString& append(int val);
	MxString& append(double val,int nachkomma);


	// Accessors for the String as an Array
	int		length() const;
	bool	isEmpty() const;
	void	empty();
	TCHAR	getAt(int nIndex);
	void	setAt(int nIndex, TCHAR ch);
	TCHAR	operator[] (int nIndex);

	// Conversions
	void	toUpper();
	void	toLower();
	void	reverse();
	void	trimLeft();
	void	trimRight();
	void    trim();
	int     toInt();
	double  toDouble();

	// Searching
	int		find(TCHAR ch) const;
	int		find(TCHAR ch, int nStart) const;
	int		find(LPCTSTR lpszSub);
	int		find(LPCTSTR lpszSub, int nStart);
	int		findOneOf(LPCTSTR lpszCharSet) const;

	// Extraction
	MxString mid(int nFirst) const;
	MxString mid(int nFirst, int nCount) const;
	MxString left(int nCount) const;
	MxString right(int nCount) const;
	MxString spanIncluding(LPCTSTR lpszCharSet) const;
	MxString spanExcluding(LPCTSTR lpszCharSet) const;

	// Comparison
	int compare(MxString& str) const;
	int compare(LPCTSTR lpsz) const;
	int compareNoCase(MxString& str) const;
	int compareNoCase(LPCTSTR lpsz) const;
	int collate(MxString& str) const;
	int collate(LPCTSTR lpsz) const;

	// Formatting
	MxString arg(const MxString& val);
	MxString arg(int val);
	MxString arg(double val,int nachkomma);

	void format(LPCTSTR pszCharSet, ...);


	// Replacing
	int replace(TCHAR chOld, TCHAR chNew);
	int replace(LPCTSTR lpszOld, LPCTSTR lpszNew);
	
protected:
	LPTSTR	m_pszString;
	void	stringCopy(MxString& str, int nLen, int nIndex, int nExtra) const;
	void	stringCopy(int nSrcLen, LPCTSTR lpszSrcData);
	void	concatCopy(LPCTSTR lpszData);
	void	concatCopy(TCHAR ch);
	void	concatCopy(LPCTSTR lpszData1, LPCTSTR lpszData2);
	void	allocString(int nLen);
	void	reAllocString(int nLen);
};	

// Compare operations
bool operator==(const MxString& s1, const MxString& s2);
bool operator==(const MxString& s1, LPCTSTR s2);
bool operator==(LPCTSTR s1, const MxString& s2);
bool operator!=(const MxString& s1, const MxString& s2);
bool operator!=(const MxString& s1, LPCTSTR s2);
bool operator!=(LPCTSTR s1, const MxString& s2);

// Compare implementations
inline bool operator==(const MxString& s1, const MxString& s2)
	{ return s1.compare(s2) == 0; }
inline bool operator==(const MxString& s1, LPCTSTR s2)
	{ return s1.compare(s2) == 0; }
inline bool operator==(LPCTSTR s1, const MxString& s2)
	{ return s2.compare(s1) == 0; }
inline bool operator!=(const MxString& s1, const MxString& s2)
	{ return s1.compare(s2) != 0; }
inline bool operator!=(const MxString& s1, LPCTSTR s2)
	{ return s1.compare(s2) != 0; }
inline bool operator!=(LPCTSTR s1, const MxString& s2)
	{ return s2.compare(s1) != 0; }

#endif // !defined(AFX_COMSTRING_H__C514A391_FB4C_11D3_AABC_0000E215F0C2__INCLUDED_)
