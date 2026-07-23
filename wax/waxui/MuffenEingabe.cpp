#include "stdafx.h"


#include "resource.h"
	
#include <mxcontrols/msgbox.h>
using namespace MxGui;

		#include "muffeneingabe.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


#define UPDATE_MSG WM_USER+1235

IMPLEMENT_DYNCREATE(MuffenEingabe, WaxPartForm)

BEGIN_MESSAGE_MAP(MuffenEingabe, WaxPartForm)
	ON_CBN_KILLFOCUS(IDC_BOHR_DURCHMESSER, DoUpdate)
	ON_EN_KILLFOCUS(IDC_ABSTAND1,	DoUpdate) 
	ON_EN_KILLFOCUS(IDC_ABSTAND2,	DoUpdate) 
	ON_EN_KILLFOCUS(IDC_ABSTAND3,	DoUpdate) 
	ON_EN_KILLFOCUS(IDC_ABSTAND4,	DoUpdate) 
	ON_EN_KILLFOCUS(IDC_ABSTAND5,	DoUpdate) 
	ON_EN_KILLFOCUS(IDC_ABSTAND6,	DoUpdate) 
	ON_EN_KILLFOCUS(IDC_ABSTAND7,	DoUpdate) 
	ON_EN_KILLFOCUS(IDC_ABSTAND8,	DoUpdate) 
	ON_EN_KILLFOCUS(IDC_ABSTAND9,	DoUpdate) 
	ON_EN_KILLFOCUS(IDC_ABSTAND10,	DoUpdate) 
	ON_EN_KILLFOCUS(IDC_MUFFEN_LAENGE,DoUpdate) 
	ON_BN_CLICKED(IDC_MUFFEN_GEKEHLT,doUpdate)
	ON_COMMAND(UPDATE_MSG, doUpdate) 
	ON_CBN_SELCHANGE(IDC_MUFFEN_ID, OnCbnSelchangeElementType)
	ON_CBN_SELCHANGE(IDC_MUFFEN_DN, OnCbnSelchangeMuffenDn)
END_MESSAGE_MAP()

MuffenEingabe::~MuffenEingabe()
{

}

MuffenEingabe::MuffenEingabe() : WaxPartForm(IDD_MUFFEN, getStammdaten().getText("IDD_MUFFEN").c_str())
{
	muffenDN = 15;
	muffenID = MUFFE;
	m_bMuffenGekehlt = false;
	muffenRestabstand = 5000;
	for (int i=0; i<=Strangrohr::MAX_ANZAHL_STDELEMENTE; i++)
	{
		muffenRelAbst[i] = 0;
	}
	err = "";

	durchmesser = 0.0;
	ed_durchmesser.initVal(&durchmesser);

	muffenLaenge = 0;
}


/**
Zugriff auf das Entity Objekt Strangrohr aus WaxPart
*/
Strangrohr* MuffenEingabe::getStrangrohr()
{
	//WaxPart Objekt holen
	WaxPart* rohrData = dynamic_cast<WaxPart*>(get_data());
	XASSERT(rohrData);
	if (!rohrData)
		return NULL;


	//Aus dem WaxPart (BasisKlasse von StrangrohrData)  Strangrohr holen
	Strangrohr* strang = dynamic_cast<Strangrohr*>(rohrData->get_werkauftrag());
	XASSERT(strang);
	if (!strang)
		return NULL;

	return strang;
}




