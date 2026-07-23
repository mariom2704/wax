#pragma once
#include "..\waxprj\gpbase.h"
#include "GpWerkauftrag.h"

/**
Erzeugung der Schnittstellendatei für die Strangrohrstrasse
*/
class GpUebergabeStrangrohrstrasse :
	public GpWerkauftrag
{
public:
	GpUebergabeStrangrohrstrasse(void);
	virtual ~GpUebergabeStrangrohrstrasse(void);


	
	virtual bool writeUebergabeDaten();							//Werkaufträge in Excelübergabedatei schreiben

	virtual bool writeUebersicht();								//Werkaufträge in einer Html Datei ausgeben

	void create_Leerzeile(WaxHtmlFile& w);

	bool create_strangrohrliste(WaxHtmlFile& w,  Strangrohr*  strang, int& anzahlPositionen, int& anzahlMuffen, int& wanr, Werkauftrag* wa );
	
	void createSum(WaxHtmlFile& w, int anzahlPositionen, int anzahlMuffen );
	virtual bool checkPlausis();								//checkPlausi für Strohrstr

	virtual bool checkWerkauftragOk(Werkauftrag *wa);			//prüfen ob Werkauftrag für Strangrohrstr. ok ist

	virtual bool appendWerkauftraege(const CUIntArray &arNewWa); //Werkaufträge zu den WA Nummern aus der DB lesen und an interne Liste hängen

	virtual bool deleteOldUebergabeWerte();						//Alte Exceldateien löschen

	const CString &get_gesPfadUebersichtsDatei()const;

protected:

	CString getRohrAnfangEndeElementAsString(int eleId);

	bool setKopfwerte();

	CString makeGesPfad(const CString &pfad,const CString& waNr,const CString &ext);

	const CString& setGesPfadUebergabeDatei(const CString &stWaNr);			//Pfad + Datei für Übergabedatei (Exceldatei)

	const CString& setGesPfadUebersichtsDatei(const CString &stWaNr);	//Pfad + Datei für Übersichtsdatei (Html Datei)

private:

	//Kopfdaten
	CString		m_stWaNr,
				m_stBauabschnitt,
				m_stAuftragsBez,
				m_stPlz,
				m_stOrt,
				m_stSachbearbeiter,
				m_stSachbearbeiterOd,
				m_stFreigabeDat,
				m_stWunschLieferDat,
				m_stBearbeitungDat,
				m_stErstellungDat,
				m_stSdAuftrag,
				m_stSdMaterial,
				m_stSdPos,
				m_stPsp,
				m_stBemerkung,
				m_stNl,
				m_dnMuffe,
				m_stVariabel,
				m_stGesPfadUebersichtsdatei,
				m_stGesPfadUebergabedatei;

	int			m_dnRohr,
				m_muffenlaenge,
				m_muffenkehlung,
				m_rohrQuali;

	bool		m_isMultiWA; //Wenn mehrere Werkaufträge zusammengeasst werden = true


};
