#pragma once

#include "ElementeEingabe.h"

class VerteilerData;

/**
Klasse zur Eingabe von Verteilerdaten
Diese Klasse implementiert eine Eingabemaske für den Verteiler
*/
class WAXUIDLL VerteilerEingabe : public WaxPartForm  
{
	DECLARE_DYNCREATE(VerteilerEingabe)
	DECLARE_MESSAGE_MAP()
public:
	/// Konstruktor
	VerteilerEingabe();

	/// Destruktor
	virtual ~VerteilerEingabe();

	/// Plausibilitätscheck, wird aufgerufen, bevor 
	/// UpdateData(TRUE) aufgerufen wird.
	virtual bool check_plausis();

	/// Dialog initialisieren
	virtual BOOL OnInitDialog();

private:
	CString fertigungshinweis;

	int rohrEnde;

	int rohrAnfang;

	int rohrKFarbe;

	int rohrQuali;

	int rohrLaenge;

	int rohrDN;

	int schnittlaenge;

	int rohrVerb;

	bool m_Ueberlaenge;


protected:
	/// Sorgt für ein Auslesen der Dialogelemente und Neuerzeugen der Grafik, wenn 
	/// check_plausis true liefert.
	virtual void commitUpdate();

	Rohrleitung* rohr;

	virtual void DoDataExchange(CDataExchange* pDX);

	void OnSelendokVerteiler_rohr_verb();
	
	void OnChangeVerteiler_rohr_laenge();
	void OnSelchangeVerteiler_rohr_dn();
	void onSelchangeVerteiler_rohr_anfang();
	afx_msg void OnBnClickedCheckUeberlaenge();
};
