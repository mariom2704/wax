#include "stdafx.h"


#include "resource.h"
#include "AskDifferentWerkauftraege.h"
#include <afxcoll.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(AskDifferentWerkauftraege, ServerDBOpen)
	ON_CBN_SELCHANGE(IDC_NL, onSelchangeNl)
	ON_CBN_EDITCHANGE(IDC_NL, onEditchangeNl)
	ON_BN_CLICKED(IDC_SPEZ, onClickedSpez)
	ON_EN_KILLFOCUS(IDC_PSPELEMENT, onFormatPspelement)
	ON_BN_CLICKED(IDC_RADIO_PSP_ELEMENT, &AskDifferentWerkauftraege::OnBnClickedRadioPspElement)
	ON_BN_CLICKED(IDC_RADIO_WERKAUFTRAG, &AskDifferentWerkauftraege::OnBnClickedRadioWerkauftrag)
	ON_BN_CLICKED(IDC_RADIO_SD_AUFTRAG, &AskDifferentWerkauftraege::OnBnClickedRadioSDAuftrag)
	ON_BN_CLICKED(IDC_LETZTE_ZIFFERN_DREI, &AskDifferentWerkauftraege::OnBnClickedLetzteZiffernDrei)
	ON_BN_CLICKED(IDC_LETZTE_ZIFFERN_VIER, &AskDifferentWerkauftraege::OnBnClickedLetzteZiffernVier)
END_MESSAGE_MAP()

AskDifferentWerkauftraege::~AskDifferentWerkauftraege()
{
}

AskDifferentWerkauftraege::AskDifferentWerkauftraege() : checkLetzteZiffern(-1), projektLeitkarteErstellen(false),lWanr(0),SDAuftragsNr(0)
{
	m_lpszTemplateName = MAKEINTRESOURCE(IDD_GET_DIFFERENT_AUFTRAEGE);
}

BOOL AskDifferentWerkauftraege::DestroyWindow()
{
	return CWnd::DestroyWindow();
}

const CString AskDifferentWerkauftraege::createSQLSelect() const
{	
	CString sql;
	switch(iSearchOption)
	{
		case 0:
		{
			CString psp(lt.prj_pspElement);
			if (checkLetzteZiffern == -1)
			{
				psp = lt.prj_pspElement.Left(lt.prj_pspElement.GetLength()-3);
				psp += "%";
			}
			sql.Format("pspelement like '%s' AND nl = %i AND spezialloesch = %i AND wanr > 0", psp, lt.get_prj_nl(), lt.get_prj_produktlinie());
		}
		break;

		case 1:
		{
			sql.Format("wanr = %i", lWanr );
		}
		break;

		case 2:
		{
			sql.Format("sdauftragsnr = %i AND wanr > 0", SDAuftragsNr);
		}
		break;

	}
	
	return sql;
}



const CString AskDifferentWerkauftraege::createSQLSort(CString tablename) const
{
	return "waid";
}

