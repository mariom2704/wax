#include "stdafx.h"
#include "resource.h"

#include "..\waxstamm\verzinkung.h"
#include <mxutil/ddx_extension.h>
#include "BeschichtungEingabeForm.h"


BEGIN_MESSAGE_MAP(BeschichtungEingabeForm, WaxPartForm)
	ON_COMMAND(IDC_BUTKFARBE, onKFarbe)
	ON_COMMAND(WM_AUTOSOKO, on_autosoko)
	ON_BN_CLICKED(IDC_SICHTBEREICH, &BeschichtungEingabeForm::OnBnClickedSichtbereich)
END_MESSAGE_MAP()

IMPLEMENT_DYNCREATE(BeschichtungEingabeForm, WaxPartForm)


BeschichtungEingabeForm::BeschichtungEingabeForm()
 : WaxPartForm(IDD_BESCHICHTUNG, getStammdaten().getText("IDD_BESCHICHTUNG").c_str()), sichtbereich(false), zeugnis(false)
{
	m_KollilisteExport = 0;
	rohrKFarbe       = 0; 
}

BeschichtungEingabeForm::~BeschichtungEingabeForm()
{
}

//Plausibilitätscheck, wird aufgerufen, bevor UpdateData(TRUE) 
//aufgerufen wird.
bool BeschichtungEingabeForm::check_plausis()
{
	if (!beschichtung.check_plausis())
		return false;

	Werkauftrag& wa = *get_data()->get_werkauftrag();
	
	for (POSITION pos = getStammdaten().get_dummymaterialnummern().GetHeadPosition(); pos;)
	{
		DummyMaterialnummern::data* dat= (DummyMaterialnummern::data*)getStammdaten().get_dummymaterialnummern().GetNext(pos);
		
		if (wa.get_leitkarte().get_sdMaterialnummer() == dat->get_nummer())
		{
			// bei Strang gibs nur 1 Nr 
			if (dat->get_klasse() == "Strangrohr ApaxWerkauftrag SammelWerkauftrag")
				break;

#ifndef __WAX_FUER_DRITTE__
			if (dat->get_verzinkt() != wa.get_beschichtung().isVerzinkt())
			{
				CString str; str = getStammdaten().getText("IDS_SDMATNR_BESCH").c_str();
				AfxMessageBox(str, MB_OK | MB_ICONERROR);
				return true; // Nur warnen, Tab kann trotzdem gewechselt werden
			}
#endif
			break;
		}
	}
		
	if (edBemerkung.GetLineCount() > MAXLINE_BEMERKUNG)
	{
		CString msg, str; str = getStammdaten().getText("IDS_ZEILEN_BEMERKUNG").c_str();
		msg.Format(str, MAXLINE_BEMERKUNG);
		AfxMessageBox(msg, MB_OK | MB_ICONERROR);
		edBemerkung.SetFocus();
		return false;
	}
	return true;
}