BOOL MuffenEingabe::OnInitDialog()
{
	/*
	Dialoginitialisierung 
	Hier werden den Comboboxtexten ein Integerwert zugewiesen, 
	der einer ElementID entspricht. 
	*/
	if (!WaxPartForm::OnInitDialog())
		return false;

	// Combobox MuffenID
	BauelementGruppen bg;
	CString str; str = getStammdaten().getText("IDS_LEER").c_str();
	cbinitdata3 initID[] = 
	{
		str,							Element::LEER,	 IDC_MUFFEN_ID,
		bg.getBezeichnung(MUFFE),		MUFFE,			 IDC_MUFFEN_ID,
		"RobbiMuffe BackupProzess",				NEW_ROBBI_MUFFE, IDC_MUFFEN_ID,
		bg.getBezeichnung(BOHR24),		BOHR24,			 IDC_MUFFEN_ID,
		bg.getBezeichnung(SONDERMUFFE),	SONDERMUFFE,	 IDC_MUFFEN_ID,
		"",			0,		0
	};
	INIT_CBDATA3(initID);

	//Dn setzen
	dnComboBox.setRange(10,25,true);


	//StrangrohrData holen
	StrangrohrData* rohr = dynamic_cast<StrangrohrData*>(get_data());
	XASSERT(rohr);
	if (!rohr)
		return false;


	//Strangrohr holen
	Strangrohr* strang = getStrangrohr();
	if (!strang)
		return false;
	
	// Synchronisieren
	bool bIsDependedSyncObject=true;
	if (rohr->doSync(false) || (strang->get_status().get_wanr() != 0 && !WaxSys::isAV()))
	{
		GetDlgItem(IDC_MUFFEN_ID)->EnableWindow(FALSE);
		GetDlgItem(IDC_MUFFEN_DN)->EnableWindow(FALSE);
		GetDlgItem(IDC_BOHR_DURCHMESSER)->EnableWindow(FALSE);
		GetDlgItem(IDC_MUFFEN_GEKEHLT)->EnableWindow(FALSE);
		GetDlgItem(IDC_MUFFEN_LAENGE)->EnableWindow(FALSE);
		GetDlgItem(IDC_MUFFEN_LAENGE_TEXT)->EnableWindow(FALSE);
		GetDlgItem(IDC_SONDERMUFFEN_KOSTEN)->EnableWindow(FALSE);

		bIsDependedSyncObject = false;
	}
	else
	{
		GetDlgItem(IDC_MUFFEN_ID)->EnableWindow(TRUE);
		GetDlgItem(IDC_MUFFEN_DN)->EnableWindow(TRUE);
		GetDlgItem(IDC_BOHR_DURCHMESSER)->EnableWindow(TRUE);
		GetDlgItem(IDC_MUFFEN_GEKEHLT)->EnableWindow(TRUE);
		GetDlgItem(IDC_MUFFEN_LAENGE)->EnableWindow(TRUE);
		GetDlgItem(IDC_MUFFEN_LAENGE_TEXT)->EnableWindow(TRUE);
		GetDlgItem(IDC_SONDERMUFFEN_KOSTEN)->EnableWindow(TRUE);
	}

	

	//allg. Muffendaten setzen
	muffenDN	 = strang->get_muffenDN();
	muffenID	 = strang->get_muffenID();

	// Falls bereits Robbi Muffen gesetzt sind, im Dialog Standardmuffen auswählen
	if (muffenID == 6 || muffenID == 7)
	{
		muffenID = 5;
		strang->set_muffenID(muffenID);
	}
	
	

	//Abstände Muffen
	for (int i=0; i<Strangrohr::MAX_ANZAHL_STDELEMENTE; i++)
	{
		muffenRelAbst[i] = strang->get_muffenRelAbst(i);
	}

	muffenRestabstand = strang->get_muffenRestabstand(); 
	SetDlgItemInt(IDC_ABSTAND11, muffenRestabstand, false);

	//Durchmesser Bohrung setzen
	durchmesser = strang->get_durchmesser();
	
	//Länge Sondermuffe
	muffenLaenge = strang->get_sonderMuffenLaenge();
	

	//Dialogboxwerte anhand der MuffenID anpassen
	initSelchangeElementType(bIsDependedSyncObject);

	//Muffe gekehlt setzen
	m_bMuffenGekehlt= strang->get_muffenGekehlt();
	int indexGek = (m_bMuffenGekehlt) ? 1 : 0;
	CButton *bt = (CButton*)GetDlgItem(IDC_MUFFEN_GEKEHLT);
	bt->SetCheck(indexGek);

	//Dialogboxwerte anhand des DN Wertes anpassen
	initSelchangeMuffenDn(bIsDependedSyncObject);


	setWindowsText(IDC_STATIC_ELETYP, "IDS_ELETYP");
	setWindowsText(IDC_MUFFEN_LAENGE_TEXT, "IDC_MUFFEN_LAENGE_TEXT");
	setWindowsText(IDC_SONDERMUFFEN_KOSTEN, "IDC_SONDERMUFFEN_KOSTEN");
	setWindowsText(IDC_STATIC_18, "IDS_REST");
	setWindowsText(IDC_MUFFEN_GEKEHLT, "IDS_MUFFEN_GEKEHLT");
	setWindowsText(IDC_STATIC_ABST, "IDC_STATIC_ABST");


	// Update
	UpdateData(FALSE);

	DisableAllControls();

	return true;
}




