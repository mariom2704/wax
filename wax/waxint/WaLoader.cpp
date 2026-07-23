#include "stdafx.h"


#include <../src/mfc/afximpl.h> // für AfxLockGlobals(CRIT_DYNLINKLIST);

#include "../waxprj/WaxPart.h"
#include <mxodbc/mxodbc.h>


#include "WaLoader.h"
using namespace ServerInterface;
using namespace std;

void WaLoader::set_user(const string& userval) 
{
	/**
	Aus dem Usernamen wird eine temporäre Tabelle erzeugt über die das weitere Laden läuft.
	Alle Leerzeichen im Usernamen werden durch Leerzeichen ersetzt, damit der Tabellenname
	gültig bleibt.
	*/

	user = userval;
	algorithm::replace_all(user, " ", "_");
}

WaLoader::RETVAL WaLoader::prepare_load(BOOL ForceWAX_GID, BOOL ReadOnlyRecordsWithGUID)
{
	if (!Server::checkServerDB().GetLength())
		return NO_SERVER_CONNECT;
	
	int count = load_waids(ForceWAX_GID, ReadOnlyRecordsWithGUID);
	if (!count)
		return SELECT_RETURNED_NULL;
	return OK;
}

vector<WaxPart*> WaLoader::load_objects()
{
	vector<WaxPart*> wplist;
	for (vector<WaIdentity>::iterator it = waidlst.begin(); it != waidlst.end(); it++)
	{
		WaxPart *pt = load_object(it->waid);
		if (!pt)
			continue;
		wplist.push_back(pt);
		
		serverdb.updateLeitkarte(pt->GetRuntimeClass()->m_lpszClassName);
		serverdb.updateAnhang();
		serverdb.updateMaterial();
		try
		{
			serverdb.updateRohrleitung();
			serverdb.updateElemente();
			serverdb.updateObject();
		}
		catch (Element::Error perror)
		{
			// Elementfehler werden später sowieso noch angezeigt
		}
//		serverdb.updatePreise();
		serverdb.updateSoko();
		::SendMessage((HWND)parent, progmsg, (WPARAM)pt, 0); // SendMessage WM_STEPIT
	}
	return wplist;
}


//Lädt Werkaufträge, wie in der SQL Anweisung angegeben. Dazu wird eine 
//temporäre Tabelle erstellt
// Diese Methode wird nicht nur zum Einlesen der Aufträge benutzt,
// sondern auch bei meine Aufträge usw. daher der Schlater ReadGUIDFile
int WaLoader::load_waids(BOOL ReadGUIDFile, BOOL ReadOnlyRecordsWithGUID)
{
	

	sapWittenbergeAuftragsdaten.readDatei();
	CStringArray stringArray;
	sapWittenbergeAuftragsdaten.findEintragReadAllGUID(stringArray);


	MXTable newOrder(serverdb.get_dbhandle(), "tmpTable");

	// Tabellenfelder definieren
	DECLARE_COL(Long, waid, newOrder);
	DECLARE_COL(BigInt, sdauftragsnr, newOrder);
	DECLARE_COL(String, GUID, newOrder);

	string sql = "SELECT werkauftrag.waid, werkauftrag.sdauftragsnr, werkauftrag.GUID ";
	sql +=		 "FROM werkauftrag INNER JOIN status ON werkauftrag.waid = status.waid ";

	if (select.length() || !load_storno || status > 0)
		sql += " WHERE ";

	if (select.length())
		sql += select;
	if (!load_storno)
	{
		if (select.length())
			sql += " AND";
		sql += " NOT status.storniert";
	}

	if (status > 0)
	{
		if (select.length() || !load_storno)
			sql += " AND";

		CString str;
		if (status == Status::Freigegeben)
			//Aus Kompatibilität zu alten Datensätzen auch nach Status is null prüfen
			str.Format(" (status.status=%i or status.status is null)", status);
		else 
			str.Format(" status.status=%i", status);
		
		sql += str;	
	}

	newOrder.set_sql(sql);
	
	// Tabelle öffnen und Daten laden
	newOrder.load();

	
	waidlst.clear();
	std::vector<WaIdentity> waidlstLocal = std::vector<WaIdentity>();
	while (!newOrder.isEof())
	{
		string temp = *GUID.data; 
		if(ReadGUIDFile == FALSE)
		{
			temp = "";
		}

		if(!temp.empty())
		{
			for( int h=0; h < stringArray.GetSize(); h++)
			{	string temp = *GUID.data;	
				CString DataEntry = stringArray[h];
				if(temp.compare(DataEntry) == 0)
				{
					WaIdentity waIdentity = WaIdentity();
					waIdentity.waid			= waid.data;
					waIdentity.sdauftragsnr = sdauftragsnr.data;
					waIdentity.GUID			= *GUID.data;
					waidlstLocal.push_back(waIdentity);
				}
			}
		}
		else
		{
			if(ReadOnlyRecordsWithGUID == FALSE)
			{
				WaIdentity waIdentity = WaIdentity();
				waIdentity.waid			= waid.data;
				waIdentity.sdauftragsnr = sdauftragsnr.data;
				waIdentity.GUID			= *GUID.data;
				waidlst.push_back(waIdentity);
			}
		}
		newOrder.moveNext();
	}

	for(std::vector<WaIdentity>::iterator it = waidlstLocal.begin(); it != waidlstLocal.end(); ++it) 
	{
		waidlst.push_back(*it);
	}


	return waidlst.size();
}



