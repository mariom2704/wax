#pragma once

#include "SpanxRohrDialog.h"




class WAXUIDLL SpanxStrangrohrDialog : public SpanxRohrDialog
{
	DECLARE_MESSAGE_MAP()
public:
	SpanxStrangrohrDialog(CWnd *parent, const string& data);
	virtual ~SpanxStrangrohrDialog();

	virtual string xml_data();


protected:

	int prallblech_anzahl;
	string m_PrallblechArtikelNr;

	CComboBox *cb_Prallblechvorauswahl;
	CComboBox *cb_Prallbleche;
	
	void init_attr(const string& data);

	virtual string format_langtext();

	/// Daten aus Eingabefeldern lesen
	virtual void DoDataExchange(CDataExchange* pDX);

	/// Dialog initialisieren
	virtual void init_ui();
	
	/// Rohrtyp-Enum für Zugriff auf Vortexte
	virtual int get_rohrtyp() const;
	
	/// Rohrtyp-Enum für Zugriff auf Montagezeiten
	virtual int get_rohrtyp_montage() const;

	/// XML Tag für die Datensatzkennung
	virtual string get_xmltag();

	
	virtual void initRohrAnfangEnde();

	virtual void initElemente();

	virtual void initNennweite();
	
	virtual void initSchellenVorauswahl();

	virtual void initPrallblechVorauswahl();
	
	virtual void  initSprinklerVorauswahl();

	virtual double get_montagezeit() const;

	/// Wird bei DN Änderungen aufgerufen, bei Strangrohr werden auch die Sprinkler gesetzt
	virtual void updateDN();
	void selChangeSprinklerVorauswahl();
	void selChangePrallblechVorauswahl();

	void update_Anzahl_Sprinkler_with_Keystroke_Test();
	void update_Anzahl_Prallbleche_with_Keystroke_Test();

private:
	/// Sprinklerpreis setzen (bis 1/2 Zoll 2,5 danach 3,5 Euro)
	void set_sprinklerpreis();
	void set_Prallblechpreis();
	Strangrohr strang;

		
	/// Update der Muffendn 
	void check_muffe();
	

	int muffenRelAbst[Strangrohr::MAX_ANZAHL_STDELEMENTE+1];
};