bool  MuffenEingabe::ForceCheckGekehlteMuffen()
{

	Strangrohr* strang = getStrangrohr();

	if (!strang)
		return false;

	if(muffenID == MUFFE)
	{
		if ((strang->get_dnvl() == 32 || strang->get_dnvl() == 40) && muffenDN == 25 || strang->get_dnvl() == 80 && ( muffenDN == 20 || muffenDN == 25 ))
		{
			m_bMuffenGekehlt = true;
			strang->set_muffenGekehlt(true);
			CButton *bt = (CButton*)GetDlgItem(IDC_MUFFEN_GEKEHLT);
			bt->SetCheck(m_bMuffenGekehlt);
			GetDlgItem(IDC_MUFFEN_GEKEHLT)->EnableWindow(FALSE);
			return true;
		} 
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}


void MuffenEingabe::initSelchangeElementType(bool pos1)
{
	CString str;

	if( !ForceCheckGekehlteMuffen() )
	{
		CButton *bt = (CButton*)GetDlgItem(IDC_MUFFEN_GEKEHLT);
		bt->SetCheck(FALSE);
		if(pos1)
		{
			GetDlgItem(IDC_MUFFEN_GEKEHLT)->EnableWindow(TRUE);
		}
	}

	if (muffenID == BOHR24)
	{
		str = getStammdaten().getText("IDS_DURCHMESSER").c_str();
		SetDlgItemText(IDC_NENNWEITE_DURCHMESSER, "&"+str);
		GetDlgItem(IDC_MUFFEN_DN)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BOHR_DURCHMESSER)->ShowWindow(SW_NORMAL);
		GetDlgItem(IDC_MUFFEN_GEKEHLT)->EnableWindow(FALSE);
		if (muffenDN != 0)
		{
			muffenDN = 0;
			Strangrohr* strang = getStrangrohr();
			strang->set_muffenDN(muffenDN);
			SetDlgItemInt(IDC_MUFFEN_DN,  muffenDN, false);
		}
	}
	else //Muffe oder Sondermuffe
	{
		str = getStammdaten().getText("IDS_DN").c_str();
		SetDlgItemText(IDC_NENNWEITE_DURCHMESSER, "&"+str);
		GetDlgItem(IDC_MUFFEN_DN)->ShowWindow(SW_NORMAL);
		GetDlgItem(IDC_BOHR_DURCHMESSER)->ShowWindow(SW_HIDE);

		if (muffenDN == 0)
		{
			muffenDN = 15;
			Strangrohr* strang = getStrangrohr();
			strang->set_muffenDN(muffenDN);
			SetDlgItemInt(IDC_MUFFEN_DN,  muffenDN, false);
		}

		if (muffenID != SONDERMUFFE)
			muffenLaenge = 0;
	}



	//Einstellungen für Sondermuffe
	if (muffenID == SONDERMUFFE && pos1)
	{
		//ShowWindow funktioniert hier leider nicht!!! Warum auch immer??

		//GetDlgItem(IDC_MUFFEN_LAENGE)->ShowWindow(SW_NORMAL);
		//GetDlgItem(IDC_MUFFEN_LAENGE_TEXT)->ShowWindow(SW_NORMAL);
		//GetDlgItem(IDC_SONDERMUFFEN_KOSTEN)->ShowWindow(SW_NORMAL);


		GetDlgItem(IDC_MUFFEN_LAENGE)->EnableWindow(true);
		GetDlgItem(IDC_MUFFEN_LAENGE_TEXT)->EnableWindow(true);
		GetDlgItem(IDC_SONDERMUFFEN_KOSTEN)->EnableWindow(true);
	
		
	}
	else
	{

		//ShowWindow funktioniert hier leider nicht!!! Warum auch immer??

		//GetDlgItem(IDC_MUFFEN_LAENGE)->ShowWindow(SW_HIDE);
		//GetDlgItem(IDC_MUFFEN_LAENGE_TEXT)->ShowWindow(SW_HIDE);
		//GetDlgItem(IDC_SONDERMUFFEN_KOSTEN)->ShowWindow(SW_HIDE);
	
		GetDlgItem(IDC_MUFFEN_LAENGE)->EnableWindow(false);
		GetDlgItem(IDC_MUFFEN_LAENGE_TEXT)->EnableWindow(false);
		GetDlgItem(IDC_SONDERMUFFEN_KOSTEN)->EnableWindow(false);

	}


}



