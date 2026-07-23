#include "stdafx.h"

#include "resource.h"
#include <mxutil/ddx_extension.h>

#include <mxcontrols/msgbox.h>
using namespace MxGui;

#include "..\waxui\GridCtrl\InPlaceEdit.h"
#include "..\waxui\GridCtrl\WaxGridCellCombo.h"

	#include "CO II VerteilerEingabeFormBemassung.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


IMPLEMENT_DYNCREATE(COIIVerteilerBemassung, WaxPartForm)

BEGIN_MESSAGE_MAP(COIIVerteilerBemassung, WaxPartForm)
	
	ON_CBN_SELENDOK(IDC_EDIT_DN_VL,		SelChangeRohrDN)
	ON_CBN_SELENDOK(IDC_COMBO_VERT_ANF, SelChangeRohrAnfang)
	ON_CBN_SELENDOK(IDC_COMBO_VERT_END, SelChangeRohrEnde)
	ON_CBN_SELENDOK(IDC_EDIT_ANZAHL_ANSCHLUESSE, OnEnChangeEditAnzahlAnschluesse)
	ON_BN_CLICKED(IDC_CHECK_ENTLEERUNG, doUpdate)
	ON_BN_CLICKED(IDC_CHECK_ENTLEERUNG_ENTFAELLT, doUpdate)
	ON_BN_CLICKED(IDC_RADIO_ENTLEERUNG_LINKS, doUpdate)
	ON_BN_CLICKED(IDC_RADIO_ENTLEERUNG_RECHTS, doUpdate)
	ON_BN_CLICKED(IDC_CHECK_VERT_SICHERHEITSVENTIL, doUpdate)
	ON_BN_CLICKED(IDC_COMBO_VERT_SICHERHEITS_ENTFAELLT, doUpdate)
	ON_BN_CLICKED(IDC_RADIO_LINKS_SICHERHEITSVENTIL, doUpdate)
	ON_BN_CLICKED(IDC_RADIO_RECHTS_SICHERHEITSVENTIL, doUpdate)
	ON_EN_KILLFOCUS(IDC_EDIT_LINKER_BOCK, doUpdate)
	ON_EN_KILLFOCUS(IDC_EDIT_MITTLERER_BOCK_LINKS, doUpdate)
	ON_EN_KILLFOCUS(IDC_EDIT_MITTLERER_BOCK_RECHTS, doUpdate)
	ON_EN_KILLFOCUS(IDC_EDIT_RECHTER_BOCK, doUpdate)
	ON_EN_KILLFOCUS(IDC_EDIT_ABGOBEN_LANG,	doUpdate)
	ON_EN_KILLFOCUS(IDC_EDIT_DN_ABGOBEN,	doUpdate)
	ON_EN_KILLFOCUS(IDC_EDIT_ABG_LANG,	doUpdate)
	ON_EN_KILLFOCUS(IDC_EDIT_LAENGE,	doUpdate)
	ON_EN_KILLFOCUS(IDC_EDIT_VL_LAENGE, doUpdate)
	ON_EN_KILLFOCUS(IDC_EDIT_DN1,		doUpdate)
	ON_EN_KILLFOCUS(IDC_EDIT_DN2,		doUpdate)
	ON_EN_KILLFOCUS(IDC_EDIT_DN3,		doUpdate)
	ON_EN_KILLFOCUS(IDC_EDIT_DN4,		doUpdate)
	ON_EN_KILLFOCUS(IDC_EDIT_DN5,		doUpdate)
	ON_EN_KILLFOCUS(IDC_EDIT_DN6,		doUpdate)
	ON_EN_KILLFOCUS(IDC_EDIT_L0,		doUpdate)
	ON_EN_KILLFOCUS(IDC_EDIT_L1,		doUpdate)
	ON_EN_KILLFOCUS(IDC_EDIT_L2,		doUpdate)
	ON_EN_KILLFOCUS(IDC_EDIT_L3,		doUpdate)
	ON_EN_KILLFOCUS(IDC_EDIT_L4,		doUpdate)
	ON_EN_KILLFOCUS(IDC_EDIT_L5,		doUpdate)
	ON_EN_KILLFOCUS(IDC_EDIT_POSNR, doUpdate)
	ON_EN_KILLFOCUS(IDC_EDIT_ANZAHL, doUpdate)
	
	ON_NOTIFY(GVN_ENDLABELEDIT, IDC_GRID, OnGridEndEdit)
	ON_MESSAGE(WM_GRIDENDEDIT_POSTED, OnGridEndEditPosted)
	
	ON_WM_CTLCOLOR()
	ON_WM_SIZE()
	ON_WM_VSCROLL()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_SETCURSOR()
	
END_MESSAGE_MAP()




COIIVerteilerBemassung::COIIVerteilerBemassung() : WaxPartForm(IDD_COIIVERT_BEMASSUNG, getStammdaten().getText("IDD_STATVERT_BEMASSUNG").c_str()), m_StretchBitmapControl(IDB_BITMAP_COIIVERT_EDIT), m_WhiteBrush(RGB(255,255,255))
{
	int i = 0;
	
	elementID     = ROHR;
	
	for (i=0; i< COIIVerteiler::MAXCOUNT; i++) 
	{
		elementDN[i]					= 0;
		elementRelAbst[i]				= 0;
		elementTypAbgangEnde[i]			= 0;
		elementTypGegenFLansch[i]		= 0;
		elementLaengeGegenFLansche[i]	= 1230;
	}
	elementEndschalterTyp = 0;
	elementlEnde    = FLANSCH_COII;
	elementrEnde    = FLANSCH_COII; 
	
	iAbstandBock[0] = 0;
	iAbstandBock[1] = 0;
	iAbstandBock[2] = 0;
	iAbstandBock[3] = 0;

	m_CheckSicherheitsventil = -1;
	m_bRadioSicherheitsventilRechts = 0;
	m_CheckEntLeerung = -1;
	m_bRadioEntLeerungRechts = 0;
	bDialogHardWorking = false;
	rohrDN          = 100;
	rohrLaenge      = 6000;
	m_iAnzahlAnschluesse = 1;
}

