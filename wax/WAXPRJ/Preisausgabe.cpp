#include "stdafx.h"


#include <afxcoll.h>



BEGIN_MESSAGE_MAP(Preisausgabe, CDialog)
END_MESSAGE_MAP()

Preisausgabe::Preisausgabe(Werkauftrag* value) : CDialog(IDD_PREISAUSGABE), list(0,0,0)
{
	wa = value;

	typ = MATERIAL;
	gf  = false; 

	summeMatP = 0; summeMatGK = 0; summeMatWAP = 0; 
	summeAfoP = 0; summeAfoGK = 0; summeAfoWAP = 0; 
	summeAfoAV = 0; sonderkosten = 0;
}

Preisausgabe::~Preisausgabe()
{
	matlist.RemoveAll();
	afolist.RemoveAll();
}

BOOL Preisausgabe::DestroyWindow()
{
	return CWnd::DestroyWindow();
}

BOOL Preisausgabe::OnInitDialog()
{
	if (!CDialog::OnInitDialog())
		return (FALSE);

	CString bez, str;
	int index = 0;
		

	if (typ == MATERIAL)
	{
		bez = getStammdaten().getText("IDS_PREISAUSGABE_MAT").c_str();
		str = getStammdaten().getText("IDS_POSNR_ABK").c_str();
		list.InsertColumn(index, str,		LVCFMT_LEFT,  50); index++;
		str = getStammdaten().getText("IDS_ANZ_STK").c_str();
		list.InsertColumn(index, str,		LVCFMT_LEFT,  80); index++;
		str = getStammdaten().getText("IDS_ART_NR").c_str();
		list.InsertColumn(index, str,		LVCFMT_LEFT,  55); index++;
		str = getStammdaten().getText("IDS_TEXT").c_str();
		list.InsertColumn(index, str,		LVCFMT_LEFT, 300); index++;
		if (gf)
		{
			str = getStammdaten().getText("IDS_GRUNDPREIS").c_str();
			list.InsertColumn(index, str,	LVCFMT_RIGHT, 95);index++;
			str = getStammdaten().getText("IDS_HERSTELLKOSTEN").c_str();
			list.InsertColumn(index, str,	LVCFMT_RIGHT, 95);	index++;
		}
		str = getStammdaten().getText("IDS_WAP").c_str();
		list.InsertColumn(index, str,		LVCFMT_RIGHT, 95);
	}
	else if (typ == AFO) 
	{
		bez = getStammdaten().getText("IDS_PREISAUSGABE_ARB").c_str();
		str = getStammdaten().getText("IDS_POSNR_ABK").c_str();
		list.InsertColumn(index, str,	    LVCFMT_LEFT,  50); index++;
		str = getStammdaten().getText("IDS_ANZ").c_str();
		list.InsertColumn(index, str,		LVCFMT_LEFT,  35); index++;
		str = getStammdaten().getText("IDS_CODE").c_str();
		list.InsertColumn(index, str,		LVCFMT_LEFT,  45); index++;
		str = getStammdaten().getText("IDS_TEXT").c_str();
		list.InsertColumn(index, str,		LVCFMT_LEFT, 250); index++;
		str = getStammdaten().getText("IDS_ARBEITSPLATZ").c_str();
		list.InsertColumn(index, str,		LVCFMT_LEFT,  60); index++;
		if (gf)
		{
			
			str = getStammdaten().getText("IDS_GRUNDPREIS").c_str();
			list.InsertColumn(index, str,	LVCFMT_RIGHT, 95); index++;
			str = getStammdaten().getText("IDS_AV_PREIS").c_str();
			list.InsertColumn(index, str,	LVCFMT_RIGHT, 95); index++;
			str = getStammdaten().getText("IDS_HERSTELLKOSTEN").c_str();
			list.InsertColumn(index, str,	LVCFMT_RIGHT, 95); index++;
		}
		str = getStammdaten().getText("IDS_WAP").c_str();
		list.InsertColumn(index, str,		LVCFMT_RIGHT, 95);
	} 
	else if (typ == KOMPLETT)
	{
		bez = getStammdaten().getText("IDS_GESAMTPREISE").c_str();

		str = getStammdaten().getText("IDS_POSNR_ABK").c_str();
		list.InsertColumn(index, str,		LVCFMT_LEFT,  50); index++;
		str = getStammdaten().getText("IDS_TEXT").c_str();
		list.InsertColumn(index, str,		LVCFMT_LEFT, 300); index++;
		
		if (gf)
		{
			str = getStammdaten().getText("IDS_GRUNDPREIS").c_str();
			list.InsertColumn(index, str,	LVCFMT_RIGHT, 95);	index++;
			str = getStammdaten().getText("IDS_HERSTELLKOSTEN").c_str();
			list.InsertColumn(index, str,	LVCFMT_RIGHT, 95);	index++;
		}
		str = getStammdaten().getText("IDS_WAP").c_str();
		list.InsertColumn(index, str,		LVCFMT_RIGHT, 95);
	}
	else if (typ == VK3_KOMPLETT)
	{
		bez = getStammdaten().getText("IDS_VK3_GESAMTPREISE").c_str();

		str = getStammdaten().getText("IDS_POSNR_ABK").c_str();
		list.InsertColumn(index, str,		LVCFMT_LEFT,  50); index++;
		str = getStammdaten().getText("IDS_TEXT").c_str();
		list.InsertColumn(index, str,		LVCFMT_LEFT, 300); index++;
		str = getStammdaten().getText("IDS_WAP").c_str();
		list.InsertColumn(index, str,		LVCFMT_RIGHT, 95);
	}
	else if (typ == VK3_ELEMENTE)
	{
		bez = getStammdaten().getText("IDS_VK3_ELEMENTE").c_str();

		str = getStammdaten().getText("IDS_POSNR_ABK").c_str();
		list.InsertColumn(index, str,		LVCFMT_LEFT,  50); index++;
		str = getStammdaten().getText("IDS_ANZAHL_LAENGE").c_str();
		list.InsertColumn(index, str,		LVCFMT_LEFT,  95); index++;
		str = getStammdaten().getText("IDS_TEXT").c_str();
		list.InsertColumn(index, str,		LVCFMT_LEFT, 300); index++;
		str = getStammdaten().getText("IDS_NENNW_DN").c_str();
		list.InsertColumn(index, str,		LVCFMT_RIGHT, 95); index++;
		str = getStammdaten().getText("IDS_ROHRNENNW_DN").c_str();
		list.InsertColumn(index, str,		LVCFMT_RIGHT, 95); index++;
		str = getStammdaten().getText("IDS_WAP").c_str();
		list.InsertColumn(index, str,		LVCFMT_RIGHT, 95);
	}
	SetWindowText(bez.GetBuffer(10));

	UpdateData();
	return true;
}

