#pragma once

#include "FlexMaskedEdit.h"


//Bearbeitungsklasse für PrjLeitkarteData
class WAXPRJDLL PrjLeitkarteForm : 
public WaxPartForm  
{

public:
	//{{AFX_DATA(PrjLeitkarteForm)
	enum { IDD = IDD_PROJEKTLEITKARTE };
	CComboBox	m_NLNummer;
	//}}AFX_DATA
	
protected:
	//Destruktor
	virtual ~PrjLeitkarteForm();

	//Dynamische Erstellung verwendet geschützten Konstruktor
	PrjLeitkarteForm();

	//{{AFX_VIRTUAL(PrjLeitkarteForm)
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung
	//}}AFX_VIRTUAL

	//{{AFX_MSG(PrjLeitkarteForm)
	virtual BOOL OnInitDialog();
	 void OnSelchangeNlkennung();
	 void OnSelendokNlkennung();
	 void OnEditchangeNlkennung();
	 void OnSelchangeBrandschutztechnik();
	 void OnFormatPSP();
	
	DECLARE_DYNCREATE(PrjLeitkarteForm)
	DECLARE_MESSAGE_MAP()
	afx_msg void OnBnClickedIppeKnoten();

private:
	//Eingabecontrol für PSP Element
	FlexMaskedEdit editPSP;

};

