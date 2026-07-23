#include "stdafx.h"


#include <mxutil/ddx_extension.h>


BEGIN_MESSAGE_MAP(EditArbeitsfolge, CDialog)
	ON_CBN_SELCHANGE(IDC_ARBPL, onSelchangeArbpl)
	ON_EN_KILLFOCUS(IDC_EINZELZEIT, onKillfocusEinzelzeit)
	ON_BN_CLICKED(IDC_AENDERN, &EditArbeitsfolge::OnBnClickedAendern)
END_MESSAGE_MAP()

//Konstruktor
EditArbeitsfolge::EditArbeitsfolge(CWnd* parent, Arbeitsfolge* af)
: WaxPartForm(IDD_ARBEITSFOLGE, getStammdaten().getText("IDD_ARBEITSFOLGE").c_str(), parent)
{
	/*
	Die IDs müssen gesetzt werden, da diese Klasse in Sondermz/Apax abgeleitet ist. Dort können die
	IDs nicht benutzt werden, bzw. sie müssten den gleichen Wert erhalten. Das wäre aber zu 
	fehlerträchtig.
	*/
	posnrID = IDC_POSNR;
	txtID = IDC_TEXT;
	codeID = IDC_CODE;
	arbplID = IDC_ARBPL;
	anzahlID = IDC_ANZAHL;
	preisID = IDC_PREIS;
	zeitID = IDC_ZEIT;
	bezID = IDC_BEZEICHNUNG;
	ruestzeitID = IDC_RUESTZEIT;
	einzelzeitID = IDC_EINZELZEIT;
	sapID = IDC_SAP;
	isTextgeaendertID = IDC_AENDERN;
	preis1ID = IDC_PREIS1;

	anzahlctrl.set_autodetect_decimal(true);
	preisctrl.set_autodetect_decimal(true);
	zeitctrl.set_Decimals(4);
	ruestzeitctrl.set_Decimals(4);
	einzelzeitctrl.set_Decimals(4);
	preis1ctrl.set_Decimals(2);

	paf = af;
	XASSERT(paf);
	if (paf)
	{
		posnr	= paf->get_nr();
		anzahl	= paf->get_anz();
		txt		= paf->get_txt();
		arbpl	= paf->get_arbpl();
		Werkauftrag wa;
		tarif = paf->getTarif(paf->getKostst(arbpl));
		zeit	= paf->get_zeit();
		code	= paf->get_id();
		sap     = paf->get_sap();
		isTextgeaendert = paf->get_isTextGeandert();
		einzelzeit = zeit/anzahl;
		anzahlctrl.initVal(&anzahl);
		preisctrl.initVal(&tarif);
		zeitctrl.initVal(&zeit);
		ruestzeitctrl.initVal(&ruestzeit);
		einzelzeitctrl.initVal(&einzelzeit);
		preis = paf->get_preis();
		preis1ctrl.initVal(&preis);
		
		Arbeitsfolge afo;
		bez = afo.getArbplBez(arbpl);
	}
}

//Dialog initialisieren
BOOL EditArbeitsfolge::OnInitDialog()
{
	// Combobox Arbeitsplatz
	CComboBox *bx = (CComboBox*)GetDlgItem(arbplID);

	int index = 0;
	Arbeitsplaetze::data *arb = NULL;
	for (POSITION apos = getStammdaten().get_arbeitsplaetze().GetHeadPosition(); apos;)
	{
		arb = (Arbeitsplaetze::data *)getStammdaten().get_arbeitsplaetze().GetNext(apos);
		
		CString txt;
		txt = arb->get_nr();
		index = bx->AddString(txt);
	//	if (index != CB_ERR)
	//		bx->SetItemData(index, arb->get_nr());
	}
	SetDlgItemText(bezID, bez);

	loadTexte();

	if (paf->get_typ() == 0)
		GetDlgItem(IDC_PREIS1)->EnableWindow(FALSE);

	((CButton*)GetDlgItem(isTextgeaendertID))->SetCheck(isTextgeaendert);
	OnBnClickedAendern();

	return CDialog::OnInitDialog();
}

void EditArbeitsfolge::loadTexte()
{
	setWindowsTextWithColon(IDC_STATIC_POS, "IDS_POS");
	setWindowsTextWithColon(IDC_STATIC_Anzahl, "IDS_ANZAHL");
	setWindowsTextWithColon(IDC_STATIC_BEZ, "IDS_BEZEICHNUNG");
	setWindowsTextWithColon(IDC_STATIC_ZEIT, "IDS_ZEIT");
	setWindowsText(IDC_SAP, "IDS_SAP");
	setWindowsTextWithColon(IDC_STATIC_CODE, "IDS_CODE");
	setWindowsTextWithColon(IDC_STATIC_EINZELZEIT, "IDS_EINZELZEIT");
	setWindowsTextWithColon(IDC_STATIC_RUESTZEIT, "IDS_RUESTZEIT");
	setWindowsTextWithColon(IDC_STATIC_PREIS, "IDS_PREIS");
	setWindowsText(IDC_STATIC_ARBEITSPLATZ, "IDS_ARBEITSPLATZ");
	setWindowsTextWithColon(IDC_STATIC_PREIS_H, "IDS_PREIS_PRO_STUNDE");
}