BOOL AskDifferentWerkauftraege::OnInitDialog()
{
	iSearchOption = 0;
	if (!CDialog::OnInitDialog())
		return false;

	for (POSITION pos = getStammdaten().get_niederlassungen().GetHeadPosition(); pos != NULL;)
	{
		Niederlassungen::data *nl = (Niederlassungen::data*)(getStammdaten().get_niederlassungen().GetNext(pos));
		CString nr;
		nr.Format("%i",  nl->get_nlnummer());
		int index = nlkennung.AddString(nr);
		nlkennung.SetItemData(index, (DWORD)nl->get_nlnummer());
	}
	/*
	Initialisierung der NL Anzeige im Klartext
	*/
	onSelchangeNl();

	edpspelement.actasPSPElement(lt.get_prj_nl(), lt.get_prj_pspElement(), lt.get_prj_produktlinie());
	// Unsichtbare Vorgabe muss gesetzt sein, damit die Basisklasse nicht aussteigt.
	((CButton*)(GetDlgItem(IDC_SORT_WA)))->SetCheck(1); 
	createProgressWnd();
	OnBnClickedRadioPspElement();
	GetDlgItem(IDC_STATIC_PSP_ELEMENT)->SetWindowText(getStammdaten().getText("IDS_PSP_ELE").c_str());
	GetDlgItem(IDC_STATIC_WERKAUFTRAG)->SetWindowText(getStammdaten().getText("IDS_WERKAUFTRAGS_NR").c_str());
	GetDlgItem(IDC_STATIC_SD_AUFTRAGS_NR)->SetWindowText(getStammdaten().getText("IDS_SDNR").c_str());
	SetWindowText(getStammdaten().getText("ID_OPEN_DATABASE_PSP_WANR_SD_ELEMENT").c_str());
	GetDlgItem(IDC_LETZTE_ZIFFERN_DREI)->SetWindowText(getStammdaten().getText("IDC_LETZTE_ZIFFERN_EGAL").c_str());
	GetDlgItem(IDC_LETZTE_ZIFFERN_VIER)->SetWindowText(getStammdaten().getText("IDC_LETZTE_ZIFFERN_VIER").c_str());
	GetDlgItem(IDC_PROJEKTLEITKARTE)->SetWindowText(getStammdaten().getText("IDC_PROJEKTLEITKARTE").c_str());
	GetDlgItem(IDC_STATIC_NL_KENNNG)->SetWindowText(getStammdaten().getText("IDC_STATIC_NL_KENNNG").c_str());
	GetDlgItem(IDCANCEL)->SetWindowText(getStammdaten().getText("IDCANCEL").c_str());

	return true;
}

void AskDifferentWerkauftraege::DoDataExchange(CDataExchange* pDX)
{
	CWnd::DoDataExchange(pDX);

	DDX_Radio(pDX, IDC_LETZTE_ZIFFERN_DREI, checkLetzteZiffern);
	DDX_Check(pDX, IDC_PROJEKTLEITKARTE, projektLeitkarteErstellen);
	
	DDX_Radio(pDX, IDC_RADIO_PSP_ELEMENT, iSearchOption);

	FlexMaskedEdit::DDX_mEdit(pDX, IDC_PSPELEMENT, edpspelement, lt.prj_pspElement);
	FlexMaskedEdit::DDX_mEdit(pDX, IDC_PSPELEMENT, edpspelement, lt.prj_produktlinie, 1);
	
	// Das MaskedEditControl setzt Nullen im String für nicht ausgefüllte Zeichen. Die Eins
	// am Ende wird automatisch hinzugefügt: 0000001 heisst also nichts eingetragen
	if (atoi(lt.prj_pspElement) < 2)
		lt.prj_pspElement.Empty();

	DDX_Control(pDX, IDC_NL, nlkennung);
	DDX_Text(pDX, IDC_NL, lt.prj_nl);
	DDX_Text(pDX, IDC_EDIT_WERKAUFTRAG, lWanr);
	DDX_Text(pDX, IDC_EDIT_SD_AUFTRAG, SDAuftragsNr);


}

bool AskDifferentWerkauftraege::serialize_project(CObList* project)
{
	if (projektLeitkarteErstellen)
	{
		WaxPart *ersteGliederung = dynamic_cast<WaxPart*>(project->GetHead());
		XASSERT(ersteGliederung);
		
		WaxPart *prjleit = new PrjLeitkarteData();	
		
		WaxPart *pt = dynamic_cast<WaxPart*>(ersteGliederung->get_subobjects().GetHead());
		XASSERT(pt);
		if (!pt)
			return false;


		WaxPart *firstobj = dynamic_cast<WaxPart*>(pt->get_subobjects().GetHead());
		XASSERT(firstobj);
		if (!firstobj)
			return false;
		prjleit->get_werkauftrag()->get_leitkarte().clone(firstobj->get_werkauftrag()->get_leitkarte());
		project->AddHead(prjleit);
	}
	return ServerDBOpen::serialize_project(project);
}

