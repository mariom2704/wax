// Copyright (C) 2001 by Minimax GmbH

#if defined (_MSC_VER) && (_MSC_VER >= 1000)
#pragma once
#endif
#ifndef _INC_EDITFORMSTUECKEUNDFITTINGE_3FFA9DFA0203_INCLUDED
#define _INC_EDITFORMSTUECKEUNDFITTINGE_3FFA9DFA0203_INCLUDED

class EditFormstueckeUndFittinge 
: public WaxPartForm
{
public:
	virtual ~EditFormstueckeUndFittinge();

	EditFormstueckeUndFittinge::EditFormstueckeUndFittinge(CWnd* parent, Material* ma);

private:
	double preis;

	double pulverpreis;

	FloatEditCtrl prectrl;

	FloatEditCtrl pulprectrl;

	FloatEditCtrl gewctrl;

	Material* mat;

	int anzahl;

	CString bezeichnung;

	int artnr;

	double gewicht;

	//{{AFX_VIRTUAL(EditFormstueckeUndFittinge)
	public:
	virtual void OnOK();

	virtual BOOL OnInitDialog();

	virtual void DoDataExchange(CDataExchange* pDX);
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(EditFormstueckeUndFittinge)
	
	DECLARE_MESSAGE_MAP()
};

#endif /* _INC_EDITFORMSTUECKEUNDFITTINGE_3FFA9DFA0203_INCLUDED */
