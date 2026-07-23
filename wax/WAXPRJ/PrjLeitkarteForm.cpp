#include "stdafx.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(PrjLeitkarteForm, WaxPartForm)
	ON_CBN_SELCHANGE(IDC_NLKENNUNG, OnSelchangeNlkennung)
	ON_CBN_EDITCHANGE(IDC_NLKENNUNG, OnEditchangeNlkennung)
	ON_EN_KILLFOCUS(IDC_PSPELEMENT, OnFormatPSP)
	ON_BN_CLICKED(IDC_CHECK_IPPE_MAT_KNOTEN_SD, OnBnClickedIppeKnoten)
END_MESSAGE_MAP()


IMPLEMENT_DYNCREATE(PrjLeitkarteForm, WaxPartForm)

//Dynamische Erstellung verwendet geschützten Konstruktor
PrjLeitkarteForm::PrjLeitkarteForm()
: WaxPartForm(IDD_PROJEKTLEITKARTE, getStammdaten().getText("IDD_PROJEKTLEITKARTE").c_str())
{
}


//Destruktor
PrjLeitkarteForm::~PrjLeitkarteForm()
{
	
}


void PrjLeitkarteForm::DoDataExchange(CDataExchange* pDX)
{ 
	/*
	Datenaustausch mit dem Datenobjekt
	Wichtig ist bei dieser Methode, dass beim Speichern überprüft wird, ob sich Daten geändert haben. 
	Ist dies der Fall, wird GetDocument()->SetModifiedFlag() aufgerufen, damit beim Schliessen zur Speicherung auf
	gefordert wird.
	*/
	CDialog::DoDataExchange(pDX);
	
	DDX_Text(pDX, IDC_BAUABSCHNITT, get_data()->get_leitkarte().prj_bauAbschnitt);
	DDX_Text(pDX, IDC_AUFTRAGSBEZ,	get_data()->get_leitkarte().prj_auftragsbez);
	DDX_Text(pDX, IDC_PLZ,			get_data()->get_leitkarte().prj_bauPlz);
	DDX_Text(pDX, IDC_ORT,			get_data()->get_leitkarte().prj_bauOrt);
	DDX_Text(pDX, IDC_COAUFTRAG,	get_data()->get_leitkarte().prj_COauftrag);
	
	MaskedEdit::DDX_mEdit(pDX, IDC_PSPELEMENT, editPSP, get_data()->get_leitkarte().prj_pspElement);
	MaskedEdit::DDX_mEdit(pDX, IDC_PSPELEMENT, editPSP, get_data()->get_leitkarte().prj_produktlinie, 1);
	
	DDX_Control(pDX, IDC_NLKENNUNG, m_NLNummer);
	
	DDX_Text(pDX, IDC_NLKENNUNG, get_data()->get_leitkarte().prj_nl);

	int IsChoosenIppe = 0;
	if(pDX->m_bSaveAndValidate)
	{		
		DDX_Check(pDX, IDC_CHECK_IPPE_MAT_KNOTEN_SD, IsChoosenIppe);
		get_data()->get_leitkarte().set_IsChoosenIppeNumbers( IsChoosenIppe == 1  ? true : false);
	}
	else
	{
		get_data()->get_leitkarte().get_IsChoosenIppeNumbers() ? IsChoosenIppe = 1 : IsChoosenIppe = 0;
		DDX_Check(pDX, IDC_CHECK_IPPE_MAT_KNOTEN_SD, IsChoosenIppe);
	}

	MaskedEdit::DDX_mEdit(pDX, IDC_PSPELEMENT, editPSP, get_data()->get_leitkarte().prj_pspElement);
	if(get_data()->get_leitkarte().get_IsChoosenIppeNumbers())
	{	
		MaskedEdit::DDX_mEdit(pDX, IDC_PSPELEMENT, editPSP, get_data()->get_leitkarte().m_Brandschutztechnik, 1);
	}
	else
	{
		MaskedEdit::DDX_mEdit(pDX, IDC_PSPELEMENT, editPSP, get_data()->get_leitkarte().prj_produktlinie, 1);
	}


	if (pDX->m_bSaveAndValidate)
	{
		/*
		Vererbung der Projektleitkarte an alle vorhandenen Leitkarten anstossen
		*/
		AfxGetMainWnd()->SendMessage(WM_COMMAND, WAXID_PRJLEITKARTE);
	}
}

