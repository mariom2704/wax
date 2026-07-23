#include "stdafx.h"


#include "EditMaterial.h"
#include "EditProfile.h"

BEGIN_MESSAGE_MAP(EditMaterial, CPropertyPage)
	ON_BN_CLICKED(IDC_STANDARD, onClickedStandard)
	ON_EN_CHANGE(IDC_ANZAHL, onChangeAnzahl)
	ON_EN_CHANGE(IDC_GEWICHT, onChangeAnzahl)
	ON_EN_CHANGE(IDC_PREIS, onChangeAnzahl)
	ON_EN_CHANGE(IDC_PULVERPREIS, onChangeAnzahl)
	ON_EN_CHANGE(IDC_LAENGE, onChangeLaenge)
END_MESSAGE_MAP()

EditMaterial::EditMaterial(Material *mat  )
: CPropertyPage(IDD_EDITMATERIAL)
{
	/*
	Flags 0x80 und 0x08 werden nicht benutzt, deshalb steht an den korrespondierenden Stellen
	in profileSymbole ein Leerzeichen.
	*/
	if (mat)
	{
		material = mat;
		id			= mat->get_id(); 
		posnr		= mat->get_posnr();
		anzahl		= mat->get_anzahl();
		laenge		= mat->get_mass1();
		artnr		= mat->get_artikelnr();
		bezeichnung = mat->get_bezeichnung();
		preis		= mat->get_sapPreis();
		pulverpreis = mat->get_pulverpreis();
		gewicht		= mat->get_gewicht();
		gesamtpreis = 0.0;
		gesamtpulverpreis = 0.0;
		gesamtgewicht = 0.0;
		if (anzahl)
		{
			gesamtpreis = kann_laenge() ? preis / 1000.0 * laenge * anzahl : preis * anzahl;
			gesamtpulverpreis = pulverpreis * anzahl;
			gesamtgewicht = kann_laenge() ? gewicht / 1000.0 * laenge * anzahl : gesamtgewicht * anzahl;
		}	
		ed_gesamtgewicht.set_autodetect_decimal(true);
		ed_preis.initVal(&preis);
		ed_pulverpreis.initVal(&pulverpreis);
		ed_gewicht.initVal(&gewicht);
		ed_gesamtpreis.initVal(&gesamtpreis);
		ed_gesamtpulverpreis.initVal(&gesamtpulverpreis);
		ed_gesamtgewicht.initVal(&gesamtgewicht);
		einkauf = mat->get_einkauf();
		halbzeug = mat->get_halbzeug();
	}
}

EditMaterial::~EditMaterial()
{
}

BOOL EditMaterial::OnInitDialog()
{
	if (!CDialog::OnInitDialog())
		return false;
	onChangeAnzahl();


	setWindowsText(IDC_STANDARD, "IDC_STANDARD");

	setWindowsTextWithColon(IDC_STATIC_TXT, "IDS_TEXT");
	setWindowsTextWithColon(IDC_STATIC_ART, "IDS_ARTIKELNR_KURZ");
	setWindowsTextWithColon(IDC_STATIC_Anzahl, "IDS_ANZAHL");
	setWindowsTextWithColon(IDC_STATIC_GEWICHT, "IDS_GEWICHT");
	setWindowsTextWithColon(IDC_STATIC_PREIS, "IDS_PREIS");
	setWindowsTextWithColon(IDC_STATIC_PULVERPREIS, "IDS_PULVERPREIS");
	setWindowsTextWithColon(IDC_STATIC_GESPULVERPREIS, "IDS_GES_PULVERPREIS");
	setWindowsTextWithColon(IDC_STATIC_GESPREIS, "IDS_GESAMTPREIS");
	setWindowsTextWithColon(IDC_STATIC_GESGEWICHT, "IDS_GESAMTGEWICHT");
	setWindowsTextWithColon(IDC_STATIC_LAENGE, "IDS_LAENGE");
	setWindowsTextWithColon(IDC_STATIC_ID, "IDS_ID2");
	setWindowsText(IDC_EINKAUF, "IDS_EINKAUF");
	setWindowsText(IDC_HALBZEUG, "IDS_HALBZEUG");



	return true;
}

void EditMaterial::OnOK()
{
	CDialog::OnOK();
	
	material->set_id(id);
	material->set_posnr(posnr);
	material->set_anzahl(anzahl);
	material->set_mass1(laenge); 
	material->set_artikelnr(artnr);
	bezeichnung.TrimRight();
	bezeichnung.TrimLeft();
	material->set_bezeichnung(bezeichnung); 
	material->set_sapPreis(preis); 
	material->set_pulverpreis(pulverpreis); 
	material->set_gewicht(gewicht); 
	material->set_einkauf(einkauf ? true : false);
	material->set_halbzeug(halbzeug ? true : false);
}

