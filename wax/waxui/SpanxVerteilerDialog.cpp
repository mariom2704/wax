#include "stdafx.h"

#include "..\waxdef.h"
#include "resource.h"

#include <mxcontrols/msgbox.h>
using namespace MxGui;

#include "SpanxRohrDialog.h"

		
			#include "SpanxVerteilerDialog.h"





#ifdef _DEBUG
#define new DEBUG_NEW
#endif


BEGIN_MESSAGE_MAP(SpanxVerteilerDialog, SpanxRohrDialog)
	ON_CBN_SELCHANGE(IDC_MUFFEN_ID, check_muffe)
	
	ON_EN_CHANGE(IDC_ELEMENT_LAENGE, update_Rohrlaenge_with_Keystroke_Test)
	ON_EN_KILLFOCUS(IDC_ELEMENT_LAENGE, update_preis)

	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()



SpanxVerteilerDialog::SpanxVerteilerDialog(CWnd *parent, const string& data) : SpanxRohrDialog(parent, IDD_SPANX_VERTEILER, data) ,
EditBkBrush(::GetSysColor(COLOR_3DFACE))
{
	werkauftrag = &verteiler;
	max_elements = 99;
	// Default ist nur Verteilerleitung, keine Abgänge o.ä.
	sprinkler_anzahl = 0;
	schellen_anzahl = 0;
	m_Muffen_anzahl = 0;

	if (xmldata.length())
	{
		init_attr(xmldata.c_str());
	}
	 
}


SpanxVerteilerDialog::~SpanxVerteilerDialog()
{}



void SpanxVerteilerDialog::update_Rohrlaenge_with_Keystroke_Test()
{
	CString strTemp; 
	GetDlgItem(IDC_ELEMENT_LAENGE)->GetWindowText(strTemp);

	if(strTemp.IsEmpty())
		return;
	
	update_preis();
};



