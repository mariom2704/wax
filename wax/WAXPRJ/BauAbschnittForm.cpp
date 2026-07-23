#include "stdafx.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(BauAbschnittAVSammelauftragGliederungForm, WaxPartForm)

//Standardkonstruktor
BauAbschnittAVSammelauftragGliederungForm::BauAbschnittAVSammelauftragGliederungForm( )
: WaxPartForm(IDD_GLIEDERUNG, getStammdaten().getText("IDD_GLIEDERUNG").c_str())
{}

void BauAbschnittAVSammelauftragGliederungForm::DoDataExchange(CDataExchange* pDX) 
{
	PrjGliederung* glied = dynamic_cast<PrjGliederung*>(get_data());
	XASSERT(glied);
	if (!glied)
		return;
	CString bauabschnitt = glied->get_werkauftrag()->get_leitkarte().get_prj_bauAbschnitt();
	CString bezeichnung = glied->get_werkauftrag()->get_bezeichnung();
	if (!bezeichnung.GetLength()) 
		bezeichnung = bauabschnitt;
	
	CDialog::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_EDIT2, glied->kommentar);
	DDX_Text(pDX, IDC_BAUABSCHNITT, bauabschnitt);
	glied->get_werkauftrag()->get_leitkarte().set_prj_bauAbschnitt(bauabschnitt);

	DDX_Text(pDX, IDC_BEZEICHNUNG, bezeichnung);
	glied->get_werkauftrag()->set_bezeichnung(bezeichnung);

	
	DDX_Text(pDX, IDC_AUFTRAGSBEZ, get_data()->get_leitkarte().prj_auftragsbez);
	DDX_Text(pDX, IDC_PLZ, get_data()->get_leitkarte().prj_bauPlz);
	DDX_Text(pDX, IDC_ORT, get_data()->get_leitkarte().prj_bauOrt);

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
	DDX_Text(pDX, IDC_COAUFTRAG, get_data()->get_leitkarte().prj_COauftrag);
	DDX_Text(pDX, IDC_NLKENNUNG, get_data()->get_leitkarte().prj_nl);

	GetDlgItem(IDC_EDIT_NIEDERLASSUNG2)->SetWindowText(getStammdaten().getNLBezeichnung(get_data()->get_leitkarte().prj_nl));

}


BEGIN_MESSAGE_MAP(BauAbschnittAVSammelauftragGliederungForm, WaxPartForm)
	ON_COMMAND(IDC_SAVE_AS_STANDARD, OnSaveAsStandard)
	ON_BN_CLICKED(IDC_CHECK_IPPE_MAT_KNOTEN_SD, OnBnClickedIppeMatKnoten)
END_MESSAGE_MAP()



BOOL BauAbschnittAVSammelauftragGliederungForm::OnInitDialog()
{
	if (!WaxPartForm::OnInitDialog())
		return FALSE;

	int avitems[] = { IDC_NLKENNUNG, IDC_AUFTRAGSBEZ, IDC_PLZ, IDC_ORT, 0 };

	if (!WaxSys::isAV())
	{
		for (int i = 0; avitems[i]; i++)
			((CEdit*)GetDlgItem(avitems[i]))->SetReadOnly();
	}

	
	if(get_data()->get_leitkarte().get_IsChoosenIppeNumbers())
	{
		editPSP.SetDefFormat(_T("[0-9]-[0-9][0-9][0-9][0-9][0-9][0-9]-#2[0-9][0-9][0-9]-#1[0-9][0-9]-#0[0-9][0-9][0-9][0-9]"));
	}
	else
	{
		editPSP.SetDefFormat(_T("[0-9]-[0-9][0-9][0-9][0-9][0-9][0-9]-#2[0-9][0-9][0-9]-#1[0-9][0-9]-#0[0-9][0-9][0-9]"));
	}
	editPSP.actasPSPElement(get_data()->get_leitkarte().get_prj_nl(), get_data()->get_leitkarte().get_prj_pspElement(),get_data()->get_leitkarte().get_prj_produktlinie() );
	
	OnBnClickedIppeMatKnoten();
#ifdef __WAX_FUER_DRITTE__
		GetDlgItem(IDC_PSPELEMENT)->EnableWindow(FALSE);
		GetDlgItem(IDC_COAUFTRAG)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_IPPE_MAT_KNOTEN_SD)->EnableWindow(FALSE);
#endif

	
	setWindowsText(IDC_STATIC_PRJEINST, "IDC_STATIC_PRJEINST");

	setWindowsTextWithColon(IDC_STATIC_NL, "IDS_NIEDERLASSUNG");
	setWindowsTextWithColon(IDC_STATIC_AUFTRAG, "IDS_AUFTRAG_BAU");
	setWindowsTextWithColon(IDC_STATIC_ORT, "IDS_ORT");
	setWindowsTextWithColon(IDC_STATIC_BA, "IDS_BAUABSCHNITT");
	setWindowsTextWithColon(IDC_STATIC_PLZ, "IDS_PLZ");
	setWindowsTextWithColon(IDC_STATIC_PSP, "IDS_PSPAUFTRAG");
	setWindowsTextWithColon(IDC_AUFTRAGSNR, "IDS_COAUFTRAG");
	GetDlgItem(IDC_CHECK_IPPE_MAT_KNOTEN_SD)->SetWindowText(getStammdaten().getText("IDD_NEXTGENERATION").c_str());



	setWindowsTextWithColon(IDS_STATIC_BEZ, "IDS_BEZEICHNUNG");
	setWindowsTextWithColon(IDC_STATIC_KOMMENTAR, "IDS_KOMMENTAR");

	setWindowsText(IDC_SAVE_AS_STANDARD, "IDC_SAVE_AS_STANDARD");

	return TRUE;
}


//Speichert die Bezeichnung des Werkauftrages als neuen Standard ab 
void BauAbschnittAVSammelauftragGliederungForm::OnSaveAsStandard()
{
	UpdateData();
	PrjGliederung* glied = dynamic_cast<PrjGliederung*>(get_data());
	XASSERT(glied);
	if (glied)
	{
		CString titel(glied->get_werkauftrag()->get_bezeichnung());
		glied->getsetTitel(true, titel);
		glied->get_werkauftrag()->set_bezeichnung(titel);
	}
}



void BauAbschnittAVSammelauftragGliederungForm::OnBnClickedIppeMatKnoten()
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

