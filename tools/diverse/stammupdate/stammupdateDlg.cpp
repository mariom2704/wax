#include "stdafx.h"
#include "stammupdate.h"
#include "stammupdateDlg.h"
#include <mxodbc\mxodbc.h>
#include <mxcontrols/OpenFiledlg.h>
#include <math.h>
#include "MaterialStamm/SapTextFile.h"
#include <MxControls/MsgBox.h>

using namespace MxGui;

#include "DB/Detect.h"
#include "DB/SpanxStammdb.h"
#include "DB/WaxStammdb.h"
#include "DB/MXDMStammdb.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


BEGIN_MESSAGE_MAP(StammupdateDlg, CDialog)
	ON_BN_CLICKED(IDOK, on_update)
	ON_BN_CLICKED(IDC_SELECT_SAP, on_select_matstamm)
	ON_BN_CLICKED(IDC_SELECT_DB, on_select_db)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


StammupdateDlg::StammupdateDlg() : BaseDialog(IDD_WAXSTAMMUPDATE_DIALOG)
{
	sap_txt = (LPCSTR)AfxGetApp()->GetProfileString("Database", "sap", "SAP Textdatei.txt");
	db = AfxGetApp()->GetProfileString("Database", "db", "Spanx/Wax Datenbank.mdb");
}

void StammupdateDlg::on_select_db()
{
	std::string fn = MxGui::OpenFileDlg(get_dlgitem_text(IDC_DB)).get_file();
	if (fn.length())
		set_dlgitem_text(IDC_DB, fn);

	test_db(fn);
}


void StammupdateDlg::test_db(const std::string& fn)
{
	DB::Detect::Type tp = DB::Detect(fn).get_type();
	switch (tp)
	{
		case DB::Detect::Spanx:
			set_status_text("Spanx Datenbank ausgewählt");
			break;
		case DB::Detect::Wax:
			set_status_text("Wax Datenbank ausgewählt");
			break;
		case DB::Detect::MXDM:
			set_status_text("MxDesignManager Datenbank ausgewählt");
			break;
		default:
			set_status_text("Unbekannte Datenbank ausgewählt");
			break;
	}
}

void StammupdateDlg::set_status_text(const std::string& txt)
{
	set_dlgitem_text(IDC_STATUS,txt);
}

void StammupdateDlg::on_select_matstamm()
{
	std::string fn = MxGui::OpenFileDlg(get_dlgitem_text(IDC_SAP)).get_file();
	if (fn.length())
		set_dlgitem_text(IDC_SAP, fn);
}

void StammupdateDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	if (pDX->m_bSaveAndValidate)
	{
		db = get_dlgitem_text(IDC_DB);
		sap_txt = get_dlgitem_text(IDC_SAP);

		AfxGetApp()->WriteProfileString("Database", "sap", sap_txt.c_str() );
		AfxGetApp()->WriteProfileString("Database", "db", db.c_str());
	}
}

void StammupdateDlg::init_ui()
{
	set_icon(IDR_MAINFRAME);
		
	set_dlgitem_text(IDC_DB, db);
	
	set_dlgitem_text(IDC_SAP, sap_txt);
}

void StammupdateDlg::on_update() 
{
	UpdateData();

	DB::Detect::Type tp = DB::Detect(db).get_type();
	if ( (tp != DB::Detect::Spanx) && (tp != DB::Detect::Wax) && (tp != DB::Detect::MXDM) )
	{
		set_status_text("Falsche Datenbank: Zur Zeit sind nur Spanx, Wax und MXDM implementiert");
		return;
	}
		
	set_dlgitem_text(IDC_STATUS, "Lade SAP Stammdaten...");
	progress_start();


	std::vector<MaterialStamm::Data> stammdat;


	DB::Detect::Type tpstamm = DB::Detect(sap_txt).get_type();

	if (tpstamm == DB::Detect::Spanx)
	{
		DB::SpanxStammdb spanxstamm(sap_txt);
		spanxstamm.signal_percent_loaded.connect( boost::bind(&StammupdateDlg::progress_step, this, _1) );
		spanxstamm.load();
		stammdat = spanxstamm.get_data();
	}
	else
	{
		MaterialStamm::SapTextFile sapfile(sap_txt);
		sapfile.signal_percent_loaded.connect( boost::bind(&StammupdateDlg::progress_step, this, _1) );
		sapfile.load();
		stammdat = sapfile.get_data();
	}

	
	  

	progress_end();
	
	set_dlgitem_text(IDC_STATUS, "Update Tabelle Preise...");
	progress_start();

	if (tp == DB::Detect::Spanx)
	{
		DB::SpanxStammdb spanx(db);
		spanx.signal_percent_updated.connect( boost::bind(&StammupdateDlg::progress, this, _1, _2) );
		if ( spanx.update( stammdat ) )
			set_dlgitem_text(IDC_STATUS, "Update Tabelle Preise erfolgreich");
		else
		{
			MsgBox::show_error(spanx.error);
			set_dlgitem_text(IDC_STATUS, "Update Tabelle Preise fehlgeschlagen!");
		}
	}
	else if (tp == DB::Detect::Wax)
	{
		DB::WaxStammdb wax(db);
		wax.signal_percent_updated.connect( boost::bind(&StammupdateDlg::progress, this, _1, _2) );
		if ( wax.update( stammdat ) )
			set_dlgitem_text(IDC_STATUS, "Update Tabelle SpanxPreise erfolgreich");
		else
		{
			MsgBox::show_error(wax.error);
			set_dlgitem_text(IDC_STATUS, "Update Tabelle SpanxPreise fehlgeschlagen!");
		}
	}
	else 
	{
		set_dlgitem_text(IDC_STATUS, "Update Tabelle ArtikelTexte...");

		DB::MXDMStammdb mxdm(db);
		mxdm.signal_percent_updated.connect( boost::bind(&StammupdateDlg::progress, this, _1, _2) );
		if ( mxdm.update( stammdat ) )
			set_dlgitem_text(IDC_STATUS, "Update Tabelle ArtikelTexte erfolgreich");
		else
		{
			MsgBox::show_error(mxdm.error);
			set_dlgitem_text(IDC_STATUS, "Update Tabelle ArtikelTexte fehlgeschlagen!");
		}
	}
	progress_end();
}


void StammupdateDlg::progress(int percent, int artikelnr)
{
	progress_step(percent);
	std::string msg = boost::str(boost::format("Update Artikelnr: %i") % artikelnr);
	set_status_text(msg);
}