void AskDifferentWerkauftraege::onSelchangeNl()
{
	/*
	NL Kennung in der ComboBox wurde geändert
	*/
	CString combo;
	int index = nlkennung.GetCurSel();

	if (index != -1)
		nlkennung.GetLBText(index, combo);
	else
		nlkennung.GetWindowText(combo);
	
	edpspelement.actasPSPElement(atoi(combo), lt.prj_pspElement, lt.get_prj_produktlinie());
}

void AskDifferentWerkauftraege::onEditchangeNl()
{
	onSelchangeNl();
}

void AskDifferentWerkauftraege::onClickedSpez()
{
	/*
	Macht nichts anderes ...
	*/
	onFormatPspelement();
}

void AskDifferentWerkauftraege::onFormatPspelement()
{
	/*
	Wenn ein PSP Element eingeben wurde, muss die Formatierung erneuert werden durch führende
	Nullen und 001 am Ende, wenn nicht eingeben.
	*/
	UpdateData();
	edpspelement.actasPSPElement(lt.get_prj_nl(), lt.get_prj_pspElement(), lt.get_prj_produktlinie());
}

void AskDifferentWerkauftraege::OnBnClickedRadioPspElement()
{
	EnableRadioPspElement(TRUE);
	EnableRadioWerkauftrag(FALSE);
	EnableRadioSDAuftrag(FALSE);
}

void AskDifferentWerkauftraege::EnableRadioPspElement(BOOL bEnable)
{	
	GetDlgItem(IDC_PSPELEMENT)->EnableWindow(bEnable);
	GetDlgItem(IDC_NL)->EnableWindow(bEnable);
	GetDlgItem(IDC_LETZTE_ZIFFERN_DREI)->EnableWindow(bEnable);
	GetDlgItem(IDC_LETZTE_ZIFFERN_VIER)->EnableWindow(bEnable);
	GetDlgItem(IDC_PROJEKTLEITKARTE)->EnableWindow(bEnable);
}


void AskDifferentWerkauftraege::OnBnClickedRadioWerkauftrag()
{
	EnableRadioPspElement(FALSE);
	EnableRadioWerkauftrag(TRUE);
	EnableRadioSDAuftrag(FALSE);
}


void AskDifferentWerkauftraege::EnableRadioWerkauftrag(BOOL bEnable)
{
	GetDlgItem(IDC_EDIT_WERKAUFTRAG)->EnableWindow(bEnable);
}



void AskDifferentWerkauftraege::OnBnClickedRadioSDAuftrag()
{
	EnableRadioPspElement(FALSE);
	EnableRadioWerkauftrag(FALSE);
	EnableRadioSDAuftrag(TRUE);
}



void AskDifferentWerkauftraege::EnableRadioSDAuftrag(BOOL bEnable)
{
	GetDlgItem(IDC_EDIT_SD_AUFTRAG)->EnableWindow(bEnable);
}



void AskDifferentWerkauftraege::OnOK()
{
	UpdateData();
	if(lWanr == 0 && iSearchOption == 1)
	{
		AfxMessageBox(getStammdaten().getText("IDS_WANR_BIGGER_THAN_NULL").c_str());
		return;
	}
	ServerDBOpen::OnOK();
}

void AskDifferentWerkauftraege::OnBnClickedLetzteZiffernDrei()
{
	edpspelement.SetDefFormat(_T("[0-9]-[0-9][0-9][0-9][0-9][0-9][0-9]-#2[0-9][0-9][0-9]-#1[0-9][0-9]-#0[0-9][0-9][0-9]"));
	edpspelement.actasPSPElement(lt.get_prj_nl(), lt.get_prj_pspElement(), lt.get_prj_produktlinie());
}

void AskDifferentWerkauftraege::OnBnClickedLetzteZiffernVier()
{
	edpspelement.SetDefFormat(_T("[0-9]-[0-9][0-9][0-9][0-9][0-9][0-9]-#2[0-9][0-9][0-9]-#1[0-9][0-9]-#0[0-9][0-9][0-9][0-9]"));
	edpspelement.actasPSPElement(lt.get_prj_nl(), lt.get_prj_pspElement(), lt.get_prj_produktlinie());
}