//Destruktor
COIIVerteilerBemassung::~COIIVerteilerBemassung()
{
}

//Dialog initialisieren
BOOL COIIVerteilerBemassung::OnInitDialog()
{
		
	if (!WaxPartForm::OnInitDialog())
		return false;
	
	rohr = getRohrData();
	XASSERT(rohr);

	m_hCursor1=AfxGetApp()->LoadCursor(IDC_CURSOR1);
	m_hCursor2=AfxGetApp()->LoadCursor(IDC_CURSOR2);
	
	GetWindowRect(m_rcOriginalRect);
	m_nScrollPos = 0; 
	m_bDragging = FALSE;
	
	cbinitdata3 cbinitAnschluss[] =
	{
			
			"1"				,	1			,IDC_EDIT_ANZAHL_ANSCHLUESSE,
			"2"				,   2			,IDC_EDIT_ANZAHL_ANSCHLUESSE,
			"3"				,   3			,IDC_EDIT_ANZAHL_ANSCHLUESSE,
			"4"				,	4			,IDC_EDIT_ANZAHL_ANSCHLUESSE,
			"5"				,   5			,IDC_EDIT_ANZAHL_ANSCHLUESSE,
			"6"				,   6			,IDC_EDIT_ANZAHL_ANSCHLUESSE,
			"7"				,   7			,IDC_EDIT_ANZAHL_ANSCHLUESSE,
			"", 0, 0
	};
	INIT_CBDATA3(cbinitAnschluss);

	if(WaxSys::isAV())
	{
		GetDlgItem(IDC_STATIC_MITTLERER_BOCK)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_EDIT_MITTLERER_BOCK_LINKS)->ShowWindow(SW_SHOW);
		CString strText = getStammdaten().getText("IDS_STATIC_MITTLERER_BOCK").c_str();
		GetDlgItem(IDC_STATIC_MITTLERER_BOCK)->SetWindowText(strText);

		GetDlgItem(IDC_STATIC_MITTLERER_BOCK2)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_EDIT_MITTLERER_BOCK_RECHTS)->ShowWindow(SW_SHOW);
		strText = getStammdaten().getText("IDS_STATIC_MITTLERER_BOCK2").c_str();
		GetDlgItem(IDC_EDIT_MITTLERER_BOCK_RECHTS)->SetWindowText(strText);
	}

	CString strText =   getStammdaten().getText("IDC_STATIC_POS_COII").c_str();
	GetDlgItem(IDC_STATIC_POS)->SetWindowText(strText);
	strText = getStammdaten().getText("IDS_ANZAHL_COII").c_str();
	GetDlgItem(IDC_STATIC_ANZAHL)->SetWindowText(strText);
	strText = getStammdaten().getText("IDS_STATIC_GRUND_ROHR").c_str();
	GetDlgItem(IDC_STATIC_GRUND_ROHR)->SetWindowText(strText);
	GetDlgItem(IDC_STATIC_ANFANG)->SetWindowText(getStammdaten().getText("IDS_STATIC_ANFANG_COII").c_str());
	GetDlgItem(IDC_STATIC_ENDE)->SetWindowText(getStammdaten().getText("IDC_STATIC_ENDE_COII").c_str());


	strText = getStammdaten().getText("IDS_MSG_RECHTER_BOCK").c_str();
	GetDlgItem(IDC_STATIC_RECHTER_BOCK)->SetWindowText(strText);
	strText = getStammdaten().getText("IDS_MSG_LINKER_BOCK").c_str();
	GetDlgItem(IDC_STATIC_LINKER_BOCK)->SetWindowText(strText);
	strText = getStammdaten().getText("IDS_STATIC_ANZAHL_ANSCHLUESSE").c_str();
	GetDlgItem(IDC_STATIC_ANZAHL_ANSCHLUESEE)->SetWindowText(strText);


	strText = getStammdaten().getText("IDS_ENDSCHALTER_VENTILE").c_str();
	GetDlgItem(IDC_STATIC_ENDSCHALTER_TYP)->SetWindowText(strText);

	strText = getStammdaten().getText("IDS_CHECK_VERT_SICHERHEITSVENTIL").c_str();
	GetDlgItem(IDC_CHECK_VERT_SICHERHEITSVENTIL)->SetWindowText(strText);
	strText = getStammdaten().getText("IDS_LINKS").c_str();
	GetDlgItem(IDC_RADIO_LINKS_SICHERHEITSVENTIL)->SetWindowText(strText);
	strText = getStammdaten().getText("IDS_RECHTS").c_str();
	GetDlgItem(IDC_RADIO_RECHTS_SICHERHEITSVENTIL)->SetWindowText(strText);
	strText = getStammdaten().getText("IDS_COMBO_VERT_SICHERHEITS_ENTFAELLT").c_str();
	GetDlgItem(IDC_COMBO_VERT_SICHERHEITS_ENTFAELLT)->SetWindowText(strText);


	strText = getStammdaten().getText("IDS_CHECK_ENTLEERUNG").c_str();
	GetDlgItem(IDC_CHECK_ENTLEERUNG)->SetWindowText(strText);
	strText = getStammdaten().getText("IDS_LINKS").c_str();
	GetDlgItem(IDC_RADIO_ENTLEERUNG_LINKS)->SetWindowText(strText);
	strText = getStammdaten().getText("IDS_RECHTS").c_str();
	GetDlgItem(IDC_RADIO_ENTLEERUNG_RECHTS)->SetWindowText(strText);
	strText = getStammdaten().getText("IDS_COMBO_VERT_SICHERHEITS_ENTFAELLT").c_str();
	GetDlgItem(IDC_CHECK_ENTLEERUNG_ENTFAELLT)->SetWindowText(strText);


	init();
	InitializeTheGrid();
	

	cbinitdata3 cbinit[] =
	{
			
			"65"				,	65			,IDC_EDIT_DN_VL,
			"80"				,   80			,IDC_EDIT_DN_VL,
			"100"				,   100			,IDC_EDIT_DN_VL,
			"", 0, 0
	};

	INIT_CBDATA3(cbinit);

	cbinitdata3 cbinitEndschalter[] =
	{
			
			getStammdaten().getText("IDS_KEIN_ENDSCHALTER")		,	0					 , IDC_COMBO_ENDSCHALTER_TYP,
			getStammdaten().getText("IDS_ENDSCHALTER_ZWEI")		,   ENDSCHALTER_ZWEI_KON , IDC_COMBO_ENDSCHALTER_TYP,
			getStammdaten().getText("IDS_ENDSCHALTER_VIER")		,   ENDSCHALTER_VIER_KON , IDC_COMBO_ENDSCHALTER_TYP,
			"", 0, 0
	};

	INIT_CBDATA3(cbinitEndschalter);
	InitAnfangEndeCheckBoxen();
	
	setCtrlStatus();

	// Sychronisieren
	rohr->doSync();
	// Update
	UpdateData(FALSE);
	

	COIIVerteiler* pCOIIVerteiler = dynamic_cast<COIIVerteiler*>( rohr->get_werkauftrag());

	CString strError;
	strError = pCOIIVerteiler->checkPlausis();
	if(strError.GetLength() > 0)
	{
		AfxMessageBox(strError, MB_OK);
	}
	return true;
}

