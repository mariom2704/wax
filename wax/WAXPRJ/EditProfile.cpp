#include "stdafx.h"


#include "EditProfile.h"


BEGIN_MESSAGE_MAP(EditProfile, CPropertyPage)
	ON_BN_CLICKED(IDC_FORM1, onClickedFormx)
	ON_BN_CLICKED(IDC_FORM2, onClickedFormx)
	ON_BN_CLICKED(IDC_FORM3, onClickedFormx)
	ON_BN_CLICKED(IDC_FORM4, onClickedFormx)
	ON_BN_CLICKED(IDC_FORM5, onClickedFormx)
	ON_BN_CLICKED(IDC_FORM6, onClickedFormx)
	ON_BN_CLICKED(IDC_FORM7, onClickedFormx)
	ON_BN_CLICKED(IDC_FORM8, onClickedFormx)
	ON_BN_CLICKED(IDC_FORM9, onClickedFormx)
END_MESSAGE_MAP()


EditProfile::EditProfile(Material * material)
: mat(material)
{
	if (!material->get_mass2())
		material->set_mass2(decodeMass2String("90 1A 90"));
	Material::getMass2(material->get_mass2(), winkelLinks, winkelRechts, ansicht, seitenansicht);

}

EditProfile::~EditProfile()
{
}

BOOL EditProfile::OnApply()
{
	return CPropertyPage::OnApply();
}

BOOL EditProfile::OnSetActive()
{
	return CPropertyPage::OnSetActive();
}

BOOL EditProfile::OnInitDialog()
{
	if (!CDialog::OnInitDialog())
		return false;

	return true;
}

void EditProfile::DoDataExchange(CDataExchange* pDX)
{
	const UINT ansicht_ids[] = {
		IDC_FORM1,
		IDC_FORM2,
		IDC_FORM3,
		IDC_FORM4,
		IDC_FORM5,
		IDC_FORM6,
		IDC_FORM7,
		IDC_FORM8,
		IDC_FORM9,
		0
	};

	const UINT seitenansicht_ids[] = {
		IDC_L_A,
		IDC_L_B,
		IDC_L_C,
		0
	};

	CPropertyPage::DoDataExchange(pDX);
	if (pDX->m_bSaveAndValidate)
	{
		getsetRadio(false, ansicht_ids, ansicht);
		getsetRadio(false, seitenansicht_ids, seitenansicht);
		winkelLinks = GetDlgItemInt(IDC_WINKEL_LINKS);
		winkelRechts = GetDlgItemInt(IDC_WINKEL_RECHTS);
		// Dialogelemente auslesen
		mat->set_mass2(mat->setMass2(winkelLinks, winkelRechts, ansicht, seitenansicht));
#ifdef _DEBUG
		AfxMessageBox(encodeMass2String(mat->get_mass2()));
#endif
	}
	else
	{
		// Dialogelemente setzen
		mat->getMass2(mat->get_mass2(), winkelLinks, winkelRechts, ansicht, seitenansicht);
		getsetRadio(true, ansicht_ids, ansicht);
		getsetRadio(true, seitenansicht_ids, seitenansicht);
		SetDlgItemInt(IDC_WINKEL_LINKS, winkelLinks);
		SetDlgItemInt(IDC_WINKEL_RECHTS, winkelRechts);
		onClickedFormx();
	}
}

int EditProfile::decodeMass2String(const CString& var) 
{
	if (!var.GetLength())
		return 0;
	int winkelLinks = 0, winkelRechts = 0, ansicht = 0;
	char seitenansicht = 0;

	sscanf(var, "%i %i%c %i",&winkelLinks, &ansicht, &seitenansicht, &winkelRechts);  
	ansicht -= 1;
	seitenansicht -= 'A';
	if (seitenansicht > 2)
		seitenansicht = 0;
	return Material::setMass2(winkelLinks, winkelRechts, ansicht, (int)seitenansicht);
}


