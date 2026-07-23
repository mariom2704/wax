#pragma once

#include <mxcontrols/binaryarchive.h>
#include "mxdate.h"
#include "Preisanfrage.h"

/** 
Daten der Leitkarte
Diese Klasse definiert die Daten der allgemeinen Leitkarte. Die Leitkarte 
enthält die Daten des einzelnen WA.
Die Memberattribute beginnend mit prj_ sind für ein NL Projekt quasi statisch. 
'Quasi' deshalb, weil sie durch das Wax
synchron gehalten werden müssen. Dies gilt aber nur für ein Projekt in einer 
NL, nicht für die AV. 
Lädt die AV die WAs aus der Datenbank, so sind die Attribute nicht statisch, 
sie sind für jeden WA unterschiedlich. 

Unterschiede NL - AV

- Projekt in NL hat eine Projektleitkarte. Bei jeder Änderung der Daten in der 
Projektleitkarte, werden alle WAs geändert. Bei bereits freigebenen WAs ist es 
komplizierter. Hier muss dafür gesorgt werden, dass entweder die Leitkarte 
nicht geändert wird oder der WA nochmals freigeben wird. 

- Beim Öffnen der Datenbank in der AV wird ein Projekt generiert, dass keine 
Projektleitkarte besitzt. Es kann somit jede Leitkarte einzeln bearbeitet werden.
*/
class ROHRKONSTDLL Leitkarte 
{
public:
	friend class PreisanfrageForm;
	friend class PrjLeitkarteForm;
	friend class BauAbschnittAVSammelauftragGliederungForm;
	friend class SammelAuftragForm;
	friend class ServerDBPSPElement;
	friend class AskDifferentWerkauftraege;
	/// Konstruktor
	Leitkarte();

	/// Destruktor
	virtual ~Leitkarte();

	enum sdMaterialnummer
	{
		NichtGesetzt				= 0,
			
		VerteilerGepulvert			= 400010,
		VerteilerVerzinkt			= 400011,
		CO2VerteilerGepulvert		= 400015,
		CO2VerteilerVerzinkt		= 400016,

		StrangrohrGepulvertVerzinkt = 400020,	
		
		GewinderohreGepulvert		= 400031,
		GewinderohreVerzinkt		= 400030,

		ApaxGepulvert				= 400040,
		ApaxVerzinkt				= 400041,// @todo evtl. nachpflegen 400015, 400016, 400032, 400033, 0,
	};

	inline const CString get_ippeKnotenNr()const;
	inline void set_ippeKnotenNr(CString value);
	inline const CString get_ippeMatNr() const;
	inline void set_ippeMatNr(CString value);
	inline const bool get_IsChoosenIppeNumbers()const;

	inline void set_IsChoosenIppeNumbers(bool value);
	
	/// Prüft die Angaben in der Leitkarte auf Vollständigkeit
	CString checkVars(bool checksap = true, bool checkSd = true) const;

	/// Setzt den Wunschliefertermin auf heute + Systemparameter "LIEFERFRIST".
	void init_wunschliefertermin();

	/// / Plausibilitätsüberprüfung
	CString check() const;

	/// Kopiert die Daten der Leitkarte, aber nicht die Statusdaten wie z.B. 
	/// Freigabedatum
	void copy(const Leitkarte& other);

	/// Kopiert alle Daten der Leitkarte, wird für das Setzen der Leitkarte eines 
	/// Sammelauftrages benötigt
	void clone(const Leitkarte& other);

	/// Liefert einen formatierten String mit dem PSP Element zur Ausgabe 
	/// in z.B. Drucklisten
	CString getPSPElement();


	/// Serialisierung der statischen Attribute
	void serializeStatics(MxFilesystem::BinaryArchive& ar);

	/// Serialisierung
	virtual void serialize(MxFilesystem::BinaryArchive& ar);

	bool setPrjdata(const Leitkarte& other);

	inline const CString& get_prj_bauAbschnitt() const;

	inline void set_prj_bauAbschnitt(const CString& value);

	inline const int get_prj_produktlinie() const;

	inline void set_prj_produktlinie(int value);

	inline const CString& get_prj_pspElement() const;

