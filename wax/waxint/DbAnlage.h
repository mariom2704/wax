#pragma once
#include "..\waxprj\dbbase.h"


/**
Tabellen für die Schnittstellen zu den Fertigungsmaschinen füllen. 
- Sägenutanlage
- Verteilerrohranlage
*/
class DbAnlage :
	public DbBase
{
public:
	DbAnlage(DWORD dbhandle);
	~DbAnlage(void);

	
	//Tabellennamen
	MxString get_tabAuftraege()const;
	MxString get_tabSaegenutRohre()const;
	MxString get_tabSaegenutRohreDaten()const;
	MxString get_tabSaegeNutLoecher()const;
	MxString get_tabVerteilerRohre()const;
	MxString get_tabVerteilerNutAbgang()const;

	///Kopfdaten für FertAuftraege
	struct kopfsatz
	{
		int wanr;
		int status;
		int anlagentyp;
		__int64 sdAuftrag;
		int sdPos;
		int sdMaterialNr;
		MxString kcAuftrag;
		int kcPos;
		MXDate date;
		MXDate fertDate;
		MxString ort;
	};
	bool updateKopfsatz(DbBase::MODE modus, kopfsatz &data);

	
	struct saegeNutRohre		
	{
		int wanr;
		int waid;
		int teilRohrCounter;
		int anzahl;
		int status;
		int sortierung;
		int typ;
		int ablageplatz;
		MxString posNr;
		int dn;
		MxString beschriftung;
		int laenge;
		bool nutAnfang;
		bool nutEnde;
		int anzLoecher;
	};
	bool updateFertSaegeNutRohre(DbBase::MODE modus, saegeNutRohre data, int &rohrCounter);
	bool updateFertSaegeNutRohreDaten(DbBase::MODE modus, saegeNutRohre data);

	struct saegeNutLoecher
	{
		int waid;
		int teilRohrCounter;
		int lochCounter;
		int lochTyp;
		int lochDn;
		int abstand;
		int winkel;
		double durchmesser; //zur Zeit ohne Verwendung
	};
	bool updateFertSaegeNutLoecher(DbBase::MODE modus, saegeNutLoecher data);

	struct verteilerRohre
	{
		int waid;
		int wanr;
		MxString posNr;
		int anzahl;
		int laenge;
		int dn;
		int din;
		MxString kFarbe;
		int anzUpos;
	};
	bool updateFertVerteilerRohre(DbBase::MODE modus, verteilerRohre &data);

	struct verteilerNutAbgang
	{
		int waid;
		int counter;
		MxString posNr;
		int uPosNr;
		int teilRohrCounter;
		int anzRohre;
		int anzElemente;
		MxString beschrLi;
		MxString beschrRe;
		int dn;
		int din;
		int laenge;
		MxString anfang;
		MxString ende;
		MxString elementId;
		int elementDn;
		int absAbst;
		int winkel;
		int anzSoll;
		int elementLaenge;
	};
	bool updateFertVerteilerNutAbgang(DbBase::MODE modus, verteilerNutAbgang &data);

    bool deleteDatenWaid(const MxString &tabName, int waid);
	bool deleteDatenWanr(const MxString &tabName, int wanr);

	
	bool readWaidsFromSaegeNutRohre(int wanr, list<int> &waids);

	bool readAuftraege(int status, list<kopfsatz> &data, MXDate vonDate);

	bool updateFertDatum(int wanr, MXDate date);

	private:
	//Tabellennamen
	MxString tabAuftraege,
		     tabSaegenutRohre,
			 tabSaegenutRohreDaten,
			 tabSaegeNutLoecher,
			 tabVerteilerRohre,
			 tabVerteilerNutAbgang;
};
