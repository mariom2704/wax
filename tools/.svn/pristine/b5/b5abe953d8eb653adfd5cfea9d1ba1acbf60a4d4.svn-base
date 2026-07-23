#pragma once

#include <mxodbc\mxodbc_lib.h>
#include <mxcontrols/basedialog.h>
#include "preislistentbl.h"

#include <mxutil/tracer.h>
#include <boost/boost.h>

class MkStammdbDlg : public BaseDialog
{
	DECLARE_MESSAGE_MAP()
// Konstruktion
public:
	MkStammdbDlg(CWnd* pParent = NULL);	// Standard-Konstruktor
	~MkStammdbDlg();

	template<class T> void xmove(T &pos, const CString& code, int lfdnr = -1)
	{
		TRY
		{
			CString cd(code);
			cd.TrimLeft();
			cd.TrimRight();
			TRACE("%s\n", code);
			pos.cur_code = cd;
			int count = pos.load();
			if (count < 1)
			{
				MxUtil::Tracer::trace2file("trace.txt", boost::str(boost::format("%s : %s\n") % pos.tblname() % (LPCSTR)cd));
				return;
			}
			
			T newpos(get_stammdb(), cd);
			newpos.copy(pos, lfdnr);
			pos.tbl.close();
		}
		CATCH(CNotSupportedException, e)
		{
		}
		AND_CATCH(CDBException, de)
		{
			de->ReportError();
		}
		END_CATCH

	};

	CProgressCtrl	progress;
	std::string	spanx_mdb;
	std::string	stamm_mdb;
	std::string	progress_txt;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);

	HICON m_hIcon;

	virtual void before_init_ui();

	virtual void init_ui();

	void OnPaint();
	HCURSOR OnQueryDragIcon();
	void on_start();
	
private:
	void on_select_spanxstamm();

	void on_select_spanxdb();

	MXTable *stammdb;
	MXTable *serverdb;

	MXDatabase* get_stamm();
	MXDatabase* get_server();
	
	DWORD get_stammdb();
	DWORD get_serverdb();
	

	void add_sprachen(DWORD db);
	
	bool create_katalog();
	
	void process_spl();
	
	void create_indizes();

	void show_statustxt(const std::string& txt);

	bool testDB();

	void storeDBEntries();

	void resetStammdb();
};


