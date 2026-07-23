#include "stdafx.h"

#include "..\waxdef.h"
#include "resource.h"
#include <xml\xmlparser\xml.h>

#include <mxcontrols/msgbox.h>
using namespace MxGui;


			#include "SpanxRohrDialog.h"




#ifdef _DEBUG
#define new DEBUG_NEW
#endif







BEGIN_MESSAGE_MAP(SpanxRohrDialog, CDialog)

	ON_EN_CHANGE(IDC_STRANGROHR_ANZAHL, update_Anzahl_Rohre_with_Keystroke_Test)
	ON_EN_KILLFOCUS(IDC_STRANGROHR_ANZAHL, update_preis)

	ON_CBN_SELCHANGE(IDC_SCHELLE, set_schellenpreis)
	ON_CBN_SELCHANGE(IDC_SCHELLEN_VORAUSWAHL, selChangeSprinklerschellenVorauswahl)
	ON_CBN_SELCHANGE(IDC_HALTER_VORAUSWAHL, selChangeHalterVorauswahl)
	ON_CBN_SELCHANGE(IDC_HALTER, set_halterpreis)
	ON_CBN_SELCHANGE(IDC_KUPPLUNG, set_kupplungpreis)
	ON_CBN_SELCHANGE(IDC_KUPPLUNG_VORAUSWAHL, selChangeKupplungVorauswahl)
	
	

	ON_CBN_SELCHANGE(IDC_STRANGROHR_ANFANG, update_preis)
	ON_CBN_SELCHANGE(IDC_STRANGROHR_ENDE, update_preis)
	ON_CBN_SELCHANGE(IDC_STRANGROHR_DIN, update_preis)
	ON_CBN_SELCHANGE(IDC_STRANGROHR_DN, updateDN)
	ON_CBN_SELCHANGE(IDC_MUFFEN_DN, updateDN)
	
	ON_EN_KILLFOCUS(IDC_ANZ_MUFFEN, update_Anzahl_Sprinkler)
	ON_EN_CHANGE(IDC_ANZ_MUFFEN, update_Anzahl_Muffen_with_Keystroke_Test)

	ON_EN_KILLFOCUS(IDC_STRANGROHR_ANFANG, update_preis)
	ON_EN_KILLFOCUS(IDC_STRANGROHR_ENDE, update_preis)
	
	ON_EN_KILLFOCUS(IDC_STRANGROHR_DIN, update_preis)

	ON_EN_KILLFOCUS(IDC_STRANGROHR_LAENGE, update_preis)
	ON_EN_CHANGE(IDC_STRANGROHR_LAENGE, update_preis)

	
	ON_EN_KILLFOCUS(IDC_ANZ_SCHELLEN, update_preis)
	ON_EN_CHANGE(IDC_ANZ_SCHELLEN, updateAnzahlSchellenWithKeystrokeTest)
	
	ON_EN_KILLFOCUS(IDC_ANZ_HALTERUNG, update_preis)
	ON_EN_CHANGE(IDC_ANZ_HALTERUNG, update_preis)
	
	ON_EN_KILLFOCUS(IDC_ANZ_KUPPLUNG, update_preis)
	ON_EN_CHANGE(IDC_ANZ_KUPPLUNG, updateAnzahlKupplungWithKeystrokeTest)
	
	ON_BN_CLICKED(IDC_VERZINKT, updateDN)
	ON_COMMAND(WM_PREISUPDATE, update_preis)

END_MESSAGE_MAP()



bool SpanxRohrDialog::sortVectorNameStringItem (NAMESTRINGITEM& first,NAMESTRINGITEM& second) 
{
	 if (first.strName.compare(second.strName) < 0)  
		 return true;  
	 else   
		 return false;
}



void SpanxRohrDialog::update_Anzahl_Rohre_with_Keystroke_Test()
{
	CString strTemp; 
	GetDlgItem(IDC_STRANGROHR_ANZAHL)->GetWindowText(strTemp);

	if(strTemp.IsEmpty())
		return;
	
	update_preis();
};




void SpanxRohrDialog::update_Anzahl_Muffen_with_Keystroke_Test()
{
	CString strTemp; 
	GetDlgItem(IDC_ANZ_MUFFEN)->GetWindowText(strTemp);

	if(strTemp.IsEmpty())
		return;


	m_Muffen_anzahl_Before = m_Muffen_anzahl;
	if(!UpdateData(TRUE))
	{
		m_Muffen_anzahl = m_Muffen_anzahl_Before;
		return;
	}
	
	update_Anzahl_Sprinkler();
	m_Muffen_anzahl_Before = m_Muffen_anzahl;
};



