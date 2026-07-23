#include "stdafx.h"


#include "resource.h"
#include "EditFormstueckeUndFittinge.h"

BEGIN_MESSAGE_MAP(EditFormstueckeUndFittinge, CDialog)
END_MESSAGE_MAP()

EditFormstueckeUndFittinge::~EditFormstueckeUndFittinge()
{
}

EditFormstueckeUndFittinge::EditFormstueckeUndFittinge(CWnd* parent, Material* ma)
: WaxPartForm(IDD_FUF, getStammdaten().getText("IDD_FUF").c_str(), parent)
{
	gewctrl.set_autodetect_decimal(true);
	prectrl.set_autodetect_decimal(true);
	pulprectrl.set_autodetect_decimal(true);
	mat = ma;
	XASSERT(mat);
	if (mat)
	{
		anzahl	= mat->get_anzahl();
		bezeichnung	= mat->get_bezeichnung();
		artnr	= mat->get_artikelnr();
		gewicht = mat->get_gewicht();
		gewctrl.initVal(&gewicht);
		preis   = mat->get_festPreis();
		prectrl.initVal(&preis); 
		pulverpreis   = mat->get_pulverpreis();
		pulprectrl.initVal(&pulverpreis); 
	}
}

BOOL EditFormstueckeUndFittinge::OnInitDialog()
{
	if (!CDialog::OnInitDialog())
		return false;

	CComboBox *bx = (CComboBox*)GetDlgItem(IDC_BEZ);
	for (POSITION pos = getStammdaten().get_formstueckefittinge().GetHeadPosition(); pos;)
	{
		FormstueckeUndFittinge::data *f = (FormstueckeUndFittinge::data *)getStammdaten().get_formstueckefittinge().GetNext(pos);
		XASSERT(f);
		if (f)
		{
			int index = bx->AddString(f->get_kurztext());
			if (index != CB_ERR)
				bx->SetItemDataPtr(index, f);
		}
	}
	if (!WaxSys::isAV())
	{
		GetDlgItem(IDC_PREIS)->EnableWindow(FALSE);
		GetDlgItem(IDC_PULVERPREIS)->EnableWindow(FALSE);
	}


	setWindowsTextWithColon(IDC_STATIC_ANZAHL, "IDS_ANZAHL");
	setWindowsTextWithColon(IDC_STATIC_BEZ, "IDS_BEZEICHNUNG");
	setWindowsTextWithColon(IDC_STATIC_ARTIKEL, "IDS_ARTIKELNR");
	setWindowsTextWithColon(IDC_STATIC_GEWICHT, "IDS_GEWICHT");
	setWindowsTextWithColon(IDC_STATIC_PREIS, "IDS_PREIS");
	setWindowsTextWithColon(IDC_STATIC_PULVERPREIS, "IDS_PULVERPREIS");

	return true;
}

void EditFormstueckeUndFittinge::OnOK()
{
	CDialog::OnOK();
	XASSERT(mat);
	if (mat)
	{
		mat->set_anzahl(anzahl);
		mat->set_bezeichnung(bezeichnung);
		mat->set_artikelnr(artnr);
		mat->set_gewicht(gewicht);
		mat->set_festPreis(preis);
		mat->set_pulverpreis(pulverpreis);
	}
}

void EditFormstueckeUndFittinge::DoDataExchange(CDataExchange* pDX)
{
	XASSERT(mat);
	if (mat)
	{
		DDX_Text(pDX, IDC_ANZ, anzahl);
		DDX_Text(pDX, IDC_BEZ, bezeichnung);
		DDX_Text(pDX, IDC_ART, artnr);

		DDX_Control(pDX, IDC_GEW, gewctrl);
		DDX_Control(pDX, IDC_PREIS, prectrl);
		DDX_Control(pDX, IDC_PULVERPREIS, pulprectrl);
		
		gewctrl.doDataExchange(pDX);
		prectrl.doDataExchange(pDX);
		pulprectrl.doDataExchange(pDX);
	}
	CWnd::DoDataExchange(pDX);
}
