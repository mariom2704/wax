#pragma once
#include <mxcontrols\medit.h>

class WAXPRJDLL FlexMaskedEdit :
	public MaskedEdit
{
	CString strDefFormat;
public:
	FlexMaskedEdit(void);
	void SetDefFormat(CString strLocalDefFormat)
	{
		strDefFormat = strLocalDefFormat;
	};

	CString GetDefFormat()
	{
		return strDefFormat;
	};
	void actasPSPElement( int nl, const CString& psp, int produktlinie);
	bool setValidChars(const CString& strMask);
//	bool init(CString strMask, CString strInitialData);
	~FlexMaskedEdit(void);
};