void SpanxVerteilerDialog::init_ui()
{
		
		SpanxRohrDialog::init_ui();

	SetWindowText(getStammdaten().getText("IDS_VERTEILERROHRE").c_str());

	GetDlgItem(IDC_STATIC_MUFFEN_DN)->SetWindowText(getStammdaten().getText("IDS_DN").c_str());
	GetDlgItem(IDC_STATIC_LAENGE_MUFFE)->SetWindowText(getStammdaten().getText("IDC_LAENGE").c_str());
	string strTemp = getStammdaten().getText("IDS_VORAUSWAHL")  + " " + getStammdaten().getText("IDS_ANBOHRSCHELLE");
	GetDlgItem(IDC_STATIC_ANBOHRSCHELLE_VORAUSWAHL)->SetWindowText(strTemp.c_str());
	GetDlgItem(IDC_STATIC_ANBOHRSCHELLE)->SetWindowText(getStammdaten().getText("IDS_ANBOHRSCHELLE").c_str());
	strTemp = getStammdaten().getText("IDS_ARBEITSZEIT")  + " " + getStammdaten().getText("IDS_PRO")  + " " + getStammdaten().getText("IDS_METER"); 
	GetDlgItem(IDC_STATIC_ARBEITSZEIT_PRO_METER)->SetWindowText(strTemp.c_str());
	

	if (xmldata.length())
	{

			
			CComboBox *rohrdin = (CComboBox*)GetDlgItem(IDC_STRANGROHR_DIN);
			SetCurlSelDependOnItemData(rohr_typ, rohrdin);
			
				
			int iArtikelNummer = atoi(m_SchellenArtikelNr.c_str());
			SetCurlSelOfGruppe(iArtikelNummer, cb_schellenvorauswahl);
			selChangeSprinklerschellenVorauswahl();
			SetCurlSelDependOnItemData(iArtikelNummer, cb_schellen);
			set_schellenpreis();

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


void SpanxVerteilerDialog::DoDataExchange(CDataExchange* pDX)
{
	if(!m_Cancel)
	{
			SpanxRohrDialog::DoDataExchange(pDX);
		
		if (GetDlgItem(IDC_ELEMENT_LAENGE))
			DDX_Text(pDX, IDC_ELEMENT_LAENGE, ele_laenge);

		if (pDX->m_bSaveAndValidate) 
		{
			try
			{
				int ityp = getItemData(cb_schellen);
				m_SchellenArtikelNr = boost::str(boost::format("%i")  % ityp);
				
				ityp = getItemData(cb_halter);
				m_HalterArtikelNr = boost::str(boost::format("%i")  % ityp);

				ityp = getItemData(cb_kupplung);
				m_KupplungArtikelNr = boost::str(boost::format("%i")  % ityp);

				verteiler.get_beschichtung().set_typ(verzinkt ? Beschichtung::VERZINKT : Beschichtung::ROT_GEPULV);
				verteiler.initDaten(rohr_anzahl, rohr_dn, rohr_laenge, "1", rohr_typ, SNAHT);
				verteiler.set_rohrAnfang(rohr_anfang);
				verteiler.set_rohrEnde(rohr_ende);


				EnContainerRohrElemente &elemente = verteiler.get_elementArray();
				elemente.deleteAll();
				bool ungueltig = false;
				for (int i = 0; i < m_Muffen_anzahl; i++)
				{
					Element *ele = NULL;
					ele = new Element(muffen_typ, 100, muffen_dn, rohr_dn, 150, (i+1)*150, ele_laenge);
					try
					{
						ele->initStammdaten(muffen_dn, rohr_dn, muffen_typ);
					}
					catch(Element::Error e)
					{
						ungueltig = true;
						
						delete ele;
						continue;
					}
					elemente.Add(ele);
				}
				

				CString strUngueltig = getStammdaten().getText("IDS_EIN").c_str();
		
				CString strTemp;
				GetDlgItem(IDC_MUFFEN_ID)->GetWindowTextA(strTemp);
				strUngueltig += " " + strTemp + " DN(";
				CString strTempII;
				GetDlgItem(IDC_MUFFEN_DN)->GetWindowTextA(strTempII);
				strUngueltig += strTempII;
				strUngueltig +=  ")";
				strUngueltig +=  getStammdaten().getText("IDS_ERROR_VK_BERECH").c_str();
				GetDlgItem(IDC_STATUS)->SetWindowText(ungueltig ? strUngueltig : "");
				verteiler.createModell();
			}
			catch(Element::Error e)
			{
				MsgBox::show_error(e.get_errorText());
			}
		} 
	}
}




string SpanxVerteilerDialog::format_langtext()
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
				string format = (string)"<langtext>" + (string)(LPCSTR)dat->get_langtext() + (string)"</langtext>";
				langtext = boost::str( boost::format(format) % rohr_dn);
				break;
			}
		}
	}
	return langtext;
}

	
string SpanxVerteilerDialog::xml_data()
{
	return SpanxRohrDialog::xml_data();	
}


/// Rohrtyp-Enum für Zugriff auf Vortexte
int SpanxVerteilerDialog::get_rohrtyp() const
{
	return RohrTexte::data::VERTEILER;
}

	
/// Rohrtyp-Enum für Zugriff auf Montagezeiten
int SpanxVerteilerDialog::get_rohrtyp_montage() const
{
	return Montagezeiten::data::VERTEILERLEITUNG;
}

/// XML Tag für die Datensatzkennung
string SpanxVerteilerDialog::get_xmltag()
{
	return "VERTEILER";
}


