#include "stdafx.h"
#include "mxparam_dll.h"

#include "PropPageSystem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

USE_PARAID

IMPLEMENT_DYNCREATE(PropPageSystem, WaxPropPage)

BEGIN_MESSAGE_MAP(PropPageSystem, WaxPropPage)
	ON_EN_UPDATE(IDC_PARA_STAMMDB,		onParaUpdate)
	ON_BN_CLICKED(IDC_STAMMDB,			onStammDBpfad)

	ON_EN_CHANGE(IDC_PARA_SERVERDB,		onParaUpdate)
	ON_EN_CHANGE(IDC_PARA_MAXPOSPERSA,	onParaUpdate)
	ON_EN_CHANGE(IDC_PARA_MAXPOSPERSITE,onParaUpdate)

	ON_BN_CLICKED(IDC_PARA_AV,			onParaUpdate)
	ON_BN_CLICKED(IDC_PARA_DEBUGNL,		onParaUpdate)

	ON_BN_CLICKED(IDC_STANDARD_SYSTEM,	onStandard)
END_MESSAGE_MAP()

/// Destructor
PropPageSystem::~PropPageSystem()
{
}

/// Konstructor
PropPageSystem::PropPageSystem() : WaxPropPage(IDD_PARA_SYSTEM)
{

}

/// Default-Werte setzen
void PropPageSystem::setDefault()
{
	paraStammDB			=  MxParameter::getDefvalStr(STAMMDB);
	paraServerDB		=  MxParameter::getDefvalStr(SERVERDB);

	paraMaxPosPerSA		=  MxParameter::getDefvalInt(MAXPOSPERSA);
	paraMaxPosPerSite	=  MxParameter::getDefvalInt(MAXPOSPERSITE);

	paraAV				= (MxParameter::getDefvalInt(AV)		== 1) ? true : false;
	paraDebugNL			= (MxParameter::getDefvalInt(DEBUGNL)	== 1) ? true : false;
}

/// Parameter auf Existenz checken
void PropPageSystem::checkPara()
{
	// Parameter in Registry od. Waxuser.mdb suchen.
	// Wenn nicht gefunden, dann neu setzen.
	checkparabool checkpb[] = 
	{
		AV,				paraAV,						true,  false, 0,//IDS_KEY_AV,
		DEBUGNL,		paraDebugNL,				true,  false, 0,//IDS_KEY_DEBUGNL,
		0,				paraDebugNL,				false, false, 0,
	};
	checkParameter(checkpb); 

	checkparaint checkpi[] = 
	{
		MAXPOSPERSA,	paraMaxPosPerSA,			true,  false, 0,//IDS_KEY_SAMMELAUFTRAG_MAXPOS,
		MAXPOSPERSITE,	paraMaxPosPerSite,			true,  false, 0,//IDS_KEY_PR_MAX_POS_PER_SITE,
		0,				paraMaxPosPerSite,			false, false, 0,
	};
	checkParameter(checkpi); 

	checkparastring checkps[] = 
	{
		STAMMDB,		paraStammDB.GetBuffer(1),	true,  false, 0,//IDS_KEY_STAMMDB,
		SERVERDB,		paraServerDB.GetBuffer(1),	true,  false, 0,//IDS_KEY_SERVERDB,
		0,				paraServerDB.GetBuffer(1),	false, false, 0,
	};
	checkParameter(checkps); 
}

