#include "stdafx.h"


#include "DbAnlage.h"
#include <mxodbc\mxodbc.h>
#include "..\rohrkonst\StringPair.h"

DbAnlage::DbAnlage(DWORD dbhandle): DbBase( dbhandle)
{
	tabAuftraege = "FertAuftraege";
	tabSaegenutRohre = "FertSaegeNutRohre";
	tabSaegenutRohreDaten = "FertSaegeNutRohreDaten";
	tabSaegeNutLoecher = "FertSaegeNutLoecher";
	tabVerteilerRohre = "FertVerteilerRohre";
	tabVerteilerNutAbgang = "FertVerteilerNutAbgang";
}

DbAnlage::~DbAnlage(void)
{
}


MxString DbAnlage::get_tabAuftraege()const
{
	return tabAuftraege;
}

MxString DbAnlage::get_tabSaegenutRohre()const
{
	return tabSaegenutRohre;
}
MxString DbAnlage::get_tabSaegenutRohreDaten()const
{
	return tabSaegenutRohreDaten;
}
MxString DbAnlage::get_tabSaegeNutLoecher()const
{
	return tabSaegeNutLoecher;
}
MxString DbAnlage::get_tabVerteilerRohre()const
{
	return tabVerteilerRohre;
}
MxString DbAnlage::get_tabVerteilerNutAbgang()const
{
	return tabVerteilerNutAbgang;
}



/**
Kopfsatz 
*/
bool DbAnlage::updateKopfsatz(DbBase::MODE modus, kopfsatz &data)
{
	MXTable kopfsatz(dbhandle, tabAuftraege.getString(), false);

	DECLARE_COL(Int,	Wanr,		kopfsatz);
	DECLARE_COL(Int,	Status,		kopfsatz);
	DECLARE_COL(BigInt, SdAuftrag,	kopfsatz);
	DECLARE_COL(Int,    SdPosition,	kopfsatz);
	DECLARE_COL(Int,    SdMaterialNr,kopfsatz);
	DECLARE_COL(String, Ort,		kopfsatz);
	DECLARE_COL(Date,	DatumStatusSaegeNut, kopfsatz);
	DECLARE_COL(Date,	FertDatum, kopfsatz);
	DECLARE_COL(Int,	Anlagentyp, kopfsatz);
	DECLARE_COL(BigInt, KcAuftrag,	kopfsatz);
	DECLARE_COL(Int,    KcPosition,	kopfsatz);

	DatumStatusSaegeNut.set_dateOnly(false);
	
	if (data.wanr <= 0)
	{
		AfxMessageBox("Ungültige WANR!", MB_OK | MB_ICONERROR);
		return false;
	}

	list<StringPair> keys;
	keys.push_back(StringPair("Wanr",MxString("{0}").arg(data.wanr)));

	TRY
	{
		if (modus == DbBase::save)
		{
			//vorhandenen Datensatz evt. löschen
			this->deleteDataset(getFilter(keys), kopfsatz);

			if (!kopfsatz.load(getFilter(keys)))
				return false;
			kopfsatz.addNew();

			Wanr.data = data.wanr;
			Status.data = data.status;

			SdAuftrag.data = data.sdAuftrag;
			SdPosition.data = data.sdPos;
			SdMaterialNr.data = data.sdMaterialNr;

			KcAuftrag.data = _atoi64(data.kcAuftrag);
			KcPosition.data = data.kcPos;

			if (data.ort.isEmpty())
				*Ort.data = " ";
			else
				*Ort.data = data.ort;

			DatumStatusSaegeNut.set_datum(MXDate::GetCurrentTime());
			FertDatum.set_datum(data.fertDate);
			
			Anlagentyp.data = data.anlagentyp;

			kopfsatz.update();
		}
		else
		{
			kopfsatz.load(getFilter(keys));
			if (kopfsatz.isEof())
				return false;
			
			data.wanr = Wanr.data;
			data.status = Status.data;
			data.sdAuftrag = SdAuftrag.data;
			data.sdPos = SdPosition.data;
			data.sdMaterialNr = SdMaterialNr.data;
			data.ort = *Ort.data;	
			data.kcAuftrag = MxString("{0}").arg((double)KcAuftrag.data,0);
			data.kcPos = KcPosition.data;
			data.date = DatumStatusSaegeNut.get_datum();
			data.fertDate = FertDatum.get_datum();
		}
	}
	CATCH(CDBException, e)
	{ 
		// Schwerwiegender Fehler aufgetreten, Daten sind u.U. korrupt
		AfxMessageBox(e->m_strError, MB_OK | MB_ICONERROR);
		return false;
	}
	END_CATCH
	
	return true;
}

