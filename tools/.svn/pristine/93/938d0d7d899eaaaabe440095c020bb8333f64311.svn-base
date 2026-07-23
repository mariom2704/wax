// mkStammdbDlg.cpp : Implementierungsdatei
//

#include "stdafx.h"
#include "mkStammdb.h"
#include "mkStammdbDlg.h"


#include "artikel.h"
#include "gruppe.h"
#include "liste.h"
#include "angebotsblatt.h"
#include "hinweistexte.h"
#include "stueckliste.h"
#include "verkaufspreise.h"
#include "produktinfo.h"

#include <mxcontrols/OpenFileDlg.h>

#include "components/KatalogCreator.h"
#include "components/KatalogLister.h"
#include "components/PreisCopier.h"

#include "helpers/SysParam.h"

using namespace MxGui;

#include <boost/boost.h>
#include <boost/boost_filesystem.h>

using namespace boost;
using namespace boost::filesystem;
using namespace MkStammDb;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

static char* sprachen[] = { "English", 

"Francais", "Nederlands", NULL };




BEGIN_MESSAGE_MAP(MkStammdbDlg, BaseDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_START, on_start)
	ON_BN_CLICKED(IDC_SPANXSTAMM, on_select_spanxstamm)
	ON_BN_CLICKED(IDC_SPANX_MDB, on_select_spanxdb)
END_MESSAGE_MAP()

MkStammdbDlg::MkStammdbDlg(CWnd* pParent) : BaseDialog(IDD_MKSTAMMDB_DIALOG, pParent)
{
	enable_window_position("MKSTAMMDB", "Spanx");
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	stammdb = NULL;
	serverdb = NULL;
}

MkStammdbDlg::~MkStammdbDlg()
{
	storeDBEntries();
	delete stammdb;
	delete serverdb;
}

void MkStammdbDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS, progress);

	if (pDX->m_bSaveAndValidate)
	{
		spanx_mdb = get_dlgitem_text(IDC_PFAD_SPANX_MDB);
		stamm_mdb = get_dlgitem_text(IDC_PFAD_SPANXSTAMM);
	}
	
}

void MkStammdbDlg::before_init_ui()
{
	spanx_mdb = (LPCSTR)AfxGetApp()->GetProfileString("DB", "spanxmdb", spanx_mdb.c_str()); 
	stamm_mdb = (LPCSTR)AfxGetApp()->GetProfileString("DB", "spanxstamm", stamm_mdb.c_str());
}


void MkStammdbDlg::init_ui()
{
	SetIcon(m_hIcon, TRUE);			// Großes Symbol verwenden
	SetIcon(m_hIcon, FALSE);		// Kleines Symbol verwenden

	set_dlgitem_text(IDC_PFAD_SPANX_MDB, spanx_mdb);
	set_dlgitem_text(IDC_PFAD_SPANXSTAMM, stamm_mdb);
	set_dlgitem_text(IDC_PROGRESS_TXT, progress_txt);
}

void MkStammdbDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // Gerätekontext für Zeichnen

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Symbol in Client-Rechteck zentrieren
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Symbol zeichnen
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

HCURSOR MkStammdbDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void MkStammdbDlg::on_start() 
{
	if (!testDB())
		return;

	show_statustxt("Stammdatenbank wird initialisiert...");
	resetStammdb();

	show_statustxt("Spanx Datenbank wird angepasst...");
	add_sprachen(get_serverdb());

	show_statustxt("Katalog wird erstellt aus " + Helpers::SysParam(get_server(), "DIR_SPL").load_str());
	create_katalog();
	
	show_statustxt("Preislisten werden verarbeitet...");
	process_spl();

	show_statustxt("Erstelle Indizes...");
	create_indizes();
	
	show_statustxt("Fertig...");
}

