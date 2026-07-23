// Copyright (C) 2001 Minimax GmbH

#if defined (_MSC_VER) && (_MSC_VER >= 1000)
#pragma once
#endif
#ifndef _INC_APAXEINGABEFORM_3E479F0C018A_INCLUDED
#define _INC_APAXEINGABEFORM_3E479F0C018A_INCLUDED

class WAXUIDLL ApaxEingabeForm : 
public ArbeitEingabeForm  
  
{
	DECLARE_DYNCREATE(ApaxEingabeForm)
public:
	ApaxEingabeForm();

	virtual ~ApaxEingabeForm();

	//{{AFX_VIRTUAL(ApaxEingabeForm)
	//}}AFX_VIRTUAL

protected:
	virtual bool editAfo();
	virtual BOOL OnInitDialog();

	//{{AFX_MSG(ApaxEingabeForm)
	
	DECLARE_MESSAGE_MAP()
};

#endif /* _INC_APAXEINGABEFORM_3E479F0C018A_INCLUDED */
