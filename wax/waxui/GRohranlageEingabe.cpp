
#include "stdafx.h"


#include <math.h>
#include "resource.h"
#include <afxadv.h> // fuer CSharedFile


#include <mxcontrols/msgbox.h>
using namespace MxGui;


#define WM_UPDATE_FIELDS WM_USER+1234

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(GRohranlageEingabe, WaxPartForm)

BEGIN_MESSAGE_MAP(GRohranlageEingabe, WaxPartForm)
	ON_EN_KILLFOCUS(IDC_GROHR_FERTIGUNGSHINWEIS,onUpdate)		
	ON_CBN_SELCHANGE(IDC_GROHR_KFARBE,			onUpdate)
	ON_CBN_SELCHANGE(IDC_GROHR_TYP,				onUpdate)
	ON_CBN_SELCHANGE(IDC_GROHR_DICHTMITTEL,		onUpdate)
	ON_BN_CLICKED(IDC_RADIO_EIS, &GRohranlageEingabe::OnBnClickedRadioEis)
END_MESSAGE_MAP()

GRohranlageEingabe::~GRohranlageEingabe()
{

}

GRohranlageEingabe::GRohranlageEingabe() : WaxPartForm(IDD_GROHRANLAGE, getStammdaten().getText("IDD_GROHRANLAGE").c_str())
{
	rohrKFarbe	= Rohrleitung::KEINE;
	rohrQuali	= Rohrleitung::RQ2440VZIN;
	typ = Gewinderohr::WASSER;
	oldtyp = -1;
	dichtmittel = Gewinderohr::LOCTITE;
	enableOK = false;
	fertigungshinweis = "";
	m_iAuswaertsfertigung = FALSE;
}

