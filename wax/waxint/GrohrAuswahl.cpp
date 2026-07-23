#include "stdafx.h"


#include "resource.h"
#include "grohrauswahl.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(GrohrAuswahl, CDialog)
	ON_NOTIFY(LVN_ENDLABELEDIT, IDC_GROHRLIST, onEndlabeleditGrohrlist)
	ON_NOTIFY(NM_DBLCLK, IDC_GROHRLIST, onDblclkGrohrlist)
END_MESSAGE_MAP()

GrohrAuswahl::~GrohrAuswahl()
{

}

GrohrAuswahl::GrohrAuswahl(CObList* walist) : CDialog(IDD_GROHRAUSWAHL), list(0,0,0)
{
	this->walist = walist;
	isInit = false;
}

BOOL GrohrAuswahl::OnInitDialog()
{
	if (!CDialog::OnInitDialog())
		return false;

	init_list();	

	return true;
}

void GrohrAuswahl::init_list()
{
	//ListView_SetExtendedListViewStyle(list.m_hWnd, LVS_EX_FULLROWSELECT);
	CString bez;

	bez = getStammdaten().getText("IDS_CHECK").c_str();			list.InsertColumn(CHECK,		bez, LVCFMT_RIGHT, 50);
	bez = getStammdaten().getText("IDS_POSNR_ABK").c_str();		list.InsertColumn(POSNR,		bez, LVCFMT_RIGHT, 40);
	bez = getStammdaten().getText("IDS_LFDNR").c_str();			list.InsertColumn(LFDNR,		bez, LVCFMT_RIGHT, 40);
	bez = getStammdaten().getText("IDS_GROHR_ANZAHL").c_str();	list.InsertColumn(ANZAHL,		bez, LVCFMT_RIGHT, 50);
	bez = getStammdaten().getText("IDS_GROHR_LAENGE").c_str();	list.InsertColumn(LAENGE,	   	bez, LVCFMT_LEFT,  50);
	bez = getStammdaten().getText("IDS_NENNWEITE").c_str();		list.InsertColumn(NENNWEITE,	bez, LVCFMT_LEFT,  60);
	bez = getStammdaten().getText("IDS_TEXT").c_str();			list.InsertColumn(TEXT,			bez, LVCFMT_LEFT, 250); 
	bez = getStammdaten().getText("IDS_WINKEL").c_str();		list.InsertColumn(LST_WINKEL,		bez, LVCFMT_LEFT,  50);
	bez = getStammdaten().getText("IDS_VERKETTUNG").c_str();	list.InsertColumn(VERKETTUNG,	bez, LVCFMT_LEFT,  40);

	isInit = true; 
	//list.setDblclkMsg(IDC_EDIT);

	fill_list();
}

void GrohrAuswahl::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_GROHRLIST,	list);
}

