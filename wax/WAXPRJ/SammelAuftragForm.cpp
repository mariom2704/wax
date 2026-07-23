#include "stdafx.h"


#include "server.h"
#include <typeinfo.h>
#include <mxodbc\mxodbc.h>

#include <mxutil/ddx_extension.h>
#include "WanrSperrung.h"
#include "DbFreigabe.h"
#include "GpVk3Preis.h"
#include "SammelAuftragForm.h"
#include "DrucklisteWap.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define IDC_WANR_01 (WM_USER+111)

#define ANZ_NRKREIS 7
const int nummernkreis[ANZ_NRKREIS] = { 400010,  400020, 400030, 400032, 400033, 400035, 400040 };

IMPLEMENT_DYNCREATE(SammelAuftragForm, WaxPartForm)

BEGIN_MESSAGE_MAP(SammelAuftragForm, WaxPartForm)
	ON_EN_KILLFOCUS(IDC_SDAUFTRAGSNR, OnKillfocusSdauftragsnr)
	ON_EN_KILLFOCUS(IDC_PSPELEMENT, OnKillfocusPsp)
	ON_EN_KILLFOCUS(IDC_SDPOSITIONSNUMMER, OnKillfocusSdpositionsnummer)
	ON_COMMAND(IDC_WANRNEU, onWaNrNeu)
	ON_COMMAND(IDC_WANRLOESCHEN, onWaNrLoeschen)
	ON_COMMAND(IDC_EDITWANR, onEditwanr)
	ON_COMMAND(IDC_MANUELL_WANR, onManuellWanr)
	ON_COMMAND_RANGE(IDC_WANR_01, IDC_WANR_01+ANZ_NRKREIS, on_setwanr)
	ON_COMMAND(IDC_BUTTON_CALC, onCalcPreis)
	ON_EN_KILLFOCUS(IDC_PA_PREIS,  onCalcProzent)
	ON_BN_CLICKED(IDC_CHECK_IPPE_MAT_KNOTEN_SD, OnCheckBoxClickedIPPEMatNr)
	ON_BN_CLICKED(IDC_BUTTON_IPPE_NR, OnBnClickedButtonIppeNr)
END_MESSAGE_MAP()

SammelAuftragForm::SammelAuftragForm() : WaxPartForm(IDD_SAMMELAUFTRAG_LEITKARTE, getStammdaten().getText("IDD_LEITKARTE").c_str()) 
{}


void SammelAuftragForm::OnCheckBoxClickedIPPEMatNr()
{
	
	Leitkarte& lt = get_data()->get_leitkarte();
	int iChecked = ((CButton*)GetDlgItem(IDC_CHECK_IPPE_MAT_KNOTEN_SD))->GetCheck();

	if(iChecked > 0)
	{
		
		if( lt.sdAuftragsnr !=0 )
		{

			if( AfxMessageBox(getStammdaten().getText("IDS_OUESTION_SDAUFTRAG_SDPOS").c_str(), MB_ICONQUESTION | MB_YESNO) == IDYES )
			{
				GetDlgItem(IDC_BUTTON_IPPE_NR)->ShowWindow(SW_SHOW);
				GetDlgItem(IDC_BUTTON_IPPE_NR)->SetWindowText(getStammdaten().getText("IDS_IPPE_MAT_ NR_ VON_ SAP").c_str());
				GetDlgItem(IDC_STATIC_SDPOS)->SetWindowText(getStammdaten().getText("IDS_IPPE_KNOTEN_NR").c_str());
				
				CWnd* pWnd = GetDlgItem(IDC_SDAUFTRAGSNR);
				pWnd->ModifyStyle(ES_NUMBER, 0);
				pWnd->SendMessage(EM_LIMITTEXT, 10, 0);
				pWnd->SetWindowText("");
				lt.sdAuftragsnr = 0;

				pWnd = GetDlgItem(IDC_SDPOSITIONSNUMMER);
				pWnd->ModifyStyle(ES_NUMBER, 0);
				pWnd->SendMessage(EM_LIMITTEXT, 40, 0);
				pWnd->SetWindowText("");
				pWnd->EnableWindow(FALSE);
				lt.sdPositionsnr = 0;
			}
			else
			{
				((CButton*)GetDlgItem(IDC_CHECK_IPPE_MAT_KNOTEN_SD))->SetCheck(FALSE);
			}
		}
		else
		{
			GetDlgItem(IDC_BUTTON_IPPE_NR)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BUTTON_IPPE_NR)->SetWindowText(getStammdaten().getText("IDS_IPPE_MAT_ NR_ VON_ SAP").c_str());
			GetDlgItem(IDC_STATIC_SDPOS)->SetWindowText(getStammdaten().getText("IDS_IPPE_KNOTEN_NR").c_str());
			
			CWnd* pWnd = GetDlgItem(IDC_SDAUFTRAGSNR);
			pWnd->ModifyStyle(ES_NUMBER, 0);
			pWnd->SendMessage(EM_LIMITTEXT, 10, 0);
			pWnd->SetWindowText(lt.m_ippeMatNr);
			lt.sdAuftragsnr = 0;

			pWnd = GetDlgItem(IDC_SDPOSITIONSNUMMER);
			pWnd->EnableWindow(FALSE);
			pWnd->ModifyStyle(ES_NUMBER, 0);
			pWnd->SendMessage(EM_LIMITTEXT, 40, 0);
			pWnd->SetWindowText(lt.m_ippeKnotenNr);
			lt.sdPositionsnr = 0;
		}
	}
	else
	{
		if( !lt.m_ippeMatNr.IsEmpty())
		{
			if( AfxMessageBox(getStammdaten().getText("IDS_OUESTION_IPPEMAT_IPPEKNOTEN").c_str(), MB_ICONQUESTION | MB_YESNO) == IDYES )
			{
				GetDlgItem(IDC_BUTTON_IPPE_NR)->ShowWindow(SW_HIDE);
				ShowWindow(SW_HIDE);
				ShowWindow(SW_SHOW);
				GetDlgItem(IDC_STATIC_SDNR)->SetWindowText(getStammdaten().getText("IDS_SDNR").c_str());
				GetDlgItem(IDC_STATIC_SDPOS)->SetWindowText(getStammdaten().getText("IDS_SDPOS").c_str());
				lt.m_ippeMatNr	 = "";	
				lt.m_ippeKnotenNr	= "";
				CString strTemp;
				
				strTemp.Format( "%I64d", lt.sdAuftragsnr);
				CWnd* pWnd = GetDlgItem(IDC_SDAUFTRAGSNR);
				pWnd->SendMessage(EM_LIMITTEXT, 10, 0);
				pWnd->ModifyStyle(0, ES_NUMBER);
				GetDlgItem(IDC_SDAUFTRAGSNR)->SetWindowText(strTemp);
				
				strTemp.Format("%ld", lt.sdPositionsnr);
				pWnd = GetDlgItem(IDC_SDPOSITIONSNUMMER);
				pWnd->EnableWindow(TRUE);
				pWnd->SendMessage(EM_LIMITTEXT, 4, 0);
				pWnd->ModifyStyle(0, ES_NUMBER);
				pWnd->SetWindowText(strTemp);
			}
			else
			{
				((CButton*)GetDlgItem(IDC_CHECK_IPPE_MAT_KNOTEN_SD))->SetCheck(TRUE);
			}
		}
		else
		{
			GetDlgItem(IDC_BUTTON_IPPE_NR)->ShowWindow(SW_HIDE);
			ShowWindow(SW_HIDE);
			ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STATIC_SDNR)->SetWindowText(getStammdaten().getText("IDS_SDNR").c_str());
			GetDlgItem(IDC_STATIC_SDPOS)->SetWindowText(getStammdaten().getText("IDS_SDPOS").c_str());
			lt.m_ippeMatNr	  = "";	
			lt.m_ippeKnotenNr = "";
			CString strTemp;
			
			strTemp.Format( "%I64d", lt.sdAuftragsnr);
			CWnd* pWnd = GetDlgItem(IDC_SDAUFTRAGSNR);
			pWnd->SendMessage(EM_LIMITTEXT, 10, 0);
			pWnd->ModifyStyle(0, ES_NUMBER);
			GetDlgItem(IDC_SDAUFTRAGSNR)->SetWindowText(strTemp);
			
			strTemp.Format("%ld", lt.sdPositionsnr);
			pWnd = GetDlgItem(IDC_SDPOSITIONSNUMMER);
			pWnd->EnableWindow(TRUE);
			pWnd->SendMessage(EM_LIMITTEXT, 4, 0);
			pWnd->ModifyStyle(0, ES_NUMBER);
			pWnd->SetWindowText(strTemp);
		}
	}	
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