BOOL GRohranlageEingabe::OnInitDialog()
{
	/*
	Dialoginitialisierung 
	Hier werden den Comboboxtexten ein Integerwert zugewiesen, 
	der einer ElementID entspricht. 
	*/
	if (!WaxPartForm::OnInitDialog())
		return false;

	
	// Combobox Typ
	cbinitdata3 initTyp[] = 
	{
		getStammdaten().getText("IDS_LEER").c_str(),	Gewinderohr::LEERTYP,	IDC_GROHR_TYP,
		getStammdaten().getText("IDS_WASSER").c_str(),	Gewinderohr::WASSER,	IDC_GROHR_TYP,
		getStammdaten().getText("IDS_SCHAUMANLAGE").c_str(),Gewinderohr::SCHAUM,	IDC_GROHR_TYP,
		getStammdaten().getText("IDS_GAS").c_str(),		Gewinderohr::GAS,	IDC_GROHR_TYP,
		"",			0,		0
	};
	INIT_CBDATA3(initTyp);

	// PR: Dichtmittel "keine" muss auswählbar sein, damit auch 
	// Rohre ohne Fittinge und ohne Vorbehandlung bestellt werden können
	// (Hr. Hirsekorn 2.6.04) -> IDS_LEER in IDS_KEINE geändert, gilt auch für Kappenfarbe
	cbinitdata3 initDichtmittel[] = 
	{
		getStammdaten().getText("IDS_KEINE").c_str(),		Gewinderohr::LEER,					IDC_GROHR_DICHTMITTEL,
		getStammdaten().getText("IDS_LOCTITE").c_str(),		Gewinderohr::LOCTITE,				IDC_GROHR_DICHTMITTEL,
		getStammdaten().getText("IDS_HANF").c_str(),		Gewinderohr::HANF,					IDC_GROHR_DICHTMITTEL,
		getStammdaten().getText("IDS_TEFLON").c_str(),		Gewinderohr::TEFLON,				IDC_GROHR_DICHTMITTEL,
		getStammdaten().getText("IDS_SYNTHESOL").c_str(),	Gewinderohr::SYNTHESOL,				IDC_GROHR_DICHTMITTEL,
		getStammdaten().getText("IDS_NEOFERMIT").c_str(),	Gewinderohr::NEOFERMIT,				IDC_GROHR_DICHTMITTEL,
		getStammdaten().getText("IDS_BESTMK2241").c_str(),	Gewinderohr::BESTMK2241,			IDC_GROHR_DICHTMITTEL,
		"",			0,		0
	};
	INIT_CBDATA3(initDichtmittel);
	
	// Combobox Rohrquali
	BauelementGruppen bg;
	int offset = 30;
	cbinitdata3 initRohrquali[] = 
	{
		getStammdaten().getText("IDS_LEER").c_str(),			Rohrleitung::LEER,			    IDC_GROHR_DIN,
		bg.getBezeichnung(Rohrleitung::RQ2440GSCH + offset),	Rohrleitung::RQ2440GSCH,        IDC_GROHR_DIN,
		bg.getBezeichnung(Rohrleitung::RQ2440VZIN + offset),	Rohrleitung::RQ2440VZIN,		IDC_GROHR_DIN,
		bg.getBezeichnung(Rohrleitung::RQ2458VZIN + offset),	Rohrleitung::RQ2458VZIN,		IDC_GROHR_DIN,
		bg.getBezeichnung(Rohrleitung::RQEN10217VERZ + offset),	Rohrleitung::RQEN10217VERZ,		IDC_GROHR_DIN,
		"",					0,		0
	};
	INIT_CBDATA3(initRohrquali);
	
	// Combobox Kappenfarbe
	cbinitdata3 initKFarbe[] = 
	{
		getStammdaten().getText("IDS_KEINE").c_str(),	Rohrleitung::KEINE,	IDC_GROHR_KFARBE,
		getStammdaten().getText("IDS_GELB").c_str(),	Rohrleitung::GELB,	IDC_GROHR_KFARBE,
		getStammdaten().getText("IDS_ROT").c_str(),	    Rohrleitung::ROT,	IDC_GROHR_KFARBE,
		getStammdaten().getText("IDS_BLAU").c_str(),	Rohrleitung::BLAU,	IDC_GROHR_KFARBE,
		getStammdaten().getText("IDS_GRUEN").c_str(),	Rohrleitung::GRUEN,	IDC_GROHR_KFARBE,
		getStammdaten().getText("IDS_WEISS").c_str(),	Rohrleitung::WEISS,	IDC_GROHR_KFARBE,
		getStammdaten().getText("IDS_SCHWARZ").c_str(), Rohrleitung::SCHWARZ,IDC_GROHR_KFARBE,
		"",			0,		0
	};
	INIT_CBDATA3(initKFarbe);
	
	rohr = dynamic_cast<GewinderohrData*>(get_data());
	XASSERT(rohr);
	if (!rohr)
		return false;
	Gewinderohr* grohr = getGewinderohr();
	
	
	// Sychronisieren
	oldtyp = grohr->get_typ();
	if (rohr->doSync() || (grohr->get_status().get_wanr() != 0 && !WaxSys::isAV()))
	{
		GetDlgItem(IDC_GROHR_DIN)->EnableWindow(FALSE);
		GetDlgItem(IDC_GROHR_TYP)->EnableWindow(FALSE);
		GetDlgItem(IDC_GROHR_DICHTMITTEL)->EnableWindow(FALSE);	
	}
	else 
	{
		GetDlgItem(IDC_GROHR_DIN)->EnableWindow(TRUE);
		GetDlgItem(IDC_GROHR_TYP)->EnableWindow(TRUE);
		GetDlgItem(IDC_GROHR_DICHTMITTEL)->EnableWindow(TRUE);
		enableOK = true; 
	}

	CEdit* ed = (CEdit*)GetDlgItem(IDC_GROHR_FERTIGUNGSHINWEIS);
	ed->SetLimitText(512);

	// Dialogfelder: Rohrdaten setzen
	rohrQuali   = grohr->get_rohrQuali();
	rohrKFarbe  = grohr->get_rohrKFarbe();
	dichtmittel = grohr->get_dichtmittel();
	typ         = grohr->get_typ();
	fertigungshinweis = grohr->get_fertigungshinweis();
	
	
	setWindowsTextWithColon(IDC_STATIC_TYP, "IDS_TYP");
	setWindowsTextWithColon(IDC_STATIC_DICHTMITTEL, "IDS_DICHTMITTEL");
	setWindowsTextWithColon(IDC_STATIC_KAPPENFARBE, "IDS_KAPPENFARBE");
	setWindowsTextWithColon(IDC_STATIC_FH, "IDS_FERTIGUNGSHINWEIS");
	setWindowsText(IDC_STATIC_TXT, "IDC_STATIC_TXT");



	// Update
	UpdateData(FALSE);
	return true;
}