/**
Alle Datensätze zu der Wanr aus der Tabelle löschen
*/
bool DbAnlage::deleteDatenWanr(const MxString &tabName, int wanr)
{
	MXTable tab(dbhandle, tabName.getString(), false);
	DECLARE_COL(Int, Wanr, tab);

	list<StringPair> keys;
	keys.push_back(StringPair("Wanr",MxString("{0}").arg(wanr)));

	//vorhandenen Datensatz evt. löschen
	this->deleteDataset(getFilter(keys),tab);

	return true;
}

bool DbAnlage::deleteDatenWaid(const MxString &tabName, int waid)
{
	MXTable tab(dbhandle, tabName.getString(), false);
	DECLARE_COL(Int, Waid, tab);

	list<StringPair> keys;
	keys.push_back(StringPair("Waid",MxString("{0}").arg(waid)));

	//vorhandenen Datensatz evt. löschen
	this->deleteDataset(getFilter(keys),tab);

	return true;
}

/**
Tabelle FertSaegeNutRohre
*/
bool  DbAnlage::updateFertSaegeNutRohre(DbBase::MODE modus, saegeNutRohre data, int &rohrCounter)
{
	MXTable tabSaegeNutRohre(dbhandle, tabSaegenutRohre.getString(), false);

	DECLARE_COL(Int,	Wanr,			tabSaegeNutRohre);
	DECLARE_COL(Int,	RohrCounter,	tabSaegeNutRohre);
	DECLARE_COL(Int,    TeilrohrCounter,tabSaegeNutRohre);
	DECLARE_COL(Int,    Status,			tabSaegeNutRohre);
	DECLARE_COL(Int,    Sortierung,		tabSaegeNutRohre);
	DECLARE_COL(Int,    Waid,			tabSaegeNutRohre);


	list<StringPair> keys;
	

	TRY
	{
		if (modus == DbBase::save)
		{
			int ende = rohrCounter + data.anzahl;
			for (; rohrCounter < ende ; rohrCounter++)
			{
				keys.clear();
				keys.push_back(StringPair("Wanr",MxString("{0}").arg(data.wanr)));
				keys.push_back(StringPair("RohrCounter",MxString("{0}").arg(rohrCounter)));
				keys.push_back(StringPair("TeilrohrCounter",MxString("{0}").arg(data.teilRohrCounter)));

				//vorhandenen Datensatz evt. löschen
				this->deleteDataset(getFilter(keys), tabSaegeNutRohre);

				if (!tabSaegeNutRohre.load(getFilter(keys)))
					return false;
				tabSaegeNutRohre.addNew();

				Wanr.data = data.wanr;
				RohrCounter.data = rohrCounter;
				TeilrohrCounter.data = data.teilRohrCounter;
				Status.data = data.status;
				Sortierung.data = data.sortierung;
				Waid.data = data.waid;

				tabSaegeNutRohre.update();
			}
		}
		else
		{
			tabSaegeNutRohre.load(getFilter(keys));
			if (tabSaegeNutRohre.isEof())
				return false;

			//ToDo

			XASSERT(false);
			
		}
	}
	CATCH(CDBException, e)
	{ 
		// Schwerwiegender Fehler aufgetreten, Daten sind u.U. korrupt
		AfxMessageBox(e->m_strError, MB_OK | MB_ICONERROR);
		return false;
	}
	END_CATCH
	

	//jetzt noch die Rohrdaten in die Tabelle schreiben
	if (!updateFertSaegeNutRohreDaten(modus, data))
		return false;


	return true;
}