void SammelAuftragForm::DoDataExchange(CDataExchange* pDX)
{
	/**
	Datenaustausch der Dialogelemente mit einem Objekt vom Typ LeitkarteData
	*/
 	WaxPartForm::DoDataExchange(pDX);

	Leitkarte& lt = get_data()->get_leitkarte();
		
	DDX_Text(pDX, IDC_BAUABSCHNITT,	lt.prj_bauAbschnitt);
	DDX_Text(pDX, IDC_AUFTRAGSBEZ,	lt.prj_auftragsbez);
	DDX_Text(pDX, IDC_PLZ,			lt.prj_bauPlz);
	DDX_Text(pDX, IDC_ORT,			lt.prj_bauOrt);
	DDX_Text(pDX, IDC_COAUFTRAG,	lt.prj_COauftrag);

	MaskedEdit::DDX_mEdit(pDX, IDC_PSPELEMENT, editPSP, lt.prj_pspElement);
	
	checkFreigabe();

	long wan = get_data()->get_werkauftrag()->get_status().get_wanr();
	DDX_Text(pDX, IDC_WANR, wan);
	get_data()->get_werkauftrag()->get_status().set_wanr(wan);

	DDX_Text(pDX, IDC_NLKENNUNG, lt.prj_nl);

	GetDlgItem(IDC_EDIT_NIEDERLASSUNG2)->SetWindowText(getStammdaten().getNLBezeichnung(lt.prj_nl));

	if (av)
	{
		CButton *bt = (CButton*)GetDlgItem(IDC_WERKAUFTRAG_VK3);
		if (pDX->m_bSaveAndValidate) 
			lt.set_vk3(bt->GetCheck() ? true : false);
		else
			bt->SetCheck(lt.get_vk3());
	}


	CButton *bt = (CButton*)GetDlgItem(IDC_CHECK_IPPE_MAT_KNOTEN_SD);
	
	if (pDX->m_bSaveAndValidate) 
	{
		lt.m_IsChoosenIppeNr =  bt->GetCheck() ? true : false;
	//	MaskedEdit::DDX_mEdit(pDX, IDC_PSPELEMENT, editPSP, get_data()->get_leitkarte().prj_pspElement);
		if(get_data()->get_leitkarte().get_IsChoosenIppeNumbers())
		{	
			MaskedEdit::DDX_mEdit(pDX, IDC_PSPELEMENT, editPSP, get_data()->get_leitkarte().m_Brandschutztechnik, 1);
		}
		else
		{
			MaskedEdit::DDX_mEdit(pDX, IDC_PSPELEMENT, editPSP, get_data()->get_leitkarte().prj_produktlinie, 1);
		}
	}
	else
	{
		bt->SetCheck(lt.m_IsChoosenIppeNr);
		if(lt.m_IsChoosenIppeNr == true)
		{
			GetDlgItem(IDC_SDPOSITIONSNUMMER)->EnableWindow(FALSE);
		}
		else
		{
			if(WaxSys::isAV())
			{
				GetDlgItem(IDC_SDPOSITIONSNUMMER)->EnableWindow(TRUE);
			}
			else
			{
				if(get_data()->get_werkauftrag()->get_status().get_status() == Status::Freigegeben)
				{
					GetDlgItem(IDC_SDPOSITIONSNUMMER)->EnableWindow(FALSE);
					GetDlgItem(IDC_CHECK_IPPE_MAT_KNOTEN_SD)->EnableWindow(FALSE);
				}
			}
		}
	}
	
	DDX_Text(pDX, IDC_SACHBEARBEITER, lt.sachbearbeiterNL);

	int i=0;

	i= lt.get_zulassungsnummer();
	int zul= get_data()->get_werkauftrag()->get_leitkarte().get_zulassungsnummer();
	get_data()->get_werkauftrag()->set_zulassungsstelle(zul);


	DDX_Text(pDX, IDC_MAIL,	lt.emailNl);
	
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER1 , lt.wunschliefertermin);
	//DDX_Text(pDX, IDC_SDAUFTRAGSNR ,lt.sdAuftragsnr);

	CString tmp;
    if (pDX->m_bSaveAndValidate) 
	{
		if(!lt.m_IsChoosenIppeNr)
		{
			DDX_Text(pDX, IDC_SDAUFTRAGSNR , tmp);
			lt.sdAuftragsnr = _atoi64(tmp);
		}
		else
		{
			DDX_Text(pDX, IDC_SDAUFTRAGSNR, lt.m_ippeMatNr);
		}
	}
	else
	{		
		if( !lt.m_IsChoosenIppeNr )
		{
			tmp.Format("%I64d", lt.sdAuftragsnr);                          
			DDX_Text(pDX, IDC_SDAUFTRAGSNR ,tmp);
		}
		else
		{	
			editPSP.SetDefFormat(_T("[0-9]-[0-9][0-9][0-9][0-9][0-9][0-9]-#2[0-9][0-9][0-9]-#1[0-9][0-9]-#0[0-9][0-9][0-9][0-9]"));
			editPSP.actasPSPElement(get_data()->get_leitkarte().get_prj_nl(), get_data()->get_leitkarte().get_prj_pspElement(), get_data()->get_leitkarte().get_Brandschutztechnik());

			DDX_Text(pDX, IDC_SDAUFTRAGSNR ,lt.m_ippeMatNr);
			DDX_Text(pDX, IDC_SDPOSITIONSNUMMER , lt.m_ippeKnotenNr);
		}
	}


