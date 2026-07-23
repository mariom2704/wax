#include "stdafx.h"


#include "..\waxdef.h"
#include "resource.h"

#include <mxcontrols/msgbox.h>
#include "..\rohrkonst\MxError.h"
using namespace MxGui;
#include <xml\xmlparser\xml.h>


	#include "SpanxRohrDialog.h"
		#include "SpanxStrangrohrDialog.h"




#ifdef _DEBUG
#define new DEBUG_NEW
#endif



BEGIN_MESSAGE_MAP(SpanxStrangrohrDialog, SpanxRohrDialog)
	ON_CBN_SELCHANGE(IDC_SPRINKLER_VORAUSWAHL, selChangeSprinklerVorauswahl)
	ON_CBN_SELCHANGE(IDC_SPRINKLER, set_sprinklerpreis)
	ON_EN_CHANGE(IDC_ANZ_SPRINKLER, update_Anzahl_Sprinkler_with_Keystroke_Test)
	ON_EN_KILLFOCUS(IDC_ANZ_SPRINKLER, update_preis)

	ON_CBN_SELCHANGE(IDC_PRALLBLECH_VORAUSWAHL, selChangePrallblechVorauswahl)
	ON_CBN_SELCHANGE(IDC_PRALLBLECH, set_Prallblechpreis)
	ON_EN_CHANGE(IDC_ANZ_PRALLBLECH, update_Anzahl_Prallbleche_with_Keystroke_Test)
	ON_EN_KILLFOCUS(IDC_ANZ_PRALLBLECH, update_preis)

	ON_CBN_SELCHANGE(IDC_MUFFEN_ID, check_muffe)
END_MESSAGE_MAP()




SpanxStrangrohrDialog::SpanxStrangrohrDialog(CWnd *parent, const string& data) : SpanxRohrDialog(parent, IDD_SPANX_STRANGROHR, data) 
{
	werkauftrag = &strang;
	max_elements = Strangrohr::MAX_ANZAHL_STDELEMENTE;
	prallblech_anzahl = 0;

	if (muffen_typ != BOHR24)
		schellen_anzahl = 0;
	if (xmldata.length())
	{
		init_attr(xmldata.c_str());
	}
}



SpanxStrangrohrDialog::~SpanxStrangrohrDialog()
{}



void SpanxStrangrohrDialog::init_attr(const string& data)
{
	SpanxRohrDialog::init_attr(data);

	string tag_Prallblech= xml_read_string("Prallblech", data.c_str());
	prallblech_anzahl = xml_read_double("anzahl", tag_Prallblech.c_str());
	m_PrallblechArtikelNr = xml_read_string("typ", tag_Prallblech.c_str());
}


void SpanxStrangrohrDialog::update_Anzahl_Sprinkler_with_Keystroke_Test()
{
	CString strTemp; 
	GetDlgItem(IDC_ANZ_SPRINKLER)->GetWindowText(strTemp);

	if(strTemp.IsEmpty())
		return;
	
	update_preis();
};



void SpanxStrangrohrDialog::update_Anzahl_Prallbleche_with_Keystroke_Test()
{
	CString strTemp; 
	GetDlgItem(IDC_ANZ_PRALLBLECH)->GetWindowText(strTemp);

	if(strTemp.IsEmpty())
		return;
	
	update_preis();
};