bool  DbAnlage::updateFertSaegeNutRohreDaten(DbBase::MODE modus, saegeNutRohre data)
{
	
	MXTable tabSaegeNutRohreDaten(dbhandle, tabSaegenutRohreDaten.getString(), false);

	DECLARE_COL(Int,	Waid,			tabSaegeNutRohreDaten);
	DECLARE_COL(Int,    TeilrohrCounter,tabSaegeNutRohreDaten);
	DECLARE_COL(Int,    Typ,			tabSaegeNutRohreDaten);
	DECLARE_COL(Int,    Ablageplatz,	tabSaegeNutRohreDaten);
	DECLARE_COL(String, PosNr,			tabSaegeNutRohreDaten);
	DECLARE_COL(Int,    Dn,				tabSaegeNutRohreDaten);
	DECLARE_COL(String, Beschriftung,	tabSaegeNutRohreDaten);
	DECLARE_COL(Int,    Laenge,			tabSaegeNutRohreDaten);
	DECLARE_COL(Bool,   NutAnfang,		tabSaegeNutRohreDaten);
	DECLARE_COL(Bool,   NutEnde,		tabSaegeNutRohreDaten);
	DECLARE_COL(Int,    AnzLoecher,		tabSaegeNutRohreDaten);



	list<StringPair> keys;
	keys.push_back(StringPair("Waid",MxString("{0}").arg(data.waid)));
	keys.push_back(StringPair("TeilrohrCounter",MxString("{0}").arg(data.teilRohrCounter)));

	TRY
	{
		if (modus == DbBase::save)
		{
			//vorhandenen Datensatz evt. löschen
			this->deleteDataset(getFilter(keys), tabSaegeNutRohreDaten);

			if (!tabSaegeNutRohreDaten.load(getFilter(keys)))
				return false;
			tabSaegeNutRohreDaten.addNew();

			Waid.data = data.waid;
			TeilrohrCounter.data = data.teilRohrCounter;
			Typ.data = data.typ;
			Ablageplatz.data = data.ablageplatz;
			*PosNr.data = data.posNr;
			Dn.data = data.dn;
			*Beschriftung.data = data.beschriftung;
			Laenge.data = data.laenge;
			NutAnfang.data = data.nutAnfang;
			NutEnde.data = data.nutEnde;
			AnzLoecher.data = data.anzLoecher;

			tabSaegeNutRohreDaten.update();
		}
		else
		{
			tabSaegeNutRohreDaten.load(getFilter(keys));
			if (tabSaegeNutRohreDaten.isEof())
				return false;

			//ToDo

			XASSERT(false);
				
			
		}
	}
	CATCH(CDBException, e)
	{ 
		// Schwerwiegender Fehler aufgetreten, Daten sind u.U. korrupt
		AfxMessageBox(e->m_strError, MB_OK | MB_ICONERROR);
		return false;
	}
	END_CATCH
	

	return true;
}


/**
Tabelle FertSaegeNutLoecher
*/
bool DbAnlage::updateFertSaegeNutLoecher(DbBase::MODE modus, saegeNutLoecher data)
{
	MXTable tabSaegeNutLoecher(dbhandle, tabSaegeNutLoecher.getString(), false);

	DECLARE_COL(Int,	Waid,			tabSaegeNutLoecher);
	DECLARE_COL(Int,    TeilrohrCounter,tabSaegeNutLoecher);
	DECLARE_COL(Int,    LochCounter,	tabSaegeNutLoecher);
	DECLARE_COL(Int,    LochTyp,		tabSaegeNutLoecher);
	DECLARE_COL(Int,    LochDn,			tabSaegeNutLoecher);
	DECLARE_COL(Int,    Abstand,		tabSaegeNutLoecher);
	DECLARE_COL(Int,    Winkel,			tabSaegeNutLoecher);
	DECLARE_COL(Double, Durchmesser,	tabSaegeNutLoecher);
	
	list<StringPair> keys;
	keys.push_back(StringPair("Waid",MxString("{0}").arg(data.waid)));
	keys.push_back(StringPair("TeilRohrCounter",MxString("{0}").arg(data.teilRohrCounter)));
	keys.push_back(StringPair("LochCounter",MxString("{0}").arg(data.lochCounter)));

	TRY
	{
		if (modus == DbBase::save)
		{
			//vorhandenen Datensatz evt. löschen
			this->deleteDataset(getFilter(keys), tabSaegeNutLoecher);

			if (!tabSaegeNutLoecher.load(getFilter(keys)))
				return false;
			tabSaegeNutLoecher.addNew();

			Waid.data = data.waid;
			TeilrohrCounter.data = data.teilRohrCounter;
			LochCounter.data = data.lochCounter;
			LochTyp.data = data.lochTyp;
			LochDn.data = data.lochDn;
			Abstand.data = data.abstand;
			Winkel.data = data.winkel;
			Durchmesser.data = data.durchmesser;

			tabSaegeNutLoecher.update();
		}
		else
		{
			tabSaegeNutLoecher.load(getFilter(keys));
			if (tabSaegeNutLoecher.isEof())
				return false;

			//ToDo

			XASSERT(false);
				
			
		}
	}
	CATCH(CDBException, e)
	{ 
		// Schwerwiegender Fehler aufgetreten, Daten sind u.U. korrupt
		AfxMessageBox(e->m_strError, MB_OK | MB_ICONERROR);
		return false;
	}
	END_CATCH

	return true;
}