	inline void set_prj_pspElement(const CString& value);

	inline const int get_prj_nl() const;

	inline const int get_Brandschutztechnik() const;

	inline void set_Brandschutztechnik(int brandschutztechnik);

	inline void set_prj_nl(int value);

	inline const CString& get_prj_bauOrt() const;

	inline void set_prj_bauOrt(const CString& value);

	inline const CString& get_prj_bauPlz() const;

	inline void set_prj_bauPlz(const CString& value);

	inline const CString& get_prj_auftragsbez() const;

	inline void set_prj_auftragsbez(const CString& value);

	inline const CString& get_sachbearbeiterNL() const;

	inline const int get_zulassungsnummer() const;

	inline void set_sachbearbeiterNL(const CString& value);

	inline void set_zulassungsnummer(const int value);

    inline const CString& get_emailNl() const;

	inline void set_emailNl(const CString& value);

	inline const __int64& get_sdAuftragsnr() const;

	inline void set_sdAuftragsnr(const __int64& value);

	inline const CString& get_kcAuftragsnr() const;

	inline void set_kcAuftragsnr(const CString& value);

	int get_sdMaterialnummer() const;

	void set_sdMaterialnummer(int value);

	inline const UINT& get_sdPositionsnr() const;

	inline void set_sdPositionsnr(const UINT& value);

	inline const UINT& get_kcPositionsnr() const;

	inline void set_kcPositionsnr(const UINT& value);

	inline const MXDate& get_wunschliefertermin() const;

	inline void set_wunschliefertermin(const MXDate& value);

	inline const bool get_vk3() const;

	inline void set_vk3(const bool value);

	inline const CString& get_prj_COauftrag() const;


	inline void set_prj_COauftrag(const CString& value);

	inline Preisanfrage& get_preisanfrage();

	inline const Preisanfrage& get_preisanfrage()const;


private:

	CString m_ippeMatNr;
	CString m_ippeKnotenNr;
	bool	m_IsChoosenIppeNr;

	int m_Brandschutztechnik;

	/// Es kann statt eines PSP Elementes auch ein CO Auftrag angegeben werden
	CString prj_COauftrag;

	/// Bauabschnitt
	CString prj_bauAbschnitt;

	/// Abrechnung nach VK3 oder Aufwand
	bool vk3_abrechnung;
	
	/// Produktlinie kann 11 (Wasser) oder 21 (Spezial) sein.
	int prj_produktlinie;

	/// Niederlassungskennung
	int prj_nl;

	/// Postleitzahl der Baustelle
	CString prj_bauPlz;

	/// Ort der Baustelle
	CString prj_bauOrt;

	/// PSP Element des Projektes
	CString prj_pspElement;

	/// Auftragsbezeichnung
	CString prj_auftragsbez;

	/// Besteller / Sachbearbeiter
	CString sachbearbeiterNL;

	int zulassungsnummer;

	CString emailNl;

	/// SD Auftragsnummer
	__int64 sdAuftragsnr;

	/// SD Positionsnummer
	UINT sdPositionsnr;

	/// KC Auftragsnummer (Kundencenter=Werk)
	CString kcAuftragsnr;

	/// SD Positionsnummer
	UINT kcPositionsnr;

	/// SD Materialnummer
	int sdMaterialnummer;

	/// Wunschliefertermin
	MXDate wunschliefertermin;

	Preisanfrage preisanfrage;
};



inline const int Leitkarte::get_Brandschutztechnik() const
{
 return m_Brandschutztechnik;
}


inline void Leitkarte::set_Brandschutztechnik(int brandschutztechnik)
{
	m_Brandschutztechnik = brandschutztechnik;
}







inline const CString& Leitkarte::get_prj_pspElement() const
{
	return prj_pspElement;
}

inline void Leitkarte::set_prj_pspElement(const CString& value)
{
	prj_pspElement = value;
}

inline const int Leitkarte::get_prj_nl() const
{
	return prj_nl;
}

inline void Leitkarte::set_prj_nl(int value)
{
	prj_nl = value;
}

inline const int Leitkarte::get_prj_produktlinie() const
{
	return prj_produktlinie;
}