void SpanxStrangrohrDialog::init_ui()
{


		SpanxRohrDialog::init_ui();

	cb_Prallblechvorauswahl =  (CComboBox*)GetDlgItem(IDC_PRALLBLECH_VORAUSWAHL);
	cb_Prallbleche = (CComboBox*)GetDlgItem(IDC_PRALLBLECH);

	SetWindowText(getStammdaten().getText("ID_STRANGROHRE").c_str());

	string strTemp = getStammdaten().getText("IDS_MUFFE") + " " + getStammdaten().getText("IDS_DN");
	GetDlgItem(IDC_STATIC_MUFFEN_DN)->SetWindowText(strTemp.c_str());
	
	GetDlgItem(IDC_STATIC_EINZELPREISIII)->SetWindowText(getStammdaten().getText("IDS_EINZELPREIS").c_str());

	strTemp = getStammdaten().getText("IDS_VORAUSWAHL")  + " " + getStammdaten().getText("IDS_SPRINKLER");
	GetDlgItem(IDC_STATIC_SPRINKLER_VORAUSWAHL)->SetWindowText(strTemp.c_str());
	GetDlgItem(IDC_STATIC_SPRINKLER)->SetWindowText(getStammdaten().getText("IDS_SPRINKLER").c_str());

	strTemp = getStammdaten().getText("IDS_VORAUSWAHL")  + " " + getStammdaten().getText("IDS_SPRINKLER_SCHELLEN");
	GetDlgItem(IDC_STATIC_SCHELLEN_VORAUSWAHL)->SetWindowText(strTemp.c_str());
	GetDlgItem(IDC_STATIC_SCHELLEN)->SetWindowText(getStammdaten().getText("IDS_SPRINKLER_SCHELLEN").c_str());

	strTemp = getStammdaten().getText("IDS_VORAUSWAHL")  + " " + getStammdaten().getText("IDS_PRALLBLECH");
	GetDlgItem(IDC_STATIC_PRALLBLECH_VORAUSWAHL)->SetWindowText(strTemp.c_str());
	strTemp =  getStammdaten().getText("IDS_PRALLBLECH");
	GetDlgItem(IDC_STATIC_PRALLBLECH)->SetWindowText(strTemp.c_str());

	strTemp = getStammdaten().getText("IDS_ARBEITSZEIT")  + " " + getStammdaten().getText("IDS_PRO")  + " " + getStammdaten().getText("IDS_SPRINKLER"); 
	GetDlgItem(IDC_STATIC_ARBEIT_PRO_SPRINKLER)->SetWindowText(strTemp.c_str());

	initPrallblechVorauswahl();
	selChangePrallblechVorauswahl();
	cb_Prallblechvorauswahl->EnableWindow(FALSE);
	selChangeSprinklerVorauswahl();

	if (xmldata.length())
	{
		CComboBox *rohrdin = (CComboBox*)GetDlgItem(IDC_STRANGROHR_DIN);
		SetCurlSelDependOnItemData(rohr_typ, rohrdin);
		
	
		int iArtikelNummer = atoi(m_SprinklerArtikelNr.c_str());
		SetCurlSelOfGruppe(iArtikelNummer, cbSprinklervorauswahl);
		selChangeSprinklerVorauswahl();
		SetCurlSelDependOnItemData(iArtikelNummer, cb_sprinkler);
		set_sprinklerpreis();
			
		iArtikelNummer = atoi(m_SchellenArtikelNr.c_str());
		SetCurlSelOfGruppe(iArtikelNummer, cb_schellenvorauswahl);
		selChangeSprinklerschellenVorauswahl();
		SetCurlSelDependOnItemData(iArtikelNummer, cb_schellen);
		set_schellenpreis();

		iArtikelNummer = atoi(m_PrallblechArtikelNr.c_str());
		SetCurlSelOfGruppe(iArtikelNummer, cb_Prallblechvorauswahl);
		selChangePrallblechVorauswahl();
		SetCurlSelDependOnItemData(iArtikelNummer, cb_Prallbleche);
		set_Prallblechpreis();

		iArtikelNummer = atoi(m_HalterArtikelNr.c_str());
		SetCurlSelOfGruppe(iArtikelNummer, cb_haltervorauswahl);
		selChangeHalterVorauswahl();
		SetCurlSelDependOnItemData(iArtikelNummer, cb_halter);
		set_halterpreis();

		iArtikelNummer = atoi(m_KupplungArtikelNr.c_str());
		SetCurlSelOfGruppe(iArtikelNummer, cb_kupplungvorauswahl);
		selChangeKupplungVorauswahl();
		SetCurlSelDependOnItemData(iArtikelNummer, cb_kupplung);
		set_kupplungpreis();
		
	}

	updateDN();
	//check_muffe();
}



string SpanxStrangrohrDialog::xml_data()
{
	string ret = SpanxRohrDialog::xml_data();
	int found=	ret.find("/kupplung");
	string tmp1 = ret.substr(0, found + 12);
	string tmp2 = ret.substr(found + 12, ret.length());

	tmp1 += str(format("<Prallblech>\n\t\t<anzahl>%i</anzahl>\n\t\t<typ>%s</typ></Prallblech>\n\t") % prallblech_anzahl % m_PrallblechArtikelNr);

	ret = tmp1 + tmp2;
  return ret;
}