//Dialog initialisieren
BOOL BeschichtungEingabeForm::OnInitDialog()
{
	if (!WaxPartForm::OnInitDialog())
		return FALSE;

	// Control Rohrbeschichtung
	beschichtung.create(this, IDC_BESCHICHTUNGDUMMY);
	edBemerkung.SetLimitText(512);
	
	SammelAuftrag *sa = dynamic_cast<SammelAuftrag*>(get_data());
	if (!sa)
	{
		GetDlgItem(IDC_KAPPENFARBE)->EnableWindow(false);
		GetDlgItem(IDC_BUTKFARBE)->EnableWindow(false);
		GetDlgItem(IDC_TEXTKFARBE)->EnableWindow(false);
	}

	// Combobox Kappenfarbe
	cbinitdata3 initKFarbe[] = 
	{
		getStammdaten().getText("IDS_LEER").c_str(),	Rohrleitung::KEINE,	IDC_KAPPENFARBE,
		getStammdaten().getText("IDS_GELB").c_str(),	Rohrleitung::GELB,	IDC_KAPPENFARBE,
		getStammdaten().getText("IDS_ROT").c_str(),		Rohrleitung::ROT,	IDC_KAPPENFARBE,
		getStammdaten().getText("IDS_BLAU").c_str(),	Rohrleitung::BLAU,	IDC_KAPPENFARBE,
		getStammdaten().getText("IDS_GRUEN").c_str(),	Rohrleitung::GRUEN,	IDC_KAPPENFARBE,
		getStammdaten().getText("IDS_WEISS").c_str(),	Rohrleitung::WEISS,	IDC_KAPPENFARBE,
		getStammdaten().getText("IDS_SCHWARZ").c_str(),	Rohrleitung::SCHWARZ,IDC_KAPPENFARBE,
		getStammdaten().getText("IDS_GOLD").c_str(),	Rohrleitung::GOLD,IDC_KAPPENFARBE,
		getStammdaten().getText("IDS_WEISSALU").c_str(),	Rohrleitung::WEISSALU,IDC_KAPPENFARBE,
		getStammdaten().getText("IDS_PINK").c_str(),	Rohrleitung::PINK,IDC_KAPPENFARBE,
		getStammdaten().getText("IDS_BRAUN").c_str(),	Rohrleitung::BRAUN,IDC_KAPPENFARBE,
		getStammdaten().getText("IDS_ORANGE").c_str(),	Rohrleitung::ORANGE,IDC_KAPPENFARBE,
		getStammdaten().getText("IDS_LILA").c_str(),	Rohrleitung::LILA,IDC_KAPPENFARBE,
		"",			0,		0
	};
	INIT_CBDATA3(initKFarbe);

	setWindowsText(IDC_STATIC_BE, "IDS_BESCHICHTUNG");
	setWindowsText(IDC_STATIC_BEMERKUNG,"IDS_BEMERKUNG");
	setWindowsText(IDC_STATIC_ZU,"IDS_KOLLILISTE_EPORT");
	setWindowsText(IDC_STATIC_SO,"IDS_SONSTIGESa");
	setWindowsText(IDC_TEXTKFARBE,"IDS_KAPPENFARBE");
	setWindowsText(IDC_KOLLILISTE,"IDS_KOLLILISTE_EPORT"); 
	setWindowsText(IDC_SICHTBEREICH,"IDS_SICHTBEREICH");
	setWindowsText(IDC_ZEUGNIS,"IDS_ZEUGNIS");
	setWindowsText(IDC_BUTKFARBE,"IDC_BUTKFARBE");

	return TRUE;
}

void BeschichtungEingabeForm::DoDataExchange(CDataExchange* pDX)
{
	WaxPartForm::DoDataExchange(pDX);

	DDX_Control(pDX,IDC_BESCH_BEMERKUNG, edBemerkung);
	ddx_cbdata(pDX, IDC_KAPPENFARBE, rohrKFarbe);

	Werkauftrag& wa = *get_data()->get_werkauftrag();
	Beschichtung besch;
	CButton *bt = NULL;
	bt = (CButton*)GetDlgItem(IDC_KOLLILISTE);

	if (WaxSys::isAV())
	{
		bt->EnableWindow(TRUE);
		m_KollilisteExport = bt->GetCheck();
	}
	else
	{
		bt->EnableWindow(FALSE);
	}
	bt = (CButton*)GetDlgItem(IDC_SICHTBEREICH);
	sichtbereich = bt->GetCheck() ? true : false;
	bt = (CButton*)GetDlgItem(IDC_ZEUGNIS);
	zeugnis = bt->GetCheck() ? true : false;     
	
	if (pDX->m_bSaveAndValidate) 
	{
		edBemerkung.GetWindowText(bemerkung);
		beschichtung.doDataExchange(pDX, &besch);
		wa.set_beschichtung(besch);
		wa.set_bemerkung(bemerkung);
		wa.set_sichtbereich(sichtbereich);
		wa.set_zeugnis(zeugnis);
		wa.set_KollilisteExport(m_KollilisteExport);
		
		// Daten an alle Unteraufträge vererben
		for (POSITION pos = get_data()->get_subobjects().GetHeadPosition(); pos;)
		{
			WaxPart *subwaxpart = dynamic_cast<WaxPart*>(get_data()->get_subobjects().GetNext(pos));
			XASSERT(subwaxpart);
			if (!subwaxpart)
				continue;
			Werkauftrag *subwa = subwaxpart->get_werkauftrag();
			XASSERT(subwa);
			if (!subwa)
				 continue;
			subwa->get_beschichtung().copy(wa.get_beschichtung());			
			subwa->set_bemerkung(wa.get_bemerkung());
			subwa->set_sichtbereich(wa.get_sichtbereich());
			subwa->set_zeugnis(wa.get_zeugnis());
		}
	} 
	else 
	{
		beschichtung.doDataExchange(pDX, &wa.get_beschichtung());
		bemerkung = wa.get_bemerkung();
		sichtbereich = wa.get_sichtbereich() ? true : false;
		zeugnis = wa.get_zeugnis() ? true : false;
		m_KollilisteExport = wa.get_KollilisteExport();
		edBemerkung.SetWindowText(bemerkung);

		
		bt = (CButton*)GetDlgItem(IDC_KOLLILISTE);
		bt->SetCheck(m_KollilisteExport);
		bt = (CButton*)GetDlgItem(IDC_SICHTBEREICH);
		bt->SetCheck(sichtbereich);
		bt = (CButton*)GetDlgItem(IDC_ZEUGNIS);
		bt->SetCheck(zeugnis);
	}
}