//	tmp.Format("%u", lt.sdAuftragsnr);
	if(!lt.m_IsChoosenIppeNr)
	{
		DDV_MaxChars(pDX, tmp, 10);

		DDX_Text(pDX, IDC_SDPOSITIONSNUMMER, lt.sdPositionsnr);
		tmp.Format("%i", lt.sdPositionsnr);
		DDV_MaxChars(pDX, tmp, 4);
	}
	else
	{
		DDV_MaxChars(pDX, lt.m_ippeMatNr, 10);
		DDX_Text(pDX, IDC_SDPOSITIONSNUMMER , lt.m_ippeKnotenNr);
		DDV_MaxChars(pDX, lt.m_ippeKnotenNr, 40);

	}

	unsigned long sdtemp = get_data()->get_leitkarte().get_sdMaterialnummer();
	ddx_cbdata(pDX, IDC_SDMATERIALNUMMER, sdtemp);
	get_data()->get_leitkarte().set_sdMaterialnummer(sdtemp);

	unsigned int zultemp = get_data()->get_leitkarte().get_zulassungsnummer();
	ddx_cbdata(pDX, IDC_GUIDLINE, zultemp);
	get_data()->get_leitkarte().set_zulassungsnummer(zultemp);


	DDX_Text(pDX, IDC_KCAUFTRAGSNR ,lt.kcAuftragsnr);
	tmp.Format("%u", lt.kcAuftragsnr);
	DDV_MaxChars(pDX, tmp, 10);

	DDX_Text(pDX, IDC_KCPOSITIONSNUMMER ,lt.kcPositionsnr);
	tmp.Format("%i", lt.kcPositionsnr);
	DDV_MaxChars(pDX, tmp, 4);
	
		
	CString nichtfreigegeben; nichtfreigegeben = getStammdaten().getText("IDS_NICHTFREIGEGEBEN").c_str();
	CString nichtbearbeitet; nichtbearbeitet = getStammdaten().getText("IDS_NICHTBEARBEITET").c_str();
	GetDlgItem(IDC_ERSTELLUNGSDATUM)->SetWindowText(get_data()->get_werkauftrag()->get_erstellungsdatum().Format());
	GetDlgItem(IDC_FREIGABEDATUM)->SetWindowText(get_data()->get_werkauftrag()->get_status().get_freigabedatum().m_dt <= 0.0 ? nichtfreigegeben : get_data()->get_werkauftrag()->get_status().get_freigabedatum().Format());	
	GetDlgItem(IDC_BEARBEITUNGSDATUM)->SetWindowText(get_data()->get_werkauftrag()->get_status().get_bearbeitungsdatum().m_dt <= 0.0 ? nichtbearbeitet : get_data()->get_werkauftrag()->get_status().get_bearbeitungsdatum().Format());

	CString tmpstr = get_data()->get_werkauftrag()->get_status().get_sachbearbeiterSegment();
	DDX_Text(pDX, IDC_SACHBEARBEITEROD, tmpstr);
	get_data()->get_werkauftrag()->get_status().set_sachbearbeiterSegment(tmpstr);
	
	
	MXDate tmptime = get_data()->get_werkauftrag()->get_status().get_mbtermin();

	if (pDX->m_bSaveAndValidate || (!pDX->m_bSaveAndValidate && tmptime.is_valid()))
		DDX_DateTimeCtrl(pDX, IDC_MBTERMIN,tmptime);
		
	if (GetDlgItem(IDC_MBTERMIN)->IsWindowVisible() && pDX->m_bSaveAndValidate)
	{
		get_data()->get_werkauftrag()->get_status().set_mbtermin(tmptime );
	}

	if (!pDX->m_bSaveAndValidate)
	{
		if(!lt.m_IsChoosenIppeNr)
		{
			// Felder mit Nullen auffüllen
			initNullVals(IDC_SDPOSITIONSNUMMER, 4);
			initNullVals(IDC_SDAUFTRAGSNR, 10);
		}
	}
	else
	{
		//Änderungen auf der Leitkarte an die untergeordneten Positionen vererben
		get_data()->vererbe_leitkarte();
	}
	show_aenderungsdatum();

	double dtmp = 0;
	MXDate date;
	if (pDX->m_bSaveAndValidate)
	{
		DDX_Text(pDX, IDC_PA_PREIS , tmpstr);
		tmpstr.Replace(",",".");
		lt.preisanfrage.set_preis(atof(tmpstr));	

		DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_GUELTIGBIS, date);
		lt.preisanfrage.set_gueltigBis(date);

		DDX_Text(pDX, IDC_PA_INFO , tmpstr);
		lt.preisanfrage.set_info(tmpstr.GetBuffer());	
	}
	else
	{
		dtmp = lt.preisanfrage.get_preis();
		DDX_Text(pDX, IDC_PA_PREIS , dtmp);		
		
		WaxPart* waxPart = get_data();
		Werkauftrag* werkauftrag =  waxPart->get_werkauftrag();
		double Preis = 0;

		BOOL bFound = FALSE;
		if(werkauftrag->get_GUID().IsEmpty() == TRUE)
		{
			for (POSITION posG = waxPart->get_subobjects().GetHeadPosition(); posG != NULL; )
			{
				WaxPart* waxpt = dynamic_cast<WaxPart*>(waxPart->get_subobjects().GetNext(posG));
				Werkauftrag* werkauftragII = waxpt->get_werkauftrag();
				if(werkauftragII->get_GUID().IsEmpty()== FALSE)
				{
					bFound = TRUE;
					break;
				}
			}
		}
		else
		{
			bFound = TRUE;
		}

		if(bFound == TRUE)
		{	
			HtmlDruck file; 
			WaxDruck::DrucklisteWap DrucklisteWap(file, waxPart, GpWapPreis::PREISART::Vollkosten); 
			Preis = DrucklisteWap.createWap();
		}
		else
		{
			GpVk3Preis vk3Preis;
			Preis = vk3Preis.getVk3Preis(get_data());
		}
		tmp.Format("%.2f", Preis);
		DDX_Text(pDX, IDC_VK3_PREIS, tmp);	

		if (dtmp > 0 && Preis > 0)
			tmp.Format("%.2f",(dtmp / Preis - 1.0) * 100.0);
		else
			tmp = "";
		DDX_Text(pDX, IDC_EDIT_PROZENT , tmp);	

		date = lt.preisanfrage.get_gueltigBis();
		DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_GUELTIGBIS, date);		

		tmp.Format("%s: %s", getStammdaten().getText("IDC_STATIC_STATUS").c_str(), get_data()->getStatusText());
		DDX_Text(pDX, IDC_STATIC_STATUS , tmp);

		tmp =  lt.preisanfrage.get_info().c_str();
		DDX_Text(pDX, IDC_PA_INFO , tmp);
	}
}


