#include "stdafx.h"


#include "resource.h"
#include "PumpenprobierleitungEingabe.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(PumpenprobierleitungEingabe, WaxPartForm)
	ON_EN_KILLFOCUS(IDC_L1, doUpdate)
	ON_EN_KILLFOCUS(IDC_L2, doUpdate)
	ON_EN_KILLFOCUS(IDC_L3, doUpdate)
	ON_EN_KILLFOCUS(IDC_POSNR, doUpdate)
	ON_EN_KILLFOCUS(IDC_ANZAHL, doUpdate)
	ON_BN_CLICKED(IDC_FLACHFLANSCH_A, doUpdate)
	ON_BN_CLICKED(IDC_NUTSTUTZEN_A, doUpdate)
	ON_BN_CLICKED(IDC_GLATT_A, doUpdate)
	ON_BN_CLICKED(IDC_FLACHFLANSCH_B, doUpdate)
	ON_BN_CLICKED(IDC_NUTSTUTZEN_B, doUpdate)
	ON_BN_CLICKED(IDC_GLATT_B, doUpdate)
	ON_CBN_KILLFOCUS(IDC_DN1, onKillfocusDn)
	ON_CBN_KILLFOCUS(IDC_DN2, onKillfocusDn)
END_MESSAGE_MAP()

IMPLEMENT_DYNCREATE(PumpenprobierleitungEingabe, WaxPartForm)

#define pp ((Pumpenprobierleitung*)get_data()->get_werkauftrag())

PumpenprobierleitungEingabe::PumpenprobierleitungEingabe()
: WaxPartForm(IDD_PUMPENPROBIERLEITUNG)
{
	radiobuttons.set_dlg(this);
	int i = 0;
	radiobuttons.set_dlgid(i++, IDC_FLACHFLANSCH_A);
	radiobuttons.set_dlgid(i++, IDC_NUTSTUTZEN_A);
	radiobuttons.set_dlgid(i++, IDC_GLATT_A);
	radiobuttons.set_dlgid(i++, IDC_FLACHFLANSCH_B);
	radiobuttons.set_dlgid(i++, IDC_NUTSTUTZEN_B);
	radiobuttons.set_dlgid(i++, IDC_GLATT_B);
}

PumpenprobierleitungEingabe::~PumpenprobierleitungEingabe()
{
	
}

//Plausibilitätscheck, wird aufgerufen, bevor UpdateData(TRUE) aufgerufen 
//wird.
bool PumpenprobierleitungEingabe::check_plausis()
{	
	if (!WaxPartForm::check_plausis())
		return false;

	CString dn1, dn2;
	GetDlgItem(IDC_DN1)->GetWindowText(dn1);
	GetDlgItem(IDC_DN2)->GetWindowText(dn2);
	int idn1 = atoi(dn1), idn2 = atoi(dn2);
		
	int lx[] = { pp->get_laenge1(), pp->get_laenge2(),pp->get_laenge3() };
	CString minlx[] = { pp->get_minval(Pumpenprobierleitung::L1, idn1, idn2), pp->get_minval(Pumpenprobierleitung::L2, idn1, idn2), pp->get_minval(Pumpenprobierleitung::L3, idn1, idn2) };
	int idx[] = { IDC_L1, IDC_L2, IDC_L3, 0 };

	CString msg;
	int i = -1;
	CString minl1 = pp->get_minval(Pumpenprobierleitung::L1, idn1, idn2);

	
	// Rohr und Elemente checken
	CString err = pp->checkPlausis();
	if (err.GetLength() > 0)
	{
		disableCheck();
		AfxMessageBox(err, MB_OK | MB_ICONINFORMATION);

		if (atoi(minl1))
		{
			// Minimallängen abprüfen
			for (i = 0; i < 3; i++)
			{
				if (lx[i] < atoi(minlx[i]))
				{
					break;
				}
			}
		}
		if (i >= 0 && idx[i] != 0)
			GetDlgItem(idx[i])->SetFocus();

		return false;
	}
	return true;
}