void COIIVerteilerBemassung::init()
{
	COIIVerteiler& statVert = *rohr->get_CommonCOMXVerteiler();

	// Rohrdaten	
	rohrLaenge = statVert.get_laenge();
	rohrDN     = statVert.get_dnvl();
	

	elementlEnde = statVert.get_rohrAnfang();
	elementrEnde = statVert.get_rohrEnde();

	for (int i = 0; i < COIIVerteiler::MAXCOUNT; i++)
	{
		// Abstaende der oberen Abgaenge
		elementRelAbst[i] = statVert.get_elementRelAbst(i);
		// Nennweiten der Abgänge
		elementDN[i] = statVert.get_elementDN(i);
		elementTypAbgangEnde[i] = statVert.get_elementTypeAbgang(i);
		elementTypGegenFLansch[i] = statVert.get_elementGegenflansch(i);
		elementLaengeGegenFLansche[i] = statVert.get_ElementLaengeGegenflansch(i);
	}
	
	elementEndschalterTyp =  statVert.get_EndschalterTyp();
	iAbstandBock[0] = statVert.GetAbstandBockZumLinkenRohrEnde();
	iAbstandBock[1] = statVert.GetAbstandErsterMittlererBockZumLinkenRohrEnde();
	iAbstandBock[2] = statVert.GetAbstandZweiterMittlererBockZumErstenMittlerenBock();
	iAbstandBock[3] = statVert.GetAbstandBockZumRechtenRohrEndeInput();
	
	
	m_iAnzahlAnschluesse = statVert.get_AnzahlAnschluesse();

	m_CheckSicherheitsventil		=  statVert.GetAbgangSicherheitsventilVorhanden();
	m_bRadioSicherheitsventilRechts =  statVert.GetAbgangSicherheitsventilRechts();

	m_CheckEntLeerung		 = statVert.GetAbgangEntleerungVorhanden();
	m_bRadioEntLeerungRechts = statVert.GetAbgangEntleerungRechts();
	set_rohrRestlaenge();
}


COIIVerteilerData* COIIVerteilerBemassung::getRohrData()
{
	return  dynamic_cast<COIIVerteilerData*>(get_data());
}


void COIIVerteilerBemassung::DoDataExchange(CDataExchange* pDX)
{
	
	WaxPartForm::DoDataExchange(pDX);

	ddx_posnr(pDX, IDC_EDIT_POSNR);
	ddx_anzahl(pDX, IDC_EDIT_ANZAHL);


	//{{AFX_DATA_MAP(COIIVerteilerBemassung)
	DDX_Radio(pDX, IDC_CHECK_ENTLEERUNG_ENTFAELLT, m_CheckEntLeerung);
	DDX_Radio(pDX, IDC_RADIO_ENTLEERUNG_LINKS, m_bRadioEntLeerungRechts);
	ddx_cbdata(pDX, IDC_EDIT_ANZAHL_ANSCHLUESSE, m_iAnzahlAnschluesse);
	DDX_Control(pDX,IDC_STATIC_VERTEILER_GRAFIC, m_StretchBitmapControl);
	//}}AFX_DATA_MAP

	ddx_cbdata(pDX, IDC_EDIT_DN_VL, rohrDN);
	ddx_cbdata(pDX, IDC_COMBO_VERT_ANF, elementlEnde);
	ddx_cbdata(pDX, IDC_COMBO_VERT_END, elementrEnde);
	ddx_cbdata(pDX, IDC_COMBO_ENDSCHALTER_TYP, elementEndschalterTyp);

	DDX_Radio(pDX, IDC_COMBO_VERT_SICHERHEITS_ENTFAELLT, m_CheckSicherheitsventil);
	DDX_Radio(pDX, IDC_RADIO_LINKS_SICHERHEITSVENTIL, m_bRadioSicherheitsventilRechts);



	rohr = getRohrData();
	XASSERT(rohr);
	COIIVerteiler& statVert = *rohr->get_CommonCOMXVerteiler();

	if (pDX->m_bSaveAndValidate)
	{
		try
		{
	
			statVert.set_dnvl(rohrDN);
			statVert.set_laenge(rohrLaenge);
			
			DDX_Text(pDX, IDC_EDIT_LINKER_BOCK, iAbstandBock[0]);
			statVert.SetAbstandBockZumLinkenRohrEnde(iAbstandBock[0]);

			DDX_Text(pDX, IDC_EDIT_MITTLERER_BOCK_LINKS, iAbstandBock[1]);
			
			if( iAbstandBock[1] == 0 &&  iAbstandBock[2] > 0 )
			{
				CString strText;
				GetDlgItem(IDC_EDIT_MITTLERER_BOCK_RECHTS)->GetWindowText(strText);
				GetDlgItem(IDC_EDIT_MITTLERER_BOCK_LINKS)->SetWindowText(strText);
				GetDlgItem(IDC_EDIT_MITTLERER_BOCK_RECHTS)->SetWindowText(_T("0"));
			}
			
			statVert.SetAbstandErsterMittlererBockZumLinkenRohrEnde(iAbstandBock[1]);

			DDX_Text(pDX, IDC_EDIT_MITTLERER_BOCK_RECHTS, iAbstandBock[2]);
			statVert.SetAbstandZweiterMittlererBockZumErstenMittlerenBock(iAbstandBock[2]);


			DDX_Text(pDX, IDC_EDIT_RECHTER_BOCK, iAbstandBock[3]);
			statVert.SetAbstandBockZumRechtenRohrEnde(iAbstandBock[3]);
			
			for (int col = 1; col < m_GridCtrl.GetColumnCount(); col++)
			{	
				elementDN[col-1] = atoi(m_GridCtrl.GetItemText(1, col));
			
				elementRelAbst[col-1] = atoi(m_GridCtrl.GetItemText(2, col));
					
				elementTypAbgangEnde[col-1] = GEW;
				elementTypGegenFLansch[col-1] = 0;
				elementLaengeGegenFLansche[col-1] =0;
				if(elementDN[col-1] > 50)
				{
					elementTypAbgangEnde[col-1] = FLANSCH_COII;
					elementTypGegenFLansch[col-1] = m_GridCtrl.GetGegenflanschType(col);
					elementLaengeGegenFLansche[col-1] = atoi(m_GridCtrl.GetItemText(CWaxGridCtrl::e_GEGENFLANSCH_LAENGE, col));
				}
			}

			statVert.set_AnzahlAnschluesse(m_iAnzahlAnschluesse);
	
			statVert.setElementdaten(elementDN, elementRelAbst, elementTypAbgangEnde, elementTypGegenFLansch, elementLaengeGegenFLansche, elementEndschalterTyp, 	
									 elementlEnde, 
									 elementrEnde,	
									  m_CheckEntLeerung,  m_bRadioEntLeerungRechts,
									  m_CheckSicherheitsventil, m_bRadioSicherheitsventilRechts);
			
		}
		catch(Element::Error e)
		{
			MsgBox::show_error(e.get_errorText());
		}
	}
	else
	{
		PopulateGridData();
		
		DDX_Text(pDX, IDC_EDIT_LINKER_BOCK, iAbstandBock[0]);
		DDX_Text(pDX, IDC_EDIT_MITTLERER_BOCK_LINKS, iAbstandBock[1]);	
		DDX_Text(pDX, IDC_EDIT_MITTLERER_BOCK_RECHTS, iAbstandBock[2]);	
		iAbstandBock[3] = statVert.GetAbstandBockZumRechtenRohrEnde();
		DDX_Text(pDX, IDC_EDIT_RECHTER_BOCK, iAbstandBock[3]);

		rohrLaenge = statVert.get_laenge();	
	}
}




