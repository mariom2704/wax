
#include "stdafx.h"
#include "resource.h"

#include "parameter.h"
#include "PropPageSysAll.h"
#include "editparameter.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


BEGIN_MESSAGE_MAP(PropPageSysAll, WaxPropPage)
	ON_BN_CLICKED(IDC_BEARBEITEN, on_bearbeiten)
	ON_NOTIFY(HDN_ITEMDBLCLICK, IDC_SYSPARAM_LIST, on_dblclick)
	ON_COMMAND(IDOK, on_bearbeiten)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_SYSPARAM_LIST, on_column_clicked)
END_MESSAGE_MAP()

/// Konstructor
PropPageSysAll::PropPageSysAll(MxParameter *para) : WaxPropPage(IDD_PARA_SYSALL, para), plist(IDC_BEARBEITEN, 0, LIST_SHORTCUTS)
{
}

/// Destructor
PropPageSysAll::~PropPageSysAll()
{
}

/// Default-Werte setzen
void PropPageSysAll::set_default()
{
}


/// Dialog initialisieren
BOOL PropPageSysAll::OnInitDialog() 
{
	WaxPropPage::OnInitDialog();

	HICON ike = AfxGetApp()->LoadIcon(IDI_SYSPARAM);
	SetIcon( ike, TRUE);
	
	const int SIZE = 60;
	
	CString tmp;
	//tmp.LoadString(IDS_SYSPARAM_NAME);
	tmp = TextDataList::getInstance()->getText("IDS_NAME").c_str();
	plist.addColumn(tmp, 0, SIZE*2);
	
	//tmp.LoadString(IDS_SYSPARAM_USER);
	tmp = TextDataList::getInstance()->getText("IDS_GUELTIGKEIT").c_str();
	plist.addColumn(tmp, 1, SIZE/2, LVCFMT_LEFT);

	//tmp.LoadString(IDS_SYSPARAM_WERT);
	tmp = TextDataList::getInstance()->getText("IDS_WERT").c_str();
	plist.addColumn(tmp, 2, SIZE, LVCFMT_LEFT);

	//tmp.LoadString(IDS_SYSPARAM_BEMERKUNG);
	tmp = TextDataList::getInstance()->getText("IDS_BEMERKUNG").c_str();
	plist.addColumn(tmp, 3, SIZE*3, LVCFMT_LEFT);
	
	DragAcceptFiles(FALSE); 	
	init_list();

	GetDlgItem(IDC_BEARBEITEN)->SetWindowText(TextDataList::getInstance()->getText("IDC_BEARBEITEN").c_str());

	UpdateData(FALSE);

	return TRUE;  
}

void PropPageSysAll::init_list()
{
	/**
	Alle Parameter in die Liste eintragen ,die für die jeweilige Versíon gültig sind
	*/
	int item = 0;
	clear_list();
	map<int, Parameter*>::iterator xit;

	for (xit = parameter->pmap.begin(); xit != parameter->pmap.end(); xit++)
	{
		// nur wenn das av Flag gesetzt ist, werden alle Parameter angezeigt
		if (!WaxSys::isAV() && xit->second->get_av())
			continue;
		string x = xit->second->get_entry();
		for (string::iterator it = x.begin(); it != x.end(); it++)
			toupper(*it);
		
		item = plist.addItem(item, 0, x.c_str(), 0);
		plist.SetItemText(item, 1, get_namespace_str(xit->second->get_valid_namespace()).c_str());	
		plist.SetItemText(item, 2, xit->second->get_val_str().c_str());
		plist.SetItemText(item, 3, xit->second->getBemerkung().c_str());
		plist.SetItemData(item, (DWORD)xit->second);
	}
	plist.adjustColumnWidth(0, 3);
	plist.selectItem(0);
}

string PropPageSysAll::get_namespace_str(Parameter::VALID_NAMESPACE nmsp) const
{
	UINT idvalid = 0;
	switch (nmsp)
	{
	case Parameter::USER:
		idvalid = IDS_USER;
		break;
	case Parameter::MACHINE:
		idvalid = IDS_MACHINE;
		break;
	default:
		idvalid = IDS_LOCATION;
	}
	CString tmp;
	tmp.LoadString(idvalid);
	return (LPCSTR)tmp;
}

/// Datenaustausch
void PropPageSysAll::DoDataExchange(CDataExchange* pDX)
{
	WaxPropPage::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_BEARBEITEN, but_bearbeiten);
	DDX_Control(pDX, IDC_SYSPARAM_LIST, plist);
}

void PropPageSysAll::on_bearbeiten() 
{
	/**
	Bearbeiten des ausgewählten Parameters
	*/
	int index = plist.getSelectedItem();
	Parameter *par = (Parameter *)plist.GetItemData(index);
	if (!par)
		return;
	EditParameter edpar(this, par);
	if (edpar.DoModal() == IDOK)
		par->save();
	else
		par->load(); // Rückgängig, falls Änderungen gemacht wurden
	plist.SetItemText(index, 1, get_namespace_str(par->get_valid_namespace()).c_str());	
	plist.SetItemText(index, 2, par->get_val_str().c_str());
}



void PropPageSysAll::on_dblclick(NMHDR* pNMHDR, LRESULT* pResult) 
{
	UNREFERENCED_PARAMETER(pNMHDR);
	*pResult = 0;
}


/// Allgemeine Einstellungen in Datenbank eintragen
BOOL PropPageSysAll::OnApply() 
{
	return true;
}

void PropPageSysAll::clear_list()
{
	plist.DeleteAllItems();
}

void PropPageSysAll::on_column_clicked(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	plist.SortItems(sort, pNMListView->iSubItem);
	*pResult = 0;
}


int CALLBACK PropPageSysAll::sort(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	Parameter *a = (Parameter*)lParam1, *b = (Parameter*)lParam2;
	string eins, zwei;
	switch (lParamSort)
	{
		case 0: eins = a->get_entry();
				zwei = b->get_entry();
				break;
		case 1: 
				if (a->get_valid_namespace() == b->get_valid_namespace())
					return 0;
				if (a->get_valid_namespace() > b->get_valid_namespace())
					return 1;
				return -1;
		case 2: eins = a->get_val_str();
				zwei = b->get_val_str();
				break;
		case 3: eins = a->getBemerkung();
				zwei = b->getBemerkung();
				break;
	}
	if (eins == zwei) return 0;
	if (eins > zwei) return 1;
	// Sortitems*/
	return -1;
	
}