inline void Leitkarte::set_prj_produktlinie(int value)
{
	prj_produktlinie = value;
}

inline const CString& Leitkarte::get_prj_bauOrt() const
{
	return prj_bauOrt;
}

inline void Leitkarte::set_prj_bauOrt(const CString& value)
{
	prj_bauOrt = value;
}

inline const CString& Leitkarte::get_prj_bauPlz() const
{
	return prj_bauPlz;
}

inline void Leitkarte::set_prj_bauPlz(const CString& value)
{
	prj_bauPlz = value;
}

inline const CString& Leitkarte::get_prj_auftragsbez() const
{
	return prj_auftragsbez;
}

inline void Leitkarte::set_prj_auftragsbez(const CString& value)
{
	prj_auftragsbez = value;
}

inline const CString& Leitkarte::get_sachbearbeiterNL() const
{
	return sachbearbeiterNL;
}



inline void Leitkarte::set_sachbearbeiterNL(const CString& value)
{
	sachbearbeiterNL = value;
}

inline const int Leitkarte::get_zulassungsnummer() const
{
	return zulassungsnummer;
}



inline void Leitkarte::set_zulassungsnummer(const int value)
{
	zulassungsnummer = value;
}



inline const CString Leitkarte::get_ippeKnotenNr() const
{
 	return m_ippeKnotenNr;
}


inline void Leitkarte::set_ippeKnotenNr(CString value)
{
	m_ippeKnotenNr = value;
}



inline const bool Leitkarte::get_IsChoosenIppeNumbers() const
{
 	return m_IsChoosenIppeNr;
}



inline void Leitkarte::set_IsChoosenIppeNumbers(bool value)
{
	m_IsChoosenIppeNr = value;
}



inline const CString Leitkarte::get_ippeMatNr() const 
{
 	return m_ippeMatNr;
}


inline void Leitkarte::set_ippeMatNr(CString value)
{
	m_ippeMatNr = value;
}


inline const __int64& Leitkarte::get_sdAuftragsnr() const
{
	return sdAuftragsnr;
}


inline void Leitkarte::set_sdAuftragsnr(const __int64& value)
{
	sdAuftragsnr = value;
}



inline const CString& Leitkarte::get_kcAuftragsnr() const
{
	return kcAuftragsnr;
}



inline void Leitkarte::set_kcAuftragsnr(const CString& value)
{
	kcAuftragsnr = value;
}



inline const UINT& Leitkarte::get_sdPositionsnr() const
{
	return sdPositionsnr;
}


inline void Leitkarte::set_sdPositionsnr(const UINT& value)
{
	sdPositionsnr = value;
}




inline const UINT& Leitkarte::get_kcPositionsnr() const
{
	return kcPositionsnr;
}

inline void Leitkarte::set_kcPositionsnr(const UINT& value)
{
	kcPositionsnr = value;
}


inline const MXDate& Leitkarte::get_wunschliefertermin() const
{
	return wunschliefertermin;
}

inline void Leitkarte::set_wunschliefertermin(const MXDate& value)
{
	wunschliefertermin = value;
}

inline const bool Leitkarte::get_vk3() const
{
	return vk3_abrechnung;
}

inline void Leitkarte::set_vk3(const bool value)
{
	vk3_abrechnung = value;
}

const CString& Leitkarte::get_prj_bauAbschnitt() const
{
	return prj_bauAbschnitt;
}

void Leitkarte::set_prj_bauAbschnitt(const CString& value)
{
	prj_bauAbschnitt = value;
}


inline const CString& Leitkarte::get_prj_COauftrag() const
{
	return prj_COauftrag;
}

inline void Leitkarte::set_prj_COauftrag(const CString& value)
{
	prj_COauftrag = value;
}


inline Preisanfrage& Leitkarte::get_preisanfrage()
{
	return preisanfrage;
}

	
inline const Preisanfrage& Leitkarte::get_preisanfrage()const
{
	return preisanfrage;
}



inline const CString& Leitkarte::get_emailNl() const
{
	return emailNl;
}

inline void Leitkarte::set_emailNl(const CString& value)
{
	emailNl = value;
}
