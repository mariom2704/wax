#pragma once

#include "WaxPartForm.h"
#include <mxcontrols/FloatEdit.h>

class EditSoko : 
public WaxPartForm   
{
private:
	Preis::Sonderkosten* soko;

	CString text;

	double preis;

	int posnr;

	FloatEditCtrl preisctrl;

	bool isfix;

public:
	virtual BOOL OnInitDialog();

	virtual void OnOK();

	virtual void DoDataExchange(CDataExchange* pDX);


public:
	EditSoko(CWnd* parent, Preis::Sonderkosten* so);


protected:

	//Eine Standardsoko wurde ausgewählt
	 void onSelchangeSoko();
	
	DECLARE_MESSAGE_MAP()
};