void MuffenEingabe::DoDataExchange(CDataExchange* pDX)
{
	/**
	Dialogdatenaustausch 
	Hier werden die Dialogdaten an die Klasse Standardverteiler übergeben |
	Erzeugung der Rohrkonstruktion und der Graphik  
	*/
	WaxPartForm::DoDataExchange(pDX);

	try
	{
		Strangrohr* strang = getStrangrohr();
		if (!strang)
			return;

	if (!pDX->m_bSaveAndValidate) 
	{
		for (int i=0; i<Strangrohr::MAX_ANZAHL_STDELEMENTE; i++)
		{
			muffenRelAbst[i] = strang->get_muffenRelAbst(i);
		}
	}
	//{{AFX_DATA_MAP(MuffenEingabe)
	DDX_Text(pDX,		IDC_ABSTAND1,		muffenRelAbst[0]);
	DDX_Text(pDX,		IDC_ABSTAND2,		muffenRelAbst[1]);
	DDX_Text(pDX,		IDC_ABSTAND3,		muffenRelAbst[2]);
	DDX_Text(pDX,		IDC_ABSTAND4,		muffenRelAbst[3]);
	DDX_Text(pDX,		IDC_ABSTAND5,		muffenRelAbst[4]);
	DDX_Text(pDX,		IDC_ABSTAND6,		muffenRelAbst[5]);
	DDX_Text(pDX,		IDC_ABSTAND7,		muffenRelAbst[6]);
	DDX_Text(pDX,		IDC_ABSTAND8,		muffenRelAbst[7]);
	DDX_Text(pDX,		IDC_ABSTAND9,		muffenRelAbst[8]);
	DDX_Text(pDX,		IDC_ABSTAND10,		muffenRelAbst[9]);
	DDX_Control(pDX,	IDC_BOHR_DURCHMESSER,ed_durchmesser);
	DDX_Text(pDX,		IDC_MUFFEN_LAENGE,	muffenLaenge);
	DDX_Control(pDX,	IDC_MUFFEN_DN,		dnComboBox);
	//}}AFX_DATA_MAP
	
	ed_durchmesser.doDataExchange(pDX);

	

	CButton *bt = (CButton*)GetDlgItem(IDC_MUFFEN_GEKEHLT);
	int indexGek = bt->GetCheck();
	m_bMuffenGekehlt = (indexGek == 1) ? true : false;
	
	
			if (pDX->m_bSaveAndValidate) 
			{

				ddx_cbdata(pDX, IDC_MUFFEN_ID, muffenID);
				ddx_cbdata(pDX, IDC_MUFFEN_DN, muffenDN);

				if(muffenID == 0)
				{
				  return;
				}

				strang->set_muffenGekehlt(m_bMuffenGekehlt);
				strang->set_muffenDN(muffenDN);
				strang->set_muffenID(muffenID);
				strang->set_muffenRestabstand(muffenRestabstand);
				strang->set_durchmesser(durchmesser);
				strang->set_sonderMuffenLaenge(muffenLaenge);

				for (int i=0; i<Strangrohr::MAX_ANZAHL_STDELEMENTE; i++)
				{
					// bererchnet die nachfolgenden relativen Abstaende neu, wenn zwischendrin einer 0 gesetzt wird
					strang->setMuffenRelAbst(i, muffenRelAbst[i]);
					if (muffenRelAbst[i] == 0)
						break;
				}
			}
			else
			{
				
				int LocalmuffenID = strang->get_muffenID();
				if( LocalmuffenID != 0)
				{
					muffenDN	 = strang->get_muffenDN();
					muffenID	 = strang->get_muffenID();
					ddx_cbdata(pDX, IDC_MUFFEN_ID, muffenID);
					ddx_cbdata(pDX, IDC_MUFFEN_DN, muffenDN);
				}
				
			}
	}
	catch(Element::Error e)
	{
		MsgBox::show_error(e.get_errorText());
	}
}