//Lädt aus der Tabelle Werkauftrag einen Datensatz, erstellt ein 
//WaxPart und setzt die Leitkartendaten
WaxPart* WaLoader::load_object(int thewaid)
{
	WaxPart* pt = NULL;
	TRY
	{
		MXTable wa(serverdb.get_dbhandle(), "werkauftrag");

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
		DECLARE_COL(Int, sdauftragsnr, wa);
		DECLARE_COL(Int, sdmaterialnr, wa);
		DECLARE_COL(Int, sdpositionsnr, wa);
		DECLARE_COL(Date, wunschliefertermin, wa);
		DECLARE_COL(Int, vk3, wa);
		DECLARE_COL(Int, beschtyp, wa);
		DECLARE_COL(Int, zulassungsstelle, wa);
		DECLARE_COL(Int, beschral, wa);
		DECLARE_COL(String, beschbez, wa);
		DECLARE_COL(String, GUID, wa);
		BlobCol bemerkung("bemerkung", 5000);
		wa.attach(&bemerkung);


		

		zulassungsstelle.data;
		
		string select = str( format("waid = %1%") % thewaid );
		wa.load(select.c_str(), sort, true);
		
		if (wa.get_count() != 1)
		{
			errors.insert(make_pair(thewaid, "Nicht gefunden"));
			return NULL;
		}
		
		pt = dyncreate_object((LPCSTR)*classid.data);
		if (!pt)
			return NULL;
		
		Leitkarte &leitkarte = pt->get_leitkarte();

		leitkarte.set_prj_bauAbschnitt(*bauabschnitt.data);
		leitkarte.set_prj_auftragsbez(*auftragsbez.data);
		leitkarte.set_prj_bauOrt(*bauort.data);
		leitkarte.set_prj_bauPlz(*bauplz.data);
		leitkarte.set_prj_nl(nl.data);
		
		leitkarte.set_sachbearbeiterNL(*sachbearbeiternl.data);
		leitkarte.set_wunschliefertermin(wunschliefertermin.get_datum());
		leitkarte.set_vk3(vk3.data ? true : false);
		
		if (bezeichnung.data->GetLength() && *bezeichnung.data != pt->get_werkauftrag()->get_stdbezeichnung())
			pt->get_werkauftrag()->set_bezeichnung(*bezeichnung.data);
		pt->get_werkauftrag()->get_beschichtung().set_typ(beschtyp.data);
		pt->get_werkauftrag()->get_beschichtung().set_ral(beschral.data);
		pt->get_werkauftrag()->get_beschichtung().set_bezeichnung(*beschbez.data);
		pt->get_werkauftrag()->set_bemerkung(bemerkung.getString());
		pt->get_werkauftrag()->set_zulassungsstelle(zulassungsstelle.data);

		leitkarte.set_prj_pspElement(*pspelement.data);
		leitkarte.set_sdAuftragsnr(sdauftragsnr.data);
		leitkarte.set_zulassungsnummer(zulassungsstelle.data);
		leitkarte.set_sdPositionsnr(sdpositionsnr.data);
		leitkarte.set_sdMaterialnummer(sdmaterialnr.data);
		//leitkarte.set_sdPositionsnr(sdpositionsnr.data);
		pt->get_werkauftrag()->set_waid(thewaid);
		pt->get_werkauftrag()->set_GUID(*GUID.data);
	
		serverdb.SetGpSapWittenbergeAuftragsdaten(&sapWittenbergeAuftragsdaten);
		serverdb.set_werkauftrag(pt->get_werkauftrag());
		
	}
	CATCH(CDBException, e)
	{
		TCHAR   ErrorMessage[512];
		e->GetErrorMessage(ErrorMessage, _countof(ErrorMessage));

		errors.insert(make_pair<UINT, string>(thewaid, ErrorMessage));
		delete pt;
		return NULL;
	}
	END_CATCH
	return pt;
}