CString EditProfile::encodeMass2String(int mass2) 
{
	if (!mass2)
		return "";
	int winkelLinks, winkelRechts, ansicht, seitenansicht;
	Material::getMass2(mass2, winkelLinks, winkelRechts, ansicht, seitenansicht);
	
	CString str;
	str.Format("%i %i%c %i", winkelLinks, ansicht+1, seitenansicht+'A', winkelRechts); 

	return str;
}

void EditProfile::getsetRadio(bool set, const UINT * ids, int& var)
{
	CButton *but = 0;
	for (int i = 0; ids[i]; i++)
	{
		but = (CButton*)GetDlgItem(ids[i]);
		if (!but)
			continue; // diesen Radio Button gibts nicht

		if (set)
		{
			if (var == i)
				but->SetCheck(1);
			else
				but->SetCheck(0);
		}
		else
		{
			if (but->GetCheck())
			{
				var = i;
				return;
			}
		}
	}
}

void EditProfile::set_typ(const CString& value)
{
	typ = value; 
	if (typ == "F")
		Construct(IDD_PROFILE_FLACH);
	else if (typ == "U")
		Construct(IDD_PROFILE_U);
	else if (typ == "L")
		Construct(IDD_PROFILE_L);
	else
	{
		CString str; str = getStammdaten().getText("IDS_ERLAUBTER_PROFILTYP").c_str();
		XASSERT(typ == str);
	}
}


void EditProfile::set_typ(int typ)
{
	if (typ == Element::FSTAHL)
	{
		Construct(IDD_PROFILE_FLACH);
		set_typ("F");
	}
	else if (typ == Element::USTAHL)
	{
		Construct(IDD_PROFILE_U);
		set_typ("U");
	}
	else if (typ == Element::LSTAHL)
	{
		Construct(IDD_PROFILE_L);
		set_typ("L");
	}
	else
	{
		CString str; str = getStammdaten().getText("IDS_ERLAUBTER_PROFILTYP").c_str();
		XASSERT(this->typ == str);
	}
}



void EditProfile::onClickedFormx()
{
	// Struktur zur Initialisierung der Winkelangaben. Ist hier true gesetzt, muss der Winkel 90 Grad
	// sein. Dieser Wert wird vorbelegt und das Feld grau geschaltet.
	struct _but
	{
		UINT id;
		bool lwink90;
		bool rwink90;
	} ids[] = 
	{
		IDC_FORM1, true, true,
		IDC_FORM2, true, false,
		IDC_FORM3, true, false, 
		IDC_FORM4, true, false,
		IDC_FORM5, false, true,
		IDC_FORM6, false, false,
		IDC_FORM7, false, false,
		IDC_FORM8, false, false,
		IDC_FORM9, false, false,
		0, false, false
	};

	// Einziger Unterschied zwischen L Stahl und U Stahl ist Nummer vier, wo linkes und rechtes Ende
	// vertauscht sind

	if (typ == "L")
	{
		ids[3].lwink90 = false;
		ids[3].rwink90 = true;
	}

	int i;
	for (i = 0; ids[i].id; i++)
	{
		CButton *but = (CButton*)GetDlgItem(ids[i].id);
		if (but)
		{
			if (but->GetCheck())
				break;
		}
	}
	if (ids[i].id >= 0)
	{
		if (ids[i].lwink90)
		{
			winkelLinks = 90;
			SetDlgItemInt(IDC_WINKEL_LINKS, winkelLinks);
			GetDlgItem(IDC_WINKEL_LINKS)->EnableWindow(false);
		}
		else
			GetDlgItem(IDC_WINKEL_LINKS)->EnableWindow(true);

		if (ids[i].rwink90)
		{
			winkelRechts = 90;
			SetDlgItemInt(IDC_WINKEL_RECHTS, winkelRechts);
			GetDlgItem(IDC_WINKEL_RECHTS)->EnableWindow(false);
		}
		else
			GetDlgItem(IDC_WINKEL_RECHTS)->EnableWindow(true);
	}
}

