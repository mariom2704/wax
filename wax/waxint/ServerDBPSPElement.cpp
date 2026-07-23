#include "stdafx.h"


#include "resource.h"
#include "ServerDBPSPElement.h"
#include <afxcoll.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(ServerDBPSPElement, ServerDBOpen)
	ON_CBN_SELCHANGE(IDC_NL, onSelchangeNl)
	ON_CBN_EDITCHANGE(IDC_NL, onEditchangeNl)
	ON_BN_CLICKED(IDC_SPEZ, onClickedSpez)
	ON_EN_KILLFOCUS(IDC_PSPELEMENT, onFormatPspelement)
	ON_BN_CLICKED(IDC_LETZTE_ZIFFERN_VIER, &ServerDBPSPElement::OnBnClickedLetzteZiffernVier)
	ON_BN_CLICKED(IDC_LETZTE_ZIFFERN_DREI, &ServerDBPSPElement::OnBnClickedLetzteZiffernDrei)
END_MESSAGE_MAP()

ServerDBPSPElement::~ServerDBPSPElement()
{
}

ServerDBPSPElement::ServerDBPSPElement() : checkLetzteZiffern(-1), projektLeitkarteErstellen(false)
{
	m_lpszTemplateName = MAKEINTRESOURCE(IDD_OPEN_DATABASE_PSPELEMENT);
}

BOOL ServerDBPSPElement::DestroyWindow()
{
	return CWnd::DestroyWindow();
}

const CString ServerDBPSPElement::createSQLSelect() const
{
	CString psp(lt.prj_pspElement);
	if (checkLetzteZiffern == -1)
	{
		psp = lt.prj_pspElement.Left(lt.prj_pspElement.GetLength()-3);
		psp += "%";
	}
	
	CString sql;
	sql.Format("pspelement like '%s' AND nl = %i AND spezialloesch = %i", psp, lt.get_prj_nl(), lt.get_prj_produktlinie());
	return sql;
}

const CString ServerDBPSPElement::createSQLSort(CString tablename) const
{
	return "waid";
}

BOOL ServerDBPSPElement::OnInitDialog()
{
	if (!CDialog::OnInitDialog())
		return false;

	GetDlgItem(IDC_LETZTE_ZIFFERN_DREI)->SetWindowText(getStammdaten().getText("IDC_LETZTE_ZIFFERN_EGAL").c_str());
	GetDlgItem(IDC_LETZTE_ZIFFERN_VIER)->SetWindowText(getStammdaten().getText("IDC_LETZTE_ZIFFERN_VIER").c_str());

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
	return true;
}

void ServerDBPSPElement::DoDataExchange(CDataExchange* pDX)
{
	CWnd::DoDataExchange(pDX);

	DDX_Check(pDX, IDC_LETZTE_ZIFFERN_DREI, checkLetzteZiffern);
	DDX_Check(pDX, IDC_PROJEKTLEITKARTE, projektLeitkarteErstellen);
	
	FlexMaskedEdit::DDX_mEdit(pDX, IDC_PSPELEMENT, edpspelement, lt.prj_pspElement);
	FlexMaskedEdit::DDX_mEdit(pDX, IDC_PSPELEMENT, edpspelement, lt.prj_produktlinie, 1);
	
	// Das MaskedEditControl setzt Nullen im String für nicht ausgefüllte Zeichen. Die Eins
	// am Ende wird automatisch hinzugefügt: 0000001 heisst also nichts eingetragen
	if (atoi(lt.prj_pspElement) < 2)
		lt.prj_pspElement.Empty();

	DDX_Control(pDX, IDC_NL, nlkennung);
	DDX_Text(pDX, IDC_NL, lt.prj_nl);
}

bool ServerDBPSPElement::serialize_project(CObList* project)
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

void ServerDBPSPElement::onSelchangeNl()
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

void ServerDBPSPElement::onEditchangeNl()
{
	onSelchangeNl();
}

void ServerDBPSPElement::onClickedSpez()
{
	/*
	Macht nichts anderes ...
	*/
	onFormatPspelement();
}

void ServerDBPSPElement::onFormatPspelement()
{
	/*
	Wenn ein PSP Element eingeben wurde, muss die Formatierung erneuert werden durch führende
	Nullen und 001 am Ende, wenn nicht eingeben.
	*/
	UpdateData();
	edpspelement.actasPSPElement(lt.get_prj_nl(), lt.get_prj_pspElement(), lt.get_prj_produktlinie());
}

void ServerDBPSPElement::OnBnClickedLetzteZiffernVier()
{
	edpspelement.SetDefFormat(_T("[0-9]-[0-9][0-9][0-9][0-9][0-9][0-9]-#2[0-9][0-9][0-9]-#1[0-9][0-9]-#0[0-9][0-9][0-9][0-9]"));
	edpspelement.actasPSPElement(lt.get_prj_nl(), lt.get_prj_pspElement(), lt.get_prj_produktlinie());
}

void ServerDBPSPElement::OnBnClickedLetzteZiffernDrei()
{
	edpspelement.SetDefFormat(_T("[0-9]-[0-9][0-9][0-9][0-9][0-9][0-9]-#2[0-9][0-9][0-9]-#1[0-9][0-9]-#0[0-9][0-9][0-9]"));
	edpspelement.actasPSPElement(lt.get_prj_nl(), lt.get_prj_pspElement(), lt.get_prj_produktlinie());
}
