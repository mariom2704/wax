#pragma once

#include "mxliste.h"
#include "EnStueckliste.h"
#include "Werkauftrag.h"


class ROHRKONSTDLL WerkauftragListe : public MxListe
{
public:
	WerkauftragListe();
	virtual ~WerkauftragListe();

	
	EnStueckliste*	getStuecklisteFromElement(const MxIterator& iterator);	//vom aktuellen Element die Stückliste ausgeben
	int				getWaidFromElement(const MxIterator& iterator);			//Waid des aktuellen Werkauftrages
	Werkauftrag		*getWerkauftragFromElement(const MxIterator& iterator); //Werkauftrag des aktuellen Elementes

	//Listen extrahieren
	bool getAllWaNr(CArray<int,int> &ar);				//Alle WA Nummern auslesen
	bool getAllBauabschnitte(CStringArray &ar);		//Alle Bauabschnitte lesen
	bool getAllAuftragsBez(CStringArray &ar);
	bool getAllPlz(CStringArray &ar);
	bool getAllOrte(CStringArray &ar);
	bool getAllNls(CArray<int,int> &ar);
	bool getAllSachbearbeiterNl(CStringArray &ar);
	bool getAllSachbearbeiterOd(CStringArray &ar);
	bool getAllErstelldatum(CStringArray &ar);
	bool getAllBearbeitungsdatum(CStringArray &ar);
	bool getAllWunschliefertermin(CStringArray &ar);
	bool getAllFreigabedatum(CStringArray &ar);
	bool getAllBemerkungen(CStringArray &ar);
	bool getAllPsp(CStringArray &ar);
	bool getAllSdAuftragsNr(CArray<__int64,__int64> &ar);
	bool getAllSdMaterialNr(CArray<int,int> &ar);
	bool getAllSdPositionsNr(CArray<int,int> &ar);
	bool getAllOberflaechen(CStringArray &ar);


	bool getAllRohrDn(CArray<int,int> &ar);
	bool getAllRohrQuali(CArray<int,int> &ar);

	bool getAllMuffenDn(CArray<int,int> &ar);
	bool getAllMuffenID(CArray<int,int> &ar);
	bool getAllMuffenLaenge(CArray<int,int> &ar);
	bool getAllMuffenKehlung(int &wert);
	bool getAllDuchmesser(CArray<double,double> &ar);



	int getAnzahlMuffenVonWaNr(int WaNr);


protected:
	void appendString(const CString &str,CStringArray &ar,bool bCaseSensitiv);
	void appendInt(int wert,CArray<int,int> &ar);
	void appendULong(ULONG wert,CArray<ULONG,ULONG> &ar);
	void appendDouble(double wert,CArray<double,double> &ar);
	void appendInt64u(__int64 wert,CArray<__int64 , __int64> &ar);

private:
	EnStueckliste m_stueckliste;

};
