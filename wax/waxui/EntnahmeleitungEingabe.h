// Copyright (C) 2001 Minimax GmbH

#if defined (_MSC_VER) && (_MSC_VER >= 1000)
#pragma once
#endif
#ifndef _INC_ENTNAHMELEITUNGEINGABE_3E2D4E61008C_INCLUDED
#define _INC_ENTNAHMELEITUNGEINGABE_3E2D4E61008C_INCLUDED

//Eingabe der Daten der Entnahmeleitung
class WAXUIDLL EntnahmeleitungEingabe : public WaxPartForm  
{
private:
	//Enabled oder disabled die Radiobuttons für die Seitenangabe der Entleerung
	void enableEntleerungSeite(bool enable);

	DECLARE_DYNCREATE(EntnahmeleitungEingabe)


public:
	EntnahmeleitungEingabe();

	virtual ~EntnahmeleitungEingabe();

	virtual BOOL OnInitDialog();

	//Plausibilitätscheck, wird aufgerufen, bevor UpdateData(TRUE) aufgerufen 
	//wird.
	virtual bool check_plausis();


protected:
	virtual void DoDataExchange(CDataExchange* pDX);

	//Ohne Entleerung angeklickt, Entleerung auf Seite muss abgeschaltet oder 
	//angeschaltet werden.
	 void onClickedOhne();
	//Indicates the user clicked a button
	 void onClickedEntleerung_rechts();
	//Indicates the user clicked a button
	 void onClickedEntleerung_links();
	//Indicates the user clicked a button
	 void onClickedTypA();
	//Indicates the user clicked a button
	 void onClickedTypB();
	//Indicates the user clicked a button
	 void onClickedRohrnippel();

	DECLARE_MESSAGE_MAP()
};

#endif /* _INC_ENTNAHMELEITUNGEINGABE_3E2D4E61008C_INCLUDED */