void COIIVerteilerBemassung::PopulateGridData()
{
		
	
	for(int z =m_GridCtrl.GetColumnCount()-1; z < COIIVerteiler::MAXCOUNT; z++)
	{
		elementDN[z] = 0;
		elementRelAbst[z] = 0;
		elementTypAbgangEnde[z] = GEW;
		elementTypGegenFLansch[z] = 0;
		elementLaengeGegenFLansche[z] =0;
	}

	for (int col = 1; col < m_GridCtrl.GetColumnCount(); col++)
	{ 
		CString strText;
		
		m_GridCtrl.SetItemText(2, col, _T(""));
		//m_GridCtrl.SetItemState(2, col, m_GridCtrl.GetItemState(2, col) & ~GVIS_READONLY);	

		setColumnComboxDn(col);

		strText.Format(_T("%d"), elementDN[col-1]);
		if(strText.Compare(_T("2")) == 0)
		{
			strText += getStammdaten().getText("IDS_EINSPEISUNG").c_str();
		}
		m_GridCtrl.SetItemText(1, col, strText);
	
		strText.Format(_T("%d"), elementRelAbst[col-1]);
		
		m_GridCtrl.SetItemText(2, col, strText);		

		if( elementDN[col-1] > 50 )
		{	
			m_GridCtrl.SetGegenflanschType(col, elementTypGegenFLansch[col-1]);	
		}
		else
		{
			m_GridCtrl.SetItemText(3, col, _T(""));
			m_GridCtrl.SetItemState(3, col, m_GridCtrl.GetItemState(3, col) | GVIS_READONLY);
		}

		strText.Format(_T("%d"), elementLaengeGegenFLansche[col-1]);
		m_GridCtrl.SetItemText(CWaxGridCtrl::e_GEGENFLANSCH_LAENGE, col, strText);

	}	
}

//Plausibilitätscheck, wird aufgerufen, bevor UpdateData(TRUE) 
//aufgerufen wird.
bool COIIVerteilerBemassung::check_plausis()
{
	set_disableCheckPlausis(FALSE);
	
	if (!WaxPartForm::check_plausis())
		return false;

	COIIVerteiler& statVert = *rohr->get_CommonCOMXVerteiler();
 	CString err = statVert.checkPlausis();
	
	if (err.GetLength() > 0)
	{
		init();
		UpdateData(FALSE);
		
		disableCheck();
		if(!bDialogHardWorking)
		{
			bDialogHardWorking = true;
			
			AfxMessageBox(err, MB_OK | MB_ICONINFORMATION);

			((CFrameWnd*)AfxGetMainWnd())->SetMessageText(err);

			bDialogHardWorking = false;
		}
		return false;
	}

	// OK
	CString str; str = getStammdaten().getText("IDS_EINGABEN_KORREKT").c_str();
	((CFrameWnd*)AfxGetMainWnd())->SetMessageText(str);

	return true;
}

//Update Control
//set rohrRestlaenge
void COIIVerteilerBemassung::set_rohrRestlaenge()
{
	int rohrRestlaenge = rohrLaenge - elementRelAbst[0] - elementRelAbst[1]
									- elementRelAbst[2] - elementRelAbst[3]
									- elementRelAbst[4] - elementRelAbst[5];
	SetDlgItemInt(IDC_EDIT_LX, rohrRestlaenge);
}



///Enabled und disabled die Dialogelemente in Abhängigkeit der Eingaben
void COIIVerteilerBemassung::setCtrlStatus()
{
	
}