void SpanxRohrDialog::update_Anzahl_Sprinkler()
{

	int iSprinkler  = GetDlgItemInt(IDC_ANZ_SPRINKLER);

	if(m_Muffen_anzahl_Before == iSprinkler)
	{
		CString strTemp; 
		GetDlgItem(IDC_ANZ_MUFFEN)->GetWindowText(strTemp);

		if(!strTemp.IsEmpty())
		{
			m_Muffen_anzahl = GetDlgItemInt(IDC_ANZ_MUFFEN);
			SetDlgItemInt(IDC_ANZ_SPRINKLER, m_Muffen_anzahl);
		}
	}
	
	update_preis();
};




void SpanxRohrDialog::updateAnzahlSchellenWithKeystrokeTest()
{
	CString strTemp; 
	GetDlgItem(IDC_ANZ_SCHELLEN)->GetWindowText(strTemp);

	if(strTemp.IsEmpty())
		return;
	
	update_preis();
};



void SpanxRohrDialog::updateAnzahlKupplungWithKeystrokeTest()
{
	CString strTemp; 
	GetDlgItem(IDC_ANZ_KUPPLUNG)->GetWindowText(strTemp);

	if(strTemp.IsEmpty())
		return;
	
	update_preis();
};




SpanxRohrDialog::~SpanxRohrDialog()
{}


SpanxRohrDialog::SpanxRohrDialog(CWnd *parent, UINT dlgid, const string& data)
: BaseDialog(dlgid, parent), 
  m_SpanxPreiseArtikelstamm(getStammdaten().get_SpanxPreiseArtikelstamm()), 
  bauele(getStammdaten().get_bauelemente()), fittinge(getStammdaten().get_formstueckefittinge()),
  m_iRegion(288),m_Cancel(false)
{
	preis = 0.0;
	rohr_anzahl = 1;
	rohr_laenge = 6000;
	rohr_dn = 50;
	rohr_typ = Rohrleitung::RQ2440_2458GSCH; 
	rohr_anfang = NUT;	
	rohr_ende = NUT;	
	m_Muffen_anzahl = 1;
	m_Muffen_anzahl_Before = m_Muffen_anzahl;
	muffen_dn = 15;
	muffen_typ = MUFFE;
	ele_laenge = 0;
	sprinkler_anzahl = 1;
	schellen_anzahl = 1;
	halter_anzahl = 0.0;
	kupplung_anzahl = 0;
	cb_schellen = NULL;
	vortext_einfuegen = false;
	zoll = WaxSys::para_int(GF_DN2ZOLL) ? true : false;
	hoehe = 8; // default bis 8 m Hoehe
	max_elements = 0;
	anzHalterCtrl.initVal(&halter_anzahl);
	anzHalterCtrl.set_Decimals(2);
	verzinkt = false;
	xmldata = data;

	AfxGetMainWnd()->ShowWindow(SW_MINIMIZE);

	cb_sprinkler  = 0;
}



void SpanxRohrDialog::postUpdatePreisMsg()
{
	PostMessage(WM_COMMAND, BN_CLICKED<<16 | IDC_BERECHNEN, 0);
}