void SpanxVerteilerDialog::initRohrAnfangEnde()
{
	// Combobox Rohranfang
	BauelementGruppen bg;
	cbinitdata3 initRohranfang[] = 
	{
		bg.getBezeichnung(GLATT),		GLATT,		IDC_STRANGROHR_ANFANG,
		bg.getBezeichnung(NUT),			NUT,		IDC_STRANGROHR_ANFANG,
		bg.getBezeichnung(GEW),		GEW,		IDC_STRANGROHR_ANFANG,
		bg.getBezeichnung(FFL),		FFL,		IDC_STRANGROHR_ANFANG,
		bg.getBezeichnung(FFL16),	FFL16,		IDC_STRANGROHR_ANFANG,
		bg.getBezeichnung(FFL16),	FFL10,		IDC_STRANGROHR_ANFANG,
		bg.getBezeichnung(SFL),		SFL,		IDC_STRANGROHR_ANFANG,
		bg.getBezeichnung(VFL10),	VFL10,		IDC_STRANGROHR_ANFANG,
		bg.getBezeichnung(VFL10NEU),VFL10NEU,		IDC_STRANGROHR_ANFANG,
		bg.getBezeichnung(VFL16),	VFL16,		IDC_STRANGROHR_ANFANG,
		bg.getBezeichnung(VFL16NEU),VFL16NEU,		IDC_STRANGROHR_ANFANG,
		bg.getBezeichnung(AFL),		AFL,		IDC_STRANGROHR_ANFANG,
		bg.getBezeichnung(SCH),		SCH,		IDC_STRANGROHR_ANFANG,
		bg.getBezeichnung(BOD),		BOD,		IDC_STRANGROHR_ANFANG,
		"",			0,		0
	};
	INIT_CBDATA3(initRohranfang);
	
	for (int i = 0; initRohranfang[i].item; i++)
		initRohranfang[i].item = IDC_STRANGROHR_ENDE;
	INIT_CBDATA3(initRohranfang);

	ddx_select_cbitem(this, IDC_STRANGROHR_ANFANG, rohr_anfang);
	ddx_select_cbitem(this, IDC_STRANGROHR_ENDE, rohr_ende);
}

void SpanxVerteilerDialog::initElemente()
{
	// Combobox MuffenID
	BauelementGruppen bg;
	CString str; str = getStammdaten().getText("IDS_KEINELEMENT").c_str();
	cbinitdata3 cbdat[] = 
	{
		str,								Element::LEER,	IDC_MUFFEN_ID,
		bg.getBezeichnung(NUTST),			NUTST,	IDC_MUFFEN_ID,
		bg.getBezeichnung(MUFFE),			MUFFE,	IDC_MUFFEN_ID,
		bg.getBezeichnung(ROHR),			ROHR,	IDC_MUFFEN_ID,
		bg.getBezeichnung(GWRST),			GWRST,	IDC_MUFFEN_ID,
		bg.getBezeichnung(ELEMENT_BOGEN),	ELEMENT_BOGEN,	IDC_MUFFEN_ID,
		bg.getBezeichnung(BOG2S),			BOG2S,	IDC_MUFFEN_ID,
		bg.getBezeichnung(KRED),			KRED,	IDC_MUFFEN_ID,
		bg.getBezeichnung(ERED),			ERED,	IDC_MUFFEN_ID,
		bg.getBezeichnung(EINZ),			EINZ,	IDC_MUFFEN_ID,
		bg.getBezeichnung(BOHR50),			BOHR50, IDC_MUFFEN_ID,
		bg.getBezeichnung(NUT),				NUT,	IDC_MUFFEN_ID,
		bg.getBezeichnung(GEW),				GEW,	IDC_MUFFEN_ID,
		bg.getBezeichnung(FFL),				FFL,	IDC_MUFFEN_ID,
		bg.getBezeichnung(SFL),				SFL,	IDC_MUFFEN_ID,
		bg.getBezeichnung(VFL10),			VFL10,	 IDC_MUFFEN_ID,
		bg.getBezeichnung(VFL10NEU),		VFL10NEU,IDC_MUFFEN_ID,
		bg.getBezeichnung(VFL16),			VFL16,	 IDC_MUFFEN_ID,
		bg.getBezeichnung(VFL16NEU),		VFL16NEU,IDC_MUFFEN_ID,
		bg.getBezeichnung(AFL),				AFL,	 IDC_MUFFEN_ID,
		bg.getBezeichnung(GLATT),			GLATT,	 IDC_MUFFEN_ID,
		"", 0, 0
	};
	INIT_CBDATA3(cbdat);

	CComboBox* pMuffenTyp = (CComboBox*)GetDlgItem(IDC_MUFFEN_ID);
	for(int k = 0; k < pMuffenTyp->GetCount(); k++)
	{
		int dTyp= pMuffenTyp->GetItemData(k);
			if( dTyp == muffen_typ)
			{
				pMuffenTyp->SetCurSel(k);
			}
	}
	//ddx_select_cbitem(this, IDC_MUFFEN_ID, muffen_typ);
}