void MkStammdbDlg::create_indizes()
{
	struct index
	{
		std::string tab;
		std::string field;
	} indizes[] = {
		"PreislistenArtikel", "count",
		"PreislistenArtikel", "ArtikelNummer",
		"PreislistenArtikel", "Liste",
		"PreislistenArtikel", "Gruppe",
		"PreislistenArtikel", "Code",
		"PreislistenArtikel", "Kurztext",
		"PreislistenArtikel", "KurzTextEnglish",
		"PreislistenArtikel", "KurzTextFrancais",
		"PreislistenArtikel", "KurzTextNederlands",
		"", "",		
	};
	
	MXTable tbl(get_stammdb(), "stundensaetze");

	for (int i = 0; indizes[i].tab.length(); i++)
	{
		show_statustxt( str(format("Erzeuge Index auf %s->%s...") % indizes[i].tab % indizes[i].field) );
		string sql = str(format("CREATE INDEX Index_%2% ON %1%(%2%)") % indizes[i].tab % indizes[i].field);
		tbl.executeSQL(sql.c_str());
	}
	
}

void MkStammdbDlg::on_select_spanxstamm() 
{
	OpenFileDlg ofd(get_dlgitem_text(IDC_PFAD_SPANXSTAMM));

	string f = ofd.get_file();
	if (!f.length())
		return;

	set_dlgitem_text(IDC_PFAD_SPANXSTAMM, f);	
	stamm_mdb = f;
}

void MkStammdbDlg::on_select_spanxdb() 
{
	OpenFileDlg ofd(get_dlgitem_text(IDC_PFAD_SPANX_MDB));

	string f = ofd.get_file();
	if (!f.length())
		return;
	
	set_dlgitem_text(IDC_PFAD_SPANX_MDB, f);	
	spanx_mdb = f;
}

bool MkStammdbDlg::testDB()
{
	UpdateData();
	DBConnection db;
	
	string error;
	
	show_statustxt("Prüfe Datenbankverbindungen...");
	db.set_db(spanx_mdb);
	error = db.get_connection_info();
	if (error == "Nicht erreichbar")
	{
		AfxMessageBox(error.c_str(), MB_ICONERROR | MB_OK);
		return false;
	}
	delete serverdb;
	serverdb = new MXTable(db.create_connectstring(spanx_mdb).c_str(), "Systemparameter");
	
	db.set_db(stamm_mdb);
	error = db.get_connection_info();
	if (error == "Nicht erreichbar")
	{
		AfxMessageBox(error.c_str(), MB_ICONERROR | MB_OK);
		return false;
	}
	delete stammdb;
	stammdb = new MXTable(db.create_connectstring(stamm_mdb).c_str(), "stundensaetze");

	return true;
}

void MkStammdbDlg::resetStammdb()
{
	MXTable tbl(get_stammdb(), "stundensaetze");
	
	
	show_statustxt("Erzeuge Tabellen...");
	
	tbl.executeSQL("DROP TABLE Preislisten");
	tbl.executeSQL("DROP TABLE PreislistenGruppen");
	tbl.executeSQL("DROP TABLE PreislistenArtikel");
	tbl.executeSQL("DROP TABLE Stuecklisten");
	tbl.executeSQL("DROP TABLE angebotsblatt");
	tbl.executeSQL("DROP TABLE HinweisTexte");
	tbl.executeSQL("DROP TABLE Textbausteine");
	tbl.executeSQL("DROP TABLE verkaufspreise");
	tbl.executeSQL("DROP TABLE produktinfo");

	
	tbl.executeSQL("CREATE TABLE Preislisten (Code varchar(254), Datum DATETIME, Beschreibung varchar(254), email varchar(254), PRIMARY KEY (Code))");
	tbl.executeSQL("CREATE TABLE PreislistenGruppen (Code varchar(254),	Liste varchar(254), sparte INT, Beschreibung varchar(254), PRIMARY KEY (Code))");
	tbl.executeSQL("CREATE TABLE PreislistenArtikel (Liste varchar(255), Gruppe varchar(255), code varchar(255), \
		count int, ArtikelNummer varchar(50), Montagezeit varchar(15), WM varchar(15), WL varchar(15), \
		KurzText varchar(255), LangText memo, LVText memo, \
		Hinweis varchar(255), posnr varchar(255), IstFremdleistung INT, IstSprinkler INT, IstGeloescht INT, IstGeaendert INT, IstNeu INT, GeaendertVon varchar(20), GeaendertAm varchar(20), \
		gewicht DOUBLE, festpreis INT, \
		  PRIMARY KEY (code))");
	

	tbl.executeSQL("CREATE TABLE HinweisTexte (nIndex varchar(50), HinweisText memo, PRIMARY KEY (nIndex))");

	tbl.executeSQL("CREATE TABLE Stuecklisten(Code varchar(254), Anzahl varchar(50),  Artikelnummer varchar(254), Bezeichnung varchar(254),  PRIMARY KEY (Code, Artikelnummer))");
	tbl.executeSQL("CREATE TABLE angebotsblatt (code varchar(255), blatt varchar(255), PRIMARY KEY (code))");
	tbl.executeSQL("CREATE TABLE InstallierteSprachen(sprache varchar(255), PRIMARY KEY(sprache))");
	tbl.executeSQL("CREATE TABLE Textbausteine(Code varchar(255), RtfText memo, Bemerkung varchar(255), PRIMARY KEY(Code))");
	tbl.executeSQL("CREATE TABLE verkaufspreise (code varchar(255), preis double, doc varchar(255), PRIMARY KEY (code))");
	tbl.executeSQL("CREATE TABLE produktinfo (code varchar(255),  datei varchar(255), PRIMARY KEY (code))");
	

	for (int i = 0; sprachen[i]; i++)
	{
		CString sql;
		sql.Format("INSERT INTO InstallierteSprachen (sprache) VALUES ('%s')", sprachen[i]);
	}
	add_sprachen(get_stammdb());
}

