// Copyright (C) 2001 Minimax GmbH

#if defined (_MSC_VER) && (_MSC_VER >= 1000)
#pragma once
#endif
#ifndef _INC_PRJANHANGFORM_3C0360B002BD_INCLUDED
#define _INC_PRJANHANGFORM_3C0360B002BD_INCLUDED

#include "WaxPartForm.h"

//Dialog zur Eingabe eines Anhanges
class WAXPRJDLL PrjAnhangForm : 
public WaxPartForm  
{
	DECLARE_DYNCREATE(PrjAnhangForm)

public:
	//virtuelle Methode, nach einer Prüfung zurückliefert, ob der Tab eine 
	//Kennzeichnung erhalten soll. Siehe PrjAnhangForm.
	virtual int get_highlight() const;



	PrjAnhangForm();

	virtual ~PrjAnhangForm();

	//{{AFX_VIRTUAL(PrjAnhangForm)
	//}}AFX_VIRTUAL

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();

	//{{AFX_MSG(PrjAnhangForm)
	//Indicates the user clicked a button
	 void onClickedAnhang_einfuegen();
	//Indicates the user clicked a button
	 void onClickedAnhang_einsehen();
	//Indicates the user clicked a button
	 void onClickedAnhang_loeschen();

	 void onShowAnhang();
	
	DECLARE_MESSAGE_MAP()
private:
	static CString m_strCurrentPath;
};

#endif /* _INC_PRJANHANGFORM_3C0360B002BD_INCLUDED */