void SpanxVerteilerDialog::initNennweite()
{
// Nennweite
	cbinitdata2 initNennweite[] = 
	{
		10, IDC_MUFFEN_DN,
		15, IDC_MUFFEN_DN,
		20, IDC_MUFFEN_DN,
		25, IDC_MUFFEN_DN,
		32, IDC_MUFFEN_DN,
		40, IDC_MUFFEN_DN,
		50, IDC_MUFFEN_DN,
		65, IDC_MUFFEN_DN,
		80, IDC_MUFFEN_DN,
		100, IDC_MUFFEN_DN,
		125, IDC_MUFFEN_DN,
		150, IDC_MUFFEN_DN,
		200, IDC_MUFFEN_DN,
		250, IDC_MUFFEN_DN,
		300, IDC_MUFFEN_DN,
		25, IDC_STRANGROHR_DN,
		32, IDC_STRANGROHR_DN,
		40, IDC_STRANGROHR_DN,
		50, IDC_STRANGROHR_DN,
		65, IDC_STRANGROHR_DN,
		80, IDC_STRANGROHR_DN,
		100, IDC_STRANGROHR_DN,
		125, IDC_STRANGROHR_DN,
		150, IDC_STRANGROHR_DN,
		200, IDC_STRANGROHR_DN,
		250, IDC_STRANGROHR_DN,
		300, IDC_STRANGROHR_DN,
		0,  0
	};
	CStringArray cbinitstr;
	for (int i = 0; initNennweite[i].data; i++)
		cbinitstr.Add(WaxStamm::dn2str(zoll, initNennweite[i].data));

	INIT_CBDATA2(initNennweite, cbinitstr);
	
	CDataExchange pdx(this, false);
	ddx_cbdata(&pdx, IDC_STRANGROHR_DN, rohr_dn);
	ddx_cbdata(&pdx, IDC_MUFFEN_DN, muffen_dn);
}


void SpanxVerteilerDialog::check_muffe()
{
	UpdateData();
	bool enable_items = true;
	
	if (muffen_typ == Element::LEER)
	{
		SetDlgItemInt(IDC_ANZ_MUFFEN, 0);
		enable_items = false;
	}

	GetDlgItem(IDC_ELEMENT_LAENGE)->EnableWindow(enable_items);
	GetDlgItem(IDC_MUFFEN_DN)->EnableWindow(enable_items);
	GetDlgItem(IDC_ELEMENT_LAENGE)->EnableWindow(enable_items);

	GetDlgItem(IDC_ANZ_MUFFEN)->EnableWindow(enable_items);
	update_preis();
	
}