///Dialog initialisieren
BOOL PrjLeitkarteForm::OnInitDialog() 
{
	// Es die ComboBox m_NLNummer mit allen Kennziffern aus der Tabelle Niederlassungen gefüllt.
	// @xref <f GetWaxStamm>.<mf WaxStamm::get_Niederlassungen>

	if (WaxPartForm::OnInitDialog())
	{

		for (POSITION pos = getStammdaten().get_niederlassungen().GetHeadPosition(); pos != NULL;)
		{
			Niederlassungen::data *nl = (Niederlassungen::data*)(getStammdaten().get_niederlassungen().GetNext(pos));
			CString nr;
			nr.Format("%i",  nl->get_nlnummer());
			int index = m_NLNummer.AddString(nr);
			m_NLNummer.SetItemData(index, (DWORD)nl->get_nlnummer());
		}
		// Initialisierung der NL Anzeige im Klartext
		OnSelchangeNlkennung();	
		//editPSP.actasPSPElement(get_data()->get_leitkarte().get_prj_nl(), get_data()->get_leitkarte().get_prj_pspElement(), get_data()->get_leitkarte().get_prj_produktlinie());
		OnBnClickedIppeKnoten();
		
		setWindowsTextWithColon(IDC_STATIC_NL, "IDS_NIEDERLASSUNG");
		setWindowsTextWithColon(IDC_STATIC_AUFTRAG, "IDS_AUFTRAG_BAU");
		setWindowsTextWithColon(IDC_STATIC_ORT, "IDS_ORT");
		setWindowsTextWithColon(IDC_STATIC_BA, "IDS_BAUABSCHNITT");
		setWindowsTextWithColon(IDC_STATIC_PLZ, "IDS_PLZ");
		setWindowsTextWithColon(IDC_STATIC_PSP, "IDS_PSPAUFTRAG");
		setWindowsTextWithColon(IDC_AUFTRAGSNR, "IDS_COAUFTRAG");
		GetDlgItem(IDC_CHECK_IPPE_MAT_KNOTEN_SD)->SetWindowText(getStammdaten().getText("IDD_NEXTGENERATION").c_str());


#ifdef __WAX_FUER_DRITTE__
		GetDlgItem(IDC_PSPELEMENT)->EnableWindow(FALSE);
		GetDlgItem(IDC_COAUFTRAG)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_IPPE_MAT_KNOTEN_SD)->EnableWindow(FALSE);
#endif
		return TRUE;  
	}
	else return FALSE;
}


void PrjLeitkarteForm::OnSelchangeNlkennung() 
{
	/*
	NL Kennung in der ComboBox wurde geändert
	*/
	CString combo;
	int index = m_NLNummer.GetCurSel();

	if (index != -1)
		m_NLNummer.GetLBText(index, combo);
	else
		m_NLNummer.GetWindowText(combo);
	
	GetDlgItem(IDC_EDIT_NIEDERLASSUNG)->SetWindowText(getStammdaten().getNLBezeichnung(atoi(combo)));
	int iChecked = ((CButton*)GetDlgItem(IDC_CHECK_IPPE_MAT_KNOTEN_SD))->GetCheck();
	if(iChecked > 0)
	{
		editPSP.SetDefFormat(_T("[0-9]-[0-9][0-9][0-9][0-9][0-9][0-9]-#2[0-9][0-9][0-9]-#1[0-9][0-9]-#0[0-9][0-9][0-9][0-9]"));
	}
	else
	{
		editPSP.SetDefFormat(_T("[0-9]-[0-9][0-9][0-9][0-9][0-9][0-9]-#2[0-9][0-9][0-9]-#1[0-9][0-9]-#0[0-9][0-9][0-9]"));
	}
	editPSP.actasPSPElement(atoi(combo), get_data()->get_leitkarte().get_prj_pspElement(), get_data()->get_leitkarte().get_prj_produktlinie());
}

void PrjLeitkarteForm::OnEditchangeNlkennung() 
{
	OnSelchangeNlkennung();
}


void PrjLeitkarteForm::OnFormatPSP()
{
	/*
	Wenn ein PSP Element eingeben wurde, muss die Formatierung erneuert werden durch führende
	Nullen und 001 am Ende, wenn nicht eingeben.
	*/
	UpdateData();
	editPSP.actasPSPElement(get_data()->get_leitkarte().get_prj_nl(), get_data()->get_leitkarte().prj_pspElement, get_data()->get_leitkarte().get_prj_produktlinie());
}


void PrjLeitkarteForm::OnBnClickedIppeKnoten()
{
	int iChecked = ((CButton*)GetDlgItem(IDC_CHECK_IPPE_MAT_KNOTEN_SD))->GetCheck();
	if(iChecked > 0)
	{
		editPSP.SetDefFormat(_T("[0-9]-[0-9][0-9][0-9][0-9][0-9][0-9]-#2[0-9][0-9][0-9]-#1[0-9][0-9]-#0[0-9][0-9][0-9][0-9]"));
		CString pspElement = get_data()->get_leitkarte().get_prj_pspElement();
		pspElement += "0";
		editPSP.actasPSPElement(get_data()->get_leitkarte().get_prj_nl(), pspElement, get_data()->get_leitkarte().get_Brandschutztechnik());
	}
	else
	{
		editPSP.SetDefFormat(_T("[0-9]-[0-9][0-9][0-9][0-9][0-9][0-9]-#2[0-9][0-9][0-9]-#1[0-9][0-9]-#0[0-9][0-9][0-9]"));
		editPSP.actasPSPElement(get_data()->get_leitkarte().get_prj_nl(), get_data()->get_leitkarte().get_prj_pspElement(), get_data()->get_leitkarte().get_prj_produktlinie());
	}	
}