BOOL SammelAuftragForm::OnInitDialog() 
{


	Werkauftrag* wa= get_data()->get_werkauftrag();


	wa->set_zulassungsstelle(wa->get_leitkarte().get_zulassungsnummer());

	//GetDlgItem(IDC_CHECK_NEXTGENERATION)->EnableWindow(FALSE);
	initSDMaterialnummer();

	initZulassungsDropDown(wa);

	WaxPartForm::OnInitDialog();

	CComboBox *bx = (CComboBox*)GetDlgItem(IDC_SACHBEARBEITER);
	XASSERT(bx);
	if (bx) 
		WaxSys::user_list(bx);
	bx = (CComboBox*)GetDlgItem(IDC_SACHBEARBEITEROD);
	XASSERT(bx);
	if (bx) 
	{
		WaxSys::user_list(bx);
	}
	

	GetDlgItem(IDC_CHECK_IPPE_MAT_KNOTEN_SD)->SetWindowText(getStammdaten().getText("IDD_NEXTGENERATION").c_str());
	
	if (!av)
	{
		// AV Elemente bei NL Version abschalten
		((CEdit*)GetDlgItem(IDC_WANR))->SetReadOnly();
		((CEdit*)GetDlgItem(IDC_WANR))->ModifyStyle(WS_TABSTOP, 0);
		GetDlgItem(IDC_MBTERMIN)->EnableWindow(FALSE);
		GetDlgItem(IDC_WERKAUFTRAG_VK3)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_MBTERMIN_BEZEICHNUNG)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDITWANR)->ShowWindow(SW_HIDE);
		((CEdit*)GetDlgItem(IDC_KCAUFTRAGSNR))->SetReadOnly();
		((CEdit*)GetDlgItem(IDC_KCPOSITIONSNUMMER))->SetReadOnly();
		((CEdit*)GetDlgItem(IDC_PA_PREIS))->SetReadOnly();
		((CEdit*)GetDlgItem(IDC_EDIT_PROZENT))->SetReadOnly();
		GetDlgItem(IDC_BUTTON_CALC)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DATETIMEPICKER1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_PA_INFO)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_INFO)->ShowWindow(SW_HIDE);

		// Sachbearbeiter OD
		if (bx)
		{
			bx->ShowDropDown(FALSE);
			bx->EnableWindow(FALSE);
		}

		
	}	

	//Wenn Preisanfrage dann SD Nummer sperren
	if (get_data()->get_werkauftrag()->get_status().get_status() != Status::Angebot && get_data()->isPreisanfrage())
	{
		GetDlgItem(IDC_SDAUFTRAGSNR)->EnableWindow(FALSE);
		GetDlgItem(IDC_SDPOSITIONSNUMMER)->EnableWindow(FALSE);
	}

	if (get_data()->get_leitkarte().get_emailNl().IsEmpty())
	{
		get_data()->get_leitkarte().set_emailNl(WaxSys::getEmail().c_str());
		//GetDlgItem(IDC_MAIL)->SetWindowText();
	}

	//OnBnClickedIppeKnoten();
	int iChecked = ((CButton*)GetDlgItem(IDC_CHECK_IPPE_MAT_KNOTEN_SD))->GetCheck();
	if(iChecked > 0)
	{
		editPSP.SetDefFormat(_T("[0-9]-[0-9][0-9][0-9][0-9][0-9][0-9]-#2[0-9][0-9][0-9]-#1[0-9][0-9]-#0[0-9][0-9][0-9][0-9]"));
		editPSP.actasPSPElement(get_data()->get_leitkarte().get_prj_nl(), get_data()->get_leitkarte().get_prj_pspElement(), get_data()->get_leitkarte().get_Brandschutztechnik());
	}
	else
	{
		editPSP.SetDefFormat(_T("[0-9]-[0-9][0-9][0-9][0-9][0-9][0-9]-#2[0-9][0-9][0-9]-#1[0-9][0-9]-#0[0-9][0-9][0-9]"));
		editPSP.actasPSPElement(get_data()->get_leitkarte().get_prj_nl(), get_data()->get_leitkarte().get_prj_pspElement(), get_data()->get_leitkarte().get_prj_produktlinie());
	}

	disablePrjleitkartenItem();

	if(WaxSys::isAV())
	{
	 ((CEdit*)GetDlgItem(IDC_NLKENNUNG))->SetReadOnly(FALSE);
	}

	

#ifdef __WAX_FUER_DRITTE__
		GetDlgItem(IDC_PSPELEMENT)->EnableWindow(FALSE);
		GetDlgItem(IDC_COAUFTRAG)->EnableWindow(FALSE);
		GetDlgItem(IDC_SDPOSITIONSNUMMER)->EnableWindow(FALSE);
		GetDlgItem(IDC_SDAUFTRAGSNR)->EnableWindow(FALSE);
		GetDlgItem(IDC_SDMATERIALNUMMER)->EnableWindow(FALSE);

		GetDlgItem(IDC_KCAUFTRAGSNR)->EnableWindow(FALSE);
		GetDlgItem(IDC_KCPOSITIONSNUMMER)->EnableWindow(FALSE);
		GetDlgItem(IDC_SACHBEARBEITEROD)->EnableWindow(FALSE);

		GetDlgItem(IDC_STATIC_Preise)->ModifyStyle(WS_VISIBLE, NULL);
		GetDlgItem(IDC_PANR)->ModifyStyle(WS_VISIBLE, NULL);
		GetDlgItem(IDC_STATIC_VK3_PREIS)->ModifyStyle(WS_VISIBLE, NULL);
		GetDlgItem(IDC_VK3_PREIS)->ModifyStyle(WS_VISIBLE, NULL);
		GetDlgItem(IDC_STATIC_EURO1)->ModifyStyle(WS_VISIBLE, NULL);
		GetDlgItem(IDC_STATIC_PANR)->ModifyStyle(WS_VISIBLE, NULL);
		GetDlgItem(IDC_STATIC_PA_PREIS)->ModifyStyle(WS_VISIBLE, NULL);				
		GetDlgItem(IDC_BUTTON_CALC)->ModifyStyle(WS_VISIBLE, NULL);
		GetDlgItem(IDC_STATIC_PROZENT)->ModifyStyle(WS_VISIBLE, NULL);
		GetDlgItem(IDC_EDIT_PROZENT)->ModifyStyle(WS_VISIBLE, NULL);
		GetDlgItem(IDC_PA_INFO)->ModifyStyle(WS_VISIBLE, NULL);
		GetDlgItem(IDC_STATIC_INFO)->ModifyStyle(WS_VISIBLE, NULL);

		GetDlgItem(IDC_PA_PREIS)->ModifyStyle(WS_VISIBLE, NULL);
		GetDlgItem(IDC_STATIC_PA_PREIS)->ModifyStyle(WS_VISIBLE, NULL);
		GetDlgItem(IDC_STATIC_EURO2)->ModifyStyle(WS_VISIBLE, NULL);
		GetDlgItem(IDC_STATIC_GUELTIGBIS)->ModifyStyle(WS_VISIBLE, NULL);
		GetDlgItem(IDC_DATETIMEPICKER_GUELTIGBIS)->ModifyStyle(WS_VISIBLE, NULL);

		GetDlgItem(IDC_STATIC_DBKEN)->ModifyStyle(WS_VISIBLE, NULL);
		GetDlgItem(IDC_DBID)->ModifyStyle(WS_VISIBLE, NULL);
		GetDlgItem(IDC_CHECK_IPPE_MAT_KNOTEN_SD)->EnableWindow(FALSE);
#endif

	GetDlgItem(IDC_AENDERUNGSDATUM)->EnableWindow(FALSE);

	setWindowsText(IDC_STATIC_PRJEINST, "IDC_STATIC_PRJEINST");

	setWindowsTextWithColon(IDC_STATIC_NL, "IDS_NIEDERLASSUNG");
	setWindowsTextWithColon(IDC_STATIC_AUFTRAG, "IDS_AUFTRAG_BAU");
	setWindowsTextWithColon(IDC_STATIC_ORTBEZ, "IDS_ORT");
	setWindowsTextWithColon(IDC_STATIC_BEZ, "IDS_BAUABSCHNITT");
	setWindowsTextWithColon(IDC_STATIC_PLZ, "IDS_PLZ");
	setWindowsTextWithColon(IDC_STATIC_PSP, "IDS_PSPAUFTRAG");
	setWindowsTextWithColon(IDC_AUFTRAGSNR, "IDS_COAUFTRAG");

	setWindowsTextWithColon(IDC_STATIC_WERKAUFTRAG, "IDC_STATIC_WERKAUFTRAG");
	setWindowsTextWithColon(IDC_STATIC_SDNR, "IDS_SDNR");
	setWindowsTextWithColon(IDC_STATIC_SDPOS, "IDS_SDPOS");
	setWindowsTextWithColon(IDC_STATIC_SDMAT, "IDS_SDMAT");
	setWindowsTextWithColon(IDC_STATIC_BESTELLER, "IDS_BESTELLER_SACH");
	setWindowsTextWithColon(IDC_STATIC_MAIL, "IDC_STATIC_MAIL");
	if (WaxSys::isAV())
	{
		setWindowsTextWithColon(IDC_STATIC_LIEFERTERMIN, "IDS_LIEFERTERMIN");
	}
	else
	{
		setWindowsTextWithColon(IDC_STATIC_LIEFERTERMIN, "IDS_LIEFER_TERMIN_SAP");
	}
	setWindowsTextWithColon(IDC_WERKAUFTRAG_VK3, "IDS_VK3_ABRECHNUNG");

	setWindowsTextWithColon(IDC_STATIC_STATUS, "IDC_STATIC_STATUS");
	setWindowsTextWithColon(IDC_STATIC_WANR, "IDS_WANR");
	setWindowsTextWithColon(IDC_STATIC_KCNR, "IDS_KCNR");
	setWindowsTextWithColon(IDC_STATIC_KCPOS, "IDS_KCPOS");
	setWindowsTextWithColon(IDC_STATIC_BEARBEITER_OD, "IDS_BEARBEITER_OD");
	setWindowsTextWithColon(IDC_STATIC_FREIGABE_DAT, "IDS_FREIGABE_DATUM");
	setWindowsTextWithColon(IDC_STATIC_BEARBEIT_DAT, "IDS_BEARBEIT_DATUM");
	setWindowsTextWithColon(IDC_STATIC_ERSTELL_DAT, "IDS_ERSTELL_DATUM");
	setWindowsTextWithColon(IDC_AENDERUNGSDATUM_BEZEICHNUNG, "IDS_AENDERUNG_DATUM");
	setWindowsTextWithColon(IDC_MBTERMIN_BEZEICHNUNG, "IDS_MB_DATUM");
	setWindowsTextWithColon(IDC_STATIC_DBKEN, "IDS_DBKENNUNG");	

	setWindowsText(IDC_STATIC_Preise, "IDC_STATIC_Preise");
	setWindowsTextWithColon(IDC_STATIC_VK3_PREIS, "IDS_VK3_PREIS");
	setWindowsTextWithColon(IDC_STATIC_PANR, "IDC_STATIC_PANR");
	setWindowsTextWithColon(IDC_STATIC_PA_PREIS, "IDS_PREIS");
	setWindowsTextWithColon(IDC_STATIC_GUELTIGBIS, "IDC_STATIC_GUELTIGBIS");
	setWindowsText(IDC_BUTTON_CALC, "IDC_BERECHNEN");
	setWindowsText(IDC_STATIC_PROZENT, "IDC_PROZENTVON");
	
	