void SpanxRohrDialog::init_ui()
{

	GetDlgItem(IDC_STATIC_ROHR)->SetWindowText(getStammdaten().getText("IDS_ROHR").c_str());
	GetDlgItem(IDC_STATIC_ELEMENT_PRO_ROHR)->SetWindowText(getStammdaten().getText("IDS_ELEMENT_PRO_ROHR").c_str());
	GetDlgItem(IDC_STATIC_ANZAHL_ROHRE)->SetWindowText(getStammdaten().getText("IDC_ANZAHL").c_str());
	GetDlgItem(IDC_STATIC_DN)->SetWindowText(getStammdaten().getText("IDC_DN").c_str());
	GetDlgItem(IDC_STATIC_LAENGE)->SetWindowText(getStammdaten().getText("IDC_LAENGE").c_str());
	GetDlgItem(IDC_STATIC_DIN)->SetWindowText(getStammdaten().getText("IDC_DIN").c_str());
	GetDlgItem(IDC_STATIC_ANFANG)->SetWindowText(getStammdaten().getText("IDS_ANFANG").c_str());
	GetDlgItem(IDC_STATIC_ENDE)->SetWindowText(getStammdaten().getText("IDS_ENDE").c_str());
	GetDlgItem(IDC_STATIC_EINBAU_HOEHE)->SetWindowText(getStammdaten().getText("IDS_EINBAU_HOEHE").c_str());
	GetDlgItem(IDC_VERZINKT)->SetWindowText(getStammdaten().getText("IDS_VERZINKT").c_str());
	GetDlgItem(IDC_STATIC_ANZAHL_ELEMENTE_PRO_ROHR)->SetWindowText(getStammdaten().getText("IDC_ANZAHL").c_str());
	GetDlgItem(IDC_STATIC_GESAMT)->SetWindowText(getStammdaten().getText("IDS_GESAMT").c_str());
	GetDlgItem(IDC_STATIC_TYP)->SetWindowText(getStammdaten().getText("IDS_TYP").c_str());

	GetDlgItem(IDC_STATIC_EINZELPREIS)->SetWindowText(getStammdaten().getText("IDS_EINZELPREIS").c_str());
	GetDlgItem(IDC_STATIC_EINZELPREISI)->SetWindowText(getStammdaten().getText("IDS_EINZELPREIS").c_str());
	GetDlgItem(IDC_STATIC_EINZELPREISII)->SetWindowText(getStammdaten().getText("IDS_EINZELPREIS").c_str());
	
	string strTemp = getStammdaten().getText("IDS_VORAUSWAHL")  + " " + getStammdaten().getText("IDS_HALTERUNG");
	GetDlgItem(IDC_STATIC_HALTERUNG_VORAUSWAHL)->SetWindowText(strTemp.c_str());
	GetDlgItem(IDC_STATIC_HALTERUNG)->SetWindowText(getStammdaten().getText("IDS_HALTERUNG").c_str());

	strTemp = getStammdaten().getText("IDS_VORAUSWAHL")  + " " + getStammdaten().getText("IDS_KUPPLUNG") + " " + getStammdaten().getText("IDS_INKLU") + " " + getStammdaten().getText("IDS_NUT");
	GetDlgItem(IDC_STATIC_KUPPPLUNG_VORAUSWAHL)->SetWindowText(strTemp.c_str());
	strTemp =  getStammdaten().getText("IDS_KUPPLUNG") + " " + getStammdaten().getText("IDS_INKLU") + " " + getStammdaten().getText("IDS_NUT");
	GetDlgItem(IDC_STATIC_KUPPPLUNG)->SetWindowText(strTemp.c_str());

	GetDlgItem(IDC_STATIC_VK_GESAMT_PREIS)->SetWindowText(getStammdaten().getText("IDC_STATIC_VK3_GESPREIS").c_str());
	GetDlgItem(IDC_VORTEXT)->SetWindowText(getStammdaten().getText("IDS_VORTEXT_EINFUEGEN").c_str());

	
	cb_muffendn = (CComboBox*)GetDlgItem(IDC_MUFFEN_DN);

	cbSprinklervorauswahl  = (CComboBox*) GetDlgItem(IDC_SPRINKLER_VORAUSWAHL);
	
	cb_sprinkler = (CComboBox*)GetDlgItem(IDC_SPRINKLER);

	cb_schellenvorauswahl = (CComboBox*)GetDlgItem(IDC_SCHELLEN_VORAUSWAHL);
	cb_schellen = (CComboBox*)GetDlgItem(IDC_SCHELLE);

	cb_haltervorauswahl = (CComboBox*)GetDlgItem(IDC_HALTER_VORAUSWAHL);
	cb_halter = (CComboBox*)GetDlgItem(IDC_HALTER);
	
	cb_kupplungvorauswahl = (CComboBox*)GetDlgItem(IDC_KUPPLUNG_VORAUSWAHL);
	cb_kupplung = (CComboBox*)GetDlgItem(IDC_KUPPLUNG);

	int nNlKennung = WaxSys::para_int(IB_NL_KENNUNG);
	
	for (POSITION pos = getStammdaten().get_niederlassungen().GetHeadPosition(); pos != NULL;)
	{
			Niederlassungen::data *nl = (Niederlassungen::data*)(getStammdaten().get_niederlassungen().GetNext(pos));
			
			if( nNlKennung == nl->get_nlnummer())
			{
				m_iRegion = nl->get_nRegion();	
			}
	}


	initNennweite();
	initElemente();
	
	// Combobox Rohrquali

	CString din_2458(getStammdaten().getText("IDS_RQ2458GSCH").c_str());

	CComboBox* pRohrDin = (CComboBox*)GetDlgItem(IDC_STRANGROHR_DIN);
	int index = pRohrDin->AddString(din_2458); 
	XASSERT(index != CB_ERR);
	if (index >= 0)
		pRohrDin->SetItemData(index, Rohrleitung::RQ2458GSCH);	

	CString din_2440(getStammdaten().getText("IDS_RQ2440GSCH").c_str());
	index = pRohrDin->AddString(din_2440); 
	XASSERT(index != CB_ERR);
	if (index >= 0)
		pRohrDin->SetItemData(index, Rohrleitung::RQ2440GSCH);	
	((CComboBox*)GetDlgItem(IDC_STRANGROHR_DIN))->SetCurSel(0);
	
	initRohrAnfangEnde();

	// Hoehe
	cbinitdata2 initHoehe[] = 
	{
		8, IDC_HOEHE,
		12, IDC_HOEHE,
		16, IDC_HOEHE,
		20, IDC_HOEHE,
		24, IDC_HOEHE,
		0,  0
	};
	CStringArray initstr_hoehe;
	for (int i = 0; initHoehe[i].data; i++)
	{
		CString strFormat(getStammdaten().getText("IDS_EINBAU_HOEHE_WERT").c_str());
		CString str;
		str.Format(strFormat, initHoehe[i].data);
		initstr_hoehe.Add(str);
	}

	INIT_CBDATA2(initHoehe, initstr_hoehe);
	ddx_select_cbitem(this, IDC_HOEHE, hoehe);

	//@todo: Errorexception
	XASSERT(cb_schellen && cb_halter && cb_kupplung);
	if (!cb_schellen || !cb_halter || !cb_kupplung)
		return;
	
	initSprinklerVorauswahl();
	initSchellenVorauswahl();
	initHalterVorauswahl();
	initKupplungenVorauswahl();
	
	selChangeSprinklerschellenVorauswahl();
	selChangeHalterVorauswahl();
	selChangeKupplungVorauswahl();
}



