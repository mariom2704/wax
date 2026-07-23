// Copyright (C) 2001 by Minimax GmbH

#if defined (_MSC_VER) && (_MSC_VER >= 1000)
#pragma once
#endif
#ifndef _INC_APAXPOSITION_412B059A0232_INCLUDED
#define _INC_APAXPOSITION_412B059A0232_INCLUDED

class WAXUIDLL ApaxPosition : public WaxPartForm
{
	DECLARE_DYNCREATE(ApaxPosition)

public:
	ApaxPosition();

	virtual ~ApaxPosition();

	//Plausibilitätscheck, wird aufgerufen, bevor UpdateData(TRUE) aufgerufen wird.
	virtual bool check_plausis();

	virtual BOOL OnInitDialog();
	

protected:
	//DDX/DDV-Unterstützung
	virtual void DoDataExchange(CDataExchange* pDX);

	ApaxWerkauftrag* getApaxWerkauftrag();

	DECLARE_MESSAGE_MAP()


	


};

#endif /* _INC_APAXPOSITION_412B059A0232_INCLUDED */