bool DbAnlage::updateFertVerteilerRohre(DbBase::MODE modus, verteilerRohre &data)
{
	MXTable fertVerteilerRohre(dbhandle, tabVerteilerRohre.getString(), false);

	DECLARE_COL(Int,	Waid,	fertVerteilerRohre);
	DECLARE_COL(Int,    Wanr,	fertVerteilerRohre);
	DECLARE_COL(String, Posnr,	fertVerteilerRohre);
	DECLARE_COL(Int,    Anzahl,	fertVerteilerRohre);
	DECLARE_COL(Int,    Laenge,	fertVerteilerRohre);
	DECLARE_COL(Int,    Dn,		fertVerteilerRohre);
	DECLARE_COL(Int,    Din,	fertVerteilerRohre);
	DECLARE_COL(Int,    AnzUpos,fertVerteilerRohre);
	DECLARE_COL(String, Kfarbe,	fertVerteilerRohre);


	if (data.waid <= 0)
	{
		AfxMessageBox("Ungültige WAID!", MB_OK | MB_ICONERROR);
		return false;
	}

	list<StringPair> keys;
	keys.push_back(StringPair("Waid",MxString("{0}").arg(data.waid)));

	TRY
	{
		if (modus == DbBase::save)
		{
			//vorhandenen Datensatz evt. löschen
			this->deleteDataset(getFilter(keys), fertVerteilerRohre);

			if (!fertVerteilerRohre.load(getFilter(keys)))
				return false;
			fertVerteilerRohre.addNew();

			Waid.data		= data.waid;
			Wanr.data		= data.wanr;
			if (data.posNr.isEmpty())
				*Posnr.data = " ";
			else
				*Posnr.data		= data.posNr;
			Anzahl.data		= data.anzahl;
			Laenge.data		= data.laenge;
			Dn.data			= data.dn;
			Din.data		= data.din;
			AnzUpos.data	= data.anzUpos;
			if (data.kFarbe.isEmpty())
				*Kfarbe.data = " ";
			else
				*Kfarbe.data	= data.kFarbe;

			fertVerteilerRohre.update();
		}
		else
		{
			fertVerteilerRohre.load(getFilter(keys));
			if (fertVerteilerRohre.isEof())
				return false;
			
			data.waid		= Waid.data;
			data.wanr		= Wanr.data;
			data.posNr		= *Posnr.data;
			data.anzahl		= Anzahl.data;
			data.laenge		= Laenge.data;
			data.dn			= Dn.data;
			data.din		= Din.data;
			data.anzUpos	= AnzUpos.data;
			data.kFarbe		= *Kfarbe.data;	
		}
	}
	CATCH(CDBException, e)
	{ 
		// Schwerwiegender Fehler aufgetreten, Daten sind u.U. korrupt
		AfxMessageBox(e->m_strError, MB_OK | MB_ICONERROR);
		return false;
	}
	END_CATCH
	
	return true;
}