void  SpanxRohrDialog::initSprinklerVorauswahl()
{
}


void SpanxRohrDialog::initHalterVorauswahl()
{

	cb_haltervorauswahl->ResetContent();
	int index = cb_haltervorauswahl->InsertString(-1, getStammdaten().getText("IDS_LEER").c_str());
	if (index != CB_ERR)
	{
		cb_haltervorauswahl->SetItemData(index, 0);
	}

	NAMESTRINGITEM NameStringItem;
	vector<NAMESTRINGITEM> vectorNameStringItem;  
	vector<NAMESTRINGITEM>::iterator itvectorNameStringItem;	

	CMapArtikelPreis MapArtikelPreis;
	for (POSITION gpos = m_SpanxPreiseArtikelstamm.GetHeadPosition(); gpos;)
	{
		CSpanxPreiseArtikelstamm::data *g = (CSpanxPreiseArtikelstamm::data *)m_SpanxPreiseArtikelstamm.GetNext(gpos);
		
		if(g->get_Classid() == ArtikelstammClassId::TypHalter ) 
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
		
		int index = cb_haltervorauswahl->InsertString(-1,  itvectorNameStringItem->strName.c_str());
		if (index != CB_ERR)
		{
			cb_haltervorauswahl->SetItemData(index, itvectorNameStringItem->iItem);
		}
			
	}
	cb_haltervorauswahl->SetCurSel(0);	
}


void SpanxRohrDialog::DoDataExchange(CDataExchange* pDX)
{
	//CDialog::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_STRANGROHR_LAENGE, rohr_laenge);
	ddx_cbdata(pDX,	IDC_STRANGROHR_DN, rohr_dn);
		
	DDX_Text(pDX, IDC_STRANGROHR_ANZAHL, rohr_anzahl);
	ddx_cbdata(pDX, IDC_STRANGROHR_DIN,	rohr_typ);
	ddx_cbdata(pDX, IDC_STRANGROHR_ANFANG, rohr_anfang);
	ddx_cbdata(pDX, IDC_STRANGROHR_ENDE, rohr_ende);
	
	// Anzahl, ID und DN der Muffen
	DDX_Text(pDX, IDC_ANZ_MUFFEN, m_Muffen_anzahl);
	DDV_MinMaxInt(pDX, m_Muffen_anzahl, 0, max_elements);
	
	if (GetDlgItem(IDC_ANZ_SPRINKLER))
		DDX_Text(pDX, IDC_ANZ_SPRINKLER, sprinkler_anzahl);

	DDX_Text(pDX, IDC_ANZ_SCHELLEN, schellen_anzahl);
	DDX_Text(pDX, IDC_ANZ_KUPPLUNG, kupplung_anzahl);

	DDX_Control(pDX, IDC_ANZ_HALTERUNG, anzHalterCtrl);
	anzHalterCtrl.doDataExchange(pDX);

	ddx_cbdata(pDX, IDC_MUFFEN_ID, muffen_typ);
	ddx_cbdata(pDX, IDC_MUFFEN_DN, muffen_dn);
	ddx_cbdata(pDX, IDC_HOEHE, hoehe);

	// Gesamtmengen
	SetDlgItemInt(IDC_ANZ_MUFFEN_GESAMT, m_Muffen_anzahl * rohr_anzahl);
	SetDlgItemInt(IDC_ANZ_SPRINKLER_GESAMT, sprinkler_anzahl * rohr_anzahl);
	SetDlgItemInt(IDC_ANZ_SCHELLEN_GESAMT, schellen_anzahl * rohr_anzahl);
	SetDlgItemInt(IDC_ANZ_KUPPLUNG_GESAMT, kupplung_anzahl * rohr_anzahl);
	CString halterzahl;
	halterzahl.Format("%.2f", halter_anzahl * (double)rohr_anzahl);
	halterzahl.Replace(".", ",");
	SetDlgItemText(IDC_ANZ_HALTERUNG_GESAMT, halterzahl);

	if (pDX->m_bSaveAndValidate)
	{
		verzinkt = ((CButton *)GetDlgItem(IDC_VERZINKT))->GetCheck() ? true : false;
		vortext_einfuegen = ((CButton *)GetDlgItem(IDC_VORTEXT))->GetCheck() ? true : false;
	}
	else
	{
		((CButton *)GetDlgItem(IDC_VERZINKT))->SetCheck(verzinkt);
		UPDATE_GESAMTPREIS
	}
}



