#pragma once

#include "Arbeitsplan.h"
#include "Beschichtung.h"
#include "EnCObListModell.h"
//#include "..\WAXPRJ\Maschinenlisten.h"



/**
Aus dem Rohrmodell den Arbeitsplan erzeugen. 
In dieser Klasse sollten nach und nach alle Prozesse zur Erzeugung des Arbeitsplanes abgelegt werden

Hinweis:
Zur Zeit wird noch der Rohrtyp übergeben, damit für nur diesen Typ die Afos erzeugt werden. Zukünftig sollten
automatisch allein aus dem Modell die richtigen Afos erzeugt werden.


ArbeitsplanGewinde sollte eigentlich in waxprj bei den anderen Gp-Klassen liegen. Da ArbeitsplanGewinde aber 
aus den Rohrklassen aufgerufen wird, funktioniert es wegen der Referenzierung nicht.
*/
class ROHRKONSTDLL ArbeitsplanGewinde
{
public:

	///für welchen Rohrtyp der Ap erzeugt werden soll (siehe Hinweis in Klassenbeschreibung)
	enum ROHRTYP
	{
		TYPNOTSET	= -1,
		VERTEILER	= 0,
		STRANGROHR	= 1,
		GEWINDEROHR = 2		
	};


	
	
	///Dichtmittel Gewinderohr
	enum DICHTMITTEL 
	{
		DICHTMITTELNOTSET = -1,
		OHNE		= 0,
		LOCTITE		= 1,
		HANF		= 2,
		TEFLON		= 3,
		SYNTHESOL	= 4,
		NEOFERMIT   = 5
	};

	ArbeitsplanGewinde(void);
	virtual ~ArbeitsplanGewinde(void);

	Werkauftrag* m_pGewinderohr;
	void createExtraAutoArbeitSaegen(RohrlisteAuftrag* RohrlisteAuftrag);

	void FilterFertigugAfos(int iAnzahl);
	//notwendigen Werte für die Erstellung des Arbeitsplanes setzen
	void set_Werte(Werkauftrag* pRohr, ArbeitsplanGewinde::ROHRTYP typ,Beschichtung &be,EnCObListModell &modell, bool bUseArbeitsplanForVk3 = false);

	//Gewinderohrwerte
	void set_Gewinderohrwerte(ArbeitsplanGewinde::DICHTMITTEL dichtmittel);

	//Arbeitsplan erstellen
	bool createArbeitsplan(Arbeitsplan &ap);

	//Afos im Arbeitsplan mit Menge des Bauteils multiplzieren
	bool setGesamtAnzahl(int anzahl);

	void Set_UseArbeitsplanForVk3()
	{
		m_bUseArbeitsplanForVk3 = true;
	}
	
	bool Get_UseArbeitsplanForVk3()
	{
		return m_bUseArbeitsplanForVk3;
	}

private:

//Attribute
	Arbeitsplan			*m_pArbeitsplan;

	Beschichtung		*m_pBeschichtung;

	EnCObListModell		*m_pModell;				// Rohrmodell mit Elementen

	bool				m_bUseArbeitsplanForVk3; // Für Vk3 wird jedes Rohr mit einer Säge gerechnet

	ArbeitsplanGewinde::ROHRTYP		m_rohrtyp;		// Rohrtyp

	ArbeitsplanGewinde::DICHTMITTEL	m_dichtmittel;	// Dichtmittel (muß nur bei Gewinderohr gesetzt werden)

//Methoden allgemein
	bool createArbeitsplanGewinderohr();		// Afos für Gewinderohr
	
	bool isSet();								// Prüfen ob Arbeitsplan, Beschichtung und Modell gesetzt sind

	int getOffsetAfoGewinderohr();				// Offset bei Gewinderohr Afos bei Dichtmittel Hanf+Kitt


//Methoden für die Erzeugung der einzelnen Arbeitsfolgen

	//Gewinderohr Afos
	bool setAfo_RohrSaegen_GewindeSchneiden();		// Afo 4000, 4010, 4200 und 4210
	bool setAfo_RohrleitungMitFittingAufdichten();	// Afo 4020-4070 und 4220-4270
	bool setAfo_FittingAufFitting_InRohrachse();	// Afo 4090 und 4290 
	bool setAfo_FittingAufFitting_Abgang();			// Afo 4080 und 4280
	bool setAfo_VNutEinrollenGewinderohr();			// Afo 4015 und 4215


	//Rohr Afos
	
};
