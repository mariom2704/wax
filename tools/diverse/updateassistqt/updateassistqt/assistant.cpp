#include <string>
#include <fstream>
#include <strstream>
#include "assistant.h"
#include <windows.h>
#include <qmessagebox>
#include <qprogressdialog>
#include <stdio.h>  /* defines FILENAME_MAX */

    #include <direct.h>
    #define GetCurrentDir _getcwd


using namespace std;

//#pragma warning (disable:4996) // fopen als veraltet deklariert

assistant::assistant()
{
	list  = NULL;
	reset_actions();
}

assistant::~assistant()
{
}

void assistant::set_model(QTableWidget* md)
{
	list = md;
}

void assistant::reset_actions()
{
	actionflags = 0;
}

void assistant::add_action(enum assistant::actions actflag)
{
	actionflags |= actflag;
}

void assistant::remove_action(enum assistant::actions actflag)
{
	actionflags &= ~actflag;
}


void assistant::process_stdnl(const string& nl)
{
	if (!actionflags)
		return;

	string dbq;

	const string mxdm_zendir = "\\vol1\\Install\\Novell\\zen\\Tech\\Ins_MXDesignManager\\";
	const string mxdm_test_zendir = "\\vol1\\Install\\Novell\\zen\\Tech\\Ins_MXDesignManager_Test_Version\\";

	const string spanx_dir = "\\vol1\\gruppe\\nl\\spanx\\";
	const string spanx_zendir = "\\vol1\\install\\novell\\zen\\allgemein\\ins_spanx_lokal\\";
	
	const string wax_dir = "\\vol1\\gruppe\\nl\\waxplus\\";
	const string wax_zendir = "\\vol1\\install\\novell\\zen\\tech\\ins_waxplus_lokal\\";
	
	vector<string> check_last_update_list;
	vector<string> check_version_info_list;
	
	if (do_action(SPANXDB))
	{
		check_last_update_list.push_back(nl + spanx_dir + "spanx.mdb");
		check_last_update_list.push_back(nl + spanx_zendir + "spanx.mdb"); 
	}

	if (do_action(WAXDB))
	{
		check_last_update_list.push_back(nl + wax_dir + "waxuser.mdb");
		check_last_update_list.push_back(nl + wax_zendir + "waxuser.mdb");
	}

	if (do_action(SPANX_EXE))
	{
		check_version_info_list.push_back(nl + spanx_dir + "spanx.exe");
		check_version_info_list.push_back(nl + spanx_zendir + "spanx.exe");
	}

	if (do_action(SPLEDIT_EXE))
	{
		check_version_info_list.push_back(nl + spanx_dir + "spledit.exe");
		check_version_info_list.push_back(nl + spanx_zendir + "spledit.exe");
	}

	if (do_action(WAX_EXE))
	{
		const string wax[] = { "wax.exe", "mxparam.dll", "rohrkonst.dll", "waxgl.dll", "waxint.dll", "waxprj.dll", "waxstamm.dll", "waxui.dll" , "" };

		for (int i = 0; wax[i].length(); i++)
			check_version_info_list.push_back(nl + wax_dir + wax[i]);
		for (int i = 0; wax[i].length(); i++)
			check_version_info_list.push_back(nl + wax_zendir + wax[i]);
	}

	if (do_action(WAX_EXE_ONLY))
	{
		check_version_info_list.push_back(nl + wax_dir + "wax.exe");
		check_version_info_list.push_back(nl + wax_zendir + "wax.exe");
	}

	if (do_action(STAMMDB_WAX))
	{
		check_version_info_list.push_back(nl + wax_dir + "waxstamm.dat");
		check_version_info_list.push_back(nl + wax_zendir + "waxstamm.dat");
	}

	if (do_action(STAMMDB_SPANX))
	{
		check_version_info_list.push_back(nl + spanx_dir + "spanxstamm.mdb");
		check_version_info_list.push_back(nl + spanx_zendir + "spanxstamm.mdb");
	}

	if (do_action(MXDM))
	{
		check_version_info_list.push_back(nl + mxdm_zendir + "MXDesignManager-setup.exe");
		check_version_info_list.push_back(nl + mxdm_zendir + "MXDesignManager-setup64.exe");
		check_version_info_list.push_back(nl + mxdm_test_zendir + "MXDesignManager-setup.exe");
		check_version_info_list.push_back(nl + mxdm_test_zendir + "MXDesignManager-setup64.exe");
	}


	//for (vector<string>::iterator it = check_last_update_list.begin(); it != check_last_update_list.end(); it++)
	//	checkLastUpdate(*it);

	for (vector<string>::iterator it = check_version_info_list.begin(); it != check_version_info_list.end(); it++)
		check_version_info(*it);
}


