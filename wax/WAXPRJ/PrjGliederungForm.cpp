#include "stdafx.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(PrjGliederungForm, WaxPartForm)

//Standardkonstruktor
PrjGliederungForm::PrjGliederungForm( )
: WaxPartForm(IDD_GLIEDERUNG, getStammdaten().getText("IDD_GLIEDERUNG").c_str())
{

}

void PrjGliederungForm::DoDataExchange(CDataExchange* pDX) 
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
	MaskedEdit::DDX_mEdit(pDX, IDC_PSPELEMENT, editPSP, get_data()->get_leitkarte().prj_pspElement);
	MaskedEdit::DDX_mEdit(pDX, IDC_PSPELEMENT, editPSP, get_data()->get_leitkarte().prj_produktlinie, 1);
	DDX_Text(pDX, IDC_COAUFTRAG, get_data()->get_leitkarte().prj_COauftrag);
	DDX_Text(pDX, IDC_NLKENNUNG, get_data()->get_leitkarte().prj_nl);

	GetDlgItem(IDC_EDIT_NIEDERLASSUNG2)->SetWindowText(getStammdaten().getNLBezeichnung(get_data()->get_leitkarte().prj_nl));
}


BEGIN_MESSAGE_MAP(PrjGliederungForm, WaxPartForm)
	ON_COMMAND(IDC_SAVE_AS_STANDARD, OnSaveAsStandard)
END_MESSAGE_MAP()



BOOL PrjGliederungForm::OnInitDialog()
{
	if (!WaxPartForm::OnInitDialog())
		return FALSE;

	int avitems[] = { IDC_NLKENNUNG, IDC_AUFTRAGSBEZ, IDC_PLZ, IDC_ORT, 0 };

	if (!WaxSys::isAV())
	{
		for (int i = 0; avitems[i]; i++)
			((CEdit*)GetDlgItem(avitems[i]))->SetReadOnly();
	}
	editPSP.actasPSPElement(get_data()->get_leitkarte().get_prj_nl(), get_data()->get_leitkarte().get_prj_pspElement(),get_data()->get_leitkarte().get_prj_produktlinie() );

#ifdef __WAX_FUER_DRITTE__
		GetDlgItem(IDC_PSPELEMENT)->EnableWindow(FALSE);
		GetDlgItem(IDC_COAUFTRAG)->EnableWindow(FALSE);
#endif

	
	setWindowsText(IDC_STATIC_PRJEINST, "IDC_STATIC_PRJEINST");

	setWindowsTextWithColon(IDC_STATIC_NL, "IDS_NIEDERLASSUNG");
	setWindowsTextWithColon(IDC_STATIC_AUFTRAG, "IDS_AUFTRAG_BAU");
	setWindowsTextWithColon(IDC_STATIC_ORT, "IDS_ORT");
	setWindowsTextWithColon(IDC_STATIC_BA, "IDS_BAUABSCHNITT");
	setWindowsTextWithColon(IDC_STATIC_PLZ, "IDS_PLZ");
	setWindowsTextWithColon(IDC_STATIC_PSP, "IDS_PSPAUFTRAG");
	setWindowsTextWithColon(IDC_AUFTRAGSNR, "IDS_COAUFTRAG");


	setWindowsTextWithColon(IDS_STATIC_BEZ, "IDS_BEZEICHNUNG");
	setWindowsTextWithColon(IDC_STATIC_KOMMENTAR, "IDS_KOMMENTAR");

	setWindowsText(IDC_SAVE_AS_STANDARD, "IDC_SAVE_AS_STANDARD");

	return TRUE;
}


//Speichert die Bezeichnung des Werkauftrages als neuen Standard ab 
void PrjGliederungForm::OnSaveAsStandard()
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
