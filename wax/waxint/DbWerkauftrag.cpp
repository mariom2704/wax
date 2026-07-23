#include "stdafx.h"


#include "WaLoader.h"
#include "DbWerkauftrag.h"
#include <mxodbc\mxodbc.h>

using namespace MxErrorHandling;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


DbWerkauftrag::DbWerkauftrag(void)
:DbBase(NULL), m_serverdb(Server::load, NULL, NULL)
{
}



DbWerkauftrag::~DbWerkauftrag(void)
{
}



/**
Alle Waids aus der Datenbank lesen. 
*/
bool DbWerkauftrag::readAllWaids(CUIntArray &arWaid)
{
	return readWaids("storniert=0",arWaid);
}


/**
Waids zu der WA Nummer aus der Db Lesen
*/
bool DbWerkauftrag::readWaids(int waNr,CUIntArray &arWaid)
{
	//Select Statement
	CString select;
	select.Format("storniert=0 and wanr=%d",waNr);

	return readWaids(select,arWaid);
}

/**
Waids aus der Db anhand der Where Bedingung lesen
*/
bool DbWerkauftrag::readWaids(const CString &select,CUIntArray &arWaid)
{
	arWaid.RemoveAll();
	
	m_serverdb.set_modus(DbBase::load);

	//Tabelle definieren
	MXTable wa(m_serverdb.get_dbhandle(), "status");
	DECLARE_COL(Long, waid, wa);

	
	//Tabelle laden
	wa.load((LPCSTR)select, "waid", false);
	
	while (!wa.isEof())
	{
		arWaid.Add(waid.data);
		wa.moveNext();
	}

	return true;
}




/**
 zu der Waid den Werkauftrag aus der DB erzeugen
*/
Werkauftrag* DbWerkauftrag::readWerkauftrag(int werkauftragId)
{
	CString stFehler = "Fehler bei Waid=%d, Tabelle: %s",
		    stTemp;

	MxError error;

	Werkauftrag *werkauftrag = NULL;


	TRY
	{
		m_serverdb.set_modus(Server::load);
		MXTable wa(m_serverdb.get_dbhandle(), "werkauftrag");

		DECLARE_COL(Long, waid, wa);
		DECLARE_COL(String, classid, wa);
		DECLARE_COL(String, bezeichnung, wa);
		DECLARE_COL(String, bauabschnitt, wa);
		DECLARE_COL(String, auftragsbez, wa);
		DECLARE_COL(String, bauort, wa);
		DECLARE_COL(String, bauplz, wa);
		DECLARE_COL(Int, nl, wa);
		DECLARE_COL(String, sachbearbeiternl, wa);
		DECLARE_COL(String, pspelement, wa);
		DECLARE_COL(BigInt, sdauftragsnr, wa);
		DECLARE_COL(Long, sdmaterialnr, wa);
		DECLARE_COL(Long, sdpositionsnr, wa);
		DECLARE_COL(Date, wunschliefertermin, wa);
		DECLARE_COL(Int, zulassungsstelle, wa);
		DECLARE_COL(Int, vk3, wa);
		DECLARE_COL(Int, anzahl, wa);
		DECLARE_COL(String, posnr, wa);
		DECLARE_COL(Int, beschtyp, wa);
		DECLARE_COL(Int, beschral, wa);
		DECLARE_COL(String, beschbez, wa);
		BlobCol bemerkung("bemerkung", 5000);
		wa.attach(&bemerkung);

		
		CString select;
		select.Format("waid = %i", werkauftragId);
		wa.load(select, NULL, false);
		
		if (wa.isEof())
		{
			stTemp.Format(stFehler,werkauftragId,"werkauftrag");
			error.setErrorDescription((LPCSTR)stTemp, MxError::Critical);
			appendError(error);
			return NULL;
		}
		
		// Klassenname für den Werkauftrag erstellen
		CString classnm;
		
		WaxPart *pt = ServerInterface::WaLoader::dyncreate_object((LPCSTR)*classid.data);
		XASSERT(pt);
		if (!pt)
			return NULL;
			
		//Pointer auf Werkauftrag
		werkauftrag = pt->detach_werkauftrag();
		delete pt; pt = NULL;

		XASSERT(werkauftrag);
		if (!werkauftrag)
			return false;
		
		//Werte für Leitkarte setzen
		Leitkarte &leitkarte = werkauftrag->get_leitkarte();

		leitkarte.set_prj_bauAbschnitt(*bauabschnitt.data);
		leitkarte.set_prj_auftragsbez(*auftragsbez.data);
		leitkarte.set_prj_bauOrt(*bauort.data);
		leitkarte.set_prj_bauPlz(*bauplz.data);
		leitkarte.set_prj_nl(nl.data);
		
		leitkarte.set_sachbearbeiterNL(*sachbearbeiternl.data);
		leitkarte.set_zulassungsnummer(zulassungsstelle.data);
		leitkarte.set_wunschliefertermin(wunschliefertermin.get_datum());
		leitkarte.set_vk3(vk3.data ? true : false);
		leitkarte.set_prj_pspElement(*pspelement.data);
		leitkarte.set_sdAuftragsnr(sdauftragsnr.data);
		leitkarte.set_sdPositionsnr(sdpositionsnr.data);
		leitkarte.set_sdMaterialnummer(sdmaterialnr.data);
		leitkarte.set_sdPositionsnr(sdpositionsnr.data);

		//Werte für Werkauftrag setzen
		if (bezeichnung.data->GetLength() && *bezeichnung.data != werkauftrag->get_stdbezeichnung())
			werkauftrag->set_bezeichnung(*bezeichnung.data);
		werkauftrag->get_beschichtung().set_typ(beschtyp.data);
		werkauftrag->get_beschichtung().set_ral(beschral.data);
		werkauftrag->get_beschichtung().set_bezeichnung(*beschbez.data);
		werkauftrag->set_bemerkung(bemerkung.getString());
		werkauftrag->set_waid(werkauftragId);
		werkauftrag->set_posNr(*posnr.data);
		werkauftrag->set_anzahl(anzahl.data);
		werkauftrag->set_zulassungsstelle(zulassungsstelle.data);


		//in Datenbankobjekt den Werkauftrag speichern und weitere Werte für Werkauftrag laden
		m_serverdb.set_werkauftrag(werkauftrag);

		m_serverdb.updateAnhang();
		m_serverdb.updateMaterial();
		m_serverdb.updateRohrleitung();
		m_serverdb.updateElemente();
		m_serverdb.updateObject();
//        m_serverdb.updatePreise();
		m_serverdb.updateSoko();
		m_serverdb.updateStatus();
	}
	CATCH(CDBException, e)
	{
		e->ReportError();
		return werkauftrag;
	}
	END_CATCH
	return werkauftrag;

}