//Sorgt für ein Auslesen der Dialogelemente und Neuerzeugen der Grafik, wenn 
//Plausicheck true liefert.
void COIIVerteilerBemassung::commitUpdate()
{
	//Updatedata hier notwendig da sonst die Druckstufe nicht richtig eingelesen wird!
    UpdateData();

	WaxPartForm::commitUpdate();
	set_rohrRestlaenge();
	setCtrlStatus();

	// Update
	UpdateData(FALSE);
}


void COIIVerteilerBemassung::InitializeTheGrid()
{
		
	CRect rectWindow, rectContainer, rectsum;
	CWnd* pContainerForGrid = GetDlgItem(IDC_STATIC_GRID);
	GetWindowRect(rectWindow);
	pContainerForGrid->GetWindowRect(rectContainer);
	rectsum.top = rectContainer.top - rectWindow.top;
	rectsum.bottom = rectsum.top + rectContainer.bottom - rectContainer.top;
	rectsum.left = rectContainer.left - rectWindow.left;
	rectsum.right = rectsum.left +  rectContainer.right - rectContainer.left;
	pContainerForGrid->DestroyWindow();
	m_GridCtrl.Create(rectsum, this, IDC_GRID);
	SetColumns();
}


void COIIVerteilerBemassung::SetColumns()
{
	try
	{
		m_GridCtrl.SetEditable(TRUE);
		m_GridCtrl.SetRowCount(5);
		m_GridCtrl.SetColumnCount(m_iAnzahlAnschluesse + 1);
		m_GridCtrl.SetFixedRowCount(1);
		m_GridCtrl.SetFixedColumnCount(1);
		m_GridCtrl.EnableDragAndDrop(FALSE);
	
	}
	catch (CMemoryException* e)
	{
		e->ReportError();
		e->Delete();
		return;
	}

	
	
		
	GV_ITEM Item;
	Item.mask = GVIF_TEXT|GVIF_FORMAT;
	Item.row = 0;
	Item.col = 0;
	Item.nFormat = DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS|DT_NOPREFIX;
	
	Item.strText = getStammdaten().getText("IDS_LAUF_NR").c_str();
	m_GridCtrl.SetItem(&Item);

	Item.mask = GVIF_TEXT|GVIF_FORMAT;
	Item.row = 1;
	Item.col = 0;
	Item.nFormat = DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS|DT_NOPREFIX;
	Item.strText = _T("DN(1...10)");
	m_GridCtrl.SetItem(&Item);

	Item.mask = GVIF_TEXT|GVIF_FORMAT;
	Item.row = 2;
	Item.col = 0;
	Item.nFormat = DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS|DT_NOPREFIX;
	Item.strText = getStammdaten().getText("IDS_ABSTAND_BV").c_str();
	m_GridCtrl.SetItem(&Item);

	Item.mask = GVIF_TEXT|GVIF_FORMAT;
	Item.row = 3;
	Item.col = 0;
	Item.nFormat = DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS|DT_NOPREFIX;
	Item.strText = getStammdaten().getText("IDS_GEGENFLANSCH").c_str();
	m_GridCtrl.SetItem(&Item);


	Item.mask = GVIF_TEXT|GVIF_FORMAT;
	Item.row = 4;
	Item.col = 0;
	Item.nFormat = DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS|DT_NOPREFIX;
	//Item.strText =_T("Laenge Gegenfl");
	Item.strText = getStammdaten().getText("IDS_GEGENFLANSCH_LAENGE").c_str();
	m_GridCtrl.SetItem(&Item);
	m_GridCtrl.SetColumnWidth(0, 64);

	for (int col = 1; col <= m_iAnzahlAnschluesse; col++)
	{	
		GV_ITEM Item;
		Item.mask = GVIF_TEXT|GVIF_FORMAT;
		Item.row = 0;
		Item.col = col;
		Item.nFormat = DT_CENTER|DT_WORDBREAK;
		Item.strText.Format(_T("%d"), col);
	
		m_GridCtrl.SetItem(&Item);	
		m_GridCtrl.SetColumnWidth(col, 104);
	}

	PopulateGridData();
	m_GridCtrl.SetSelectedRange(1, 1, 1, 1);
	m_GridCtrl.SetFocusCell(1,1);
	m_PrecGridCellFocusID.col = 1;
	m_PrecGridCellFocusID.row = 1;

	
}



void COIIVerteilerBemassung::setColumnComboxDn(int col)
{
	m_GridCtrl.SetCellType(1,col, RUNTIME_CLASS(CWaxGridCellCombo));
    
	CStringArray options;

	CString strText = "2";
	strText += getStammdaten().getText("IDS_EINSPEISUNG").c_str();	
	options.Add(strText);
	options.Add(_T("25"));
	options.Add(_T("40"));
	options.Add(_T("50"));

	if(rohrDN > 50)
	{
		options.Add(_T("65"));
	}

	if(rohrDN > 65)
	{
		options.Add(_T("80"));
	}

	if(rohrDN > 80)
	{
		options.Add(_T("100"));
	}

	CGridCellCombo *pCell = (CGridCellCombo*) m_GridCtrl.GetCell(1, col);
	pCell->SetOptions(options);
	m_GridCtrl.Invalidate();
}


BOOL COIIVerteilerBemassung::PreTranslateMessage(MSG* pMsg)
{
	if( pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN )
	{
		CWnd* pWnd = GetFocus();
		pWnd->SendMessage(WM_KILLFOCUS);
		
		CWnd* pLinkerBock			= GetDlgItem(IDC_EDIT_LINKER_BOCK);
		CWnd* pRechterBock			= GetDlgItem(IDC_EDIT_RECHTER_BOCK);
		CWnd* pMittlererBockLinks	= GetDlgItem(IDC_EDIT_MITTLERER_BOCK_LINKS);
		CWnd* pMittlererBockRechts	= GetDlgItem(IDC_EDIT_MITTLERER_BOCK_RECHTS);
		
		if( pWnd == pLinkerBock )
		{
			pRechterBock->SendMessage(WM_LBUTTONDOWN);
			pRechterBock->SendMessage(WM_LBUTTONUP);
		}

		if( pWnd == pRechterBock )
		{
			pLinkerBock->SendMessage(WM_LBUTTONDOWN);
			pLinkerBock->SendMessage(WM_LBUTTONUP);
		}

		if( pWnd == pMittlererBockLinks )
		{
			pLinkerBock->SendMessage(WM_LBUTTONDOWN);
			pLinkerBock->SendMessage(WM_LBUTTONUP);
		}

		if( pWnd == pMittlererBockRechts )
		{
			pLinkerBock->SendMessage(WM_LBUTTONDOWN);
			pLinkerBock->SendMessage(WM_LBUTTONUP);
		}
		return TRUE;
	}	
	return WaxPartForm::PreTranslateMessage( pMsg);
}