void SpanxStrangrohrDialog::DoDataExchange(CDataExchange* pDX)
{
	if(!m_Cancel)
	{
		
			SpanxRohrDialog::DoDataExchange(pDX);
		
		DDX_Text(pDX, IDC_ANZ_SPRINKLER, sprinkler_anzahl);
		DDX_Text(pDX, IDC_ANZ_PRALLBLECH, prallblech_anzahl);
		
		SetDlgItemInt(IDC_ANZ_PRALLBLECH_GESAMT, prallblech_anzahl * rohr_anzahl);

		if (pDX->m_bSaveAndValidate) 
		{
			int i_typ = getItemData(cb_sprinkler);
			m_SprinklerArtikelNr = boost::str(boost::format("%i")  % i_typ);
			
			i_typ = getItemData(cb_Prallbleche);
			m_PrallblechArtikelNr  = boost::str(boost::format("%i")  % i_typ);

			i_typ = getItemData(cb_schellen);
			m_SchellenArtikelNr = boost::str(boost::format("%i")  % i_typ);
			
			i_typ = getItemData(cb_halter);
			m_HalterArtikelNr = boost::str(boost::format("%i")  % i_typ);

			i_typ = getItemData(cb_kupplung);
			m_KupplungArtikelNr = boost::str(boost::format("%i")  % i_typ);

			strang.get_beschichtung().set_typ(verzinkt ? Beschichtung::VERZINKT : Beschichtung::ROT_GEPULV);
			strang.initDaten(rohr_anzahl, rohr_dn, rohr_laenge, "1", rohr_typ, SNAHT);
			strang.set_rohrAnfang(rohr_anfang);
			strang.set_rohrEnde(rohr_ende);
			if ((strang.get_dnvl() == 32 || strang.get_dnvl() == 40) && muffen_typ == MUFFE && muffen_dn == 25)
			{
				strang.set_muffenGekehlt(true);
			} 
			strang.set_muffenID(muffen_typ);
			strang.set_muffenDN(muffen_typ == BOHR24 ? 0 : muffen_dn);
			
			for (int i=0; i < m_Muffen_anzahl; i++)
			{
				strang.set_muffenRelAbst(i, rohr_laenge/(m_Muffen_anzahl+1));
			}
			
			for(int j = m_Muffen_anzahl; j < Strangrohr::MAX_ANZAHL_STDELEMENTE; j++)
			{
				strang.set_muffenRelAbst(j, 0);
			}

			strang.createModell();
			
			m_errorListe = strang.getErrorHandler().getErrorListe();
			strang.getErrorHandler().clearErrorList();
		}
	}
}

string SpanxStrangrohrDialog::format_langtext()
{
	string langtext;
	const CObList &rtexte = getStammdaten().get_rohrtexte();

	for (POSITION pos = rtexte.GetHeadPosition(); pos; )
	{
		RohrTexte::data *dat = (RohrTexte::data *)rtexte.GetNext(pos);
		if (dat->get_rohrtyp() == get_rohrtyp() && dat->get_verzinkt() == verzinkt)
		{
			if (!dat->get_vortext())
			{
				string tmp =  (string)"<langtext>" + (string)(LPCSTR)dat->get_langtext() + (string)"</langtext>";
				langtext = str(format(tmp) % getStammdaten().dn2str(true, muffen_dn) % rohr_dn);
				break;
			}
		}
	}
	return langtext;
}



/// Rohrtyp-Enum für Zugriff auf Vortexte
int SpanxStrangrohrDialog::get_rohrtyp() const
{
	return RohrTexte::data::STRANGROHR;
}

	
/// Rohrtyp-Enum für Zugriff auf Montagezeiten
int SpanxStrangrohrDialog::get_rohrtyp_montage() const
{
	return Montagezeiten::data::STRANGROHR;
}

/// XML Tag für die Datensatzkennung
string SpanxStrangrohrDialog::get_xmltag()
{
	return "STRANGROHR";
}



void SpanxStrangrohrDialog::initRohrAnfangEnde()
{
	// Combobox Rohranfang
	BauelementGruppen bg;
	cbinitdata3 initRohranfang[] = 
	{
		bg.getBezeichnung(GLATT),	GLATT,	IDC_STRANGROHR_ANFANG,
		bg.getBezeichnung(NUT),		NUT,	IDC_STRANGROHR_ANFANG,
		bg.getBezeichnung(GEW),	GEW,	IDC_STRANGROHR_ANFANG,
		bg.getBezeichnung(SCH),	SCH,	IDC_STRANGROHR_ANFANG,
		"",			0,		0
	};
	INIT_CBDATA3(initRohranfang);

	for (int i = 0; initRohranfang[i].item; i++)
		initRohranfang[i].item = IDC_STRANGROHR_ENDE;
	INIT_CBDATA3(initRohranfang);

	ddx_select_cbitem(this, IDC_STRANGROHR_ANFANG, rohr_anfang);
	ddx_select_cbitem(this, IDC_STRANGROHR_ENDE, rohr_ende);
}


