#pragma once

#include <mxcontrols/basedialog.h>


class StammupdateDlg : public BaseDialog   
{
	DECLARE_MESSAGE_MAP()
public:
	StammupdateDlg();


protected:
	virtual void DoDataExchange(CDataExchange* pDX);

	virtual void init_ui();
	
	void on_update();

private:

	void progress(int percent, int artikelnr);

	/// Datenbank auswählen
	void on_select_db();

	/// Testet den Typ der Datenbank
	void test_db(const std::string& fn);

	/// SAP Materialstamm Textdatei auswählen
	void on_select_matstamm();

	/// Statustext ausgeben
	void set_status_text(const std::string& txt);

	/// Pfad zur SAP Textdatei
	std::string sap_txt;

	/// Pfad zur Wax/Spanx Datenbank
	std::string db;

};