void PumpenprobierleitungEingabe::DoDataExchange(CDataExchange* pDX)
{
	WaxPartForm::DoDataExchange(pDX);
	
	ddx_posnr(pDX, IDC_POSNR);
	ddx_anzahl(pDX, IDC_ANZAHL);
	
	// DN1
	int dn1 = pp->get_dn1();
	DDX_Text(pDX, IDC_DN1, dn1);
	pp->set_dn1(dn1);

	// DN2
	int dn2 = pp->get_dn2();
	DDX_Text(pDX, IDC_DN2, dn2);
	pp->set_dn2(dn2);

		// L1
	int l1 = pp->get_laenge1();
	DDX_Text(pDX, IDC_L1, l1);
	pp->set_laenge1(l1);

	// L2
	int l2 = pp->get_laenge2();
	DDX_Text(pDX, IDC_L2, l2);
	pp->set_laenge2(l2);

	// L3
	int l3 = pp->get_laenge3();
	DDX_Text(pDX, IDC_L3, l3);
	pp->set_laenge3(l3);

	// Anschlusstypen Abgang A und B
	if (pDX->m_bSaveAndValidate)
	{
		pp->set_anschlussA(radiobuttons.getRadioButton(true)); 
		pp->set_anschlussB(radiobuttons.getRadioButton(false));
		CString laengenfeld;
		laengenfeld.Format("%i %i %i", pp->get_laenge1(), pp->get_laenge2(), pp->get_laenge3());
		pp->set_rohrDefLaengen(laengenfeld);
	}
	else
	{
		radiobuttons.setRadioButtons(pp->get_anschlussA(), pp->get_anschlussB());		
	}

	// Readonly Minimalwerte setzen
	CString tmp = pp->get_minval(Pumpenprobierleitung::L4); 
	DDX_Text(pDX, IDC_L4, tmp);
	tmp = pp->get_minval(Pumpenprobierleitung::L5); 
	DDX_Text(pDX, IDC_L5, tmp);
	tmp = pp->get_minval(Pumpenprobierleitung::D); 
	DDX_Text(pDX, IDC_D, tmp);

    int ges = l1 + l2 + l3 + atoi(pp->get_minval(Pumpenprobierleitung::MESSBLSCHIEBER)); 
	tmp.Format("%i", ges);
	GetDlgItem(IDC_LGESAMT)->SetWindowText(tmp);
	//DDX_Text(pDX, IDC_LGESAMT, tmp);
}

INT_PTR PumpenprobierleitungEingabe::OnToolHitTest(CPoint point, TOOLINFO* pTI) const 
{
	const Pumpenprobierleitung test;

	CString dn1, dn2;
	GetDlgItem(IDC_DN1)->GetWindowText(dn1);
	GetDlgItem(IDC_DN2)->GetWindowText(dn2);

	int res = CDialog::OnToolHitTest( point, pTI );
	char* msg = new char[2049];
	if (pTI && res != -1)
	{
		pTI->lpszText = (LPSTR)msg;
		pTI->uFlags |= TTF_ALWAYSTIP;
		CString minval;
		switch (res)
		{
		case IDC_L1:
			minval = test.get_minval(Pumpenprobierleitung::L1, atoi(dn1), atoi(dn2));
			break;
		case IDC_L2:
			minval = test.get_minval(Pumpenprobierleitung::L2, atoi(dn1), atoi(dn2));
			break;
		case IDC_L3:
			minval = test.get_minval(Pumpenprobierleitung::L3, atoi(dn1), atoi(dn2));
			break;
		}
		if (atoi(minval))
		{
			CString out;
			out.Format("min %s mm", minval);
			strcpy(pTI->lpszText, out.Left(2048));
			return res;
		}
	}
	int ret = WaxPartForm::OnToolHitTest(point, pTI);
	delete msg;
	return ret;
}

void PumpenprobierleitungEingabe::onKillfocusDn()
{
	UpdateData();
	// L1-L3 Minimalwerte setzen, wenn gültige Werte und Dialogwerte kleiner als Minwerte
	CString txt;
	GetDlgItemText(IDC_L1, txt);
	CString tmp = pp->get_minval(Pumpenprobierleitung::L1); 
	if (tmp != "--" && atoi(txt) < atoi(tmp))
		SetDlgItemText(IDC_L1, tmp);
	GetDlgItemText(IDC_L2, txt);
	tmp = pp->get_minval(Pumpenprobierleitung::L2); 
	if (tmp != "--" && atoi(txt) < atoi(tmp))
		SetDlgItemText(IDC_L2, tmp);
	GetDlgItemText(IDC_L3, txt);
	tmp = pp->get_minval(Pumpenprobierleitung::L3); 
	if (tmp != "--" && atoi(txt) < atoi(tmp))
		SetDlgItemText(IDC_L3, tmp);
	UpdateData();
}

BOOL PumpenprobierleitungEingabe::OnInitDialog()
{
	/*
	Dialoginitialisierung 
	Hier werden den Comboboxtexten ein Integerwert zugewiesen, 
	der einer ElementID entspricht. 
	*/
	if (!WaxPartForm::OnInitDialog())
		return false;

	setWindowsTextWithColon(IDC_STATIC_ANZAHL, "IDS_ANZAHL");
	setWindowsTextWithColon(IDC_STATIC_POS, "IDS_POS_KURZ");
	setWindowsText(IDC_STATIC_ANA, "IDC_STATIC_ANA");
	setWindowsText(IDC_STATIC_ANB, "IDC_STATIC_ANB");
	setWindowsText(IDC_FLACHFLANSCH_A, "IDS_FLACHFLANSCH");
	setWindowsText(IDC_FLACHFLANSCH_B, "IDS_FLACHFLANSCH");
	setWindowsText(IDC_NUTSTUTZEN_A, "IDS_NUTSTUTZEN");
	setWindowsText(IDC_NUTSTUTZEN_B, "IDS_NUTSTUTZEN");
	setWindowsText(IDC_GLATT_A, "IDS_GLATT");
	setWindowsText(IDC_GLATT_B, "IDS_GLATT");
	setWindowsText(IDC_STATIC_LGES, "IDC_STATIC_LGES");



	return true;

}