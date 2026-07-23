// Copyright (C) 2001 by Minimax GmbH

#if defined (_MSC_VER) && (_MSC_VER >= 1000)
#pragma once
#endif
#ifndef _INC_GROHRANLAGEEINGABE_3DAA616201AD_INCLUDED
#define _INC_GROHRANLAGEEINGABE_3DAA616201AD_INCLUDED

class GewinderohrData;

class WAXUIDLL GRohranlageEingabe : public WaxPartForm  
{


public:
	void onUpdate();
	

	virtual ~GRohranlageEingabe();

	GRohranlageEingabe();

	/// Plausibilitätscheck, wird aufgerufen, bevor UpdateData(TRUE) aufgerufen wird.
	virtual bool check_plausis();

	virtual BOOL OnInitDialog();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);

	Gewinderohr* getGewinderohr();

	
	DECLARE_MESSAGE_MAP()

private:
	CString fertigungshinweis;

	bool enableOK;
	
	int dichtmittel;

	int typ;

	int oldtyp;

	int rohrKFarbe;

	int rohrQuali;


	GewinderohrData* rohr;


	int m_iAuswaertsfertigung;

	DECLARE_DYNCREATE(GRohranlageEingabe)

public:
	afx_msg void OnBnClickedRadioEis();
};

#endif /* _INC_GROHRANLAGEEINGABE_3DAA616201AD_INCLUDED */