void Preisausgabe::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_PREISE, list);

	if (pDX->m_bSaveAndValidate) 
	{
		if (typ == VK3_KOMPLETT)
		{
			fillVK3Komplettlist();
		}
		else if (typ == VK3_ELEMENTE)
		{
			fillVK3Elementlist();
		}
	}
}

void Preisausgabe::OnOK()
{
	CDialog::OnOK();
}



int Preisausgabe::openDialog(int typ, bool gf, double summeEle, double sonderkosten, double gesamtpreisVK3)
{
	this->summeMatWAP = summeEle;   
	this->sonderkosten= sonderkosten;
	this->gesamtpreisVK3 = gesamtpreisVK3;

	this->typ = typ;
	this->gf = gf;
	
	DoModal();
	return 1;
}

int Preisausgabe::openDialog(int typ, bool gf, CObList& elelist, CObList & passlist, CObList &pulverlist, double summeEle)
{
	this->summeMatWAP = summeEle;   

	this->typ = typ;
	this->gf = gf;
	
	this->matlist.RemoveAll();

	POSITION pos;
	for (pos = elelist.GetHeadPosition(); pos;)
	{
		Element* ele = dynamic_cast<Element*>(elelist.GetNext(pos));
		XASSERT(ele); 
		if (!ele) 
			break;

		this->matlist.AddTail(ele);
	}

	for (pos = passlist.GetHeadPosition(); pos;)
	{
		Element* ele = dynamic_cast<Element*>(passlist.GetNext(pos));
		XASSERT(ele); 
		if (!ele) 
			break;

		this->matlist.AddTail(ele);
	}

	for (pos = pulverlist.GetHeadPosition(); pos;)
	{
		Element* ele = dynamic_cast<Element*>(pulverlist.GetNext(pos));
		XASSERT(ele); 
		if (!ele) 
			break;

		this->matlist.AddTail(ele);
	}

	DoModal();
	return 1;
}




void Preisausgabe::fillVK3Komplettlist(int select)
{
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

	// Preise der Materialliste
	CString txt;
	int index = list.GetItemCount();
	index = index > 0 ? index : 0;
	index = list.InsertItem(index, "1");

	txt = getStammdaten().getText("IDS_PREIS_VK3_ELE").c_str();
	list.SetItemText(index, 1, txt);
	txt.Format("%7.2f", summeMatWAP);
	list.SetItemText(index, 2, txt);
	
	// Sonderkosten
	index = list.GetItemCount();
	index = index > 0 ? index : 0;
	index = list.InsertItem(index, "2");

	txt = getStammdaten().getText("IDS_SONDERKOSTEN").c_str();
	list.SetItemText(index, 1, txt);
	txt.Format("%7.2f", sonderkosten);
	list.SetItemText(index, 2, txt);
		
	// Gesamtpreise
	index = list.GetItemCount();
	index = index > 0 ? index : 0;
	index = list.InsertItem(index, "3");

	txt = getStammdaten().getText("IDS_GESAMTPREIS").c_str();
	list.SetItemText(index, 1, txt);
	txt.Format("%7.2f", gesamtpreisVK3);
	list.SetItemText(index, 2, txt);

	// Wenn ein Eintrag vorhanden ist, den vorher selektierten anwählen, 
	// sonst den letzten in der Liste
	curselect = curselect >= list.GetItemCount() ? list.GetItemCount()-1 : curselect;
	list.selectItem(curselect);
	list.SetRedraw(TRUE); 
}