void BeschichtungEingabeForm::OnOK()
{
	WaxPartForm::OnOK();
}


void BeschichtungEingabeForm::onKFarbe()
{
	UpdateData();
	SammelAuftrag *sa = dynamic_cast<SammelAuftrag*>(get_data());

	if (sa)
	{
		for (POSITION pos = get_data()->get_subobjects().GetHeadPosition(); pos != NULL; )
		{
			WaxPart *sub = dynamic_cast<WaxPart*>(get_data()->get_subobjects().GetNext(pos));
			XASSERT(sub);
			if (!sub || sub->get_deleted())
				continue;
			Rohrleitung* rohr = dynamic_cast<Rohrleitung*>(sub->get_werkauftrag());
			if (rohr)
			{
				rohr->set_rohrKFarbe(rohrKFarbe);
			}
		}

	}
}


void BeschichtungEingabeForm::on_autosoko()
{	
	if (!WaxSys::isAV() || AfxMessageBox(getStammdaten().getText("IDS_ACHTUNG_SOKO").c_str(), MB_OKCANCEL | MB_ICONINFORMATION) == IDOK)
	{
		UpdateData();
		get_data()->createAutoSoko(false);
	}
}


void BeschichtungEingabeForm::OnBnClickedSichtbereich()
{
	SammelAuftrag *wa = dynamic_cast<SammelAuftrag*>(get_data());
	if( wa &&  !wa->get_subobjects().IsEmpty())
	{
		WaxPart* pt  =  (WaxPart*) wa->get_subobjects().GetHead();
		if(((CButton*)GetDlgItem(IDC_SICHTBEREICH))->GetCheck())
		{
			if(dynamic_cast<Strangrohr*>(pt->get_werkauftrag()))
			{
				AfxMessageBox( getStammdaten().getText("IDS__VORFERTIGUNG_STRANGROHRE").c_str(), MB_OK | MB_ICONINFORMATION);	
			}else if(dynamic_cast<Verteiler*>(pt->get_werkauftrag()))
			{
				AfxMessageBox( getStammdaten().getText("IDS__VORFERTIGUNG_VERTEILER").c_str(), MB_OK | MB_ICONINFORMATION);	
			}else if(dynamic_cast<Gewinderohr*>(pt->get_werkauftrag()))
			{
				AfxMessageBox( getStammdaten().getText("IDS__VORFERTIGUNG_GEWINDEROHRE").c_str(), MB_OK | MB_ICONINFORMATION);	
			}
		}
	}		
}