/// Dialog initialisieren
BOOL PropPageSystem::OnInitDialog() 
{
	WaxPropPage::OnInitDialog();

	bool en1 = enableItem(this, IDC_PARA_STAMMDB,		IDS_KEY_STAMMDB);
	bool en2 = enableItem(this, IDC_PARA_SERVERDB,		IDS_KEY_SERVERDB);
	bool en3 = enableItem(this, IDC_PARA_MAXPOSPERSA,	IDS_KEY_SAMMELAUFTRAG_MAXPOS);
	bool en4 = enableItem(this, IDC_PARA_MAXPOSPERSITE,	IDS_KEY_PR_MAX_POS_PER_SITE);
	bool en5 = enableItem(this, IDC_PARA_AV,			IDS_KEY_AV);
	bool en6 = enableItem(this, IDC_PARA_DEBUGNL,		IDS_KEY_DEBUG_NL);
	bool en7 = enableItem(this, IDC_STAMMDB,			IDS_KEY_STAMMDB);
	
	if (!en1 || !en2 || !en3 || !en4 || !en4 || !en6 || !en7)
        enableItem(this, IDC_STANDARD_SYSTEM, 0);

	paraStammDB			=  MxParameter::FindParaStr(STAMMDB);
	paraServerDB		=  MxParameter::FindParaStr(SERVERDB);
	paraMaxPosPerSA		=  MxParameter::FindParaInt(MAXPOSPERSA);
	paraMaxPosPerSite	=  MxParameter::FindParaInt(MAXPOSPERSITE);
	paraAV				= (MxParameter::FindParaInt(AV)		== 1) ? true : false;
	paraDebugNL			= (MxParameter::FindParaInt(DEBUGNL)== 1) ? true : false;

#ifdef _DEBUG 
	GetDlgItem(IDC_PARA_AV)->ModifyStyle( NULL, WS_VISIBLE);
	GetDlgItem(IDC_PARA_DEBUGNL)->ModifyStyle(NULL, WS_VISIBLE);
#endif

	UpdateData(FALSE);

	return TRUE;  
}

/// Datenaustausch
void PropPageSystem::DoDataExchange(CDataExchange* pDX)
{
	WaxPropPage::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_PARA_STAMMDB,		  paraStammDB);
	DDX_Text(pDX, IDC_PARA_SERVERDB,	  paraServerDB);

	DDX_Text(pDX, IDC_PARA_MAXPOSPERSA,   paraMaxPosPerSA);
	DDX_Text(pDX, IDC_PARA_MAXPOSPERSITE, paraMaxPosPerSite);

	BOOL checkAV      = paraAV		? 1 : 0;
	BOOL checkDebugNL = paraDebugNL ? 1 : 0;
	
	DDX_Check(pDX, IDC_PARA_AV,		 checkAV);
	DDX_Check(pDX, IDC_PARA_DEBUGNL, checkDebugNL);

	paraAV      = (checkAV		== 1) ? true : false; 
	paraDebugNL = (checkDebugNL == 1) ? true : false; 
}

/// Allgemeine Einstellungen in Datenbank eintragen
BOOL PropPageSystem::OnApply() 
{
	if (!GetModified())
		return true;
	SetModified(false);

	SysParm->Update(IDS_KEY_STAMMDB, paraStammDB);

	if (((CEdit*)GetDlgItem(IDC_PARA_SERVERDB))->GetModify())
		SysParm->Update(IDS_KEY_SERVERDB, paraServerDB);

	if (((CEdit*)GetDlgItem(IDC_PARA_MAXPOSPERSA))->GetModify())
		SysParm->Update(IDS_KEY_SAMMELAUFTRAG_MAXPOS, paraMaxPosPerSA);

	if (((CEdit*)GetDlgItem(IDC_PARA_MAXPOSPERSITE))->GetModify())
		SysParm->Update(IDS_KEY_PR_MAX_POS_PER_SITE, paraMaxPosPerSite);

	SysParm->Update(IDS_KEY_AV,		  paraAV);
	SysParm->Update(IDS_KEY_DEBUG_NL, paraDebugNL);

	return WaxPropPage::OnApply();
}

/// Auswahl des Datenbankpfades per Dateipfad
void PropPageSystem::onStammDBpfad() 
{
	selectPfad(paraStammDB);
	onParaUpdate();
}

/// Datei Dialog
bool PropPageSystem::selectPfad(CString& pfad)
{
	CString file, initpath;

	initpath = pfad;

	CFileDialog fo(TRUE, NULL, NULL, 
		OFN_EXPLORER|OFN_PATHMUSTEXIST | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR , 
		"Wax-DB (*.mdb)|*.mdb|Alle Dateien (*.*)|*.*||", this);
	
	fo.m_ofn.lpstrInitialDir = initpath;
	fo.m_ofn.lpfnHook=NULL;
	fo.m_ofn.Flags &= ~OFN_ENABLEHOOK;
	fo.m_ofn.hwndOwner = this->m_hWnd;

	if (GetOpenFileName(&fo.m_ofn))
	{
		pfad = fo.GetPathName();
		UpdateData(FALSE);
		Sleep(100);// Workaround für 'Öffnen schlägt fehl wg. Zugriffverletzung'
		return true;
	}
	return false;
}