//Plausibilitätscheck, wird aufgerufen, bevor UpdateData(TRUE) aufgerufen 
//wird.
bool MuffenEingabe::check_plausis()
{
	if (!WaxPartForm::check_plausis())
		return false; 

	Strangrohr* strang = getStrangrohr();
	if (!strang)
		return false;

	// Rohr und Elemente checken 
	CString err = "";
	err = strang->checkPlausis();

	if (err.GetLength() > 0)
	{
		CWnd* wnd = GetFocus();
		disableCheck();
		AfxMessageBox(err, MB_OK | MB_ICONINFORMATION);
		((CFrameWnd*)AfxGetMainWnd())->SetMessageText(err);
		// AV nur Meldung, Scheel 23.07.2004
		if (!WaxSys::isAV())
			return false;
		if (wnd)
			wnd->SetFocus();
	}

	// Nebenbei den Restabstand setzen
	strang->set_muffenRestabstand(0);
	muffenRestabstand = strang->get_muffenRestabstand(); 
	SetDlgItemInt(IDC_ABSTAND11, muffenRestabstand, false);
	UpdateData();	

	// Ok
	CString str; str = getStammdaten().getText("IDS_EINGABEN_KORREKT").c_str();
	((CFrameWnd*)AfxGetMainWnd())->SetMessageText(str);
	return true;
}

void MuffenEingabe::DoUpdate()
{
	UpdateData();

	Strangrohr* strang = getStrangrohr();
	if (!strang)
		return;

	// verschiebt die relativen Abstaende nach links, wenn zwischendrin einer 0 ist 
	strang->sortMuffen();
	for (int i=0; i<Strangrohr::MAX_ANZAHL_STDELEMENTE; i++)
		muffenRelAbst[i] = strang->get_muffenRelAbst(i);

	UpdateData(false);	
	doUpdate();
}

