#include "stdafx.h"
#include "resource.h"

#include <mxodbc\mxodbc.h>



#ifdef _DEBUG
#define new DEBUG_NEW
#endif


static WaxStamm gStammdaten;

WaxStamm &getStammdaten()
{
	/**
	Globales Stammdatenobjekt abfragen
	Es gibt in der Komponente WaxStamm ein Objekt der Klasse WaxStamm. Dieses Objekt wird 
	zur Startzeit erzeugt  und enthält alle Stammdaten, die permanent im Speicher verfügbar sein sollen.
	Initialisiert wird dieses Objekt in CWaxApp::InitInstance
	*/

	return gStammdaten;
}

WaxStamm::WaxStamm() : queryBrennzugabe(rohrstutzen_brennzugabe.get_lst())
, querySondermaterial(sondermaterial.get_lst())
, queryBauelemente(bauelemente.get_lst())
, queryFormstueckeFittinge(formstueckefittinge.get_lst())
{
	/**
	Initialisiert tbllist mit den zu ladenden Stammdatentabellen
	*/
	tbllst.push_back(&texte);
	tbllst.push_back(&profile);
	tbllst.push_back(&RAL);
	tbllst.push_back(&bauelementgruppen);
	tbllst.push_back(&beschichtungen);
	tbllst.push_back(&niederlassungen);
	tbllst.push_back(&brandschutztechniken);
	tbllst.push_back(&m_SpanxPreiseArtikelstamm);
	tbllst.push_back(&bauelemente);
	tbllst.push_back(&formstueckefittinge);
	tbllst.push_back(&arbeitsfolgen);
	tbllst.push_back(&abzugradius);
	tbllst.push_back(&dnzuaussendurchmesser);
	tbllst.push_back(&afozeiten);
	tbllst.push_back(&bauelementeafos);
	tbllst.push_back(&preisfaktoren);
	tbllst.push_back(&pulvernzeiten);
	tbllst.push_back(&aussendurchmesserwandstaerke);
	tbllst.push_back(&arbeitsplaetze);
	tbllst.push_back(&kostenstellen);
	tbllst.push_back(&dummymaterialnummern);
	tbllst.push_back(&zmass);
	tbllst.push_back(&sokos);
	tbllst.push_back(&montagezeiten);
	tbllst.push_back(&rohrtexte);
	tbllst.push_back(&sondermaterial);
	tbllst.push_back(&krantaetigkeit);
	tbllst.push_back(&profilearbeitsfolgen);
	tbllst.push_back(&zulassung);
	tbllst.push_back(&profilesaegezeiten);
	tbllst.push_back(&putzenzeiten);
	tbllst.push_back(&verzinkung);
	tbllst.push_back(&sapartikelstamm);
	tbllst.push_back(&vk3preise);
	tbllst.push_back(&rohrstutzen_brennzugabe);
	tbllst.push_back(&biegung);
}


WaxStamm::~WaxStamm()
{
}


void WaxStamm::loadData(const string& database, CProgressCtrl *prog )
{
	/**
	Hier werden alle Stammdaten, die permanent im Speicher gehalten werden sollen, geladen
	Es wird ein ProgressCtrl im Hauptfenster erzeugt, wenn prog == NULL ist. Dies kann vorkommen, wenn die
	Datenbank nicht gestartet ist.
	Nach dem Laden wird über reset_database() die temporäre Datei wieder gelöscht
	*/
	StammDat::get_database(database); 
			
	if (prog) prog->SetStep(1);
	unsigned long datanz = tbllst.size();
	if (prog) prog->SetRange32(0, datanz > 0 ? datanz :0);
	
	vector<StammDat*>::iterator it;
	string failed_tables;


	CString exception;
	for (it = tbllst.begin(); it != tbllst.end(); it++)
	{
		TRY
		{
			// Name der Tabelle im Progressindikator setzen
			if (prog) 
			{
				::SetWindowLongPtr(prog->m_hWnd, GWLP_USERDATA, (long)(*it)->get_tbl().c_str());
				prog->GetParent()->RedrawWindow();
			}
			
			(*it)->unload();
			(*it)->load(database);
			
			if (prog) prog->StepIt();
		}
		CATCH(CDBException, e)
		{
			if (exception != e->m_strError)
			{
				int iWhere = e->m_strError.Find(".");
				if(iWhere > -1)
				{
					e->m_strError.Delete(iWhere+1, 1);
					e->m_strError.Insert(iWhere+1, "\n" );
					
				}

				failed_tables += (string)e->m_strError;	
				exception = e->m_strError;
			}

			if (it != tbllst.end() && *it)				
				failed_tables += (*it)->get_tbl() + (string)"\n";			
			else
				failed_tables += "\\n";
			
		}
		CATCH_ALL(e)
		{
			e->ReportError();
			failed_tables += (string)"Allgemeiner Fehler\n";
		}
		END_CATCH_ALL
	}
	
	StammDat::reset_database();
	if (!failed_tables.length())
		return; // alles ok

	// Wenn Fehler bei Laden der Stammdaten aufgetreten sind, kann das Wax irgendwo abstürzen oder
	// Müll fabrizieren. 
	CString str; str = getStammdaten().getText("IDS_STAMMDATENFEHLER").c_str();
	CString error2;
	error2 = getStammdaten().getText("IDS_ERROR_STAMMDATEN").c_str();
	failed_tables += (string)(LPCSTR)error2;
	if (::MessageBox(NULL, (LPCSTR)failed_tables.c_str(), str, MB_YESNO | MB_ICONERROR | MB_TOPMOST | MB_APPLMODAL) == IDYES)
		AfxThrowUserException(); // Programm abbrechen
}