void GRohranlageEingabe::DoDataExchange(CDataExchange* pDX)
{
	/*
	Dialogdatenaustausch 
	Hier werden die Dialogdaten an die Klasse Standardverteiler übergeben |
	Erzeugung der Rohrkonstruktion und der Graphik  
	*/
	WaxPartForm::DoDataExchange(pDX);

	//{{AFX_DATA_MAP(GewinderohrEingabe)
 	DDX_Text(pDX, IDC_GROHR_FERTIGUNGSHINWEIS, fertigungshinweis);
	DDX_Check(pDX, IDC_RADIO_EIS, m_iAuswaertsfertigung);
	//}}AFX_DATA_MAP


	ddx_cbdata(pDX, IDC_GROHR_DIN,			rohrQuali);
	ddx_cbdata(pDX, IDC_GROHR_KFARBE,		rohrKFarbe);
 	ddx_cbdata(pDX, IDC_GROHR_DICHTMITTEL,	dichtmittel);
	ddx_cbdata(pDX, IDC_GROHR_TYP,			typ);

	
	int iCurrentSel = ((CComboBox*)GetDlgItem(IDC_GROHR_TYP))->GetItemData(((CComboBox*)GetDlgItem(IDC_GROHR_TYP))->GetCurSel());
	if(Gewinderohr::SCHAUM == iCurrentSel)
	{
		WaxPart* parent = const_cast<WaxPart*>(get_data()->get_parent());
		if (parent->IsKindOf(RUNTIME_CLASS(SammelAuftrag)))
		{
			Werkauftrag* wa = parent->get_werkauftrag();

			CString strFertigungshinweis = wa->get_bemerkung();
			if(strFertigungshinweis.Find(getStammdaten().getText("IDS_KEINE_VERZINKTEN_ROHRE").c_str()) == -1)
			{
				strFertigungshinweis +=  getStammdaten().getText("IDS_KEINE_VERZINKTEN_ROHRE").c_str();
				wa->set_bemerkung(strFertigungshinweis);
			}
		}
		else if(fertigungshinweis.Find(getStammdaten().getText("IDS_KEINE_VERZINKTEN_ROHRE").c_str()) == -1)
		{
			fertigungshinweis +=  getStammdaten().getText("IDS_KEINE_VERZINKTEN_ROHRE").c_str();
			GetDlgItem(IDC_GROHR_FERTIGUNGSHINWEIS)->SetWindowText(fertigungshinweis);
		}
		
		int iItemData = ((CComboBox*)GetDlgItem(IDC_GROHR_DICHTMITTEL))->GetItemData(Gewinderohr::PERMABOND);
		if(iItemData != Gewinderohr::PERMABOND)
		{
			((CComboBox*)GetDlgItem(IDC_GROHR_DICHTMITTEL))->InsertString(Gewinderohr::PERMABOND, getStammdaten().getText("IDS_PERMABOND").c_str());
			((CComboBox*)GetDlgItem(IDC_GROHR_DICHTMITTEL))->SetItemData(Gewinderohr::PERMABOND, Gewinderohr::PERMABOND);
			((CComboBox*)GetDlgItem(IDC_GROHR_DICHTMITTEL))->EnableWindow(FALSE);
		}
	}
	else
	{
		Gewinderohr::DICHTMITTEL dichtmittelIndex = (Gewinderohr::DICHTMITTEL)(DWORD)((CComboBox*)GetDlgItem(IDC_GROHR_DICHTMITTEL))->GetItemDataPtr(Gewinderohr::PERMABOND);
		if(dichtmittelIndex == Gewinderohr::PERMABOND)
		{
			((CComboBox*)GetDlgItem(IDC_GROHR_DICHTMITTEL))->DeleteString(Gewinderohr::PERMABOND);
			((CComboBox*)GetDlgItem(IDC_GROHR_DICHTMITTEL))->EnableWindow(TRUE);
			int iWo = -1;
			if((iWo = fertigungshinweis.Find(getStammdaten().getText("IDS_KEINE_VERZINKTEN_ROHRE").c_str())) > -1)
			{
				CString strtemp =  getStammdaten().getText("IDS_KEINE_VERZINKTEN_ROHRE").c_str();
				fertigungshinweis.Delete(iWo, strtemp.GetLength());
				((CComboBox*)GetDlgItem(IDC_GROHR_FERTIGUNGSHINWEIS))->SetWindowText(fertigungshinweis);
			}
		}
	}

	

	rohr = dynamic_cast<GewinderohrData*>(get_data());
	XASSERT(rohr);
	if (!rohr)
		return;
	Gewinderohr* grohr = getGewinderohr();
	if (!grohr)
		return;

	
	if (pDX->m_bSaveAndValidate) 
	{
		try
		{
			grohr->set_dichtmittel(dichtmittel);
			grohr->set_typ(typ);
			grohr->set_rohrKFarbe(rohrKFarbe);
			grohr->set_fertigungshinweis(fertigungshinweis);
			grohr->Set_Fertigungsstaette(m_iAuswaertsfertigung);

		}
		catch(Element::Error e)
		{
			MsgBox::show_error(e.get_errorText());
		}
	} 
	else
	{
		// Enable/Disable
		if (grohr->get_elementArray().GetSize() > 0)
		{
			GetDlgItem(IDC_GROHR_DIN)->EnableWindow(FALSE);
			GetDlgItem(IDC_GROHR_TYP)->EnableWindow(FALSE);
		}
		else if (enableOK)
		{
			GetDlgItem(IDC_GROHR_DIN)->EnableWindow(TRUE);
			GetDlgItem(IDC_GROHR_TYP)->EnableWindow(TRUE);
		}

		m_iAuswaertsfertigung = grohr->Get_Fertigungsstaette();

	}


	if(WaxSys::isAV())
	{
		GetDlgItem(IDC_RADIO_EIS)->ShowWindow(SW_HIDE);
		
		if( !grohr->get_sichtbereich() && !grohr->get_zeugnis())
		{
			if( (grohr->get_dichtmittel() == Gewinderohr::HANF || grohr->get_dichtmittel() == Gewinderohr::NEOFERMIT) 
				&&
				grohr->get_typ() == Gewinderohr::WASSER)
			{
				GetDlgItem(IDC_RADIO_EIS)->ShowWindow(SW_SHOW);
				return;
			}
			else
			{
				((CButton*)GetDlgItem(IDC_RADIO_EIS))->SetCheck(FALSE);
				GetDlgItem(IDC_RADIO_EIS)->ShowWindow(SW_HIDE);
				ShowWindow(SW_HIDE);
				ShowWindow(SW_SHOW);
			}
		}

		if( !grohr->get_sichtbereich()  && grohr->get_dichtmittel() == Gewinderohr::TEFLON && grohr->get_typ() == Gewinderohr::GAS)
		{
				GetDlgItem(IDC_RADIO_EIS)->ShowWindow(SW_SHOW);
		}
		else
		{
				((CButton*)GetDlgItem(IDC_RADIO_EIS))->SetCheck(FALSE);
				GetDlgItem(IDC_RADIO_EIS)->ShowWindow(SW_HIDE);
				ShowWindow(SW_HIDE);
				ShowWindow(SW_SHOW);
		}

	}
}