#ifndef __WAX_FUER_DRITTE__
	if (get_data()->get_leitkarte().get_preisanfrage().get_nummer() > 0)
	{
		CString str;
		str.Format("%i - %i", get_data()->get_leitkarte().get_preisanfrage().get_nummer(), get_data()->get_leitkarte().get_preisanfrage().get_position());

		GetDlgItem(IDC_PANR)->SetWindowText(str);

	}
	else
	{
		GetDlgItem(IDC_PA_PREIS)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_PROZENT)->EnableWindow(FALSE);
		GetDlgItem(IDC_PA_INFO)->EnableWindow(FALSE);
	}
#endif

	Leitkarte& lt = get_data()->get_leitkarte();
	iChecked = ((CButton*)GetDlgItem(IDC_CHECK_IPPE_MAT_KNOTEN_SD))->GetCheck();
	if(iChecked > 0)
	{
		GetDlgItem(IDC_BUTTON_IPPE_NR)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BUTTON_IPPE_NR)->SetWindowText(getStammdaten().getText("IDS_IPPE_MAT_ NR_ VON_ SAP").c_str());
		GetDlgItem(IDC_STATIC_SDPOS)->SetWindowText(getStammdaten().getText("IDS_IPPE_KNOTEN_NR").c_str());
		GetDlgItem(IDC_SDPOSITIONSNUMMER)->EnableWindow(FALSE);
		
		CWnd* pWnd = GetDlgItem(IDC_SDAUFTRAGSNR);
		pWnd->ModifyStyle(ES_NUMBER, 0);
		pWnd->SendMessage(EM_LIMITTEXT, 10, 0);
		pWnd->SetWindowText(lt.m_ippeMatNr);
		lt.sdAuftragsnr = 0;

		pWnd = GetDlgItem(IDC_SDPOSITIONSNUMMER);
		pWnd->ModifyStyle(ES_NUMBER, 0);
		pWnd->SetWindowText(lt.m_ippeKnotenNr);
		pWnd->SendMessage(EM_LIMITTEXT, 40, 0);
		lt.sdPositionsnr = 0;	
	}
	else
	{
		GetDlgItem(IDC_BUTTON_IPPE_NR)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_SDNR)->SetWindowText(getStammdaten().getText("IDS_SDNR").c_str());
		GetDlgItem(IDC_STATIC_SDPOS)->SetWindowText(getStammdaten().getText("IDS_SDPOS").c_str());
		lt.m_ippeMatNr	 = "";	
		lt.m_ippeKnotenNr	= "";
		CString strTemp;
		
		strTemp.Format( "%I64d", lt.sdAuftragsnr);
		CWnd* pWnd = GetDlgItem(IDC_SDAUFTRAGSNR);
		pWnd->ModifyStyle(0, ES_NUMBER);
		GetDlgItem(IDC_SDAUFTRAGSNR)->SetWindowText(strTemp);
		
		strTemp.Format("%ld", lt.sdPositionsnr);
		pWnd = GetDlgItem(IDC_SDPOSITIONSNUMMER);
		pWnd->ModifyStyle(0, ES_NUMBER);
		pWnd->SetWindowText(strTemp);
	}
	return TRUE;
}



//Indicates the edit control is losing the input focus
void SammelAuftragForm::OnKillfocusSdauftragsnr()
{
	UpdateData();
	if(!get_data()->get_leitkarte().m_IsChoosenIppeNr)
	{
		initNullVals(IDC_SDAUFTRAGSNR, 10);
	}
}


void SammelAuftragForm::OnKillfocusPsp()
{
	UpdateData();
}

//Indicates the edit control is losing the input focus
void SammelAuftragForm::OnKillfocusSdpositionsnummer()
{
	UpdateData();
	if(!get_data()->get_leitkarte().m_IsChoosenIppeNr)
	{
		initNullVals(IDC_SDPOSITIONSNUMMER, 4);
	}
}


void SammelAuftragForm::initNullVals(UINT id, int nullcount)
{
	CString dat;
	GetDlgItem(id)->GetWindowText(dat);
	while (dat.GetLength() < nullcount)
		dat = "0" + dat;
	GetDlgItem(id)->SetWindowText(dat);
}

//Erzeugt aus allen waids aller Unterobjekte einen String und zeigt ihn in an.
void SammelAuftragForm::displayDBids()
{
// Für alle Unterobjekte die Datenbankids anzeigen
	CString dbid, temp; 
	if (get_data()->get_werkauftrag()->get_waid() || get_data()->get_subobjects().IsEmpty())
		dbid.Format("%i", get_data()->get_werkauftrag()->get_waid());

	for (POSITION pos = get_data()->get_subobjects().GetHeadPosition(); pos; )
	{
		WaxPart *next = dynamic_cast<WaxPart*>(get_data()->get_subobjects().GetNext(pos));
		XASSERT(next);
		if (next)
		{
			temp.Format("%i", next->get_werkauftrag()->get_waid());
			dbid += temp;
			if (pos)
				dbid += "/";
			
		}
	}
	GetDlgItem(IDC_DBID)->SetWindowText(dbid);
}


///Setzt das Aenderungsdatum
void SammelAuftragForm::show_aenderungsdatum()  const
{
	// Das aktuellste Aenderungsdatum suchen und setzen
	if (get_data()->get_subobjects().IsEmpty())
		return;
	
	MXDate temp;
	temp.m_dt = 0.0;

	for (POSITION pos = get_data()->get_subobjects().GetHeadPosition(); pos; )
	{
		const WaxPart *next = dynamic_cast<const WaxPart*>(get_data()->get_subobjects().GetNext(pos));
		XASSERT(next);
		if (next && next->get_werkauftrag_const()->get_aenderungsdatum().is_newer(temp))
				temp = next->get_werkauftrag_const()->get_aenderungsdatum();
	}
	GetDlgItem(IDC_AENDERUNGSDATUM)->SetWindowText(temp.Format());	
}