double SpanxVerteilerDialog::get_montagezeit() const
{
/**	
	Definiert in Montagezeiten: aktionsnummer
	ROHR_TRANSPORT = 1,
	HALTER_SETZEN = 2,
	ANBOHRSCHELLE_MONTIEREN = 3,
	ROHR_AUF_HEBEBUEHNE = 4,
	ROHR_IN_HALTER_MONTIEREN = 5,
	ROHRKUPPLUNG_MONTIEREN = 6,
	FORMSTUECKE = 7,
*/
	double zeiten[Montagezeiten::data::V_FORMSTUECKE];
	double mengen[Montagezeiten::data::V_FORMSTUECKE];

	// Montagezeiten unter DN50 sind nicht definiert
	int tmp_rohrdn = rohr_dn >= 50 ? rohr_dn : 50;

	const CObList &montage = getStammdaten().get_montagezeiten();
	int i;
	for (i = Montagezeiten::data::V_ROHR_TRANSPORT; i <= Montagezeiten::data::V_FORMSTUECKE; i++)
	{
		for (POSITION pos = montage.GetHeadPosition(); pos; )
		{
			Montagezeiten::data *dat = (Montagezeiten::data *)montage.GetNext(pos);

			if (dat->get_dn() == tmp_rohrdn && dat->get_hoehe() == hoehe && dat->get_rohrtyp() == get_rohrtyp_montage() && dat->get_aktion() == i)
			{
				zeiten[dat->get_aktion()-1] = dat->get_zeit();
				break;
			}
		}
			
	}

	// Gesamtmengen setzen
	mengen[Montagezeiten::data::V_ROHR_TRANSPORT-1] = (double)rohr_anzahl;
	mengen[Montagezeiten::data::V_HALTER_SETZEN-1] = (double)halter_anzahl * (double)rohr_anzahl;
	mengen[Montagezeiten::data::V_ANBOHRSCHELLE_MONTIEREN-1] = (double)schellen_anzahl * (double)rohr_anzahl;
	mengen[Montagezeiten::data::V_ROHRKUPPLUNG_MONTIEREN-1] = (double)kupplung_anzahl;
	mengen[Montagezeiten::data::V_ROHR_AUF_HEBEBUEHNE-1] = (double)rohr_anzahl;
	mengen[Montagezeiten::data::V_ROHR_IN_HALTER_MONTIEREN-1] = (double)rohr_anzahl;
	mengen[Montagezeiten::data::V_FORMSTUECKE-1] = (double)rohr_anzahl;

	// alle Arbeitsschritte addieren
	double zeit = 0.0;
	for (i = 0; i < Montagezeiten::data::V_FORMSTUECKE; i++)
		zeit += mengen[i] * zeiten[i];	

	// Zeit in Std pro Meter
	return zeit / ((double)rohr_anzahl * (double)rohr_laenge / 1000) / 60;
}



void SpanxVerteilerDialog::initSchellenVorauswahl()
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
		
		if(g->get_Classid() == ArtikelstammClassId::TypAnbohrschelle) 
		{
		
			switch(g->get_artikelgruppenID())
			{

				case ArtikelstammArtikelgruppenId::eAnbohrschelleGewindeRot:
				case ArtikelstammArtikelgruppenId::eAnbohrschelleNutVerzinktViktaulik:
				case ArtikelstammArtikelgruppenId::eAnbohrschelleGewindeVerzinkt:
				case ArtikelstammArtikelgruppenId::eAnbohrschelleNutRotModgal:
				case ArtikelstammArtikelgruppenId::eAnbohrschelleNutVerzinktModgal:
				case ArtikelstammArtikelgruppenId::eKreuzanbohrschelleNutVerzinktViktaulik:
				case ArtikelstammArtikelgruppenId::eKreuzanbohrschelleNutRotViktaulik:
				case ArtikelstammArtikelgruppenId::eKreuzanbohrschelleNutVerzinktModgal:
				case ArtikelstammArtikelgruppenId::eKreuzanbohrschelleNutRotModgal:
				case ArtikelstammArtikelgruppenId::eAnbohrschelleNutRotPicoFix:
				case ArtikelstammArtikelgruppenId::eAnbohrschelleNutVerzinktPicoFix:
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
		}		
	}
	cb_schellenvorauswahl->SetCurSel(0);
}


HBRUSH SpanxVerteilerDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{

   CWnd* pStaticCtrl = GetDlgItem(IDC_STATUS);

   if( pStaticCtrl->m_hWnd == pWnd->m_hWnd )
   {  
		pDC->SetTextColor(RGB(255, 0, 0));
		pDC->SetBkColor(::GetSysColor(COLOR_3DFACE));
	 return (HBRUSH)(EditBkBrush.GetSafeHandle());
   }
   return SpanxRohrDialog::OnCtlColor(pDC, pWnd, nCtlColor);
}