void SpanxRohrDialog::OnOK()
{
	if (!UpdateData(TRUE))
	{
		return;
	}
	update_preis();
	EndDialog(IDOK);
}


	
void SpanxRohrDialog::update_preis()
{
	
	UpdateData(true);

	if(m_errorListe.size() > 0)
	{
		Element::Error strErrorCodzeile(m_errorListe[0].getErrorDescription().c_str());
		size_t iWhere = strErrorCodzeile.get_errorText().find("Code");
		string strCodzeile;
		if(iWhere!=string::npos)
		{
			strCodzeile = strErrorCodzeile.get_errorText().substr (0, iWhere); 
			strCodzeile = strCodzeile + "\nVk3 Preis kann nicht berechnet werden!!";
		}
		AfxMessageBox( strCodzeile.c_str());
		m_errorListe.clear();
		preis = 0.0;
		setpreis(IDC_GESAMTPREIS, preis);
		return;
	}

	VK3Berechnung berech;
	berech.set_werkauftrag(werkauftrag);
	berech.calc_preis();
	preis = berech.get_preis();

	// Fehlt noch Sprinkler, Schellen, Halter und Kupplung
	preis += get_preis(IDC_ANZ_SPRINKLER, IDC_PREIS_SPRINKLER);
	preis += get_preis(IDC_ANZ_SCHELLEN, IDC_PREIS_SCHELLE);
	preis += get_preis(IDC_ANZ_HALTERUNG, IDC_PREIS_HALTER);
	preis += get_preis(IDC_ANZ_KUPPLUNG, IDC_PREIS_KUPPLUNG);
	preis += get_preis(IDC_ANZ_PRALLBLECH, IDC_PREIS_PRALLBLECH);
	if (verzinkt)
	{
		Rohrleitung rohr;
		
		double gewicht = ((Rohrleitung*)werkauftrag)->getGesGewicht();
		preis += gewicht;
	}

	setpreis(IDC_GESAMTPREIS, preis);

	m_Gesamtgewicht = ((Rohrleitung*)werkauftrag)->getGesGewicht();

	
	CString zeit;
	zeit.Format("%.2f %s", get_montagezeit(), getStammdaten().getText("IDS_STUNDE").c_str());
	zeit.Replace('.', ',');

	GetDlgItem(IDC_ARBEITSZEIT)->SetWindowText(zeit);

}


double SpanxRohrDialog::get_preis(UINT idanz, UINT idpreis)
{
	if (!GetDlgItem(idpreis) || !GetDlgItem(idanz))
		return 0.0;
	CString tmp_preis, tmp_anz;
	GetDlgItemText(idpreis, tmp_preis);
	tmp_preis.Replace(',', '.');
	GetDlgItemText(idanz, tmp_anz);
	tmp_anz.Replace(',', '.');
	double ret = atof(tmp_anz) * atof(tmp_preis) * rohr_anzahl;
	return ret;
}