void GrohrAuswahl::fill_list(int select)
{
	bool dn2zoll = (WaxSys::para_int(DN2ZOLL) == 0) ? false : true;
	Element *ele;

	// Selektiertes Element suchen und merken
	int curselect;
	if (select == -1)
	{
		curselect = list.getSelectedItem();
		if (curselect == -1)
			curselect = 0;
	}
	else
		curselect = select;

	list.SetRedraw(FALSE); 
	// Löschen aller Strings
	list.DeleteAllItems();

	for (int i = 0; i < walist->GetCount(); i++)
	{
		Werkauftrag* wa = dynamic_cast<Werkauftrag*>(walist->GetHead());
		XASSERT(wa);
		if (!wa)
		{
			CObject *tmp = walist->GetHead();
			walist->RemoveHead();
			walist->AddTail(tmp);
			continue;
		}
		else
		{
			// Der erste Werkauftrag kann ein Sammelauftrag sein. 
			if ((wa->isEmpty() || dynamic_cast<SammelWerkauftrag*>(wa)) && (walist->GetCount() > 1))
			{
				CObject *tmp = walist->GetHead();
				walist->RemoveHead();
				walist->AddTail(tmp);
				continue;
			}
			Gewinderohr* grohr = dynamic_cast<Gewinderohr*>(wa);
			XASSERT(grohr);
			if (!grohr)
				return;

			CString posnr = grohr->get_posNr();
			int anzahl = grohr->get_anzahl();
			int lfdnr = 0;

			// Neu füllen mit Elementdaten
			for (int k = 0; k < grohr->get_elementArray().GetSize(); k++) 
			{
				CString txt;
				lfdnr++;

				ele = grohr->get_elementArray().get_element_at(k);	
				if (ele->get_verkettung() == 1 || ele->get_abstRel() == 0)
					continue;

				// ZMasse setzen
				int zmr = ele->get_zmass();
				int zml = grohr->get_zmassLinks();
				int index = k;
				while (index > 0)
				{
					Element *elem = grohr->get_elementArray().get_element_at(index-1);
					if (elem->get_verkettung() == 0)
					{
						if (elem->get_gruppenid() == FormstueckeUndFittinge::WINKEL || elem->get_gruppenid() == FormstueckeUndFittinge::WINKEL_30 || elem->get_gruppenid() == FormstueckeUndFittinge::WINKEL_A1_45 || elem->get_gruppenid() == FormstueckeUndFittinge::WINKEL_A4) 
							zml = elem->get_zmass2();
						else
							zml = elem->get_zmass();
						break;
					}
					index--;
				}

				index = list.GetItemCount();
				index = index > 0 ? index : 0;
				index = list.InsertItem(index, "1");

				txt.Format("%s", (ele->get_makeAnlage())?"x":"-");
				list.SetItemText(index, CHECK, txt);

				list.SetItemText(index, POSNR, posnr);

				txt.Format("%d", lfdnr);
				list.SetItemText(index, LFDNR, txt);
				
				txt.Format("%d", anzahl);
				list.SetItemText(index, ANZAHL, txt);

				txt.Format("%d", ele->get_abstRel() - zml - zmr);
				list.SetItemText(index, LAENGE, txt);
				
				if (dn2zoll)
					txt.Format("%s", WaxStamm::dn2zoll(ele->get_anschlussDN()));
				else
					txt.Format("%d", ele->get_anschlussDN());
				list.SetItemText(index, NENNWEITE,	txt);
						
				txt.Format("%s", ele->get_kurztext());
				list.SetItemText(index, TEXT, txt);
				
				txt.Format("%d", ele->get_winkel());
				list.SetItemText(index, FormstueckeUndFittinge::WINKEL, txt);

				txt.Format("%s", (ele->get_verkettung()>0)?"x":"-");
				list.SetItemText(index, VERKETTUNG, txt);
			}

			CObject *tmp = walist->GetHead();
			walist->RemoveHead();
			walist->AddTail(tmp);
		}
	}

	// Wenn ein Eintrag vorhanden ist, den vorher selektierten anwählen, 
	// sonst den letzten in der Liste
	curselect = curselect >= list.GetItemCount() ? list.GetItemCount()-1 : curselect;
	list.selectItem(curselect);
	list.SetRedraw(TRUE);
}

//Indicates the user finished editing a item label
void GrohrAuswahl::onEndlabeleditGrohrlist(NMHDR* pNMHDR, LRESULT* pResult)
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	
	if (pDispInfo->item.pszText && strlen(pDispInfo->item.pszText))
	{
		CString oldcheck = list.GetItemText(pDispInfo->item.iItem, CHECK);
		list.SetItemText(pDispInfo->item.iItem, pDispInfo->item.iSubItem, pDispInfo->item.pszText);
		CString check = list.GetItemText(pDispInfo->item.iItem, CHECK);
		CString posnr = list.GetItemText(pDispInfo->item.iItem, POSNR);
		int lfdnr = atoi(list.GetItemText(pDispInfo->item.iItem, LFDNR));
			
		if (check != "x" && check != "-")
		{
			*pResult = 0; 
			return;
		}

		if (check != oldcheck)
		{
			if (setCheck(check, posnr, lfdnr))
				*pResult = 1;
			else
			{
				*pResult = 0; 
				list.SetItemText(pDispInfo->item.iItem, pDispInfo->item.iSubItem, oldcheck);
			}
		}
	}
	else
	{
		*pResult = 0; 
	}
}