int WaxStamm::zoll2dn(const CString & zoll)
{
	/**
	Umrechnungsmethode von DN zu Zoll Angaben
	*/
	const CString zo[] = { "1/4", "3/8", "1/2", "3/4", "1", "1,1/4", "1,1/2", "2", "2,1/2", "3", "4", "5", "6", "8", "10", "12", "16", "" };
	const int dn[]	   = {   8,     10,   15,    20,    25,   32,     40,     50,   65,      80, 100, 125, 150, 200,  250, 300,  400, 0 };

	for (int i = 0; dn[i]; i++)
		 if (zo[i] == zoll)
			 return dn[i];
	TRACE("Ungültige Zollangabe %s\n", zoll);
	return -1;
}



CString WaxStamm::dn2zoll(int dn, int typ)
{
	/**
	Umrechnungsmethode von DN zu Zoll Angaben
	*/
	switch ( dn ) 
	{
		case   8 :	return (typ == 0) ? "1/4"	: "14";
		case  10 :	return (typ == 0) ? "3/8"	: "38";
		case  15 :	return (typ == 0) ? "1/2"	: "12";
		case  20 :	return (typ == 0) ? "3/4"	: "34";
		case  25 :	return "1";
		case  32 :	return (typ == 0) ? "1.1/4"	: "114";
		case  40 :	return (typ == 0) ? "1.1/2" : "112";
		case  50 :	return "2";
		case  65 :	return (typ == 0) ? "2.1/2"	: "212";
		case  80 :	return  "3";
		case 100 :	return  "4";
		case 125 :	return  "5";
		case 150 :	return  "6";
		case 200 :	return  "8";
		case 250 :	return  "10";
		case 300 :	return  "12";
		case 400 :	return  "16";
	}
	return "????";
}

CString WaxStamm::dn2str(bool conv2zoll, int dn, int typ)
{
	CString ret;
	if (conv2zoll)
		ret.Format("%s''", dn2zoll(dn, typ));
	else
		ret.Format("%d", dn);
	return ret;
}

CString WaxStamm::getNLBezeichnung(int nl)
{
	/**
	Liefert zu einer NL Kennung die NL Bezeichnung
	*/
	for (POSITION pos = niederlassungen.get_lst().GetHeadPosition(); pos != NULL;)
	{
		Niederlassungen::data *nldat = (Niederlassungen::data*)(niederlassungen.get_lst().GetNext(pos));
		if (nldat->get_nlnummer() == nl)
		{
			return nldat->get_bezeichnung();
		}
	}
	return (CString)"";
}

CString WaxStamm::getBaugruppenBezeichnung(int id)
{
	for (POSITION pos = bauelementgruppen.get_lst().GetHeadPosition(); pos != NULL;)
	{
		BauelementGruppen::data *eledat = (BauelementGruppen::data*)(bauelementgruppen.get_lst().GetNext(pos));
		if (eledat->get_elem_ID() == id)
		{
			return eledat->get_bezeichnung();
		}
	}
	return (CString)"????";
}


CString WaxStamm::getMaterialeinheit(int artikelnr)
{
	for (POSITION pos = sapartikelstamm.get_lst().GetHeadPosition(); pos != NULL;)
	{
		SapArtikelstamm::data *eledat = (SapArtikelstamm::data*)(sapartikelstamm.get_lst().GetNext(pos));
		if (eledat->get_artikelnr() == artikelnr)
		{
			return eledat->get_einheit();
		}
	}
	return (CString)"";
}


CString WaxStamm::getSapArtikelbezeichnung(int artikelnr)
{
	for (POSITION pos = sapartikelstamm.get_lst().GetHeadPosition(); pos != NULL;)
	{
		SapArtikelstamm::data *eledat = (SapArtikelstamm::data*)(sapartikelstamm.get_lst().GetNext(pos));
		if (eledat->get_artikelnr() == artikelnr)
		{
			return eledat->get_bezeichnung();
		}
	}
	return (CString)"";
}

void WaxStamm::initNL_ComboBox(CWnd *cb) const
{
	XASSERT(cb);
	if (!cb)
		return;
	CComboBox *cbnl = (CComboBox*)cb;
	XASSERT(cbnl);

	CString str;
	for (POSITION pos = get_niederlassungen().GetHeadPosition(); pos != NULL;)
	{
		Niederlassungen::data *nl = (Niederlassungen::data*)(get_niederlassungen().GetNext(pos));
		
		str.Format("%s (%d)",nl->get_bezeichnung(),nl->get_nlnummer());
		int index = cbnl->AddString(str);
		cbnl->SetItemData(index, (DWORD)nl->get_nlnummer());
	}
}