void  SpanxStrangrohrDialog::initSprinklerVorauswahl()
{

	cbSprinklervorauswahl->ResetContent();
	int index = cbSprinklervorauswahl->InsertString(-1, getStammdaten().getText("IDS_LEER").c_str());
	if (index != CB_ERR)
	{
		cbSprinklervorauswahl->SetItemData(index, 0);
	}

	NAMESTRINGITEM NameStringItem;
	vector<NAMESTRINGITEM> vectorNameStringItem;  
	vector<NAMESTRINGITEM>::iterator itvectorNameStringItem;	


	CMapArtikelPreis MapArtikelPreis;
	for (POSITION gpos = m_SpanxPreiseArtikelstamm.GetHeadPosition(); gpos;)
	{
		CSpanxPreiseArtikelstamm::data *g = (CSpanxPreiseArtikelstamm::data *)m_SpanxPreiseArtikelstamm.GetNext(gpos);
		
		if (g->get_Classid() == ArtikelstammClassId::TypSprinkler)
		{
			
			CMapArtikelPreis::iterator itCur = MapArtikelPreis.find(g->get_artikelgruppenID());
			if(itCur == MapArtikelPreis.end())
			{
				MapArtikelPreis[g->get_artikelgruppenID()];
				
				NameStringItem.strName	= g->get_Unterartikelgruppe2Kurztext();
				NameStringItem.iItem	= g->get_artikelgruppenID();
				vectorNameStringItem.push_back(NameStringItem);
			}			
		}
	}


	sort (vectorNameStringItem.begin(), vectorNameStringItem.end(), sortVectorNameStringItem); 

	for (itvectorNameStringItem = vectorNameStringItem.begin(); itvectorNameStringItem != vectorNameStringItem.end(); ++itvectorNameStringItem)
	{
	
		int index = cbSprinklervorauswahl->InsertString(-1,  itvectorNameStringItem->strName.c_str());

		if (index != CB_ERR)
		{
			cbSprinklervorauswahl->SetItemData(index,  itvectorNameStringItem->iItem);
		}
	}
	cbSprinklervorauswahl->SetCurSel(0);
}



void SpanxStrangrohrDialog::initSchellenVorauswahl()
{

	cb_schellenvorauswahl->ResetContent();
	int index = cb_schellenvorauswahl->InsertString(-1, getStammdaten().getText("IDS_LEER").c_str());
	if (index != CB_ERR)
	{
		cb_schellenvorauswahl->SetItemData(index, 0);
	}

	CMapArtikelPreis MapArtikelPreis;
	for (POSITION gpos = m_SpanxPreiseArtikelstamm.GetHeadPosition(); gpos;)
	{
		CSpanxPreiseArtikelstamm::data *g = (CSpanxPreiseArtikelstamm::data *)m_SpanxPreiseArtikelstamm.GetNext(gpos);
	
		if(g->get_Classid() == ArtikelstammClassId::TypAnbohrschelle && (g->get_artikelgruppenID() == ArtikelstammArtikelgruppenId::eSprinklerschelleVirotec ||  g->get_artikelgruppenID() == ArtikelstammArtikelgruppenId::eSprinklerschelleFig522)) 
		{
		
			CMapArtikelPreis::iterator itCur = MapArtikelPreis.find(g->get_artikelgruppenID());
			
			if(itCur == MapArtikelPreis.end())
			{
				MapArtikelPreis[g->get_artikelgruppenID()];
				int index = cb_schellenvorauswahl->InsertString(-1, g->get_Unterartikelgruppe2Kurztext());
				if (index != CB_ERR)
				{
					cb_schellenvorauswahl->SetItemData(index, g->get_artikelgruppenID());
				}
			}
		}		
	}
	cb_schellenvorauswahl->SetCurSel(0);
}