//Prüft, ob das Projekt eine Projektleitkarte besitzt. Falls ja werden alle 
//Felder der Projektleitkarte disabled.
void SammelAuftragForm::disablePrjleitkartenItem()
{
#if 0
	// Erstmal werden zur Probe immer alle Daten der Projektleitkarte abgeblendet. Wenn die AV einen WA für eine 
	// Niederlassung anfängt, dann bitte im Extraprojekt mit Projektleitkarte.
	if (!dynamic_cast<PrjLeitkarteData*>(((CWaxDoc*)get_document())->m_Objects.GetAt(0)))
		return;
#endif
	// Wenn das Projekt eine Projektleitkarte hat, werden die folgenden Felder dort gesetzt 
	int avitems[] = { IDC_NLKENNUNG, IDC_AUFTRAGSBEZ, IDC_PLZ, IDC_ORT, 0 };

	for (int i = 0; avitems[i]; i++)
	{
		((CEdit*)GetDlgItem(avitems[i]))->SetReadOnly();
		((CEdit*)GetDlgItem(avitems[i]))->ModifyStyle(WS_TABSTOP, 0);
	}
	//IDC_PSPELEMENT, 
}



//Zeigt ein Kontextmenü mit den Menüpunkten Löschen, Neu und manuell
void SammelAuftragForm::onEditwanr()
{
	CMenu pop;
	pop.CreatePopupMenu();

	if (1) // Damit vor dem Menüaufbau die Sanduhr erscheint
	{
		CWaitCursor w;
		CString str;
		str = getStammdaten().getText("IDS_MENU_NEW").c_str();
		pop.AppendMenu(MF_STRING, IDC_WANRNEU,  (LPCTSTR)str);
		str = getStammdaten().getText("IDS_MENU_DELETE").c_str();
		pop.AppendMenu(MF_STRING, IDC_WANRLOESCHEN,  (LPCTSTR)str);
		str = getStammdaten().getText("IDS_MENU_MANUELL").c_str();
		pop.AppendMenu(MF_STRING, IDC_MANUELL_WANR,  (LPCTSTR)str);

		pop.AppendMenu(MF_SEPARATOR);

		for (int i = 0; i < ANZ_NRKREIS; i++)
		{
			CString entry = "0";
			Server serv(Server::save, NULL);
			entry.Format("%i", serv.getNewWaNr(false, "", nummernkreis[i], 0, 0));
			pop.AppendMenu(MF_STRING, IDC_WANR_01+i, (LPCTSTR)entry);
		}
	}			
	CRect r;
	GetDlgItem(IDC_EDITWANR)->GetWindowRect(&r);
	CPoint pt(r.right, r.top);
	pop.TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, pt.x, pt.y, this); 
	pop.DestroyMenu();	
}

// Setzt die WA Nummer neu, indem aus den SD Nummern eine neue generiert wird
void SammelAuftragForm::onWaNrNeu()
{
	UpdateData();
	Leitkarte& lt = get_data()->get_leitkarte();
	
	WaxPart *totest = get_data();
	if (get_data()->get_subobject_count())
	{
		totest = get_data()->getFirstSubobject();
		totest->get_leitkarte().copy(lt);
	}

	if (!totest->get_werkauftrag()->get_waid())
	{
		CString str; str = getStammdaten().getText("IDS_AUFTRAG_FREIGEBEN").c_str();
		AfxMessageBox(str);
		return;
	}

	MXTable transact(WaxSys::getServerdb(), "Status");
	DbFreigabe freigabe(totest, NULL);
	if (!freigabe.check_freigabe_plausis())
		return;
	
	Server serv(Server::save, get_data()->get_werkauftrag());

	// Erst muss die Wanr zurückgesetzt werden, erst danach kann eine neue vergeben werden
	if (get_data()->get_werkauftrag()->get_status().get_wanr())
		setWaNr(0);

	// getNewWaNr liefert erst dann eine neue Nummer, wenn die alte gelöscht wurde
	UINT newwanr = serv.getNewWaNr(false, lt.get_ippeMatNr(),  lt.get_sdMaterialnummer(), lt.get_sdAuftragsnr(), lt.get_sdPositionsnr());

	if (setWaNr(newwanr))
	{
		SetDlgItemInt(IDC_WANR, newwanr);
		UpdateData();
		AfxGetMainWnd()->PostMessage(WM_COMMAND, WAXID_DOTREEUPDATE);
	}
	else
		AfxMessageBox(getStammdaten().getText("IDS_KEINE_WANR").c_str(), MB_OK | MB_ICONERROR);
}

// Setzt die WA Nummer auf Null
void SammelAuftragForm::onWaNrLoeschen()
{
	/*
	Setzt die WA Nummer auf Null. Damit ist der Werkauftrag wieder in der NL bearbeitbar.
	*/
	Server wanupdate(Server::save, get_data()->get_werkauftrag());
	if (int xwanr = get_data()->get_werkauftrag()->get_status().get_wanr())
	{
		CString str; str = getStammdaten().getText("IDS_WANR_SPERREN").c_str();
		if (AfxMessageBox(str, MB_YESNO | MB_ICONQUESTION) == IDYES)
		{
			WanrSperrung dlg(this, xwanr);
			if (!dlg.lock())
				return;
		}
	}

	if (setWaNr())
		GetDlgItem(IDC_WANR)->SetWindowText("0");
	AfxGetMainWnd()->PostMessage(WM_COMMAND, WAXID_DOTREEUPDATE);
}

///Testet auf Waid
bool SammelAuftragForm::testWaid()
{
	WaxPart* test = NULL;
	if (get_data()->get_subobjects().GetSize() > 0)
		test = get_data()->getFirstSubobject();
	if (!test)
		test = get_data();
	if (!test->get_werkauftrag()->get_waid())
	{
		CString str; str = getStammdaten().getText("IDS_AUFTRAG_FREIGEBEN").c_str();
		AfxMessageBox(str);
		return false;
	}
	return true;
}

//Versucht, die Wanr nach Userangabe zu setzen
void SammelAuftragForm::onManuellWanr()
{
	if (!testWaid())
		return;
	
	int oldwanr = get_data()->get_werkauftrag()->get_status().get_wanr();
	WanrSperrung dlg(this, oldwanr);

	int newwanr = 0;
	if (dlg.DoModal() == IDOK)
	{
		newwanr = dlg.get_val();
		
		if (setWaNr(newwanr))
		{
			// Update hat geklappt, jetzt den Wert im Dialog anzeigen und 'speichern'
			dlg.remove_val(dlg.get_val());
			GetDlgItem(IDC_WANR)->SetWindowText(dlg.get_valstring());
			UpdateData();
		}
		else
		{
			CString str; str = getStammdaten().getText("IDS_WANR_NICHT").c_str();
			AfxMessageBox(str, MB_OK | MB_ICONERROR);
		}
	}
	AfxGetMainWnd()->PostMessage(WM_COMMAND, WAXID_DOTREEUPDATE);
}