void MkStammdbDlg::add_sprachen(DWORD db)
{
	MXTable tbl(db, "Systemparameter");

	struct fields {
		char* tab;
		char* field;
		bool memo;
	} _fields[] =
	{
		"PreislistenArtikel", "KurzText", false, 
		"PreislistenArtikel", "LangText", true,
		"PreislistenArtikel", "LVText", true,
		"PreislistenGruppen", "Beschreibung", false,
		"Preislisten", "Beschreibung", false,
		"HinweisTexte", "HinweisText", true,
		"Systemparameter", "Bemerkung", false,
		"Textbausteine", "Bemerkung", false,
		"Textbausteine", "RtfText", true,
		NULL, NULL, false,
	};

	for (int i = 0; _fields[i].tab; i++)
	{
		for (int j = 0; sprachen[j]; j++)
		{
			CString sql;
			sql.Format("ALTER TABLE %s ADD COLUMN %s%s %s", _fields[i].tab, _fields[i].field, sprachen[j], _fields[i].memo ? "memo" : "varchar(255)");
			tbl.executeSQL(sql);
		}
	}
}

void MkStammdbDlg::storeDBEntries()
{
	AfxGetApp()->WriteProfileString("DB", "spanxmdb", spanx_mdb.c_str()); 
	AfxGetApp()->WriteProfileString("DB", "spanxstamm", stamm_mdb.c_str());
}

void MkStammdbDlg::show_statustxt(const std::string& txt)
{
	set_dlgitem_text(IDC_PROGRESS_TXT, txt);
}


bool MkStammdbDlg::create_katalog()
{
	std::string splpath = Helpers::SysParam(get_server(), "DIR_SPL").load_str();

	DB::KatalogCreator creator(get_stamm(), splpath);

	MxBase::Error err = creator.run();
	return err.is_valid() ? false : true;
}


