#include "stdafx.h"


#include "PreisanfrageForm.h"
#include "WpPreisanfrage.h"
#include "GpVk3Preis.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(PreisanfrageForm, WaxPartForm)

//Standardkonstruktor
PreisanfrageForm::PreisanfrageForm( )
: WaxPartForm(IDD_PREISANFRAGE, getStammdaten().getText("IDD_PREISANFRAGE").c_str())
{

}

void PreisanfrageForm::DoDataExchange(CDataExchange* pDX) 
{
	WpPreisanfrage* glied = dynamic_cast<WpPreisanfrage*>(get_data());
	XASSERT(glied);
	if (!glied)
		return;
	CString bauabschnitt = glied->get_werkauftrag()->get_leitkarte().get_prj_bauAbschnitt();
	
	CDialog::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_EDIT2, glied->kommentar);

	DDX_Text(pDX, IDC_BAUABSCHNITT, bauabschnitt);
	glied->get_werkauftrag()->get_leitkarte().set_prj_bauAbschnitt(bauabschnitt);
	
	DDX_Text(pDX, IDC_AUFTRAGSBEZ, get_data()->get_leitkarte().prj_auftragsbez);
	DDX_Text(pDX, IDC_PLZ, get_data()->get_leitkarte().prj_bauPlz);
	DDX_Text(pDX, IDC_ORT, get_data()->get_leitkarte().prj_bauOrt);
	MaskedEdit::DDX_mEdit(pDX, IDC_PSPELEMENT, editPSP, get_data()->get_leitkarte().prj_pspElement);
	MaskedEdit::DDX_mEdit(pDX, IDC_PSPELEMENT, editPSP, get_data()->get_leitkarte().prj_produktlinie, 1);
	DDX_Text(pDX, IDC_COAUFTRAG, get_data()->get_leitkarte().prj_COauftrag);
	DDX_Text(pDX, IDC_NLKENNUNG, get_data()->get_leitkarte().prj_nl);

	GetDlgItem(IDC_EDIT_NIEDERLASSUNG2)->SetWindowText(getStammdaten().getNLBezeichnung(get_data()->get_leitkarte().prj_nl));


	double dtmp = 0;
	MXDate date;
	if (pDX->m_bSaveAndValidate)
	{
		DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_GUELTIGBIS, date);
		get_data()->get_leitkarte().preisanfrage.set_gueltigBis(date);
	}
	else
	{
		dtmp = glied->getGesamtpreis(glied);
		DDX_Text(pDX, IDC_PA_PREIS , dtmp);		

		GpVk3Preis vk3Preis;
		double vk3 = vk3Preis.getVk3Preis(get_data());
		CString tmp;
		tmp.Format("%.2f", vk3);
		DDX_Text(pDX, IDC_VK3_PREIS , tmp);		

		if (dtmp > 0 && vk3 > 0)
			tmp.Format("%.2f",(dtmp / vk3 - 1.0) * 100.0);
		else
			tmp = "";
		DDX_Text(pDX, IDC_EDIT_PROZENT , tmp);	

		date = get_data()->get_leitkarte().preisanfrage.get_gueltigBis();
		DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_GUELTIGBIS, date);		
	}


}


BEGIN_MESSAGE_MAP(PreisanfrageForm, WaxPartForm)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATETIMEPICKER_GUELTIGBIS, &PreisanfrageForm::OnDtnDatetimechangeDatetimepickerGueltigbis)
END_MESSAGE_MAP()