void assistant::check_version_info(string fname, int index)
{
	if (list)
	{
		string ver = get_version_info((LPSTR)fname.c_str());
		string tmstamp = get_timestamp(fname);
		check_file_locked(add_output_string(fname, ver, tmstamp, index), fname);
	}
}

string assistant::get_timestamp(string fname)
{
	FILETIME CreationTime;
	FILETIME LastAccessTime;
	FILETIME LastWriteTime;
	
	
	HANDLE hf = ::CreateFileA(fname.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (!::GetFileTime(hf, &CreationTime, &LastAccessTime, &LastWriteTime) || hf == INVALID_HANDLE_VALUE)
		return "-";
	::CloseHandle(hf);
	SYSTEMTIME st;
	FileTimeToSystemTime(&LastWriteTime, &st);
	ostrstream ret;

	ret << st.wDay << "." << st.wMonth << "." << st.wYear << " " << st.wHour << ":" << st.wMinute << ends;
	return ret.str();
}

int assistant::checkLastUpdate(string dbq)
{
	/*

	TRY
	{
		CString cur_sv;
		DBConnection db;
		string error;
		
		db.set_serverdbentry(dbq.c_str());
		db.get_connection_info((LPCSTR)db.get_serverdb(), error);
		if (error.length())
		{
			if (listbx)
				check_file_locked(add_output_string((string)dbq, (string)error), dbq);
			return -1;
		}
		

		MXTable tbl(db.get_serverdb(), "UpdateLog");	

		tbl.getDatabaseConnect();
		
		DECLARE_COL(String, bezeichnung, tbl);
		
		
		tbl.load("", "Datum DESC");
		if (listbx)
		{
			check_file_locked(add_output_string((string)dbq, (string)*bezeichnung.data), dbq);
		}
	}
	CATCH(CDBException, e)
	{
		if (listbx)
			add_output_string((string)e->m_strError);
	}
	END_CATCH
*/
	return 0;
	
}

std::string assistant::get_version_info(char* szFile)
{
	DWORD dwLen, dwUseless;
    LPCSTR lpVI;
    std::string companyName;
    UINT verMajor = 0, verMinor = 0, verSubMajor = 0, verSubMinor = 0;

    dwLen = GetFileVersionInfoSizeA((LPCSTR)szFile, &dwUseless);
    if (dwLen==0)
        return (std::string)"-";

    lpVI = (LPCSTR) GlobalAlloc(GPTR, dwLen);
    if (lpVI)
    {
        DWORD dwBufSize;
        VS_FIXEDFILEINFO* lpFFI;
        BOOL bRet = FALSE;

        GetFileVersionInfoA((LPCSTR)szFile, NULL, dwLen, (LPVOID)lpVI);

        if (VerQueryValueA((LPVOID)lpVI, "\\",(LPVOID *) &lpFFI, (UINT *) &dwBufSize))

        {
            //We now have the VS_FIXEDFILEINFO in lpFFI
            verMajor = HIWORD(lpFFI->dwFileVersionMS);
			verMinor = LOWORD(lpFFI->dwFileVersionMS);
			verSubMajor = HIWORD(lpFFI->dwFileVersionLS);
			verSubMinor = LOWORD(lpFFI->dwFileVersionLS);
			
        }
        //Cleanup
        GlobalFree((HGLOBAL)lpVI);
		
    }
	QString ver;
	ver.sprintf("%i.%i.%i.%i", verMajor, verMinor, verSubMajor, verSubMinor);
	return ver.toStdString();
}

int assistant::add_output_string(const string &item, const string &info, const string &info_second, int idx) const
{
	list->setUpdatesEnabled(false);
	if (idx == -1)
	{
		idx = list->rowCount();
		list->insertRow(idx);
		list->setRowHeight(idx, 16);
	}
	QTableWidgetItem* base = new QTableWidgetItem(item.c_str());
	list->setItem(idx, 0, base);
	
	QTableWidgetItem* version = new QTableWidgetItem(info.c_str());
	version->setTextAlignment(Qt::AlignHCenter);
	list->setItem(idx, 1, version);
	
	QTableWidgetItem* datum = new QTableWidgetItem(info_second.c_str());
	datum->setTextAlignment(Qt::AlignHCenter);
	list->setItem(idx, 2, datum);
	
	list->scrollToBottom();
	list->setUpdatesEnabled(true);
	return idx;
}

void assistant::check_file_locked(int index, const string& fname) const
{
	bool check = false;

	if (fname.substr(fname.length()-3, 3) == "mdb")
	{
		string ldb(fname);
		ldb.replace(fname.length()-3, 3, "ldb");
		FILE* f = fopen(ldb.c_str(), "r");
		if (f)
		{
			check = true;
			fclose(f);
		}
	}
	else
	{
		FILE* f = fopen(fname.c_str(), "a+");
		if (!f)
			check = true;
		else
			fclose(f);
	}
	QTableWidgetItem *newItem = new QTableWidgetItem(check ? "x" : " ");
	newItem->setTextAlignment(Qt::AlignHCenter);
	list->setItem(index, 3, newItem);
}


bool assistant::read_ini(string entry, vector<string>& lst)
{
	lst.clear();



 char cCurrentPath[FILENAME_MAX];

 GetCurrentDir(cCurrentPath, sizeof(cCurrentPath) / sizeof(TCHAR));

	ifstream ini(".\\updateassist.ini");
	if (!ini)
	{
		QMessageBox::information(NULL, "-", "updateassist.ini konnte nicht geöffnet werden");
		return false;
	}

	if (*entry.begin() != '[')
		entry = "[" + entry + "]";

	bool tag = false;
	
	while (!ini.eof())
	{
		string line;
		ini >> line;
		if (!line.length() )
			continue;
		if (line.at(0) == '#')
			continue;
		
		if (line == entry)
		{
			tag = true;
			continue;
		}
		if (tag && line.at(0) == '[')
		{
			tag = false;
			continue;
		}
		
		if (tag)
		{
			lst.push_back(line);
		}
	}
	return true;
}

string assistant::do_update(const string file)
{
	
	string exe(file.c_str());

	int index = exe.rfind('\\');
	string path = exe.substr(0, index);
	exe = exe.substr(index+1, exe.length() - index - 1);

	vector<string> origdir;
	read_ini(exe + "-sourcedir", origdir);
	if (origdir.size() < 1)
		return (string)"Quellverzeichnis nicht angegeben unter '[xxx.exe-sourcedir]'";
	
	vector<string> filestocopy;
	read_ini(exe, filestocopy);
	if (filestocopy.size() <= 0)
		return (string)"Dateien fürs Update nicht angegeben unter '[xxx.exe]'";
	
	QProgressDialog xx(file.c_str(), "Cancel", 0, filestocopy.size() );
	xx.show();
	int i = 0;

	for (vector<string>::iterator it = filestocopy.begin(); it != filestocopy.end(); it++)
	{
		string to = path + (string)"\\" + *it;
		string from = *origdir.begin() + (string)"\\" + *it;
		QString msg = (QString)"Von: " + (QString)(from.c_str()) + (QString)"\nNach: " + (QString)(to.c_str());
		xx.setLabelText(msg);
		xx.setValue(i++);
		xx.repaint();
		
		::CopyFileA(from.c_str(), to.c_str(), false);
	}
	return "";
}


bool assistant::do_action(enum assistant::actions act) const
{
	return (act & actionflags) == act;
}