#pragma once

#include <mxcontrols/listctrl.h>

class WAXINT SapfileOpen : public CDialog   
{
	DECLARE_MESSAGE_MAP()
public:
	ListCtrl list;

	SapfileOpen();

	virtual ~SapfileOpen();

	int openDialog(bool bIsnextGeneration, CString strIppeMatNr, __int64 aufnr, int posnr, int sapfile); 

	void openDialog(CStringArray* wanummer, int sapfile); 

	const CStringArray& get_sapauftraege() const;

private:
	int findEintragDependIppeMatNummer(CString strIppeMatNummer, CString &zeile);
	bool SapfileOpen::getText(const CString &text, int offset, int pos, CString &result, bool trim);

	CStringArray* wanummer;

	int sapfile;

	__int64 aufnr;

	int posnr;

	int select;

	bool bIsNextGeneration;

	CString strIppeMatNr;

	CStringArray sapauftraege;

	void fillStringBox(int select = -1);

	void fillStringBox2();
	
	int checkSDNr();
	int checkIppeNr();

	bool bIgnoreSelection;

protected:

	//Indicates the user clicked a button
	 void OnClickedSearch();
	
public:
	virtual void DoDataExchange(CDataExchange* pDX);

	virtual BOOL DestroyWindow();

	virtual BOOL OnInitDialog();

	virtual void OnOK();

};