bool DbAnlage::updateFertVerteilerNutAbgang(DbBase::MODE modus, verteilerNutAbgang &data)
{
	MXTable fertVerteilerNutAbgang(dbhandle, tabVerteilerNutAbgang.getString(), false);

	DECLARE_COL(Int,	Waid,			fertVerteilerNutAbgang);
	DECLARE_COL(Int,    Counter,		fertVerteilerNutAbgang);
	DECLARE_COL(String, Posnr,			fertVerteilerNutAbgang);
	DECLARE_COL(Int,    Uposnr,			fertVerteilerNutAbgang);
	DECLARE_COL(Int,    Teilrohrcounter,fertVerteilerNutAbgang);
	DECLARE_COL(Int,    AnzRohre,		fertVerteilerNutAbgang);
	DECLARE_COL(Int,    AnzElemente,	fertVerteilerNutAbgang);
	DECLARE_COL(String, BeschrLi,		fertVerteilerNutAbgang);
	DECLARE_COL(String, BeschrRe,		fertVerteilerNutAbgang);
	DECLARE_COL(Int,    Dn,				fertVerteilerNutAbgang);
	DECLARE_COL(Int,    Din,			fertVerteilerNutAbgang);
	DECLARE_COL(Int,    Laenge,			fertVerteilerNutAbgang);
	DECLARE_COL(String, Anfang,			fertVerteilerNutAbgang);
	DECLARE_COL(String, Ende,			fertVerteilerNutAbgang);
	DECLARE_COL(String, ElementId,		fertVerteilerNutAbgang);
	DECLARE_COL(Int,    ElementDn,		fertVerteilerNutAbgang);
	DECLARE_COL(Int,	AbsAbst,		fertVerteilerNutAbgang);
	DECLARE_COL(Int,	Winkel,			fertVerteilerNutAbgang);
	DECLARE_COL(Int,	AnzSoll,		fertVerteilerNutAbgang);
	DECLARE_COL(Int,	ElementLaenge,	fertVerteilerNutAbgang);


	if (data.waid <= 0)
	{
		AfxMessageBox("Ungültige WAID!", MB_OK | MB_ICONERROR);
		return false;
	}

	MxString keinWert = ".";

	list<StringPair> keys;
	keys.push_back(StringPair("Waid",MxString("{0}").arg(data.waid)));
	keys.push_back(StringPair("Counter",MxString("{0}").arg(data.counter)));

	TRY
	{
		if (modus == DbBase::save)
		{
			//vorhandenen Datensatz evt. löschen
			this->deleteDataset(getFilter(keys), fertVerteilerNutAbgang);

			if (!fertVerteilerNutAbgang.load(getFilter(keys)))
				return false;
			fertVerteilerNutAbgang.addNew();

			Waid.data			= data.waid;
			Counter.data		= data.counter;
			*Posnr.data			= data.posNr;
			Uposnr.data			= data.uPosNr;
			Teilrohrcounter.data = data.teilRohrCounter;
			AnzRohre.data		= data.anzRohre;
			AnzElemente.data	= data.anzElemente;
			if (data.beschrLi.isEmpty())
				*BeschrLi.data	= keinWert;
			else
				*BeschrLi.data	= data.beschrLi;

			if (data.beschrRe.isEmpty())
				*BeschrRe.data  = keinWert;
			else
				*BeschrRe.data	= data.beschrRe;
			Dn.data				= data.dn;
			Din.data			= data.din;
			Laenge.data			= data.laenge;
			*Anfang.data		= data.anfang;
			*Ende.data			= data.ende;
			*ElementId.data		= data.elementId;
			ElementDn.data		= data.elementDn;
			AbsAbst.data		= data.absAbst;
			Winkel.data			= data.winkel;
			AnzSoll.data		= data.anzSoll;
			ElementLaenge.data	= data.elementLaenge;

			fertVerteilerNutAbgang.update();
		}
		else
		{
			fertVerteilerNutAbgang.load(getFilter(keys));
			if (fertVerteilerNutAbgang.isEof())
				return false;
			
			
			//ToDo

			XASSERT(false);
		}
	}
	CATCH(CDBException, e)
	{ 
		// Schwerwiegender Fehler aufgetreten, Daten sind u.U. korrupt
		AfxMessageBox(e->m_strError, MB_OK | MB_ICONERROR);
		return false;
	}
	END_CATCH
	
	return true;
}

