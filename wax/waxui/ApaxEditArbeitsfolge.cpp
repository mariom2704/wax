#include "stdafx.h"

#include <mxcontrols/listctrl.h>
#include <mxutil/ddx_extension.h>

#include "ApaxEditArbeitsfolge.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(ApaxEditArbeitsfolge, EditArbeitsfolge)
	ON_CBN_SELCHANGE(IDC_ARBPL, onSelchangeArbpl)
	ON_BN_CLICKED(IDC_ZEITBERECHNEN, onClickedZeitberechnen)
	ON_CBN_SELCHANGE(IDC_ELEMENT, onSelchangeElement)
	ON_EN_KILLFOCUS(IDC_EINZELZEIT, onKillfocusEinzelzeit)
END_MESSAGE_MAP()

ApaxEditArbeitsfolge::ApaxEditArbeitsfolge(Arbeitsfolge &arbeitsfolge, CWnd *parent)
: EditArbeitsfolge(parent, &arbeitsfolge)
{
	m_lpszTemplateName = MAKEINTRESOURCE(IDD_APAX_ARBEITSFOLGE);

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
	isTextgeaendertID = IDC_AENDERN;
	preis1ID = IDC_PREIS1;

	itemIds.set(IDC_MENGE, IDC_ELEMENT, IDC_DN, IDC_LAENGE, IDC_DNVL);
}

ApaxEditArbeitsfolge::~ApaxEditArbeitsfolge()
{
}

BOOL ApaxEditArbeitsfolge::OnInitDialog()
{
	// Wenn die Afonummer in den Stammdaten steht, muss das Eingabefeld disabled werden.
	// Wenn sie nicht zu finden ist (selbst eingegebene Afo) müssen die Eingabezeilen 
	// disabled werden

	addElemente();
	if (!EditArbeitsfolge::OnInitDialog())
		return false;
	
	enableItems();
	initEleDn(paf->get_xdat().eleid);

	loadTexte();
	GetDlgItem(IDC_PREIS1)->EnableWindow(TRUE);

	
	return true;
}


void ApaxEditArbeitsfolge::loadTexte()
{
	setWindowsTextWithColon(IDC_STATIC_POS, "IDS_POS");
	setWindowsTextWithColon(IDC_STATIC_ANZAHL, "IDS_ANZAHL");
	setWindowsTextWithColon(IDC_STATIC_BEZ, "IDS_BEZEICHNUNG");
	setWindowsTextWithColon(IDC_STATIC_ZEIT, "IDS_ZEIT");
	setWindowsText(IDC_SAP, "IDS_SAP");
	setWindowsTextWithColon(IDC_STATIC_CODE, "IDS_CODE");
	setWindowsTextWithColon(IDC_STATIC_EINZELZEIT, "IDS_EINZELZEIT");
	setWindowsTextWithColon(IDC_STATIC_RUESTZEIT, "IDS_RUESTZEIT");
	setWindowsText(IDC_STATIC_ARBEITSPLATZ, "IDS_ARBEITSPLATZ");
	setWindowsTextWithColon(IDC_STATIC_PREIS_H, "IDS_PREIS_PRO_STUNDE");
	setWindowsTextWithColon(IDC_STATIC_PREIS, "IDS_PREIS");

	setWindowsTextWithColon(IDC_STATIC_ZEITBER, "IDC_STATIC_ZEITBER");
	setWindowsTextWithColon(IDC_STATIC_MENGE_ST, "IDC_STATIC_MENGE_ST");
	setWindowsTextWithColon(IDC_STATIC_ROHRDN, "IDC_STATIC_ROHRDN");
	setWindowsTextWithColon(IDC_STATIC_LAENGE, "IDS_LAENGE");
	setWindowsTextWithColon(IDC_STATIC_EL, "IDS_ELEMENT");
	setWindowsTextWithColon(IDC_STATIC_ELDN, "IDC_STATIC_ELDN");

	setWindowsTextWithColon(IDC_ZEITBERECHNEN, "IDC_ZEITBERECHNEN");
}

void ApaxEditArbeitsfolge::OnOK()
{
	EditArbeitsfolge::OnOK();	
}

void ApaxEditArbeitsfolge::DoDataExchange(CDataExchange* pDX)
{
	EditArbeitsfolge::DoDataExchange(pDX);

	DDX_Text(pDX, itemIds.menge, paf->get_xdat().anzahl);
	DDX_Text(pDX, itemIds.laenge, paf->get_xdat().laenge);
	DDX_Text(pDX, itemIds.dnvl, paf->get_xdat().dnvl);
	
	ddx_cbdata(pDX, itemIds.element, paf->get_xdat().eleid);
	ddx_cbdata(pDX, itemIds.dn, paf->get_xdat().dn);




}