void COIIVerteilerBemassung::OnGridEndEdit(NMHDR *pNotifyStruct, LRESULT* pResult)
{
	COIIVerteiler& statVert = *rohr->get_CommonCOMXVerteiler();
	iAbstandBock[0] = statVert.GetAbstandBockZumLinkenRohrEnde();
	//iAbstandBock[3] = statVert.GetAbstandBockZumRechtenRohrEnde();
	GetDlgItem(IDC_EDIT_LINKER_BOCK)->SetWindowTextA( CStringConvert(iAbstandBock[0]) );
	GetDlgItem(IDC_EDIT_RECHTER_BOCK)->SetWindowTextA( CStringConvert(iAbstandBock[3]) ); 

	CString strItemText = m_GridCtrl.GetItemText(2, m_GridCtrl.GetFocusCell().col);
	m_PrecGridCellFocusID = m_GridCtrl.GetFocusCell();
	
	PostMessage(WM_GRIDENDEDIT_POSTED);
}



LRESULT COIIVerteilerBemassung::OnGridEndEditPosted(WPARAM, LPARAM)
{
	COIIVerteiler& statVert = *rohr->get_CommonCOMXVerteiler();

	CCellID CellFocusID = m_GridCtrl.GetFocusCell();
	
	statVert.set_elementDN(m_GridCtrl.GetFocusCell().col-1, atoi(m_GridCtrl.GetItemText(1, m_GridCtrl.GetFocusCell().col)));
	
	CString strTest = m_GridCtrl.GetItemText(2, m_PrecGridCellFocusID.col);

	if( strTest.Compare("0") != 0 || CellFocusID == m_PrecGridCellFocusID &&strTest.Compare("0") != 0 )
	{/*
		statVert.set_elementRelAbst(m_PrecGridCellFocusID.col-1, atoi(m_GridCtrl.GetItemText(2, m_PrecGridCellFocusID.col)));
		CString strError = statVert.TestDNvlTODNAbgang(m_PrecGridCellFocusID.col-2, m_PrecGridCellFocusID.col-1);
		if(!strError.IsEmpty())
		{
			AfxMessageBox(strError);
		}
		else
		{
			*/
			doUpdate();
	//	}
	}
	
	
	return 0;
}



HBRUSH COIIVerteilerBemassung::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = WaxPartForm::OnCtlColor(pDC, pWnd, nCtlColor);

    // Check whether which static label its.
    if( pWnd->GetDlgCtrlID() == IDC_STATIC_ANFANG || pWnd->GetDlgCtrlID() == IDC_STATIC_ENDE )
    {
		pDC->SetBkColor( RGB( 255, 255, 255 ));
		CRect rect;
		pWnd->GetClientRect(rect);
		pDC->FillRect(&rect, &m_WhiteBrush);
		return m_WhiteBrush;
    }
	return hbr;
}



void COIIVerteilerBemassung::OnEnKillfocusEditAnzahlAnschluesse()
{
	//OnEnKillfocusEditAnzahlAnschluesse();
};


void COIIVerteilerBemassung::InitAnfangEndeCheckBoxen(int iDn)
{
	BauelementGruppen bg;
	CString str; str = getStammdaten().getText("IDS_LEER").c_str();
	((CComboBox*) GetDlgItem(IDC_COMBO_VERT_ANF))->ResetContent();
	((CComboBox*) GetDlgItem(IDC_COMBO_VERT_END))->ResetContent();
	
	int ilokalDN = 0;
	if(iDn != 0)
	{
		ilokalDN = iDn;
	}
	else
	{
		ilokalDN = rohrDN;
	}

	if(ilokalDN > 65)
	{
		cbinitdata3 cbinit[] =
		{
			
			//"65"				,	65			,IDC_EDIT_DN_VL,
			//"80"				,   80			,IDC_EDIT_DN_VL,
			//"100"				,   100			,IDC_EDIT_DN_VL,

	
			getStammdaten().getText("BLINDFLANSCH_COII"),			 BLINDFLANSCH_COII,	IDC_COMBO_VERT_ANF,
			getStammdaten().getText("GEGENFLANSCH_COII_SCHARZ_HAUPTROHR"),	 GEGENFLANSCH_COII_SCHARZ_HAUPTROHR, IDC_COMBO_VERT_ANF,
			bg.getBezeichnung(FLANSCH_COII), FLANSCH_COII,	IDC_COMBO_VERT_ANF,
			getStammdaten().getText("IDS_BODEN_COII"),	 BODEN_COII,	IDC_COMBO_VERT_ANF,
		
		
			getStammdaten().getText("BLINDFLANSCH_COII"),			 BLINDFLANSCH_COII,	IDC_COMBO_VERT_END,
			getStammdaten().getText("GEGENFLANSCH_COII_SCHARZ_HAUPTROHR"),	 GEGENFLANSCH_COII_SCHARZ_HAUPTROHR, IDC_COMBO_VERT_END,
			bg.getBezeichnung(FLANSCH_COII), FLANSCH_COII,	IDC_COMBO_VERT_END,
			getStammdaten().getText("IDS_BODEN_COII"),	 BODEN_COII, IDC_COMBO_VERT_END,
		
		
			
			"", 0, 0
		};
		INIT_CBDATA3(cbinit);
	}
	else
	{
		cbinitdata3 cbinit[] =
		{
		
			//"65"				,	65			,IDC_EDIT_DN_VL,
			//"80"				,   80			,IDC_EDIT_DN_VL,
			//"100"				,   100			,IDC_EDIT_DN_VL,

	
			getStammdaten().getText("BLINDFLANSCH_COII"),					BLINDFLANSCH_COII,					 IDC_COMBO_VERT_ANF,
			getStammdaten().getText("GEGENFLANSCH_COII_SCHARZ_HAUPTROHR"),	GEGENFLANSCH_COII_SCHARZ_HAUPTROHR, IDC_COMBO_VERT_ANF,
			bg.getBezeichnung(FLANSCH_COII),								FLANSCH_COII,						 IDC_COMBO_VERT_ANF,
			getStammdaten().getText("IDS_GEWINDE"),							GEW,								 IDC_COMBO_VERT_ANF,
			getStammdaten().getText("IDS_GEWINDE_KAPPE"),					GEWINDE_KAPPE_COII,				 IDC_COMBO_VERT_ANF,


	
			getStammdaten().getText("BLINDFLANSCH_COII"),					BLINDFLANSCH_COII,					 IDC_COMBO_VERT_END,
			getStammdaten().getText("GEGENFLANSCH_COII_SCHARZ_HAUPTROHR"),	GEGENFLANSCH_COII_SCHARZ_HAUPTROHR, IDC_COMBO_VERT_END,
			bg.getBezeichnung(FLANSCH_COII),								FLANSCH_COII,						 IDC_COMBO_VERT_END,
			getStammdaten().getText("IDS_GEWINDE"),							GEW,								 IDC_COMBO_VERT_END,
			getStammdaten().getText("IDS_GEWINDE_KAPPE"),					GEWINDE_KAPPE_COII,				 IDC_COMBO_VERT_END,
			
			"", 0, 0
		};
		INIT_CBDATA3(cbinit);
	}
}