const Verzinkung& WaxStamm::get_verzinkung() const
{
	return verzinkung;
}


const CObList& WaxStamm::get_SpanxPreiseArtikelstamm() const
{
	return m_SpanxPreiseArtikelstamm.get_lst();
}


const CObList& WaxStamm::get_bauelemente() const
{
	return bauelemente.get_lst();
}


const CObList& WaxStamm::get_texte() const
{
	return texte.get_lst();
}

const CObList& WaxStamm::get_formstueckefittinge() const
{
	return formstueckefittinge.get_lst();	
}

const CObList& WaxStamm::get_beschichtungen() const
{
	return beschichtungen.get_lst();
}

const CObList& WaxStamm::get_niederlassungen() const
{
	return niederlassungen.get_lst();
}

const CObList& WaxStamm::get_Brandschutztechniken() const
{
	return brandschutztechniken.get_lst();
}

const CObList& WaxStamm::get_bauelementgruppen() const
{
	return bauelementgruppen.get_lst();
}


const CObList& WaxStamm::get_sapartikelstamm() const
{
	return sapartikelstamm.get_lst();
}


const CObList& WaxStamm::get_ral() const
{
	return RAL.get_lst();
}

const CObList& WaxStamm::get_arbeitsfolgen() const
{
	return arbeitsfolgen.get_lst();
}

const CObList& WaxStamm::get_abzugradius() const
{
	return abzugradius.get_lst();
}

const CObList& WaxStamm::get_dnzuaussendurchmesser() const
{
	return dnzuaussendurchmesser.get_lst();
}

const CObList& WaxStamm::get_afozeiten() const
{
	return afozeiten.get_lst();
}

const CObList& WaxStamm::get_bauelementeafos() const
{
	return bauelementeafos.get_lst();
}

const CObList& WaxStamm::get_preisfaktoren() const
{
	return preisfaktoren.get_lst();
}

const CObList& WaxStamm::get_pulvernzeiten() const
{
	return pulvernzeiten.get_lst();
}

const CObList& WaxStamm::get_aussendurchmesserwandstaerke() const
{
	return aussendurchmesserwandstaerke.get_lst();
}

const CObList& WaxStamm::get_kostenstellen() const
{
	return kostenstellen.get_lst();
}

const CObList& WaxStamm::get_arbeitsplaetze() const
{
	return arbeitsplaetze.get_lst();
}


const CObList& WaxStamm::get_biegung() const
{
	return biegung.get_lst();
}

const CObList& WaxStamm::get_dummymaterialnummern() const
{
	return dummymaterialnummern.get_lst();
}

const CObList& WaxStamm::get_zmass() const
{
	return zmass.get_lst();
}

const CObList& WaxStamm::get_sokos() const
{
	return sokos.get_lst();
}

const CObList& WaxStamm::get_profile() const
{
	return profile.get_lst();
}

int WaxStamm::get_zmassval(FormstueckeUndFittinge::GRUPPENID  id, int dn, int dnvl)
{
	return zmass.get_val(id, dn, dnvl);
}

const VK3Preise& WaxStamm::get_vk3preise() const
{
	return vk3preise;
}


const CObList& WaxStamm::get_rohrtexte() const
{
	return rohrtexte.get_lst();
}

const CObList& WaxStamm::get_montagezeiten() const
{
	return montagezeiten.get_lst();
}

const CObList& WaxStamm::get_sondermaterial() const
{
	return sondermaterial.get_lst();
}

const CObList& WaxStamm::get_krantaetigkeit() const
{
	return krantaetigkeit.get_lst();
}

const CObList& WaxStamm::get_profilearbeitsfolgen() const
{
	return profilearbeitsfolgen.get_lst();
}

const CObList& WaxStamm::get_zulassung() const
{
	return zulassung.get_lst();
}

const CObList& WaxStamm::get_profilesaegezeiten() const
{
	return profilesaegezeiten.get_lst();
}

const CObList& WaxStamm::get_putzenzeiten() const
{
	return putzenzeiten.get_lst();
}


double WaxStamm::getWandstaerke(int elemId,int dn)
{
	const CObList& grp = getStammdaten().get_aussendurchmesserwandstaerke();
	for (POSITION gpos = grp.GetHeadPosition(); gpos;)
	{
		AussendurchmesserWandstaerke::data *g = (AussendurchmesserWandstaerke::data *)grp.GetNext(gpos);
		if (g->get_dn() == dn && g->get_elemid() == elemId) 
		{
			return g->get_wandstaerke();
		}
	}

	return 0;
}

const std::string& WaxStamm::getText(const std::string& id, bool nullIfNotFound)
{
	return texte.getText(id, nullIfNotFound);
}

std::string WaxStamm::getTextWithColon(const std::string& id)
{
	std::string str = texte.getText(id) + ":";
	return str;
}