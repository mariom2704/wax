#include "stdafx.h"


#include "EditSoko.h"

BEGIN_MESSAGE_MAP(EditSoko, CDialog)
	ON_CBN_SELCHANGE(IDC_TEXT, onSelchangeSoko)
END_MESSAGE_MAP()


EditSoko::EditSoko(CWnd* parent, Preis::Sonderkosten* so)
: WaxPartForm(IDD_SOKO, getStammdaten().getText("IDD_SOKO").c_str(), parent)
{
	preisctrl.set_autodetect_decimal(true);
	soko = so;
	XASSERT(soko);
	if (soko)
	{
		posnr	= soko->get_nr();
		text	= soko->get_text();
		preis	= soko->get_preis();
		isfix   = soko->get_isfix();
		preisctrl.initVal(&preis);
	}
}

BOOL EditSoko::OnInitDialog()
{
	if (!CDialog::OnInitDialog())
		return false;

	CComboBox *bx = (CComboBox*)GetDlgItem(IDC_TEXT);
	for (POSITION pos = getStammdaten().get_sokos().GetHeadPosition(); pos;)
	{
		Soko::data *soko = (Soko::data*)(getStammdaten().get_sokos().GetNext(pos));
		XASSERT(soko);
		if (soko)
		{
			int index = bx->AddString(soko->get_bezeichnung());
			if (index != CB_ERR)
				bx->SetItemDataPtr(index, soko);
		}
	}

	setWindowsTextWithColon(IDC_STATIC_POS, "IDS_POS");
	setWindowsTextWithColon(IDC_STATIC_BEZ, "IDS_BEZEICHNUNG");
	setWindowsTextWithColon(IDC_STATIC_PREIS, "IDS_PREIS");

	SetDlgItemText(IDD_SOKO, getStammdaten().getText("IDS_SOKOBEARBEITEN").c_str());

	return true;
}

void EditSoko::OnOK()
{
	CDialog::OnOK();
	XASSERT(soko);
	if (soko)
	{
		soko->set_nr(posnr);
		soko->set_text(text);
		soko->set_preis(preis);
		soko->set_isfix(isfix);
	}
}

void EditSoko::DoDataExchange(CDataExchange* pDX)
{
	XASSERT(soko);
	if (soko)
	{
		DDX_Text(pDX, IDC_POSNR, posnr);
		DDX_Text(pDX, IDC_TEXT, text);

		DDX_Control(pDX, IDC_PREIS, preisctrl);
		
		preisctrl.doDataExchange(pDX);
	}


	CButton *bt = (CButton*)GetDlgItem(IDC_CHECK_FIX);

	if (pDX->m_bSaveAndValidate) 
	{
		int index = bt->GetCheck();
		isfix = (index == 1) ? true : false;
	}
	else
	{
		bt->SetCheck(isfix);
	}


	CWnd::DoDataExchange(pDX);
}


//Eine Standardsoko wurde ausgewählt
void EditSoko::onSelchangeSoko()
{
	CComboBox *bx = (CComboBox*)GetDlgItem(IDC_TEXT);
	int index = bx->GetCurSel();
	if (index < 0)
		return;
	Soko::data *soko = dynamic_cast<Soko::data*>((CObject*)bx->GetItemDataPtr(index));
	XASSERT(soko);
	if (soko)
	{
		text = soko->get_bezeichnung();
		preis = soko->get_preis();
		UpdateData(false);
	}
}