void COIIVerteilerBemassung::SelChangeRohrEnde()
{ 

	COIIVerteiler& statVert = *rohr->get_CommonCOMXVerteiler();
	int iWo = ((CComboBox*)GetDlgItem(IDC_COMBO_VERT_END))->GetCurSel();
	DWORD dItemData = ((CComboBox*)GetDlgItem(IDC_COMBO_VERT_END))->GetItemData(iWo);
	int iLastAbgangRechts = statVert.LastAbgangRechts();

	int TrestleSpaceToTubeEndRight = 0;

	switch(dItemData)
	{
		case FLANSCH_COII:
		case BLINDFLANSCH_COII:
			TrestleSpaceToTubeEndRight = statVert.GetTrestleSpaceToFlanschRight(iLastAbgangRechts);
		break;

		case BODEN_COII:
			TrestleSpaceToTubeEndRight = statVert.GetTrestleSpaceToBodenRight(iLastAbgangRechts);
		break;
		case GEW:
			TrestleSpaceToTubeEndRight = statVert.GetTrestleSpaceToGewindeRight(iLastAbgangRechts);
		break;
	}
	int AbstandBockAbsolutToRightTubeSide = statVert.GetAbstandBockZumRechtenRohrEnde();
	if(AbstandBockAbsolutToRightTubeSide >= TrestleSpaceToTubeEndRight)
	{
		TrestleSpaceToTubeEndRight = AbstandBockAbsolutToRightTubeSide;
	}
	GetDlgItem(IDC_EDIT_RECHTER_BOCK)->SetWindowTextA( CStringConvert(TrestleSpaceToTubeEndRight));
	doUpdate();
}


void COIIVerteilerBemassung::SelChangeRohrAnfang()
{
	COIIVerteiler& statVert = *rohr->get_CommonCOMXVerteiler();
	int iWo = ((CComboBox*)GetDlgItem(IDC_COMBO_VERT_ANF))->GetCurSel();
	DWORD dItemData = ((CComboBox*)GetDlgItem(IDC_COMBO_VERT_ANF))->GetItemData(iWo);
	int iFirstAbgangLinks = statVert.FirstAbgangLinks();
	int TrestleSpaceToTubeEndLeft = 0;

	switch(dItemData)
	{
		case FLANSCH_COII:
		case BLINDFLANSCH_COII: 
			TrestleSpaceToTubeEndLeft = statVert.GetTrestleSpaceToFlanschLeft(iFirstAbgangLinks);
		break;

		case BODEN_COII:
			TrestleSpaceToTubeEndLeft = statVert.GetTrestleSpaceToBodenLeft(iFirstAbgangLinks);
		break;

		case GEW:
			TrestleSpaceToTubeEndLeft = statVert.GetTrestleSpaceToGewindeLeft(iFirstAbgangLinks);
		break;
	}
	int AbstandBockAbsolutToLeftTubeSide = statVert.GetAbstandBockZumLinkenRohrEnde();
	if(AbstandBockAbsolutToLeftTubeSide >= TrestleSpaceToTubeEndLeft)
	{
		TrestleSpaceToTubeEndLeft = AbstandBockAbsolutToLeftTubeSide;
	}
	GetDlgItem(IDC_EDIT_LINKER_BOCK)->SetWindowTextA( CStringConvert(TrestleSpaceToTubeEndLeft));
	doUpdate();

}