void EditMaterial::DoDataExchange(CDataExchange* pDX)
{
	CWnd::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PREIS, ed_preis);
	DDX_Control(pDX, IDC_GESAMTPREIS, ed_gesamtpreis);
	DDX_Control(pDX, IDC_PULVERPREIS, ed_pulverpreis);
	DDX_Control(pDX, IDC_GESAMTPULVERPREIS, ed_gesamtpulverpreis);
	DDX_Control(pDX, IDC_GEWICHT, ed_gewicht);
	DDX_Control(pDX, IDC_GESAMTGEWICHT, ed_gesamtgewicht);
	DDX_Check(pDX, IDC_HALBZEUG, halbzeug);
	DDX_Check(pDX, IDC_EINKAUF, einkauf);

	ed_gesamtgewicht.doDataExchange(pDX);
	ed_gewicht.doDataExchange(pDX);
	ed_preis.doDataExchange(pDX);
	ed_gesamtpreis.doDataExchange(pDX);
	ed_pulverpreis.doDataExchange(pDX);
	ed_gesamtpulverpreis.doDataExchange(pDX);

	DDX_Text(pDX, IDC_ID, id);
	DDX_Text(pDX, IDC_POSNR, posnr);
	DDX_Text(pDX, IDC_ANZAHL, anzahl);
	DDX_Text(pDX, IDC_ARTNR, artnr);
	DDX_Text(pDX, IDC_BEZEICHNUNG, bezeichnung);
	DDX_Text(pDX, IDC_LAENGE, laenge);
	
	GetDlgItem(IDC_LAENGE)->EnableWindow(kann_laenge());
}

//Indicates the user clicked a button
void EditMaterial::onClickedStandard()
{
	CString str; str = getStammdaten().getText("IDS_DATEN_AUS_STAMMDATEN").c_str();
	if (AfxMessageBox(str,  MB_YESNO | MB_ICONQUESTION) == IDNO)
		return;

	bool found = false;
	UpdateData();
	for (POSITION pos = getStammdaten().get_bauelemente().GetHeadPosition(); pos;)
	{
		Bauelemente::data *dat = (Bauelemente::data *)getStammdaten().get_bauelemente().GetNext(pos);
		if (dat->get_artikelnr() == artnr)
		{
			if(dat->get_elemid() > 200)// 
				continue;

			preis = dat->get_sapPreis();
			gewicht = dat->get_gewicht();
			halbzeug = dat->get_halbzeug();
			einkauf = dat->get_einkauf();
			material->set_mass2(0);
				
			BauelementGruppen bg;
			bezeichnung = bg.getBezeichnung(dat->get_elemid());
			found = true;
		}
	}
	if (!found)
	{
		for (POSITION pos = getStammdaten().get_formstueckefittinge().GetHeadPosition(); pos;)
		{
			FormstueckeUndFittinge::data *dat = (FormstueckeUndFittinge::data *)getStammdaten().get_formstueckefittinge().GetNext(pos);
			if (dat->get_artikelnr() == artnr)
			{
				pulverpreis = dat->get_pulverpreis();
				preis = dat->get_sapPreis();
				gewicht = dat->get_gewicht();
				bezeichnung = dat->get_kurztext();
				halbzeug = dat->get_halbzeug();
				einkauf = dat->get_einkauf();
				material->set_mass2(0);
				found = true;
			}
		}
	}
	if (!found)
	{
		for (POSITION pos = getStammdaten().get_profile().GetHeadPosition(); pos;)
		{
			Profile::data *dat = (Profile::data *)getStammdaten().get_profile().GetNext(pos);
			if (dat->get_artikelnr() == artnr)
			{
				halbzeug = dat->get_halbzeug();
				einkauf = dat->get_einkauf();
				preis = dat->get_sapPreis();
				material->set_mass2(EditProfile::decodeMass2String("90 1A 90"));
				gewicht = 0.0;
				bezeichnung = dat->get_kurztext();
				found = true;
			}
		}
	}
	if (!found)
	{
		for (POSITION pos = getStammdaten().get_sondermaterial().GetHeadPosition(); pos;)
		{
			Sondermaterial::data *dat = (Sondermaterial::data *)getStammdaten().get_sondermaterial().GetNext(pos);
			if (dat->get_artikelnr() == artnr)
			{
				halbzeug = dat->get_halbzeug();
				einkauf = dat->get_einkauf();
				preis = dat->get_preis();
				gewicht = dat->get_gewicht();
				bezeichnung = dat->get_kurztext();
				found = true;
			}
		}
	}
	if (!found)
	{
		//aus SAP Artikelstamm lesen
		for (POSITION pos = getStammdaten().get_sapartikelstamm().GetHeadPosition(); pos;)
		{
			SapArtikelstamm::data *dat = (SapArtikelstamm::data *)getStammdaten().get_sapartikelstamm().GetNext(pos);
			if (dat->get_artikelnr() == artnr)
			{				
				preis = dat->get_preis();
				gewicht = dat->get_gewicht();
				bezeichnung = dat->get_bezeichnung();
				found = true;
			}
		}
	}

	if (found)
	{
		UpdateData(false);
		onChangeAnzahl();
		material->set_artikelnr(artnr);
		GetParent()->PostMessage(WM_COMMAND, ID_REBUILD_TABS);
	}
	else
	{
		CString str; str = getStammdaten().getText("IDS_ARTIKEL_NICHT_GEFUNDEN").c_str();
		AfxMessageBox(str, MB_OK | MB_ICONINFORMATION);
	}
}