void MkStammdbDlg::process_spl()
{
	/**
	Aus den Anweisungen unten hätte ich eine Methode gemacht. Leider nicht möglich: preislistenTable als
	generische Basisklasse gibt nicht den richtigen Konstruktor her und als Funktions-Template gibt VS 
	nur merkwürdige Fehler aus. War bei xmove ähnlich, habe ich aber irgendwann hinbekommen...
	*/

	DB::KatalogLister lister(get_stamm());
	std::set<std::string> lstlst = lister.get_lists();
	std::set<std::string> grplst = lister.get_groups();
	std::set<std::string> codelst = lister.get_positions();


	std::set<std::string> stklst;
	std::set<std::string> hwlst;

	set<string>::iterator it;


	progress.SetStep(1);
	progress.SetRange32(0, lstlst.size());
	progress.SetPos(0);

	progress.SetRange32(0, codelst.size());
	progress.SetPos(0);
	show_statustxt("Erstelle Liste mit Hinweistexten...");

	set<string>::iterator codeit;
	string dbgcur;
	for (codeit = codelst.begin(); codeit != codelst.end(); codeit++)
	{
		std::string sql = boost::str(boost::format("SELECT Hinweis FROM PreislistenArtikel WHERE code = '%s'") % *codeit);
		std::string hwtxt = get_server()->exec_SQL(sql);
		if (hwtxt.length() > 1)
			hwlst.insert(hwtxt);
		progress.StepIt();
	}	


	show_statustxt("Listen kopieren...");
	for (it = lstlst.begin(); it != lstlst.end(); it++)
	{
		liste ll(get_serverdb());
		xmove(ll, it->c_str());
		progress.StepIt();
	}

	progress.SetRange32(0, codelst.size());
	progress.SetPos(0);
	show_statustxt("Preise kopieren...");

	DB::PreisCopier* pc = new DB::PreisCopier(get_server(), get_stamm());
	for (it = codelst.begin(); it != codelst.end(); it++)
	{
		pc->copy(*it);
		progress.StepIt();
	}
	delete pc;

	show_statustxt("Stücklisten kopieren...");
	stueckliste stk(get_serverdb(), "%");
	int size = stk.load();
	
	progress.SetStep(1);

	progress.SetRange32(0, size);
	progress.SetPos(0);
	

	while (!stk.tbl.isEof())
	{
		stueckliste newstk(get_stammdb());
		set<string>::iterator cit;
		for (cit = codelst.begin(); cit != codelst.end(); cit++)
		{
			if (*cit == stk.get_code())
				break;
		}
		if (cit != codelst.end())
		{
			newstk.copy(stk, -1);
			stklst.insert(stk.get_code());
		}
		stk.tbl.moveNext();
		progress.StepIt();
	}


	progress.SetRange32(0, hwlst.size());
	progress.SetPos(0);
	show_statustxt("Hinweistexte kopieren...");
	for (it = hwlst.begin(); it != hwlst.end(); it++)
	{
		hinweistexte hw(get_serverdb());
		xmove(hw, it->c_str());
		progress.StepIt();
	}

	progress.SetRange32(0, codelst.size());
	progress.SetPos(0);
	show_statustxt("Artikel kopieren...");
	
	int i = 0;
	for (codeit = codelst.begin(); codeit != codelst.end(); codeit++, i++)
	{
		artikel aa(get_serverdb());
		xmove(aa, codeit->c_str(), i);
		progress.StepIt();
	}

	progress.SetRange32(0, grplst.size());
	progress.SetPos(0);
	show_statustxt("Gruppen kopieren...");
	for (it = grplst.begin(); it != grplst.end(); it++)
	{
		gruppe gg(get_serverdb());
		xmove(gg, it->c_str());
		progress.StepIt();
	}

	progress.SetRange32(0, codelst.size());
	progress.SetPos(0);
	show_statustxt("Angebotsblätter kopieren...");
	for (codeit = codelst.begin(); codeit != codelst.end(); codeit++)
	{
		angebotsblatt ab(get_serverdb());
		xmove(ab, codeit->c_str());
		progress.StepIt();
	}

	progress.SetRange32(0, codelst.size());
	progress.SetPos(0);
	show_statustxt("Produktinfo kopieren...");
	for (codeit = codelst.begin(); codeit != codelst.end(); codeit++)
	{
		produktinfo pi(get_serverdb());
		xmove(pi, codeit->c_str());
		progress.StepIt();
	}

	progress.SetRange32(0, codelst.size());
	progress.SetPos(0);
	show_statustxt("Verkaufspreise kopieren...");
	for (codeit = codelst.begin(); codeit != codelst.end(); codeit++)
	{
		verkaufspreise vk(get_serverdb());
		xmove(vk, codeit->c_str());
		progress.StepIt();
	}

}




MXDatabase* MkStammdbDlg::get_stamm() 
{ 
	return (MXDatabase*)stammdb->get_Db(); 
}

MXDatabase* MkStammdbDlg::get_server() 
{ 
	return (MXDatabase*)serverdb->get_Db(); 
}

DWORD MkStammdbDlg::get_stammdb() 
{ 
	return stammdb->get_Db(); 
}

DWORD MkStammdbDlg::get_serverdb() 
{ 
	return serverdb->get_Db(); 
}