//Daten aus Eingabefeldern lesen
void EditArbeitsfolge::DoDataExchange(CDataExchange* pDX)
{
	CWnd::DoDataExchange(pDX);

	posnr	= paf->get_nr();
	anzahl	= paf->get_anz();
	txt		= paf->get_txt();
	arbpl	= paf->get_arbpl();
	zeit	= paf->get_zeit();
	code	= paf->get_id();
	sap     = paf->get_sap();
	isTextgeaendert = paf->get_isTextGeandert();
	ruestzeit = paf->get_ruestzeit();
	einzelzeit = zeit/anzahl;
	preis   = paf->get_preis();

	Werkauftrag wa;

	DDX_Check(pDX, sapID, sap);
	DDX_Check(pDX, isTextgeaendertID, isTextgeaendert);
	DDX_Text(pDX, posnrID, posnr);
	DDX_Text(pDX, txtID, txt);
	DDX_Text(pDX, codeID, code);

	DDX_Text(pDX, arbplID, arbpl);

	DDX_Control(pDX, anzahlID, anzahlctrl);
	DDX_Control(pDX, preisID, preisctrl);
	DDX_Control(pDX, zeitID, zeitctrl);
	DDX_Control(pDX, ruestzeitID, ruestzeitctrl);
	DDX_Control(pDX, einzelzeitID, einzelzeitctrl);
	DDX_Control(pDX, preis1ID, preis1ctrl);
	
	anzahlctrl.doDataExchange(pDX);
	preisctrl.doDataExchange(pDX);
	zeitctrl.doDataExchange(pDX);
	ruestzeitctrl.doDataExchange(pDX);
	einzelzeitctrl.doDataExchange(pDX);
	preis1ctrl.doDataExchange(pDX);
	
	// paf->set_id muss als erstes aufgerufen werden, da die Daten automatisch aus den Stammdaten
	// initialisiert werden
	paf->set_id(code);
	paf->set_nr(posnr);
	paf->set_anz(anzahl);
	paf->set_txt(txt);
	paf->set_arbpl(arbpl);
	paf->set_preis(preis);


	Arbeitsplaetze arb;
	paf->set_typ(arb.getTyp(arbpl));
	
	paf->set_ppstd(paf->getTarif(paf->getKostst(arbpl)));  //Vollkostentarif
	
	if (einzelzeit != 0.0)
		zeit = einzelzeit*anzahl;
	paf->set_zeit(zeit);
	paf->set_sap(sap ? true : false);
	paf->set_isTextGeandert(isTextgeaendert ? true : false);
	
	paf->set_ruestzeit(ruestzeit);
}

void EditArbeitsfolge::OnOK()
{
	CDialog::OnOK();
	checkPreisUndAnzahl();
}

//Prüft die Eingabe der Menge und der Zeit auf Änderung. 
//Führt eine Neuberechnung der Zeit durch, falls Menge geändert wurde.
void EditArbeitsfolge::checkPreisUndAnzahl()
{
	/*
	Achtung: Leider lassen sich double Werte nicht zuverlässig auf == überprüfen. 
	Deshalb werden die Werte für die Anzahl und die Zeit in Strings umgewandelt.
	*/
	CString tmpzeit1, tmpzeit2, tmpmenge1, tmpmenge2;

	tmpzeit1.Format("%g", zeit);
	tmpzeit2.Format("%g", paf->get_zeit());
	bool zeitgeaendert = tmpzeit1 != tmpzeit2;

	tmpmenge1.Format("%g", anzahl);
	tmpmenge2.Format("%g", paf->get_anz());
	bool mengegeaendert = tmpmenge1 != tmpmenge2;
	TRACE("Zeit geändert: %s, Menge geändert: %s \n", zeitgeaendert ? "Ja" : "Nein", mengegeaendert ? "Ja" : "Nein"); 
	if (mengegeaendert && !zeitgeaendert)
	{
		CString str; str = getStammdaten().getText("IDS_ANZAHL_AFO").c_str();
		if (AfxMessageBox(str, MB_YESNO | MB_ICONQUESTION) == IDYES)
		{
			if (paf->get_anz() < 0.1)
			{
				CString str; str = getStammdaten().getText("IDS_ZEIT_BERECHNEN").c_str();
				AfxMessageBox(str, MB_OK | MB_ICONINFORMATION);
			}
			double zeitproaf = paf->get_zeit() / paf->get_anz();
			zeit = zeitproaf * anzahl;
		}
	}
}

//Indicates a new combo box list item is selected
void EditArbeitsfolge::onSelchangeArbpl()
{
	UpdateData();
	SetDlgItemText(bezID, paf->getArbplBez(arbpl));
	paf->set_ppstd(paf->getTarif(paf->getKostst(arbpl)));
	UpdateData(false);
}

void EditArbeitsfolge::onKillfocusEinzelzeit()
{
	UpdateData();
	UpdateData(false);
}



void EditArbeitsfolge::OnBnClickedAendern()
{
	if (((CButton*)GetDlgItem(isTextgeaendertID))->GetCheck())
		GetDlgItem(txtID)->EnableWindow(TRUE);
	else
		GetDlgItem(txtID)->EnableWindow(FALSE);
}