#include "stdafx.h"


#include <mxodbc\mxodbc.h>
#include "WanrSperrung.h"


BEGIN_MESSAGE_MAP(WanrSperrung, CDialog)
	ON_COMMAND(ID_SETWANR, onSetwanr)
	ON_BN_CLICKED(IDC_UEBERNAHME, onSetwanr)
	ON_COMMAND(ID_DELETE, unlock)
END_MESSAGE_MAP()



WanrSperrung::WanrSperrung(CWnd *parent, int value)
:CDialog(IDD_GESPERRTE_WANR, parent), list(ID_SETWANR, IDR_WASPERRUNG)
{
	val = value;
	
}

WanrSperrung::~WanrSperrung()
{
}

BOOL WanrSperrung::OnInitDialog()
{
	if (!CDialog::OnInitDialog())
		return false;

	list.addColumn("WA-Nummer", 0, 60);
	list.addColumn("User", 1, 260);
	
	MXTable wa(WaxSys::getServerdb(), "gesperrte_wanr");
	
	DECLARE_COL(Int, wanr, wa);
	DECLARE_COL(String, user, wa);
	DECLARE_COL(Date, datum, wa);
	
	wa.load();
	while (!wa.isEof())
	{
		wa.moveNext();
		locked_wanr.insert(make_pair(wanr.data, *user.data));
	}
	int i = 0;
	map<int, string>::iterator it;
	for (it = locked_wanr.begin(); it != locked_wanr.end(); it++)
	{
		CString item;
		item.Format("%i", it->first);
		int index = list.addItem(i++, 0, item);
		list.addItem(index, 1, it->second.c_str());
	}
	return true;
}

void WanrSperrung::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_WANR, val);
	DDX_Control(pDX, IDC_WALIST, list);
}

void WanrSperrung::OnOK()
{
	CDialog::OnOK();
}


int WanrSperrung::get_val()
{
	return val;
}

CString WanrSperrung::get_valstring()
{
	CString fmt;
	fmt.Format("%i", val);
	return fmt;
}

void WanrSperrung::remove_val(int wanr)
{
	/*
	Wert wird aus Tabelle entfernt
	*/
	MXTable wa(WaxSys::getServerdb(), "gesperrte_wanr");
	CString sql;
	sql.Format("DELETE FROM gesperrte_wanr WHERE wanr = %i", wanr);
	wa.executeSQL(sql);
}


bool WanrSperrung::lock()
{
	// Eintrag in Tabelle erzeugen
	MXTable wa(WaxSys::getServerdb(), "gesperrte_wanr");

	DECLARE_COL(Int, wanr, wa);
	DECLARE_COL(String, user, wa);
	DECLARE_COL(Date, datum, wa);

	wa.load();
	wa.addNew();
	wanr.data = val;
	*user.data = WaxSys::para_str(BEARBEITER);
	datum.set_datum(MXDate::GetCurrentTime());
	TRY
	{
		wa.update();
	}
	CATCH_ALL(e)
	{
		CString str; str = getStammdaten().getText("IDS_EINTRAG_SPEICHERN").c_str();
		AfxMessageBox(str, MB_OK | MB_ICONINFORMATION);
		return false;
	}
	END_CATCH_ALL
	return true;
}

void WanrSperrung::unlock()
{
	/*
	Der ausgewählte Wert wird aus der Liste und der Tabelle entfernt
	*/
	CString wanr;
	int index = list.getSelectedItem(0, &wanr);
	if (index >= 0)
	{
		remove_val(atoi(wanr));
		list.deleteItem(index);	
	}
}



void WanrSperrung::onSetwanr()
{
	CString itemtext;
	int index = list.getSelectedItem();
	if (index >= 0)
	{
		list.getSelectedItem(index, &itemtext);
		val = atoi(itemtext);
		UpdateData(false);
	}
}