void COIIVerteilerBemassung::SelChangeRohrDN()
{
	CStringConvert strText;
	int iWo = ((CComboBox*)GetDlgItem(IDC_EDIT_DN_VL))->GetCurSel();
	((CComboBox*)GetDlgItem(IDC_EDIT_DN_VL))->GetLBText(iWo, strText);
	int iDn = (int)strText;

	CString strTextAnfang, strTextEnde;
	((CComboBox*) GetDlgItem(IDC_COMBO_VERT_ANF))->GetWindowTextA(strTextAnfang);
	((CComboBox*) GetDlgItem(IDC_COMBO_VERT_END))->GetWindowTextA(strTextEnde);
	InitAnfangEndeCheckBoxen(iDn);
	int iWoAnfang = ((CComboBox*) GetDlgItem(IDC_COMBO_VERT_ANF))->FindStringExact(0,strTextAnfang);
	int iWoEnde = ((CComboBox*) GetDlgItem(IDC_COMBO_VERT_END))->FindStringExact(0,strTextEnde);
	((CComboBox*) GetDlgItem(IDC_COMBO_VERT_ANF))->SetCurSel(iWoAnfang);
	((CComboBox*) GetDlgItem(IDC_COMBO_VERT_END))->SetCurSel(iWoEnde);
	
	COIIVerteiler& statVert = *rohr->get_CommonCOMXVerteiler();
	m_GridCtrl.SetItemText(2, 1, CStringConvert(statVert.GetTrestleSpaceLeftAreaNozzle(elementDN[0], elementRelAbst[0])));
	
	for (int col = 1; col < m_GridCtrl.GetColumnCount(); col++)
	{	
		int iDnAbgang = atoi(m_GridCtrl.GetItemText(1, col));
		if(iDnAbgang > iDn)
		{
			m_GridCtrl.SetItemText(1, col, strText);
		}
	}

	doUpdate();
}



void COIIVerteilerBemassung::OnEnChangeEditAnzahlAnschluesse()
{
	CStringConvert strText;
	int iWo = ((CComboBox*)GetDlgItem(IDC_EDIT_ANZAHL_ANSCHLUESSE))->GetCurSel();
	((CComboBox*)GetDlgItem(IDC_EDIT_ANZAHL_ANSCHLUESSE))->GetLBText(iWo, strText);
	m_iAnzahlAnschluesse = (int)strText;
	SetColumns();
	doUpdate();
}


void COIIVerteilerBemassung::OnSize(UINT nType, int cx, int cy)
{
	WaxPartForm::OnSize(nType, cx, cy);
		
	SCROLLINFO si;
	si.cbSize = sizeof(SCROLLINFO);
	si.fMask = SIF_ALL; 
	si.nMin = 0;
	si.nMax = m_rcOriginalRect.Height();
	si.nPage = cy;
	si.nPos = 0;
    SetScrollInfo(SB_VERT, &si, TRUE); 	
	
	SetScrollPos(SB_VERT,m_nScrollPos,TRUE);
	m_nCurHeight = cy;
}



void COIIVerteilerBemassung::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{

	WaxPartForm::OnVScroll(nSBCode, nPos, pScrollBar);
	int nDelta;
	int nMaxPos = m_rcOriginalRect.Height() - m_nCurHeight;

	switch (nSBCode)
	{
	case SB_LINEDOWN:
		if (m_nScrollPos >= nMaxPos)
			return;

		nDelta = min(max(nMaxPos/10,5),nMaxPos-m_nScrollPos);
		break;

	case SB_LINEUP:
		if (m_nScrollPos <= 0)
			return;
		nDelta = -min(max(nMaxPos/10,5),m_nScrollPos);
		break;
	case SB_PAGEDOWN:
		if (m_nScrollPos >= nMaxPos)
			return;
		nDelta = min(max(nMaxPos,5),nMaxPos-m_nScrollPos);
		break;
	case SB_THUMBTRACK:
	case SB_THUMBPOSITION:
		nDelta = (int)nPos - m_nScrollPos;
		break;

	case SB_PAGEUP:
		if (m_nScrollPos <= 0)
			return;
		nDelta = -min(max(nMaxPos,5),m_nScrollPos);
		break;
	
         default:
		return;
	}
	m_nScrollPos += nDelta;
	SetScrollPos(SB_VERT,m_nScrollPos,TRUE);
	
	ScrollWindow(0,-nDelta);
	CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
	ShowWindow(SW_HIDE);
	ShowWindow(SW_SHOW);
}



void COIIVerteilerBemassung::OnLButtonDown(UINT nFlags, CPoint point)
{
	m_bDragging=TRUE;
	SetCapture();
	m_ptDragPoint=point;
	SetCursor(m_hCursor1);
	WaxPartForm::OnLButtonDown(nFlags, point);
}



void COIIVerteilerBemassung::OnLButtonUp(UINT nFlags, CPoint point)
{
	m_bDragging=FALSE;
	ReleaseCapture();
	WaxPartForm::OnLButtonUp(nFlags, point);
	ShowWindow(SW_HIDE);
	ShowWindow(SW_SHOW);
}



void COIIVerteilerBemassung::OnMouseMove(UINT nFlags, CPoint point)
{
	static int iRunwithoutFlicker = 0;
	if (m_bDragging)
	{
		int nDelta=m_ptDragPoint.y-point.y;
		m_ptDragPoint=point;

		int nNewPos=m_nScrollPos+nDelta;

		if (nNewPos<0)
			nNewPos=0;
		else if (nNewPos>m_rcOriginalRect.Height() - m_nCurHeight)
			nNewPos=m_rcOriginalRect.Height() - m_nCurHeight;

		nDelta=nNewPos-m_nScrollPos;
		m_nScrollPos=nNewPos;

		SetScrollPos(SB_VERT,m_nScrollPos,TRUE);
		ScrollWindow(0,-nDelta);
		if(iRunwithoutFlicker%20 == 0)
		{
			ShowWindow(SW_HIDE);
			ShowWindow(SW_SHOW);
		}
		iRunwithoutFlicker++;
		if( iRunwithoutFlicker > 1200 )
		{
			iRunwithoutFlicker = 0;
		}
	}
	WaxPartForm::OnMouseMove(nFlags, point);
}



BOOL COIIVerteilerBemassung::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	BOOL bRet=TRUE;

	CPoint point;
	CRect rect, rectCombo;
	GetCursorPos(&point);
	GetWindowRect(&rect);
	GetDlgItem(IDC_EDIT_ANZAHL_ANSCHLUESSE)->GetWindowRect(&rectCombo);
	
	rect.top	= rectCombo.top - 20;
	rect.bottom = rectCombo.bottom + 100;
	rect.left	= rectCombo.right + rectCombo.Width()*2; 
	rect.right	= rect.right - 30; 

  	if (rect.PtInRect(point))
	{
		SetCursor(m_hCursor2);
		bRet=FALSE;
	}
	else
		bRet=WaxPartForm::OnSetCursor(pWnd, nHitTest, message);

	return bRet; 
}