void SpanxRohrDialog::set_schellenpreis()
{
	
	GetDlgItem(IDC_PREIS_SCHELLE)->SetWindowText(getStammdaten().getText("IDS_NULL_EURO").c_str());
	long  artikelnr = getItemData(cb_schellen);
	for (POSITION gpos = m_SpanxPreiseArtikelstamm.GetHeadPosition(); gpos;)
	{
		CSpanxPreiseArtikelstamm::data *g = (CSpanxPreiseArtikelstamm::data *)m_SpanxPreiseArtikelstamm.GetNext(gpos);
		
		if (artikelnr == g->get_artikelnr() && g->get_Region() == m_iRegion)
		{
			setpreis(IDC_PREIS_SCHELLE, g->get_Preis());
			break;
		}
	}
	UPDATE_GESAMTPREIS
}



void SpanxRohrDialog::set_halterpreis()
{	
	GetDlgItem(IDC_PREIS_HALTER)->SetWindowText(getStammdaten().getText("IDS_NULL_EURO").c_str());
	long  artikelnr = getItemData(cb_halter);
	for (POSITION gpos = m_SpanxPreiseArtikelstamm.GetHeadPosition(); gpos;)
	{
		CSpanxPreiseArtikelstamm::data *g = (CSpanxPreiseArtikelstamm::data *)m_SpanxPreiseArtikelstamm.GetNext(gpos);
		
		if (artikelnr == g->get_artikelnr() && g->get_Region() == m_iRegion)
		{
			setpreis(IDC_PREIS_HALTER, g->get_Preis());
			break;
		}
	}
	UPDATE_GESAMTPREIS
}



void SpanxRohrDialog::set_kupplungpreis()
{

	GetDlgItem(IDC_PREIS_KUPPLUNG)->SetWindowText(getStammdaten().getText("IDS_NULL_EURO").c_str());
	long  artikelnr = getItemData(cb_kupplung);

	for (POSITION gpos = m_SpanxPreiseArtikelstamm.GetHeadPosition(); gpos;)
	{
		CSpanxPreiseArtikelstamm::data *g = (CSpanxPreiseArtikelstamm::data *)m_SpanxPreiseArtikelstamm.GetNext(gpos);
		
		if (artikelnr == g->get_artikelnr() && g->get_Region() == m_iRegion)
		{
			setpreis(IDC_PREIS_KUPPLUNG, g->get_Preis());
			break;
		}
	}
	UPDATE_GESAMTPREIS
}



void SpanxRohrDialog::updateDN()
{
	/**
	Ändert sich die DN von Rohr oder Muffe, werden die DNs der Sprinkler
	schellen, Halter und Kupplung neu gesetzt und der entsprechende Preis
	angezeigt
	*/
	
	CComboBox *rohrdn = (CComboBox*)GetDlgItem(IDC_STRANGROHR_DN);
	CComboBox *rohrdin = (CComboBox*)GetDlgItem(IDC_STRANGROHR_DIN);
	XASSERT(rohrdn && rohrdin);
	if (!rohrdn || !rohrdin)
		return;
	
	WaxPartForm::rohrquali_autoselect(rohrdn, rohrdin);
	update_preis();
}





void SpanxRohrDialog::initKupplungenVorauswahl()
{

	cb_kupplungvorauswahl->ResetContent();
	int index = cb_kupplungvorauswahl->InsertString(-1, getStammdaten().getText("IDS_LEER").c_str());
	if (index != CB_ERR)
	{
		cb_kupplungvorauswahl->SetItemData(index, 0);
	}
	
	NAMESTRINGITEM NameStringItem;
	vector<NAMESTRINGITEM> vectorNameStringItem;  
	vector<NAMESTRINGITEM>::iterator itvectorNameStringItem;	


	CMapArtikelPreis MapArtikelPreis;
	for (POSITION gpos = m_SpanxPreiseArtikelstamm.GetHeadPosition(); gpos;)
	{
		CSpanxPreiseArtikelstamm::data *g = (CSpanxPreiseArtikelstamm::data *)m_SpanxPreiseArtikelstamm.GetNext(gpos);
		
		if(g->get_Classid() == ArtikelstammClassId::TypKupplung ) 
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
		
		int index = cb_kupplungvorauswahl->InsertString(-1, itvectorNameStringItem->strName.c_str());
		if (index != CB_ERR)
		{
			cb_kupplungvorauswahl->SetItemData(index, itvectorNameStringItem->iItem);
		}
		
	}

	cb_kupplungvorauswahl->SetCurSel(0);
}




void SpanxRohrDialog::setpreis(UINT dlgid, double preis)
{
	CString str_preis;
	str_preis.Format("%.2f €", preis);
	str_preis.Replace('.', ',');
	GetDlgItem(dlgid)->SetWindowText(str_preis);
}