bool GrohrAuswahl::setCheck(CString check, CString posnr, int lfdnr)
{
	for (POSITION pos=walist->GetHeadPosition(); pos;)
	{
		Werkauftrag* wa = dynamic_cast<Werkauftrag*>(walist->GetNext(pos));
		XASSERT(wa);
		if (!wa)
			continue;
		else
		{
			// Der erste Werkauftrag kann ein Sammelauftrag sein. 
			if ((wa->isEmpty()  || dynamic_cast<SammelWerkauftrag*>(wa)) && (walist->GetCount() > 1))
				continue;
		
			Gewinderohr* grohr = dynamic_cast<Gewinderohr*>(wa);
			XASSERT(grohr);
			if (!grohr)
				return false;
			
			if (posnr == grohr->get_posNr())
			{
				// Anlagenflag setzen
				for (int k = 0; k < grohr->get_elementArray().GetSize(); k++) 
				{
					Element *ele = grohr->get_elementArray().get_element_at(k);	
					if (ele->get_verkettung() == 1 || ele->get_abstRel() == 0)
						continue;

					if (lfdnr == k+1)
					{
						// Achtung: Bei Aenderung wird nicht Anlagenbedingung abgecheckt
						ele->set_makeAnlage((check == "x") ? true : false);
						grohr->createModell();
						return true;
					}
				}
			}
		}
	}
	return false;
}

void GrohrAuswahl::OnCancel()
{
	for (int i = 0; i < walist->GetCount(); i++)
	{
		Werkauftrag* wa = dynamic_cast<Werkauftrag*>(walist->GetHead());
		XASSERT(wa);
		if (!wa)
		{
			CObject *tmp = walist->GetHead();
			walist->RemoveHead();
			walist->AddTail(tmp);
			continue;
		}
		else
		{
			// Der erste Werkauftrag kann ein Sammelauftrag sein. 
			if ((wa->isEmpty()  || dynamic_cast<SammelWerkauftrag*>(wa)) && (walist->GetCount() > 1))
			{
				CObject *tmp = walist->GetHead();
				walist->RemoveHead();
				walist->AddTail(tmp);
				continue;
			}
			Gewinderohr* grohr = dynamic_cast<Gewinderohr*>(wa);
			XASSERT(grohr);
			if (!grohr)
				return;
			
			// Anlagenflag auf true setzen
			for (int k = 0; k < grohr->get_elementArray().GetSize(); k++) 
			{
				Element *ele = grohr->get_elementArray().get_element_at(k);	
				// ZMasse setzen
				int zmr = ele->get_zmass();
				int zml = grohr->get_zmassLinks();
				int index = k;
				while (index > 0)
				{
					Element *elem = grohr->get_elementArray().get_element_at(index-1);
					if (elem->get_verkettung() == 0)
					{
						if (elem->get_gruppenid() == FormstueckeUndFittinge::WINKEL || 
							elem->get_gruppenid() == FormstueckeUndFittinge::WINKEL_30 || 
							elem->get_gruppenid() == FormstueckeUndFittinge::WINKEL_A1_45 || 
							elem->get_gruppenid() == FormstueckeUndFittinge::WINKEL_A4) 
							zml = elem->get_zmass2();
						else
							zml = elem->get_zmass();
						break;
					}
					index--;
				}

				ele->set_makeAnlage(grohr->isAnlagOK(ele->get_anschlussDN(), ele->get_abstRel()-zml-zmr));
			}
			
			CObject *tmp = walist->GetHead();
			walist->RemoveHead();
			walist->AddTail(tmp);
		}
	}
	CDialog::OnCancel();
}

void GrohrAuswahl::onDblclkGrohrlist(NMHDR* pNMHDR, LRESULT* pResult)
{
	UNREFERENCED_PARAMETER(pNMHDR);

	int index = list.getSelectedItem();
	if (index == -1)
		return;

	CString oldcheck = list.GetItemText(index, CHECK);
	CString check = list.GetItemText(index, CHECK);
	CString posnr = list.GetItemText(index, POSNR);
	int lfdnr = atoi(list.GetItemText(index, LFDNR));

	CString str; str = getStammdaten().getText("IDS_FRAGE1b").c_str();
	if (AfxMessageBox(str, MB_YESNO | MB_ICONINFORMATION) == IDYES)
		check = "x";
	else
		check = "-";

	if (check != oldcheck)
	{
		if (setCheck(check, posnr, lfdnr))
		{
			list.SetItemText(index, CHECK, check);
			*pResult = 1;
		}
		else
		{
			*pResult = 0; 
			list.SetItemText(index, CHECK, oldcheck);
		}
	}
}