//Initialisiert die Listbox mit den DummyMaterialnummern
void SammelAuftragForm::initSDMaterialnummer()
{
	// Achtung: von Verteiler abgeleitete Klassen mit anderer Nummer muesen 
	// zuerst den Text zugewiesen bekommen, hier "EntnahmeleitungDWB"
	CComboBox *bx = (CComboBox*)GetDlgItem(IDC_SDMATERIALNUMMER);
	bx->SetDroppedWidth(300);

	WaxPart *relevant = get_data()->getFirstSubobject();
	if (!relevant)
		relevant = get_data();

	CString klasse = getSDMaterialKlasse(relevant->get_werkauftrag());
	
	// Mit dem Klassennamen wird in der dummymaterialnummern Tabelle die passenden SD Materialnummern gesucht
	for (POSITION pos = getStammdaten().get_dummymaterialnummern().GetHeadPosition(); pos; )
	{
		DummyMaterialnummern::data* dat= (DummyMaterialnummern::data*)getStammdaten().get_dummymaterialnummern().GetNext(pos);
		

		// Die AV kann Materialnummern beliebig wählen
		if (!av)
			if (klasse.GetLength() && dat->get_klasse().Find(klasse) == -1)
				continue;
		
		CString txt;
		txt.Format("%i - %s", dat->get_nummer(), dat->get_bezeichnung());
		int index = bx->AddString(txt);
		XASSERT(index >= 0);
		if (index >= 0)
		{
			bx->SetItemData(index, dat->get_nummer());
			if (dat->get_nummer() == get_data()->get_werkauftrag()->get_leitkarte().get_sdMaterialnummer())
				bx->SelectString(-1, txt);
		}
	}
}






//Initialisiert die Listbox mit den Zulassungen
void SammelAuftragForm::initZulassungsDropDown(Werkauftrag* wa)
{
	// Achtung: von Verteiler abgeleitete Klassen mit anderer Nummer muesen 
	// zuerst den Text zugewiesen bekommen, hier "EntnahmeleitungDWB"
	CComboBox *bx = (CComboBox*)GetDlgItem(IDC_GUIDLINE);
	bx->SetDroppedWidth(300);

	int zulNr= wa->get_zulassungsstelle();

	CString klasse = getSDMaterialKlasse(wa);
	
	// Mit dem Klassennamen wird in der dummymaterialnummern Tabelle die passenden SD Materialnummern gesucht
	for (POSITION pos = getStammdaten().get_zulassung().GetHeadPosition(); pos; )
	{
		zulassung::data* dat= (zulassung::data*)getStammdaten().get_zulassung().GetNext(pos);
		


		CString txt;
		txt.Format("%i - %s", dat->get_code(), dat->get_typ());
		int index = bx->AddString(txt);
		XASSERT(index >= 0);
		if (index >= 0)
		{
			bx->SetItemData(index, dat->get_code());
			if (dat->get_code() == zulNr)
				bx->SelectString(-1, txt);
		}
	}
}



INT_PTR SammelAuftragForm::OnToolHitTest( CPoint point, TOOLINFO* pTI ) const
{
	/*
	Tooltipfunktion
	Prüft die Eingabe der Dummymaterialnummer und gibt eine Beschreibung der Nummer aus
	*/	
	const int MAXHELPSIZE = 2048;
	int res = CDialog::OnToolHitTest( point, pTI );
	if (res == IDC_SDMATERIALNUMMER)
	{
		
		CComboBox *bx = (CComboBox*)GetDlgItem(IDC_SDMATERIALNUMMER);
		int nr = bx->GetCurSel();
		if (nr >= 0)
		{
			nr = bx->GetItemData(nr);
			pTI->lpszText = (LPSTR)malloc(MAXHELPSIZE+1);
			pTI->uFlags |= TTF_ALWAYSTIP;
			CString out;
			
			for (POSITION pos = getStammdaten().get_dummymaterialnummern().GetHeadPosition(); pos; )
			{
				DummyMaterialnummern::data* dat= (DummyMaterialnummern::data*)getStammdaten().get_dummymaterialnummern().GetNext(pos);
				if (dat->get_nummer() == nr)
				{
					strcpy(pTI->lpszText, dat->get_verwendung());
					break;
				}
			}
			
		}
	}
	else return WaxPartForm::OnToolHitTest(point, pTI);
	return res;
}



CString SammelAuftragForm::getSDMaterialKlasse(Werkauftrag * const werk) const
{
	/*
	Liefert den Klassennamen des Werkauftrages zurück. Dieser wird benutzt, um in der Tabelle mit SD Material
	nummern die passende zu finden
	*/
	XASSERT(werk);
	if (!werk)
		return "????";
	const type_info &typ = typeid(*werk);

	CString name = typ.name();
	if (name.Find("class ") == 0)
		name = name.Right(name.GetLength() - 6);
	return name;
}


//Plausibilitätscheck, wird aufgerufen, bevor UpdateData(TRUE) aufgerufen 
//wird.
bool SammelAuftragForm::check_plausis()
{
	Leitkarte& lt = get_data()->get_leitkarte();
	

	if(lt.m_IsChoosenIppeNr)
	{
						
		int iLength = lt.m_ippeMatNr.GetLength();
		if( iLength == 10 &&  !lt.m_ippeMatNr.IsEmpty())
		{
			CString strTemp = _T("1");
			if(strTemp.Compare( lt.m_ippeMatNr.Left(1)) != 0)
			{
				AfxMessageBox(getStammdaten().getText("IDS_IPPE_NR_TEN_DIGITS").c_str());
				return false;
			}
		}

	}
	else
	{
		CString strSDAuftragNr;
		strSDAuftragNr.Format( "%I64d", lt.sdAuftragsnr);
		
		int iLength = strSDAuftragNr.GetLength();
		
		if( iLength == 10 )
		{
		
			CString strTemp = _T("1");
			if(strTemp.Compare( strSDAuftragNr.Left(1)) == 0)
			{
				AfxMessageBox(getStammdaten().getText("IDS_KEINEGUELTIGE_SDNR_IPPE").c_str());
				return false;
			}
		}
	}


	if (!WaxPartForm::check_plausis())
		return false;
	
	// Prüfung nur, wenn noch nicht freigegeben
	// Sonst kommt die Fehlermeldung auch bei Aufträgen, die noch nicht statusaktualisiert sind
	// bzw. in AV wenn Wunschliefertermin zu knapp bemessen ist. Dort soll der Termin auch nicht 
	// geändert werden, es ist ja ein WUNSCHliefertermin der NL.
	// Als Abbruch dient die Plausi, wenn in der NL freigegeben wird.
	
	if (!this->get_data()->get_werkauftrag()->get_waid())
	{
		CString err = get_data()->get_leitkarte().check();
		if (err.GetLength())
			((CFrameWnd*)AfxGetMainWnd())->SetMessageText(err);
	}
	return true; 
}



bool SammelAuftragForm::setWaNr(int newnr)
{
	// Wrapperfunktion für Server::setwanr, wobei alle Unteraufträge (Sammelauftrag) mit abgearbeitet
	// werden
	bool success = false;
	Server wanupdate(Server::save, get_data()->get_werkauftrag());
	if (wanupdate.setwanr(newnr))
		success = true;	
	
	// Evtl. Unteraufträge setzen (Sammelauftrag)
	for (POSITION pos = get_data()->get_subobjects().GetHeadPosition(); pos;)
	{
		WaxPart *pt =  dynamic_cast<WaxPart*>(get_data()->get_subobjects().GetNext(pos));
		if (pt)
		{
			Server wanupdate(Server::save, pt->get_werkauftrag());

			// Wenn schon ein Update geklappt hat (success == true) braucht die Wanr nicht
			// mehr geprüft werden
			if (wanupdate.setwanr(newnr, !success))
				success = true;		

			pt->get_werkauftrag()->get_status().set_wanr(newnr);
		}
	}
	if (success)
	{
		// Wa Nummer aus der Liste mit Sperrungen entfernen
		CString sql;
		sql.Format("DELETE FROM gesperrte_wanr WHERE wanr = %i", newnr);
		MXTable tab(wanupdate.get_dbhandle(), "gesperrte_wanr");
		tab.executeSQL(sql);

		get_data()->get_werkauftrag()->get_status().set_wanr(newnr);
	}
	return success;
}