//Plausibilitätscheck, wird aufgerufen, bevor UpdateData(TRUE) aufgerufen 
//wird.
bool GRohranlageEingabe::check_plausis()
{
	if (!WaxPartForm::check_plausis())
		return false; 

	
	Gewinderohr* grohr =getGewinderohr();
	if (!grohr)
		return false;
	
	// Rohr und Elemente checken
	CString err = "";
	err = grohr->checkPlausis();
	if (err.GetLength() > 0)
	{
		disableCheck();
		AfxMessageBox(err, MB_OK | MB_ICONINFORMATION);
		((CFrameWnd*)AfxGetMainWnd())->SetMessageText(err);
		return false;
	}

	// OK
	CString str; str = getStammdaten().getText("IDS_EINGABEN_KORREKT").c_str();
	((CFrameWnd*)AfxGetMainWnd())->SetMessageText(str);
	return true;
}



//Prüft die Plausis und sorgt für ein Update der Grafik
void GRohranlageEingabe::onUpdate()
{
	UpdateData();
	
	
	Gewinderohr* grohr = getGewinderohr();
	if (!grohr)
		return;

	CComboBox *bx = (CComboBox*)GetDlgItem(IDC_GROHR_DICHTMITTEL);

	// Rohrquali & Dichtmittel
	if (typ == Gewinderohr::GAS)
	{

		if (typ != oldtyp)
		{
			oldtyp = typ;
			grohr->set_dichtmittel(Gewinderohr::TEFLON);
			dichtmittel = grohr->get_dichtmittel();
		}
	}
	else
	{	
		if (typ == Gewinderohr::WASSER)
		{
			if (typ != oldtyp)
			{
				oldtyp = typ;
				if (grohr->get_beschichtung().isGepulvert())
				{
					grohr->set_dichtmittel(Gewinderohr::HANF);
					dichtmittel = grohr->get_dichtmittel();
				}
				else
				{
					grohr->set_dichtmittel(Gewinderohr::LOCTITE);
					dichtmittel = grohr->get_dichtmittel();
				}
			}
		}
		else
		{   //SCHAUM
			if (typ != oldtyp)
			{
				oldtyp = typ;
				grohr->set_dichtmittel(Gewinderohr::PERMABOND);
				dichtmittel = grohr->get_dichtmittel();
			}
		}
	}
	
	bx->SetCurSel(dichtmittel);

	//check Plausis
	doUpdate(); 

	// im Sammelauftrag alle Positionen synchronisieren
	WaxPart *parent = const_cast<WaxPart*>(get_data()->get_parent());
	if (parent->IsKindOf(RUNTIME_CLASS(SammelAuftrag)))
		parent->doSync();

	UpdateData();
}

/**
Zugriff auf das Entity Objekt Strangrohr aus WaxPart
*/
Gewinderohr* GRohranlageEingabe::getGewinderohr()
{
	//WaxPart Objekt holen
	WaxPart* rohrData = dynamic_cast<WaxPart*>(get_data());
	XASSERT(rohrData);
	if (!rohrData)
		return NULL;


	//Aus dem WaxPart das Gewinderohr holen
	Gewinderohr* rohr = dynamic_cast<Gewinderohr*>(rohrData->get_werkauftrag());
	XASSERT(rohr);

	return rohr;
}




void GRohranlageEingabe::OnBnClickedRadioEis()
{
	m_iAuswaertsfertigung = ((CButton*)GetDlgItem(IDC_RADIO_EIS))->GetCheck();
	
	Gewinderohr* rohr = getGewinderohr();
	rohr->Set_Fertigungsstaette(m_iAuswaertsfertigung);
	WaxPart *parent = const_cast<WaxPart*>(get_data()->get_parent());
	if (parent->IsKindOf(RUNTIME_CLASS(SammelAuftrag)))
		parent->doSync(false);}