BOOL PreisanfrageForm::OnInitDialog()
{
	if (!WaxPartForm::OnInitDialog())
		return FALSE;

	int avitems[] = { IDC_NLKENNUNG, IDC_AUFTRAGSBEZ, IDC_PLZ, IDC_ORT, 0 };

	if (!WaxSys::isAV())
	{
		for (int i = 0; avitems[i]; i++)
			((CEdit*)GetDlgItem(avitems[i]))->SetReadOnly();

		GetDlgItem(IDC_DATETIMEPICKER_GUELTIGBIS)->EnableWindow(FALSE);
	}
	editPSP.actasPSPElement(get_data()->get_leitkarte().get_prj_nl(), get_data()->get_leitkarte().get_prj_pspElement(),get_data()->get_leitkarte().get_prj_produktlinie() );

#ifdef __WAX_FUER_DRITTE__
		GetDlgItem(IDC_PSPELEMENT)->EnableWindow(FALSE);
		GetDlgItem(IDC_COAUFTRAG)->EnableWindow(FALSE);


		GetDlgItem(IDC_STATIC_Preise)->ModifyStyle(WS_VISIBLE, NULL);
		GetDlgItem(IDC_PANR)->ModifyStyle(WS_VISIBLE, NULL);
		GetDlgItem(IDC_VK3_PREIS)->ModifyStyle(WS_VISIBLE, NULL);
		GetDlgItem(IDC_STATIC_VK3_GESPREIS)->ModifyStyle(WS_VISIBLE, NULL);
		GetDlgItem(IDC_STATIC_EURO1)->ModifyStyle(WS_VISIBLE, NULL);
		GetDlgItem(IDC_STATIC_PANR)->ModifyStyle(WS_VISIBLE, NULL);
		GetDlgItem(IDC_STATIC_PA_PREIS)->ModifyStyle(WS_VISIBLE, NULL);
		GetDlgItem(IDC_PA_PREIS)->ModifyStyle(WS_VISIBLE, NULL);
		GetDlgItem(IDC_STATIC_PA_PREIS)->ModifyStyle(WS_VISIBLE, NULL);
		GetDlgItem(IDC_STATIC_EURO2)->ModifyStyle(WS_VISIBLE, NULL);
		GetDlgItem(IDC_STATIC_GUELTIGBIS)->ModifyStyle(WS_VISIBLE, NULL);
		GetDlgItem(IDC_DATETIMEPICKER_GUELTIGBIS)->ModifyStyle(WS_VISIBLE, NULL);
		GetDlgItem(IDC_STATIC_PROZENT)->ModifyStyle(WS_VISIBLE, NULL);
		GetDlgItem(IDC_EDIT_PROZENT)->ModifyStyle(WS_VISIBLE, NULL);
		
#endif

	
	setWindowsText(IDC_STATIC_PRJEINST, "IDC_STATIC_PRJEINST");

	setWindowsTextWithColon(IDC_STATIC_NL, "IDS_NIEDERLASSUNG");
	setWindowsTextWithColon(IDC_STATIC_AUFTRAG, "IDS_AUFTRAG_BAU");
	setWindowsTextWithColon(IDC_STATIC_ORT, "IDS_ORT");
	setWindowsTextWithColon(IDC_STATIC_BA, "IDS_BAUABSCHNITT");
	setWindowsTextWithColon(IDC_STATIC_PLZ, "IDS_PLZ");
	setWindowsTextWithColon(IDC_STATIC_PSP, "IDS_PSPAUFTRAG");
	setWindowsTextWithColon(IDC_AUFTRAGSNR, "IDS_COAUFTRAG");


	setWindowsText(IDC_STATIC_Preise, "IDC_STATIC_Preise");
	setWindowsTextWithColon(IDC_STATIC_VK3_GESPREIS, "IDC_STATIC_VK3_GESPREIS");
	setWindowsTextWithColon(IDC_STATIC_PANR, "IDC_STATIC_PANR");
	setWindowsTextWithColon(IDC_STATIC_PA_PREIS, "IDC_STATIC_PA_PREIS");
	setWindowsTextWithColon(IDC_STATIC_GUELTIGBIS, "IDC_STATIC_GUELTIGBIS");
	setWindowsText(IDC_STATIC_PROZENT, "IDC_PROZENTVON");


	setWindowsTextWithColon(IDS_STATIC_BEZ, "IDS_BEZEICHNUNG");
	setWindowsTextWithColon(IDC_STATIC_KOMMENTAR, "IDS_KOMMENTAR");

	CString str;
	str.Format("%i", get_data()->get_leitkarte().get_preisanfrage().get_nummer());
	GetDlgItem(IDC_PANR)->SetWindowText(str);


	GetDlgItem(IDC_BEZEICHNUNG)->SetWindowText(get_data()->getBezeichnung());

	return TRUE;
}



void PreisanfrageForm::OnDtnDatetimechangeDatetimepickerGueltigbis(NMHDR *pNMHDR, LRESULT *pResult)
{
	UNREFERENCED_PARAMETER(pNMHDR);

	//LPNMDATETIMECHANGE pDTChange = reinterpret_cast<LPNMDATETIMECHANGE>(pNMHDR);
	// TODO: Fügen Sie hier Ihren Kontrollbehandlungscode für die Benachrichtigung ein.

	UpdateData();

	WpPreisanfrage* wp = dynamic_cast<WpPreisanfrage*>(get_data());
	XASSERT(wp);
	if (!wp)
		return;

	wp->copyGueltigBisToChilds(wp, wp->get_werkauftrag()->get_leitkarte().get_preisanfrage().get_gueltigBis());

	*pResult = 0;
}