void SammelAuftragForm::checkFreigabe()
{
	// Felder abblenden/anzeigen. Wird nach Freigabe aufgerufen, wenn needUpdate gesetzt wurde
	int showflag = SW_SHOW;
	if (!av && !get_data()->get_werkauftrag()->get_status().get_mbtermin().is_valid()) 
		showflag = SW_HIDE;

	GetDlgItem(IDC_MBTERMIN)->ShowWindow(showflag);
	GetDlgItem(IDC_MBTERMIN_BEZEICHNUNG)->ShowWindow(showflag);
	
	if (!get_data()->is_freigegeben())
		return;
	
	displayDBids();
	
	if (av)
	{	
		// Button für manuelle WaNummern-Vergabe.
		GetDlgItem(IDC_EDITWANR)->ShowWindow(SW_SHOW);
	}
	else
	{
		// Ist schon passiert: Wenn erstmal freigegeben wurde, darf die SD Nummer nicht mehr verändert werden. 
		// Sonst kommt jemand auf die Idee und gibt mehrere Aufträge nacheinander frei, die
		// nacheinander überschrieben werden. Der User könnte annehmen, dass er mehrere Aufträge 
		// freigegeben hat, es kommt aber nur der letzte Auftrag in der Datenbank an.

		if (get_data()->get_werkauftrag()->get_status().get_status() == Status::Angebot)			
		{
			GetDlgItem(IDC_SDAUFTRAGSNR)->EnableWindow(true);
			GetDlgItem(IDC_SDPOSITIONSNUMMER)->EnableWindow(true);
		}
		else
		{
			GetDlgItem(IDC_SDAUFTRAGSNR)->EnableWindow(false);
			GetDlgItem(IDC_SDPOSITIONSNUMMER)->EnableWindow(false);
		}
	}
}

void SammelAuftragForm::on_setwanr(UINT id)
{
	if (!testWaid())
		return;

	// getNewWaNr liefert erst dann eine neue Nummer, wenn die alte gelöscht wurde
	Server serv(Server::save, get_data()->get_werkauftrag());
	UINT newwanr = serv.getNewWaNr(false, "", nummernkreis[id-IDC_WANR_01], 0, 0);
	if (setWaNr(newwanr))
	{
		SetDlgItemInt(IDC_WANR, newwanr);
		UpdateData();
		AfxGetMainWnd()->PostMessage(WM_COMMAND, WAXID_DOTREEUPDATE);
	}
}


void SammelAuftragForm::onCalcPreis()
{
	//double tol = 0.001;

	UpdateData();
	//Leitkarte& lt = get_data()->get_leitkarte();

	CString txt;
	((CEdit*)GetDlgItem(IDC_EDIT_PROZENT))->GetWindowText(txt);
	double prozent = atof(txt);

	//if (prozent < tol && prozent > tol * -1)
	//	return;

	((CEdit*)GetDlgItem(IDC_VK3_PREIS))->GetWindowText(txt);
	double preis = atof(txt);

	preis = preis * (1.0 + prozent / 100.0);

	txt.Format("%.2f", preis);


	((CEdit*)GetDlgItem(IDC_PA_PREIS))->SetWindowText(txt);
}

void SammelAuftragForm::onCalcProzent()
{
	UpdateData();
	GpVk3Preis vk3Preis;

	CString txt;
	
	((CEdit*)GetDlgItem(IDC_VK3_PREIS))->GetWindowText(txt);
	double vk3 = atof(txt);

	((CEdit*)GetDlgItem(IDC_PA_PREIS))->GetWindowText(txt);
	txt.Replace(",",".");
	double preis = atof(txt);

	txt.Format("%.2f",(preis / vk3 - 1.0) * 100.0);
	((CEdit*)GetDlgItem(IDC_EDIT_PROZENT))->SetWindowText(txt);
}




void SammelAuftragForm::OnBnClickedButtonIppeNr()
{
	//_T("7_000091_287_05_0001")
	//_T("PG_11_191_500_E1")
	// _T("Gewinderohr verzinkt - xx")

	CString strPSP;
	GetDlgItem(IDC_PSPELEMENT)->GetWindowText(strPSP);
	strPSP.Replace('-', '_');

	UpdateData();
	int sdAltDummydat = get_data()->get_leitkarte().get_sdMaterialnummer();
	CString striPPEClassId;
	CString strDescription;
		
	for (POSITION pos = getStammdaten().get_dummymaterialnummern().GetHeadPosition(); pos; )
	{
		DummyMaterialnummern::data* dat= (DummyMaterialnummern::data*)getStammdaten().get_dummymaterialnummern().GetNext(pos);
		if (dat->get_nummer() == sdAltDummydat)
		{
			striPPEClassId = dat->get_iPPEClassName();
			strDescription = dat->get_iPPEKurzText();
			
			strDescription += CString( " - ");
			strDescription += get_data()->get_leitkarte().get_prj_bauAbschnitt();
			int iLength = strDescription.GetLength();
			int iWhereStrich = strDescription.Find('-');
			
			int iSchnitt = (iLength - 41)/2;
			if(iLength > 40)
			{
				CString strRight = strDescription.Right(iLength - iWhereStrich+1);
				strRight = strRight.Left(strRight.GetLength()-iSchnitt -1);

				CString strLeft = strDescription.Left(iWhereStrich - 1);
				strLeft = strLeft.Left(strLeft.GetLength()- iSchnitt- 1);
				strDescription = strLeft + strRight;
				
			}
				break;
		}
	}

	if( striPPEClassId.IsEmpty() )
	{
		 AfxMessageBox(getStammdaten().getText("IDS_MATERIAL_NUMMER").c_str(), MB_OK);
		 return;
	}

	CString striPPEMaterailnummer = getStammdaten().getText("IDD_SAP_IPPE_AUFTRAG_ANLEGEN").c_str();

	if( IDNO == AfxMessageBox(striPPEMaterailnummer, MB_YESNO | MB_ICONINFORMATION))
	{
		return;
	}
	CString strReturn;

	 CString  soap_endpoi = WaxSys::getSAPSoapServer(); 
	 const TCHAR*  soap_endpoint = soap_endpoi;
		// "http://MINA9004:8000/sap/bc/srt/rfc/sap/zplm_mcc_cad_create_matpl/015/service/binding";  Testserver
		// "http://MINA9017.minimaxsap.int:8101/sap/bc/srt/rfc/sap/zplm_mcc_cad_create_matpl/015/service/binding"; Produktivserver
	if(CSoapProxyWrapper::funcZPlmMccCadCreateMatpl(soap_endpoint, strPSP, striPPEClassId, strDescription, strReturn))
	{
		int iLauf = 0;
		for(iLauf = 0; iLauf < strReturn.GetLength(); iLauf++)
		{
			if(strReturn[iLauf] != '0')
			{
				break;
			}
		}
		strReturn = strReturn.Right(strReturn.GetLength()- iLauf);
		CString strSAPiPPE = getStammdaten().getText("IDD_SAP_IPPE_NUMMER").c_str();
		AfxMessageBox(strSAPiPPE + CString(_T(" ")) + strReturn, MB_OK | MB_ICONINFORMATION);
		GetDlgItem(IDC_SDAUFTRAGSNR)->SetWindowText(strReturn);
	}
	else
	{
		AfxMessageBox(strReturn, MB_OK | MB_ICONERROR);
	}
}