DWORD SpanxRohrDialog::getItemData(CComboBox *bx)
{
	int sel = bx->GetCurSel();
	if (sel == CB_ERR)
	{
		bx->SetCurSel(0);
		return bx->GetItemData(0);
	}
	return bx->GetItemData(sel);
}



string SpanxRohrDialog::xml_data()
{
	/**
	Erzeugt aus den Eingaben einen String in 'xml' Format
	Beispiel für Verteiler und Strangrohr:
*/
	#define VERSION "1.0"
	string vortext, langtext;
	langtext = format_langtext();

	if (vortext_einfuegen)
		vortext = format_vortext();
	
	double zeit = get_montagezeit();


	string ret = str(format("<Wax+xmlV%s><%s>%s\n\t%s\n\t<rohr>\n\t\t<anzahl>%i</anzahl>\n\t\t<dn>%i</dn>\n\t\t<laenge>%i</laenge>\n\t\t"
		"<din>%i</din>\n\t\t<anfang>%i</anfang>\n\t\t<ende>%i</ende>\n\t\t<verzinkt>%s</verzinkt>\n\t</rohr>\n\t"
		"<muffen>\n\t\t<anzahl>%i</anzahl>\n\t\t<dn>%i</dn>\n\t\t<typ>%i</typ><laenge>%i</laenge></muffen>\n\t"
		"<sprinkler>\n\t\t<anzahl>%i</anzahl>\n\t\t<typ>%s</typ></sprinkler>\n\t"
		"<schellen>\n\t\t<anzahl>%i</anzahl>\n\t\t<typ>%s</typ></schellen>\n\t"
		"<halter>\n\t\t<menge>%f</menge>\n\t\t<typ>%s</typ></halter>\n\t"
		"<kupplung>\n\t\t<anzahl>%i</anzahl>\n\t\t<typ>%s</typ></kupplung>\n\t"
		"<montagehoehe>%i</montagehoehe>\n\t"
		"<zeit>%.2f</zeit>\n\t"
		"<zoll>%s</zoll>\n\t<preis>%f</preis>\n\t<Gesamtgewicht>%f</Gesamtgewicht>\n</%s>") % VERSION % get_xmltag() % vortext % langtext % rohr_anzahl % rohr_dn % rohr_laenge % rohr_typ % rohr_anfang % rohr_ende %
		(verzinkt ? "true" : "false") % m_Muffen_anzahl % muffen_dn % muffen_typ % ele_laenge % sprinkler_anzahl % m_SprinklerArtikelNr % schellen_anzahl %
		m_SchellenArtikelNr % halter_anzahl % m_HalterArtikelNr % kupplung_anzahl % m_KupplungArtikelNr % 
		hoehe % zeit % (zoll ? "true" : "false") % preis % m_Gesamtgewicht % get_xmltag() );
	return ret;
}




string SpanxRohrDialog::format_vortext() const
{
	string vortext;
	const CObList &vortexte = getStammdaten().get_rohrtexte();

	for (POSITION pos = vortexte.GetHeadPosition(); pos; )
	{
		RohrTexte::data *dat = (RohrTexte::data *)vortexte.GetNext(pos);
		if (dat->get_rohrtyp() == get_rohrtyp() && dat->get_verzinkt() == verzinkt)
		{
			if (dat->get_vortext())
			{
				vortext = (string)"<vortext>" + (string)(LPCSTR)dat->get_langtext() + (string)"</vortext>";
				break;
			}
		}
	}
	return vortext;
}





void SpanxRohrDialog::init_attr(const string& data)
{
	
	string tag_rohr = xml_read_string("rohr", data.c_str());
	rohr_anzahl = xml_read_int("anzahl", tag_rohr.c_str());
	rohr_dn = xml_read_int("dn", tag_rohr.c_str());
	rohr_laenge = xml_read_int("laenge", tag_rohr.c_str());
	rohr_typ = xml_read_int("din", tag_rohr.c_str());
	rohr_anfang = xml_read_int("anfang", tag_rohr.c_str());
	rohr_ende = xml_read_int("ende", tag_rohr.c_str());
	verzinkt = xml_read_string("verzinkt", tag_rohr.c_str()) == "true" ?  true : false;

	string tag_muffe = xml_read_string("muffen", data.c_str());
	m_Muffen_anzahl = xml_read_int("anzahl", tag_muffe.c_str());
	muffen_dn = xml_read_int("dn", tag_muffe.c_str());
	muffen_typ = xml_read_int("typ", tag_muffe.c_str());
	ele_laenge = xml_read_int("laenge", tag_muffe.c_str());

	string tag_sprinkler = xml_read_string("sprinkler", data.c_str());
	sprinkler_anzahl = xml_read_int("anzahl", tag_sprinkler.c_str());
	m_SprinklerArtikelNr = xml_read_string("typ", tag_sprinkler.c_str());

	string tag_schellen= xml_read_string("schellen", data.c_str());
	schellen_anzahl = xml_read_int("anzahl", tag_schellen.c_str());
	m_SchellenArtikelNr = xml_read_string("typ", tag_schellen.c_str());
	
	string tag_halter= xml_read_string("halter", data.c_str());
	halter_anzahl = xml_read_double("menge", tag_halter.c_str());
	m_HalterArtikelNr = xml_read_string("typ", tag_halter.c_str());
	
	string tag_kupplung = xml_read_string("kupplung", data.c_str());
	kupplung_anzahl = xml_read_int("anzahl", tag_kupplung.c_str());
	m_KupplungArtikelNr = xml_read_string("typ", tag_kupplung.c_str());
		
	hoehe = xml_read_int("montagehoehe", data.c_str());
	zoll= xml_read_string("zoll", data.c_str()) == "true" ? true : false;
}



