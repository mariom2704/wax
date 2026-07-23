#ifndef __ASSISTANT__
#define __ASSISTANT__

#include <QTableWidget>
#include <QStandardItemModel>

#include <string>
using namespace std;

class assistant
{
public:
	assistant();
	virtual ~assistant();

	enum actions
	{
		SPANXDB = 0x01,
		WAXDB = 0x02,
		SPANX_EXE = 0x04,
		SPLEDIT_EXE = 0x08,
		WAX_EXE = 0x10,
		WAX_EXE_ONLY = 0x20,
		STAMMDB_WAX = 0x40,
		STAMMDB_SPANX = 0x80,
		MXDM = 0x100,
	};

	void reset_actions();

	void add_action(enum assistant::actions actflag);

	void remove_action(enum assistant::actions actflag);

	
	void process_stdnl(const string& nl);

	int checkLastUpdate(string dbq);

	void check_version_info(string fname, int index = -1);

	bool read_ini(string entry, vector<string>& lst);

	/// Führt nach Doppelklick ein Update aus
	string do_update(const string file);

	void set_model(QTableWidget* md);
private:

	int add_output_string(const string &item, const string &info = "", const string &info_second = "", int idx = -1) const;
	
	QTableWidget* list;
	
	bool do_action(enum assistant::actions act) const;

	int actionflags;
	
	std::string get_version_info(char* szFile);

	string get_timestamp(string fname);

	/// Prüft, ob die Datei geöffnet ist
	void check_file_locked(int index, const string& fname) const;
};

#endif