void ApaxEditArbeitsfolge::ItemID::set(UINT menge, UINT element, UINT dn, UINT laenge, UINT dnvl)
{
	this->element = element;
	this->menge = menge;
	this->dn = dn;
	this->laenge = laenge;
	this->dnvl = dnvl;
}


//Fügt anhand der Afo ID die dazugehörigen Elemente in die Elemente Combobox
void ApaxEditArbeitsfolge::addElemente()
{
	// Liste mit IDs der Elemente erstellen
	CDWordArray eleids;
	for (POSITION pos = getStammdaten().get_afozeiten().GetHeadPosition(); pos; )
	{
		AfoZeiten::data *dat = (AfoZeiten::data *)getStammdaten().get_afozeiten().GetNext(pos);
		if (dat->get_afoid() == paf->get_id())
		{
			if (dat->get_elemid())
			{
				eleids.Add(dat->get_elemid());
			}
			else
			{
				// Eleid 0 heisst alle Rohre
				const int rohrids[] = {1, 31, 32, 33, 34, 37, 38, 0 };
				for (int i = 0; rohrids[i]; i++)
					eleids.Add(rohrids[i]);
			}			
		}
	}

	if (!eleids.GetSize())
		return;

	// Liste mit IDs der Elemente in Combobox eintragen
	CComboBox *bx = (CComboBox*)GetDlgItem(IDC_ELEMENT);
	for (int i = 0; i < eleids.GetSize(); i++)
	{
		for (POSITION pele = getStammdaten().get_bauelementgruppen().GetHeadPosition(); pele; )
		{
			BauelementGruppen::data *dat = (BauelementGruppen::data *)getStammdaten().get_bauelementgruppen().GetNext(pele);
			
			if (dat->get_elem_ID() == (int)eleids.GetAt(i))
			{
				if (bx->FindStringExact(-1, dat->get_bezeichnung()) == CB_ERR)
				{
					int index = bx->AddString(dat->get_bezeichnung());
					XASSERT(index >= 0);
					if (index >= 0)
					{
						bx->SetItemData(index, dat->get_elem_ID());
						bx->SetCurSel(index);
						if (!paf->get_xdat().eleid) // initialisieren, damit das DN Feld gesetzt werden kann
							paf->get_xdat().eleid = dat->get_elem_ID();

					}
				}
			}
		}
	}
}

//Prüft anhand der Afo ID welche Eingabefelder möglich sind
void ApaxEditArbeitsfolge::enableItems()
{
	bool found = false;
	bool enable_laenge = false;
	bool enable_dnvl = false;

	for (POSITION pos = getStammdaten().get_afozeiten().GetHeadPosition(); pos; )
	{
		AfoZeiten::data *dat = (AfoZeiten::data *)getStammdaten().get_afozeiten().GetNext(pos);
		if (dat->get_afoid() == paf->get_id())
		{
			found = true;
			if (dat->get_laenge_von() > 0 || dat->get_laenge_bis() > 0)
				enable_laenge = true;
			if (dat->get_dnvl_von() > 0 || dat->get_dnvl_bis() > 0)
				enable_dnvl = true;
		}
	}
	
	/*
	if (found)
	{
		GetDlgItem(IDC_ARBPL)->EnableWindow(FALSE);
	}*/
	GetDlgItem(itemIds.dnvl)->EnableWindow(enable_dnvl);
	GetDlgItem(itemIds.element)->EnableWindow(found);
	GetDlgItem(itemIds.laenge)->EnableWindow(enable_laenge);
	GetDlgItem(IDC_ZEITBERECHNEN)->EnableWindow(found);
}