void Preisausgabe::fillVK3Elementlist(int select)
{
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

	// Elementliste des Werkauftrages durchgehen
	for (POSITION pos = matlist.GetHeadPosition(); pos;)
	{

		Element *ele = dynamic_cast<Element*>(matlist.GetNext(pos));
		XASSERT(ele);
		if (!ele)
			break;

		int col = 0;
		int index = list.GetItemCount();
		index = index > 0 ? index : 0;
		index = list.InsertItem(index, "1");

		bool rohr = false; 
		CString txt = "";

		if (ele->get_id() > 100 && ele->get_id() < 200)
		{
			rohr = true;
		}
		else if (ele->get_id() > 200 && ele->get_id() < 300)
		{
			rohr = true;
		}
		else if (ele->get_id() > 300 && ele->get_id() < 400)
		{
			rohr = true;
		}
		else if (ele->get_id() > 400 && ele->get_id() < 500)
		{
			rohr = true;
		}
		else if (ele->get_id() > 500 && ele->get_id() < 600)
		{
			// Passlaenge keine Laenge Sonsern Anzahl
		}
		else if (ele->get_id() > 600 && ele->get_id() < 700)
		{
			rohr = true;
		}
		else if (ele->get_id() > 702 && ele->get_id() < 800)
		{
			rohr = true;
		}
		else if (ele->get_id() == 700)
		{
			rohr = true;
		}
		else if (ele->get_id() == 701)
		{
			rohr = true;
		}

		txt.Format("%d", index+1);
		list.SetItemText(index, col, txt); col++;
		
		if (ele->isEleVK3Rohr() || rohr)
		{
			// Die Gesamtlaenge wird auf voll Meter aufgerundet
			int rest = ele->get_laenge() % 1000;
			if (rest > 0)
			{
				int laenge = ele->get_laenge() / 1000;
				laenge = laenge * 1000;
				laenge = laenge + 1000;
				txt.Format("%.3f", (double)laenge/1000);
			}
			else
				txt.Format("%.3f", (double)ele->get_laenge()/1000);
		}
		else
			txt.Format("%d", ele->get_anzahl());
		list.SetItemText(index, col, txt); col++;

		list.SetItemText(index, col, ele->get_bezeichnung()); col++;

		txt.Format("%d", ele->get_dn());
		list.SetItemText(index, col, txt); col++;
		
		txt.Format("%d", ele->get_anschlussDN());
		list.SetItemText(index, col, txt); col++;

		// WAP		
		if (ele->isEleVK3Rohr() || rohr)
		{
			// Die Gesamtlaenge wird auf voll Meter aufgerundet
			int rest = ele->get_laenge() % 1000;
			if (rest > 0)
			{
				int laenge = ele->get_laenge() / 1000;
				laenge = laenge * 1000;
				laenge = laenge + 1000;
				txt.Format("%10.2f", ele->get_festPreis() * (double)laenge/1000);
			}
			else
				txt.Format("%10.2f", ele->get_festPreis() * (double)ele->get_laenge()/1000);
		}
		else
			txt.Format("%10.2f", ele->get_festPreis() * (double)ele->get_anzahl());
		list.SetItemText(index, col, txt);
	}

	// Leerzeile
	CString txt;
	int index = list.GetItemCount();
	index = index > 0 ? index : 0;
	index = list.InsertItem(index, "");

	// Summen der Preise aus der Elementliste
	index = list.GetItemCount();
	index = index > 0 ? index : 0;
	index = list.InsertItem(index, "");

	txt = getStammdaten().getText("IDS_SUMMEN_ELELIST").c_str();
	list.SetItemText(index, 2, txt);
	txt.Format("%10.2f", summeMatWAP);
	list.SetItemText(index, 5, txt);

	// Wenn ein Eintrag vorhanden ist, den vorher selektierten anwählen, 
	// sonst den letzten in der Liste
	curselect = curselect >= list.GetItemCount() ? list.GetItemCount()-1 : curselect;
	list.selectItem(curselect);
	list.SetRedraw(TRUE); 
}