//Zu der Wanr die Waids lesen
bool DbAnlage::readWaidsFromSaegeNutRohre(int wanr, list<int> &waids)
{
	MXTable tabSaegeNutRohre(dbhandle, tabSaegenutRohre.getString(), false);

	DECLARE_COL(Int,    Waid,			tabSaegeNutRohre);


	TRY
	{	
		//Datensäteze laden
		MxString sql = MxString("select waid from {0} where wanr={1} group by waid").arg(tabSaegenutRohre).arg(wanr);
		tabSaegeNutRohre.set_sql(sql.getString());
		tabSaegeNutRohre.load();

		//Waids einlesen
		waids.clear();
		while (!tabSaegeNutRohre.isEof())
		{
			waids.push_back(Waid.data);
			tabSaegeNutRohre.moveNext();
		}
	}
	CATCH(CDBException, e)
	{ 
		// Schwerwiegender Fehler aufgetreten, Daten sind u.U. korrupt
		AfxMessageBox(e->m_strError, MB_OK | MB_ICONERROR);
		return false;
	}
	END_CATCH
	
	return true;
}


bool DbAnlage::readAuftraege(int status, list<kopfsatz> &data, MXDate vonDate)
{
	const int FertigSaegeNut = 16;

	MXTable kopfsatz(dbhandle, tabAuftraege.getString(), false);

	DECLARE_COL(Int,	Wanr,		kopfsatz);
	DECLARE_COL(Int,	Status,		kopfsatz);
	DECLARE_COL(BigInt, SdAuftrag,	kopfsatz);
	DECLARE_COL(Int,    SdPosition,	kopfsatz);
	DECLARE_COL(Int,    SdMaterialNr,kopfsatz);
	DECLARE_COL(String, Ort,		kopfsatz);
	DECLARE_COL(Date,	DatumStatusSaegeNut, kopfsatz);
	DECLARE_COL(Date,	FertDatum, kopfsatz);
	DECLARE_COL(Int,	AnlagenTyp, kopfsatz);

	DatumStatusSaegeNut.set_dateOnly(false);

	
	MxString sql = MxString("select wanr,status,sdauftrag,sdposition,sdmaterialnr,ort,DatumStatusSaegeNut,FertDatum,AnlagenTyp from {0} where status={1}")
		                    .arg(tabAuftraege).arg(status);

	


	if (vonDate.is_valid())
	{
		sql += MxString(" and DatumStatusSaegeNut>{0}").arg(vonDate.convDateToInt());
	}

	
	if (status != FertigSaegeNut)
		sql += " order by wanr";
	else
		sql += " order by FertDatum DESC, wanr DESC"; //ASC


	TRY
	{
		kopfsatz.set_sql(sql.getString());
		kopfsatz.load();
	
		while (!kopfsatz.isEof())
		{
			DbAnlage::kopfsatz ks;
		
			ks.wanr = Wanr.data;
			ks.status = Status.data;
			ks.sdAuftrag = SdAuftrag.data;
			ks.sdPos = SdPosition.data;
			ks.sdMaterialNr = SdMaterialNr.data;
			ks.ort = *Ort.data;	
			ks.date = DatumStatusSaegeNut.get_datum();
			ks.fertDate = FertDatum.get_datum();
			ks.anlagentyp = AnlagenTyp.data;
			
			data.push_back(ks);

			kopfsatz.moveNext();
		}
	}
	CATCH(CDBException, e)
	{ 
		// Schwerwiegender Fehler aufgetreten, Daten sind u.U. korrupt
		AfxMessageBox(e->m_strError, MB_OK | MB_ICONERROR);
		return false;
	}
	END_CATCH
	
	
	return true;
}

bool DbAnlage::updateFertDatum(int wanr, MXDate date)
{
	MXTable kopfsatz(dbhandle, tabAuftraege.getString(), false);

	DECLARE_COL(Int,	Wanr,		kopfsatz);
	DECLARE_COL(Date,	FertDatum, kopfsatz);

	MxString sql = MxString("select wanr,FertDatum from {0} where wanr={1}")
		                    .arg(tabAuftraege).arg(wanr);


	TRY
	{
		
		kopfsatz.set_sql(sql.getString());
		kopfsatz.load();

		if (kopfsatz.isEof())
			return false;
			
		kopfsatz.edit();

		FertDatum.set_datum(date);

		kopfsatz.update();
		
	}
	CATCH(CDBException, e)
	{ 
		// Schwerwiegender Fehler aufgetreten, Daten sind u.U. korrupt
		AfxMessageBox(e->m_strError, MB_OK | MB_ICONERROR);
		return false;
	}
	END_CATCH

	return true;
}