//Zeitberechnung durchführen
void ApaxEditArbeitsfolge::onClickedZeitberechnen()
{
	UpdateData();

	CComboBox *bx_dnvl = (CComboBox*)GetDlgItem(IDC_DNVL);
	CComboBox *bx_laenge = (CComboBox*)GetDlgItem(IDC_LAENGE);
	CComboBox *bx_ele = (CComboBox*)GetDlgItem(IDC_ELEMENT);
	CComboBox *bx_eledn = (CComboBox*)GetDlgItem(IDC_DN);

	CString notmatched; 
	int matching = 0;
	CString str1, str2, str3, str4; 
	str1 = getStammdaten().getText("IDS_ELEMENT").c_str();
	str2 = getStammdaten().getText("IDS_ROHR_DN").c_str();
	str3 = getStammdaten().getText("IDS_LAENGE").c_str();
	str4 = getStammdaten().getText("IDS_ELEMENT_DN").c_str();
	CString missing[] = {str1, str2, str3, str4};

	for (POSITION pos = getStammdaten().get_afozeiten().GetHeadPosition(); pos; )
	{
		AfoZeiten::data *dat = (AfoZeiten::data *)getStammdaten().get_afozeiten().GetNext(pos);
		if (dat->get_afoid() == paf->get_id())
		{
			notmatched.Empty();
			int matchcode = 0;
			if (bx_ele->IsWindowEnabled())
			{
				if (dat->get_elemid() != 0 && dat->get_elemid() != paf->get_xdat().eleid)
				{
					if (matching < matchcode)
						matching = matchcode;
					continue;
				}
			}
			matchcode |= 0x01;
			if (bx_dnvl->IsWindowEnabled())
			{
				if (dat->get_dnvl_von() > paf->get_xdat().dnvl || (dat->get_dnvl_bis() && dat->get_dnvl_bis() < paf->get_xdat().dnvl))
				{
					continue;
				}
			}
			matchcode |= 2;

			if (bx_laenge->IsWindowEnabled())
			{
				if (dat->get_laenge_von() > paf->get_xdat().laenge || (dat->get_laenge_bis() && dat->get_laenge_bis() < paf->get_xdat().laenge))
				{
					if (matching < matchcode)
						matching = matchcode;
					continue;
				}
			}
				
			matchcode |= 4;	
			if (bx_eledn->IsWindowEnabled())
			{
				if (dat->get_dn() != paf->get_xdat().dn)
				{
					if (matching < matchcode)
						matching = matchcode;
					continue;
				}
			}
			paf->set_zeit((double)paf->get_xdat().anzahl * dat->get_zeit());
			paf->set_anz(1);
			
			Arbeitsplaetze arb;
			paf->set_ruestzeit(arb.getRuestzeit(paf->get_arbpl()));

			UpdateData(false);
			return;
		}
	}
	CString msg, str;
	int i;
	for (i = 0; i < 4; i++)
	{
		int mask = 0x01;
		if (i)
			mask = mask << i;
		if (!(matching & mask))
			break;
	}
	str = getStammdaten().getText("IDS_ERROR_ZEIT_BERECHNEN").c_str();
	msg.Format(str, missing[i]);
	AfxMessageBox(msg , MB_ICONERROR);
}



//Sucht aus der Tabelle  Arbeitsfolgen_zeiten die passenden DNs für das 
//übergebene Element heraus und schreibt sie in die Auswahlbox.
void ApaxEditArbeitsfolge::initEleDn(int eleid)
{
	CComboBox *bx = (CComboBox*)GetDlgItem(IDC_DN);
	bx->ResetContent();
	bx->EnableWindow(false);
	Element ele;
	ele.set_id(eleid);
	if (ele.isEleRohr() || eleid == ROHR)
	{ // 
		const char* dn[] = {"12","15","20","25","32","40","50","65","80","100","125","150","200","250","300", NULL };
		bx->ResetContent();
		bx->EnableWindow();
		for (int i = 0; dn[i]; i++)
		{
			int index = bx->AddString(dn[i]);
			bx->SetItemData(index, atoi(dn[i]));
		}
	}
	else
	{
		for (POSITION pos = getStammdaten().get_afozeiten().GetHeadPosition(); pos; )
		{
			AfoZeiten::data *dat = (AfoZeiten::data *)getStammdaten().get_afozeiten().GetNext(pos);
			
			if (dat->get_afoid() == paf->get_id() && dat->get_elemid() == eleid)
			{
				if (!dat->get_dn())
					continue;
				CString dn;
				dn.Format("%i", dat->get_dn());
				int index = bx->AddString(dn);
				bx->SetItemData(index, dat->get_dn());
				bx->EnableWindow();
			}
		}
	}
	CString tmp;
	tmp.Format("%i", paf->get_xdat().dn);
	bx->SelectString(-1, tmp);
}



//Indicates a new combo box list item is selected
void ApaxEditArbeitsfolge::onSelchangeElement()
{
	CComboBox *bx = (CComboBox*)GetDlgItem(IDC_ELEMENT);
	int curid = bx->GetCurSel();
	if (curid != CB_ERR)
		initEleDn(bx->GetItemData(curid));
}

void ApaxEditArbeitsfolge::onKillfocusEinzelzeit()
{
	UpdateData();
	UpdateData(false);
}