void SpanxStrangrohrDialog::initPrallblechVorauswahl()
{

	cb_Prallblechvorauswahl->ResetContent();
	int index = cb_Prallblechvorauswahl->InsertString(-1, getStammdaten().getText("IDS_LEER").c_str());
	if (index != CB_ERR)
	{
		cb_Prallblechvorauswahl->SetItemData(index, 0);
	}

	CMapArtikelPreis MapArtikelPreis;
	for (POSITION gpos = m_SpanxPreiseArtikelstamm.GetHeadPosition(); gpos;)
	{
		CSpanxPreiseArtikelstamm::data *g = (CSpanxPreiseArtikelstamm::data *)m_SpanxPreiseArtikelstamm.GetNext(gpos);
	
		if(g->get_Classid() == ArtikelstammClassId::TypSonstiges && g->get_artikelgruppenID() == ArtikelstammArtikelgruppenId::ePrallblech) 
		{
		
			CMapArtikelPreis::iterator itCur = MapArtikelPreis.find(g->get_artikelgruppenID());
			
			if(itCur == MapArtikelPreis.end())
			{
				MapArtikelPreis[g->get_artikelgruppenID()];
				int index = cb_Prallblechvorauswahl->InsertString(-1, g->get_Unterartikelgruppe2Kurztext());
				if (index != CB_ERR)
				{
					cb_Prallblechvorauswahl->SetItemData(index, g->get_artikelgruppenID());
				}
			}
		}		
	}
	cb_Prallblechvorauswahl->SetCurSel(1);
}



void SpanxStrangrohrDialog::initElemente()
{
	/// Bei Strangrohr gibts nur Bohrung und Muffe. Default wird Muffe ausgewählt, dann sind
	/// die Felder für die Sprinklerschelle auf Null gesetzt und disabled
	BauelementGruppen bg;
	cbinitdata3 initID[] = 
	{
		bg.getBezeichnung(MUFFE),	MUFFE,	IDC_MUFFEN_ID,
		bg.getBezeichnung(BOHR24),	BOHR24, IDC_MUFFEN_ID,
		"",			0,			0
	};
	INIT_CBDATA3(initID);
	((CComboBox*)GetDlgItem(IDC_MUFFEN_ID))->SetCurSel(0);
	//GetDlgItem(IDC_ANZ_SPRINKLERSCHELLEN)->EnableWindow(false);
	//GetDlgItem(IDC_SPRINKLERSCHELLE)->EnableWindow(false);
	ddx_select_cbitem(this, IDC_MUFFEN_ID, muffen_typ);
}

void SpanxStrangrohrDialog::initNennweite()
{
// Nennweite
	cbinitdata2 initNennweite[] = 
	{
		10, IDC_MUFFEN_DN,
		15, IDC_MUFFEN_DN,
		20, IDC_MUFFEN_DN,
		25, IDC_MUFFEN_DN,
		32, IDC_STRANGROHR_DN,
		40, IDC_STRANGROHR_DN,
		50, IDC_STRANGROHR_DN,
		65, IDC_STRANGROHR_DN,
		0,  0
	};
	CStringArray cbinitstr;
	for (int i = 0; initNennweite[i].data; i++)
		cbinitstr.Add(WaxStamm::dn2str(zoll, initNennweite[i].data));

	INIT_CBDATA2(initNennweite, cbinitstr);
	ddx_select_cbitem(this, IDC_MUFFEN_DN, muffen_dn);
	ddx_select_cbitem(this, IDC_STRANGROHR_DN, rohr_dn);
}



void SpanxStrangrohrDialog::set_sprinklerpreis()
{
	GetDlgItem(IDC_PREIS_SPRINKLER)->SetWindowText(getStammdaten().getText("IDS_NULL_EURO").c_str());
	long  artikelnr = getItemData(cb_sprinkler);
	
	for (POSITION gpos = m_SpanxPreiseArtikelstamm.GetHeadPosition(); gpos;)
	{
		CSpanxPreiseArtikelstamm::data *g = (CSpanxPreiseArtikelstamm::data *)m_SpanxPreiseArtikelstamm.GetNext(gpos);
		
		if (artikelnr == g->get_artikelnr() && g->get_Region() == m_iRegion)
		{
			setpreis(IDC_PREIS_SPRINKLER, g->get_Preis());
			break;
		}
	}
	
	UPDATE_GESAMTPREIS
}


void SpanxStrangrohrDialog::updateDN()
{
	SpanxRohrDialog::updateDN();

}