void  SpanxRohrDialog::initAllMXItems(int GruppenID, CComboBox* ItemComboBox)
{
	
	ItemComboBox->ResetContent();
	
	if( GruppenID != 0)
	{
		CMapArtikelPreis MapArtikelPreis;
		for (POSITION gpos = m_SpanxPreiseArtikelstamm.GetHeadPosition(); gpos;)
		{
			CSpanxPreiseArtikelstamm::data *g = (CSpanxPreiseArtikelstamm::data *)m_SpanxPreiseArtikelstamm.GetNext(gpos);
			
			if (g->get_artikelgruppenID() == GruppenID)
			{
				
				CMapArtikelPreis::iterator itCur = MapArtikelPreis.find(g->get_artikelnr());
				if(itCur == MapArtikelPreis.end())
				{
					MapArtikelPreis[g->get_artikelnr()];
					
					int index = ItemComboBox->InsertString(-1, g->get_ArtikelstammKurztext());
					if (index != CB_ERR)
					{
						ItemComboBox->SetItemData(index, g->get_artikelnr());
					}
				}			
			}
		}
	}
	else
	{
		int index = ItemComboBox->InsertString(-1, getStammdaten().getText("IDS_LEER").c_str());
		if (index != CB_ERR)
		{
			ItemComboBox->SetItemData(index, 0);
		}
	}

	ItemComboBox->SetCurSel(0);
}



void SpanxRohrDialog::selChangeSprinklerschellenVorauswahl()
{
	long  GruppenID = getItemData(cb_schellenvorauswahl);
	initAllMXItems( GruppenID, cb_schellen);
	set_schellenpreis();
}



void SpanxRohrDialog::selChangeHalterVorauswahl()
{
	long  GruppenID = getItemData(cb_haltervorauswahl);
	initAllMXItems( GruppenID, cb_halter);
	set_halterpreis();
}



void SpanxRohrDialog::selChangeKupplungVorauswahl()
{
	long  GruppenID = getItemData(cb_kupplungvorauswahl);
	initAllMXItems( GruppenID, cb_kupplung);
	set_kupplungpreis();
}



void SpanxRohrDialog::SetCurlSelOfGruppe(int artikelnr, CComboBox* pComboxGroup)
{	
	CSpanxPreiseArtikelstamm::data *g  = NULL;
	// artikelnummmer heraussuchen
	for (POSITION gpos = m_SpanxPreiseArtikelstamm.GetHeadPosition(); gpos;)
	{
		g = (CSpanxPreiseArtikelstamm::data *)m_SpanxPreiseArtikelstamm.GetNext(gpos);
		
		if (artikelnr == g->get_artikelnr() && g->get_Region() == m_iRegion)
		{
			break;
		}
	}
	// Listbox eintrag fuer zuvor gefundene artikelgruppe heraussuchen und selktieren
	for(int i =0; i < pComboxGroup->GetCount(); i++)
	{
		if(pComboxGroup->GetItemData(i) == (UINT)  g->get_artikelgruppenID())
		{
			pComboxGroup->SetCurSel(i);
			break;
		}
	}
	return;
}


void SpanxRohrDialog::SetCurlSelDependOnItemData(int iItemData, CComboBox* pComboxArtikel)	
{
	for(int i =0; i < pComboxArtikel->GetCount(); i++)
	{
		if(pComboxArtikel->GetItemData(i) == (UINT)iItemData)
		{
			pComboxArtikel->SetCurSel(i);
			break;
		}
	}

return;
}


void SpanxRohrDialog::OnCancel()
{
	m_Cancel = true;

	CDialog::OnCancel();
}