//Durchsucht die Liste mit CObject-Klassen und erzeugt eine neue 
//Instanz eines WaxParts
WaxPart* WaLoader::dyncreate_object(const string& name)
{
	/*
	Erzeugen eines WaxPart Objektes 
	Dieser Teil ist aus den MFC Sourcen kopiert. Alle Objekte, die mit IMPLEMENT_DYNCREATE 
	ausgestattet sind, werden in eine Liste eingetragen. Diese Liste gibt es für jedes DLL. Die
	DLLs sind wiederum in einer Liste eingetragen. Man muss also nur beide Listen durchsuchen, bis
	man den Namen der Klasse findet und dann aus dem CRuntime Object CreateObject aufrufen. 
	*/
	WaxPart *pt = NULL;

	AFX_MODULE_STATE* pModuleState = AfxGetModuleState();			// search classes in shared DLLs
	CRuntimeClass* pClass;
	AfxLockGlobals(CRIT_DYNLINKLIST);
	for (CDynLinkLibrary* pDLL = pModuleState->m_libraryList; pDLL != NULL;	pDLL = pDLL->m_pNextDLL)
	{
		for (pClass = pDLL->m_classList; pClass != NULL;
			pClass = pClass->m_pNextClass)
		{
			if (lstrcmpA(name.c_str(), pClass->m_lpszClassName) == 0)
			{
				pt = (WaxPart*)pClass->CreateObject();
				AfxUnlockGlobals(CRIT_DYNLINKLIST);
				return pt;
			}
		}
	}
	AfxUnlockGlobals(CRIT_DYNLINKLIST);
	return pt;
}

//Daten aus der Statustabelle lesen und im WaxPart setzen
bool WaLoader::update_status(WaxPart* pt)
{
	/**
	Liest den Status. Es werden keine Daten geändert. Abgeleitete Methoden setzen den Status auch
	*/
	if (!serverdb.updateStatus())
	{
		string error = str( format( "Kein Eintrag in Statustabelle für Werkauftrag %1%") % (LPCSTR)pt->get_werkauftrag()->get_leitkarte().get_prj_auftragsbez() );
		errors.insert(make_pair(pt->get_werkauftrag()->get_waid(), error));
		return false;
	}
	return true;
}