double SpanxStrangrohrDialog::get_montagezeit() const
{
/**	
	Definiert in Montagezeiten: aktionsnummer
	ROHR_TRANSPORT = 1
	HALTER_SETZEN = 2,
	SPRINKLER_MONTIEREN = 3,
	SPRINKLERSCHELLE_MONTIEREN = 4,
	ROHR_TRANSPORT2 = 5,
	ROHR_IN_HALTER_MONTIEREN = 6,
	ROHRKUPPLUNG_MONTIEREN = 7
*/
	double zeiten[Montagezeiten::data::S_ROHRKUPPLUNG_MONTIEREN];
	double mengen[Montagezeiten::data::S_ROHRKUPPLUNG_MONTIEREN];

	const CObList &montage = getStammdaten().get_montagezeiten();
	int i;
	for (i = Montagezeiten::data::S_ROHR_TRANSPORT; i <= Montagezeiten::data::S_ROHRKUPPLUNG_MONTIEREN; i++)
	{
		for (POSITION pos = montage.GetHeadPosition(); pos; )
		{
			Montagezeiten::data *dat = (Montagezeiten::data *)montage.GetNext(pos);

			if (dat->get_dn() == rohr_dn && dat->get_hoehe() == hoehe && dat->get_rohrtyp() == get_rohrtyp_montage() && dat->get_aktion() == i)
			{
				zeiten[dat->get_aktion()-1] = dat->get_zeit();
				break;
			}
		}
			
	}

	// Gesamtmengen setzen
	mengen[Montagezeiten::data::S_ROHR_TRANSPORT-1] = (double)rohr_anzahl;
	mengen[Montagezeiten::data::S_HALTER_SETZEN-1] = (double)halter_anzahl * (double)rohr_anzahl;
	mengen[Montagezeiten::data::S_SPRINKLER_MONTIEREN-1] = (double)sprinkler_anzahl * (double)rohr_anzahl;
	mengen[Montagezeiten::data::S_SPRINKLERSCHELLE_MONTIEREN-1] = (double)schellen_anzahl * (double)rohr_anzahl;
	mengen[Montagezeiten::data::S_ROHR_TRANSPORT2-1] = (double)rohr_anzahl;
	mengen[Montagezeiten::data::S_ROHR_IN_HALTER_MONTIEREN-1] = (double)rohr_anzahl;
	mengen[Montagezeiten::data::S_ROHRKUPPLUNG_MONTIEREN-1] = (double)kupplung_anzahl * (double)rohr_anzahl;

	// alle Arbeitsschritte addieren
	double zeit = 0.0;
	for (i = 0; i < Montagezeiten::data::S_ROHRKUPPLUNG_MONTIEREN; i++)
		zeit += mengen[i] * zeiten[i];	
	if (sprinkler_anzahl <= 0)
	{
		AfxMessageBox("Sprinkleranzahl darf nicht Null sein!", MB_ICONERROR | MB_OK);
		return 0.0;
	}
	return zeit / (double)sprinkler_anzahl / rohr_anzahl /60.0;
}



void SpanxStrangrohrDialog::selChangeSprinklerVorauswahl()
{
	long  GruppenID = getItemData(cbSprinklervorauswahl);
	initAllMXItems( GruppenID, cb_sprinkler);
	set_sprinklerpreis();
}



void SpanxStrangrohrDialog::selChangePrallblechVorauswahl()
{
	long  GruppenID = getItemData(cb_Prallblechvorauswahl);
	initAllMXItems( GruppenID, cb_Prallbleche);
	set_Prallblechpreis();
}



void SpanxStrangrohrDialog::check_muffe()
{

	UpdateData();
	
	bool enable_items = true;
	if (muffen_typ == MUFFE)
	{
			enable_items = false;
	}
	
	GetDlgItem(IDC_MUFFEN_DN)->EnableWindow(!enable_items);
	UPDATE_GESAMTPREIS
}





void SpanxStrangrohrDialog::set_Prallblechpreis()
{
	
	GetDlgItem(IDC_PREIS_PRALLBLECH)->SetWindowText(getStammdaten().getText("IDS_NULL_EURO").c_str());
	long  artikelnr = getItemData(cb_Prallbleche);
	for (POSITION gpos = m_SpanxPreiseArtikelstamm.GetHeadPosition(); gpos;)
	{
		CSpanxPreiseArtikelstamm::data *g = (CSpanxPreiseArtikelstamm::data *)m_SpanxPreiseArtikelstamm.GetNext(gpos);
		
		if (artikelnr == g->get_artikelnr() && g->get_Region() == m_iRegion)
		{
			setpreis(IDC_PREIS_PRALLBLECH, g->get_Preis());
			break;
		}
	}
	UPDATE_GESAMTPREIS
}