//Indicates the display is updated after text changes
void EditMaterial::onChangeAnzahl()
{
	CString anz;
	GetDlgItem(IDC_ANZAHL)->GetWindowText(anz);
	if (!atoi(anz))
		return;
	UpdateData();
	if (anzahl)
	{
		gesamtpreis = kann_laenge() ? preis * laenge / 1000 * anzahl : preis * anzahl;
		gesamtpulverpreis = pulverpreis * anzahl;
		gesamtgewicht = kann_laenge() ? gewicht * laenge / 1000 * anzahl : gewicht * anzahl;
	}	
	CDataExchange xdat(this, false);
	ed_gesamtgewicht.doDataExchange(&xdat);
	ed_gesamtpreis.doDataExchange(&xdat);
	ed_gesamtpulverpreis.doDataExchange(&xdat);
}

//Schaut in den Stammdaten nach der Artikelnummer und prüft, ob das Material eine 
//Längenangabe hat
bool EditMaterial::kann_laenge()
{
	POSITION pos;
	for (pos = getStammdaten().get_sondermaterial().GetHeadPosition(); pos;)
	{
		Sondermaterial::data *dat = (Sondermaterial::data *)getStammdaten().get_sondermaterial().GetNext(pos);
		if (dat->get_artikelnr() == artnr)
		{
			for (POSITION xpos = getStammdaten().get_bauelementgruppen().GetHeadPosition(); xpos;)
			{
				BauelementGruppen::data *bdat = (BauelementGruppen::data *)getStammdaten().get_bauelementgruppen().GetNext(xpos);
				if (bdat->get_elem_ID() == dat->get_elemid())
					return bdat->get_kannLaenge();
			}
		}
	}


	for (pos = getStammdaten().get_bauelemente().GetHeadPosition(); pos;)
	{
		Bauelemente::data *dat = (Bauelemente::data *)getStammdaten().get_bauelemente().GetNext(pos);
		if (dat->get_artikelnr() == artnr)
		{
			for (POSITION xpos = getStammdaten().get_bauelementgruppen().GetHeadPosition(); xpos;)
			{
				BauelementGruppen::data *bdat = (BauelementGruppen::data *)getStammdaten().get_bauelementgruppen().GetNext(xpos);
				if (bdat->get_elem_ID() == dat->get_elemid())
					return bdat->get_kannLaenge();
			}
		}
	}

	for (pos = getStammdaten().get_profile().GetHeadPosition(); pos;)
	{
		Profile::data *dat = (Profile::data *)getStammdaten().get_profile().GetNext(pos);
		if (dat->get_artikelnr() == artnr)
		{
			for (POSITION xpos = getStammdaten().get_bauelementgruppen().GetHeadPosition(); xpos;)
			{
				BauelementGruppen::data *bdat = (BauelementGruppen::data *)getStammdaten().get_bauelementgruppen().GetNext(xpos);
				if (bdat->get_elem_ID() == dat->get_elemid())
					return bdat->get_kannLaenge();
			}
		}
	}
	return false;
}

//Indicates the display is updated after text changes
void EditMaterial::onChangeLaenge()
{
	CString len;
	GetDlgItem(IDC_LAENGE)->GetWindowText(len);
	if (!len.GetLength())
		return;
	
	onChangeAnzahl();		
}


void EditMaterial::setWindowsText(int id, const string& txtId)
{
	GetDlgItem(id)->SetWindowText(getStammdaten().getText(txtId).c_str());
}

void EditMaterial::setWindowsTextWithColon(int id, const string& txtId)
{
	GetDlgItem(id)->SetWindowText(getStammdaten().getTextWithColon(txtId).c_str());
}