///Sorgt für ein Auslesen der Dialogelemente und Neuerzeugen der Grafik, wenn checkPlausis true liefert.
void MuffenEingabe::commitUpdate()
{
	WaxPartForm::commitUpdate();

	if (muffenRelAbst[0] == 0)
	{
		muffenRelAbst[1] = 0;
		SetDlgItemInt(IDC_ABSTAND2, muffenRelAbst[1], false);
		GetDlgItem(IDC_ABSTAND3)->EnableWindow(FALSE);
	}
	else
		GetDlgItem(IDC_ABSTAND3)->EnableWindow(TRUE);

	if (muffenRelAbst[1] == 0)
	{
		muffenRelAbst[2] = 0;
		SetDlgItemInt(IDC_ABSTAND3, muffenRelAbst[2], false);
		GetDlgItem(IDC_ABSTAND4)->EnableWindow(FALSE);
	}
	else
		GetDlgItem(IDC_ABSTAND4)->EnableWindow(TRUE);

	if (muffenRelAbst[2] == 0)
	{
		muffenRelAbst[3] = 0;
		SetDlgItemInt(IDC_ABSTAND4, muffenRelAbst[3], false);
		GetDlgItem(IDC_ABSTAND5)->EnableWindow(FALSE);
	}
	else
		GetDlgItem(IDC_ABSTAND5)->EnableWindow(TRUE);

	if (muffenRelAbst[3] == 0)
	{
		muffenRelAbst[4] = 0;
		SetDlgItemInt(IDC_ABSTAND5, muffenRelAbst[4], false);
		GetDlgItem(IDC_ABSTAND6)->EnableWindow(FALSE);
	}
	else
		GetDlgItem(IDC_ABSTAND6)->EnableWindow(TRUE);

	if (muffenRelAbst[4] == 0)
	{
		muffenRelAbst[5] = 0;
		SetDlgItemInt(IDC_ABSTAND6, muffenRelAbst[5], false);
		GetDlgItem(IDC_ABSTAND7)->EnableWindow(FALSE);
	}
	else
		GetDlgItem(IDC_ABSTAND7)->EnableWindow(TRUE);

	if (muffenRelAbst[5] == 0)
	{
		muffenRelAbst[6] = 0;
		SetDlgItemInt(IDC_ABSTAND7, muffenRelAbst[6], false);
		GetDlgItem(IDC_ABSTAND8)->EnableWindow(FALSE);
	}
	else
		GetDlgItem(IDC_ABSTAND8)->EnableWindow(TRUE);

	if (muffenRelAbst[6] == 0)
	{
		muffenRelAbst[7] = 0;
		SetDlgItemInt(IDC_ABSTAND8, muffenRelAbst[7], false);
		GetDlgItem(IDC_ABSTAND9)->EnableWindow(FALSE);
	}
	else
		GetDlgItem(IDC_ABSTAND9)->EnableWindow(TRUE);

	if (muffenRelAbst[7] == 0)
	{
		muffenRelAbst[8] = 0;
		SetDlgItemInt(IDC_ABSTAND9, muffenRelAbst[8], false);
		GetDlgItem(IDC_ABSTAND10)->EnableWindow(FALSE);
	}
	else
		GetDlgItem(IDC_ABSTAND10)->EnableWindow(TRUE);

	// im Sammelauftrag alle Positionen synchronisieren
	WaxPart *parent = const_cast<WaxPart*>(get_data()->get_parent());
	if (parent->IsKindOf(RUNTIME_CLASS(SammelAuftrag)))
		parent->doSync(false);
}


/*
Dialogboxwerte nach dem ändern des Dn Wertes neu setzen
\param pos1: Einzelne Position oder 1. Pos von Sammelauftrag
*/
void MuffenEingabe::initSelchangeMuffenDn(bool pos1)
{
	if(!ForceCheckGekehlteMuffen())
	{
		if (pos1)
		{
			CButton *bt = (CButton*)GetDlgItem(IDC_MUFFEN_GEKEHLT);
			bt->SetCheck(m_bMuffenGekehlt);
			if(m_MuffenDNPrev == 25)
			{
				bt->SetCheck(FALSE);
			}
			GetDlgItem(IDC_MUFFEN_GEKEHLT)->EnableWindow(TRUE);
		}
	}
	m_MuffenDNPrev = muffenDN;
}



void MuffenEingabe::OnCbnSelchangeElementType()
{
	UpdateData();	

	//Dialogboxwerte initialisieren
	initSelchangeElementType(true);


	UpdateData(FALSE);

	//Positionen im Sammelauftrag synchronisieren
	syncSammelauftrag();
	
}

void MuffenEingabe::OnCbnSelchangeMuffenDn()
{
	UpdateData();	

	initSelchangeMuffenDn(true);

	UpdateData(FALSE);

	//Positionen im Sammelauftrag synchronisieren
	syncSammelauftrag();
}


//Alle Positionen im Sammelauftrag syncronisieren
void MuffenEingabe::syncSammelauftrag()
{
	//check Plausis
	doUpdate();

	// im Sammelauftrag alle Positionen synchronisieren
	WaxPart *parent = const_cast<WaxPart*>(get_data()->get_parent());
	if (parent->IsKindOf(RUNTIME_CLASS(SammelAuftrag)))
		parent->doSync(false);

	UpdateData();	
}


