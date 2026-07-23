// Copyright (C) 2001 Minimax GmbH

#if defined (_MSC_VER) && (_MSC_VER >= 1000)
#pragma once
#endif
#ifndef _INC_APAXEDITARBEITSFOLGE_3E49025F030D_INCLUDED
#define _INC_APAXEDITARBEITSFOLGE_3E49025F030D_INCLUDED



class ApaxEditArbeitsfolge : 
public EditArbeitsfolge  
  
{
private:

	//Ids der Eingabefelder für die Berechnung der Zeit
	class ItemID 
	{
	public:
		int dnvl;

		UINT element;

		void set(UINT menge, UINT element, UINT dn, UINT laenge, UINT dnvl    );

		UINT menge;

		UINT dn;

		UINT laenge;

	};
public:
	ApaxEditArbeitsfolge(Arbeitsfolge &arbeitsfolge, CWnd *parent);

	virtual ~ApaxEditArbeitsfolge();

	//{{AFX_VIRTUAL(ApaxEditArbeitsfolge)
	public:
	virtual BOOL OnInitDialog();

	virtual void OnOK();

	virtual void DoDataExchange(CDataExchange* pDX);
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(ApaxEditArbeitsfolge)
	//Zeitberechnung durchführen
	 void onClickedZeitberechnen();
	//Indicates a new combo box list item is selected
	 void onSelchangeElement();

	 void onKillfocusEinzelzeit();

	 virtual void loadTexte();
	
	DECLARE_MESSAGE_MAP()
private:
	//Sucht aus der Tabelle  Arbeitsfolgen_zeiten die passenden DNs für das 
	//übergebene Element heraus und schreibt sie in die Auswahlbox.
	void initEleDn(int eleid = 0);

	//Fügt anhand der Afo ID die dazugehörigen Elemente in die Elemente Combobox
	void addElemente();

	//Prüft anhand der Afo ID welche Eingabefelder möglich sind
	void enableItems();

	ItemID itemIds;

};

#endif /* _INC_APAXEDITARBEITSFOLGE_3E49025F030D_INCLUDED */
