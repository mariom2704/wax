#include "stdafx.h"
#include "resource.h"


#include <mxutil/ddx_extension.h>

#include <AFXMT.H>
#include <afxpriv.h>

#define WM_UPDATE_FIELDS WM_USER+1234


BEGIN_MESSAGE_MAP(ElementeEingabeForm, WaxPartForm)
	
END_MESSAGE_MAP()


/*
Änderungen:
10.04.06, Jacobs: makeAnlage wird auf Standardwert false gesetzt. 
*/
ElementeEingabeForm::ElementeEingabeForm(UINT idd, const CString& chapter) : WaxPartForm(idd, chapter), list(0,0,0)
{
	abstAbs		= 0; 
	abstRel		= 0; 
	anschlussDN	= 100; 
	dn			= 0; 
	id			= 0; 
	laenge		= 0; 
	pos			= 1;
	winkel		= 0; 
	durchmesser = 0.0;
	makeAnlage  = false;

	changeRelAbst = (WaxSys::para_int(REL_ABST_AENDERN) == 0) ? false : true;
	ed_durchmesser.initVal(&durchmesser);
	m_pElementArray = NULL;
	m_iIDCNew	 = 0; 
}

ElementeEingabeForm::~ElementeEingabeForm()
{

}

//Called after the size of CWnd has changed.
void ElementeEingabeForm::OnSize(UINT nType, int cx, int cy)
{
	WaxPartForm::OnSize(nType, cx, cy);
	 
	if (::IsWindow(list.m_hWnd))
	{	
		CRect rectNewButton;
		GetDlgItem(m_iIDCNew)->GetWindowRect(&rectNewButton);
		ScreenToClient(&rectNewButton);
		list.SetWindowPos( NULL, rectNewButton.left, rectNewButton.bottom + rectNewButton.left, cx - 2 * rectNewButton.left,  cy - rectNewButton.bottom - 2 * rectNewButton.left, SWP_SHOWWINDOW | SWP_NOZORDER);
	}
}

BOOL ElementeEingabeForm::OnInitDialog()
{
	m_pElementArray = &get_rohrleitung()->getElementArray();
	
	if (!WaxPartForm::OnInitDialog())
		return FALSE;

	CString bez;
	bez = getStammdaten().getText("IDS_POSNR_ABK").c_str();		list.InsertColumn(LST_POSNR_ABK,	bez, LVCFMT_LEFT, 50);
	bez = getStammdaten().getText("IDS_ID").c_str();			list.InsertColumn(LST_ID,			bez, LVCFMT_LEFT, 160);
	bez = getStammdaten().getText("IDS_ANSCHLUSSDN").c_str();	list.InsertColumn(LST_ANSCHLUSSDN,	bez, LVCFMT_LEFT, 90);
	bez = getStammdaten().getText("IDS_DN").c_str();			list.InsertColumn(LST_DN,			bez, LVCFMT_LEFT, 50);
	bez = getStammdaten().getText("IDS_LAENGE").c_str();		list.InsertColumn(LST_LAENGE,		bez, LVCFMT_LEFT, 60);
	bez = getStammdaten().getText("IDS_WINKEL").c_str();		list.InsertColumn(LST_WINKEL,		bez, LVCFMT_LEFT, 60);
	bez = getStammdaten().getText("IDS_RELABST").c_str();		list.InsertColumn(LST_RELABST,		bez, LVCFMT_LEFT, 80);
	bez = getStammdaten().getText("IDS_ABSABST").c_str();		list.InsertColumn(LST_ABSABST,		bez, LVCFMT_LEFT, 80);
	if (av)
		bez = getStammdaten().getText("IDS_ANLAGE").c_str();	list.InsertColumn(LST_ANLAGE,	bez, LVCFMT_LEFT, 50);

	return TRUE;
}

void ElementeEingabeForm::DoDataExchange(CDataExchange* pDX)
{
	WaxPartForm::DoDataExchange(pDX);
}

bool ElementeEingabeForm::set_elemAbstAbs()
{
	Rohrleitung *rohr = dynamic_cast<Rohrleitung*>(get_data()->get_werkauftrag());
	XASSERT(rohr);
	if (!rohr)
		return false;		

	if (m_pElementArray->GetSize() > 0) 
	{
		// absolute Abstaende neu berechnen

		if (!m_pElementArray->calcAbsolutAbst())
			return false;

		abstAbs = abstAbs + abstRel;
	} 
	else 
	{
		abstAbs = abstRel;	
	}
	
	SetDlgItemInt(IDC_ELEMENTE_ABSTABS, abstAbs, false);
	return true;
}

void ElementeEingabeForm::set_elemAbstRel(int &index)
{
	
	if (index < 0 || index >= m_pElementArray->GetSize()){
		return;
	}

	Rohrleitung *rohr = dynamic_cast<Rohrleitung*>(get_data()->get_werkauftrag());
	XASSERT(rohr);
	if (!rohr)
		return;	

	int newAbstAbs = abstRel + abstAbs;

	if (m_pElementArray->GetSize() > 0) 
	{
		// Elemente nach absoluten Abstand sortieren
		m_pElementArray->sortElements(index);	

		// relative Abstaende neu berechnen
		m_pElementArray->calcRelAbst();

		// Anschlussnennweiten neu setzen, ist notwendig wegen Reduzierung
		m_pElementArray->changeElementDN(rohr->get_dnvl());
	} 
	else 
	{
		newAbstAbs = abstRel;	
	}


	SetDlgItemInt(IDC_ELEMENTE_ABSTABS, newAbstAbs, false);
}


void ElementeEingabeForm::show_elements(int select)
{
	Element *ele = NULL, *abschlussEl = NULL;
	BauelementGruppen bg;

	CString	str, idstr,	bezEl;

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

	int counter = m_pElementArray->GetSize();
	// Neu füllen mit Elementdaten

	int iPos = 1;
	for (int i=0; i<counter; i++) 
	{
		ele = m_pElementArray->get_element_at(i);	
		XASSERT(ele);
		if (!ele)
			return;


		bool bAbschluss = 0;
		//nächstes Element holen und prüfen ob Abschluelement
		abschlussEl = NULL;
		bezEl.Empty();
		if (i+1 <  m_pElementArray->GetSize())
		{
			abschlussEl = m_pElementArray->get_element_at(i+1);	
			XASSERT(abschlussEl);
			if (!abschlussEl)
				return;

			bAbschluss = abschlussEl->isEleAbgEnde();
			if (bAbschluss)
			{
				bezEl = " + " + bg.getBezeichnung(abschlussEl->get_id());

				//Schleifencounter hochsetzen, damit mit übernächsten Element weitergemacht wird
				i++;
			}
		}




		// Pos muss nach copy/delete fuer alle Elemente neu gesetzt werden
		ele->set_pos(iPos);
			
		int index = list.GetItemCount();
		index = index > 0 ? index : 0;
		index = list.InsertItem(index, "1");
	
		str.Format("%4d", ele->get_pos());
		list.SetItemText(index, LST_POSNR_ABK,			str);
		str.Format("%24s", idstr);
		list.SetItemText(index, LST_ID,				bg.getBezeichnung(ele->get_id()) + bezEl);
		str.Format("%6d", ele->get_anschlussDN());
		list.SetItemText(index, LST_ANSCHLUSSDN,	str);
		str.Format("%6d", ele->get_dn());
		list.SetItemText(index, LST_DN,				str);
		str.Format("%6d", ele->get_laenge());
		list.SetItemText(index, LST_LAENGE,			str);
		str.Format("%6d", ele->get_winkel());
		list.SetItemText(index, LST_WINKEL,			str);
		str.Format("%6d", ele->get_abstRel());
		list.SetItemText(index, LST_RELABST,		str);
		str.Format("%6d", ele->get_abstAbs());
		list.SetItemText(index, LST_ABSABST,		str);
		if (av)
		{
			str.Format("%2s", (ele->get_makeAnlage() ? "x" : " "));
			list.SetItemText(index, LST_ANLAGE,		str);
		}

		iPos++;
	}
	// Wenn ein Eintrag vorhanden ist, den vorher selektierten anwählen, 
	// sonst den letzten in der Liste
	curselect = curselect >= list.GetItemCount() ? list.GetItemCount()-1 : curselect;
	list.selectItem(curselect);
	list.SetRedraw(TRUE);
}



void ElementeEingabeForm::doInsert(Element* ele, int index)
{
	if (index == -1 || index >= m_pElementArray->GetSize())
		m_pElementArray->Add(ele);
	else
		m_pElementArray->InsertAt(index, ele);

	if (changeRelAbst)
		 set_elemAbstRel(index);
	else	
		set_elemAbstAbs();

	return;
}


/**
setzt zu dem ausgewählten Element in der Elementliste die Felder und Combobox
*/
void ElementeEingabeForm::elem_setFields()
{
	// Datensatz makiert?
	int index = list.getSelectedItem();
	if (index == -1)
		return;

	//in Arrayindex umrechnen
	index = translateSelectedIndexToArrayIndex(index,false);

	if (index >= m_pElementArray->GetSize()){
		XASSERT(index);
		return;
	}

	
	// Datensatz holen
	Element *ele = m_pElementArray->get_element_at(index);  
	
	// Werte uebergeben
	setAttributes(ele);
	//ed_durchmesser.setValue();	

	// Werte in Felder setzen
	SetDlgItemInt(IDC_ELEMENTE_ANSDN,	anschlussDN,false);
	SetDlgItemInt(IDC_ELEMENTE_LAENGE,	laenge,		false);
	SetDlgItemInt(IDC_ELEMENTE_WINKEL,	winkel,		false);
	SetDlgItemInt(IDC_ELEMENTE_ABSTREL, abstRel,	false);
	SetDlgItemInt(IDC_ELEMENTE_ABSTABS, abstAbs,	false);

	CDataExchange cb(this, false);

	//Dn Wert setzen
	ddx_cbdata(&cb, IDC_ELEMENTE_ELEDN, dn); //Wert nicht über SetDlgItemInt, da Combobox vom Typ Dropdown-Listenfeld ist
	//SetDlgItemInt(IDC_ELEMENTE_ELEDN,	dn,false);


	//Prüfen ob nächstes Element ein Abschlußelement ist, wenn ja dann id umrechnen
	ele = getElement(index+1);
	if (ele)
	{
		if (ele->isEleAbgEnde()){
			id = translateElementIdToComboBox(ele->get_id());
		}
	}

	ddx_cbdata(&cb, IDC_ELEMENTE_ID, id);
	ed_durchmesser.doDataExchange(&cb);
}

///Laenge/Durchmesser des Elements aus den Stammdaten holen
void ElementeEingabeForm::get_Stammdaten()
{
	Element ele;
	UINT uiLaenge = laenge;
	ele.get_Stammdaten(laenge, durchmesser, id, dn, anschlussDN);
	if(id == SONDERMUFFE)
	{
		laenge = uiLaenge;
	}
}

void ElementeEingabeForm::set_aktDnvl(int dnvl)
{
	aktDnvl = dnvl;
	Element *ele; 
	if (!(m_pElementArray->GetSize() < 1)) 
	{
		for (int i=0; i<m_pElementArray->GetSize(); i++) 
		{ 
			ele = m_pElementArray->get_element_at(i);  
			if (ele->get_id() == KRED || ele->get_id() == ERED || ele->get_id() == EINZ)
				aktDnvl = ele->get_dn();
		}
	}
}

void ElementeEingabeForm::onElemChange()
{
	/*
	Klick auf Button "Ändern" 
	Ändert den Eintrag in der ComboBox entsprechend der eingegebenen Daten.  
	*/
	UpdateData();	

	int index,
		select;


	//markierten Datensatz	
	index = select = list.getSelectedItem();
	if (index == -1) 
		return;


	//in Arrayindex umrechnen
	index = translateSelectedIndexToArrayIndex(index,false);

	if (index >= m_pElementArray->GetSize()){
		XASSERT(index);
		return;
	}


	// Datensatz holen und merken
	Element *ele = m_pElementArray->get_element_at(index);
	XASSERT(ele);
	if (!ele) 
		return;

	//Element kopieren
	Element *pEleMerk = new Element;
	pEleMerk->copy(*ele);
	
	
	///Wenn Endeelement auf Abgang, dann auch merken
	Element *pAnEle = NULL;
	if (index+1 < m_pElementArray->GetSize())
	{
		Element *elem = m_pElementArray->get_element_at(index+1);
		XASSERT(elem);
		if (elem && elem->get_abstRel() == 0 && elem->isEleAbgEnde())
		{
			pAnEle = new Element;
			pAnEle->copy(*elem);
		}
	}

	
	//Altes Element löschen
	deleteElement(index);

	//Absolutabstand des neues Elementes setzen
	if (index-1 >= 0) 
	{
		Element *prev = m_pElementArray->get_element_at(index-1);
		XASSERT(prev);
		if (prev) 
			abstAbs = prev->get_abstAbs() + abstRel; 	
	}
	else
		abstAbs = abstRel;


	//Werte aus den Eingabefeldern lesen
	Element *newEle = new Element;
	getAttributes(newEle);

	//neues Element einfügen
	if (!insertElement(index, id, newEle, true))
	{

		int id;

		//einfügen war nicht möglich, altes Element wieder einfügen
		if (pAnEle)
		{
			//Element hatte ein Annschlußelement
			id = translateElementIdToComboBox(pAnEle->get_id());
			delete pAnEle; pAnEle = NULL;
		}
		else
			id = pEleMerk->get_id();

		//damit Plausis durchgeführt werden
		set_disableCheckPlausis(false);

		//altes Element wieder einfügen
		insertElement(index,id,pEleMerk);
		pEleMerk = NULL;
	}



	//evt. Speicher freigeben
	if (pAnEle)
		delete pAnEle;

	if (pEleMerk)
		delete pEleMerk;

	
	 
	show_elements(-1);
	

	list.selectItem(select);	
	doUpdate();
}


/**
	Klick auf Button "Löschen" 
	Löscht den markierten Datansatz aus der ComboBox und 
	löscht die Daten aus dem Array je nach Index.  
*/
void ElementeEingabeForm::onElemDelete()
{
	
	UpdateData();
	// Datensatz makiert?
	int index = list.getSelectedItem();
	if (index == -1) 
		return;

	//in Arrayindex umrechnen
	index = translateSelectedIndexToArrayIndex(index,true);


	deleteElement(index);

	show_elements(-1);
	UpdateData();
	doUpdate();	
}


/**
Element aus dem Array löschen, 
wenn es ein Endelement auf Abgang ist, den Abgang auch löschen,
wenn Abgang Endelement hat, Endelement auch löschen
*/
bool ElementeEingabeForm::deleteElement(int index)
{
	if (index < 0 || index >= m_pElementArray->GetSize()){
		XASSERT(index);
		return false;
	}

	// Element aus Array entfernen und loeschen
	Element *ele = m_pElementArray->get_element_at(index);
	XASSERT(ele);
	if (!ele) 
		return false;
	
	
	if (ele && ele->get_abstRel() == 0 && ele->isEleAbgEnde())
	{
		//Wenn es ein Endelement ist, Rohrstutzen auch löschen
		if(index-1 >= 0)
		{
			Element* elem = m_pElementArray->get_element_at(index-1);
			XASSERT(elem);

			if (elem->get_id() == ROHR)
			{
				m_pElementArray->RemoveAt(index-1);
				delete elem;

				index--;
			}
		}
	}
	else if (index+1 < m_pElementArray->GetSize())
	{	
		// Wenn Abgang auch Endelement hat dieses auch löschen

		Element* elem = m_pElementArray->get_element_at(index+1);
		XASSERT(elem);
		if (elem && elem->get_abstRel() == 0 && elem->isEleAbgEnde())
		{
			m_pElementArray->RemoveAt(index+1);
			delete elem;
		}
	}



	m_pElementArray->RemoveAt(index);  
	delete ele;

	if (changeRelAbst)
		set_elemAbstRel(index);
	else
		set_elemAbstAbs();


	return true;
}

/**
ein neues Element in das Array einfügen
*/
bool ElementeEingabeForm::insertElement(int index, int elId, Element* newEl, bool forceinsert /* Bei Änderungen darf Element auch bei Fehlern nicht gelöscht werden*/)
{
	if (!newEl || elId <= 0)
		return false;

	Element *abschlussEl = NULL;
	
	if (elId > OFFSETID  && elId != SPRINKLERSCHELLE_VIROTEC && elId != POTENIALAUSGLEICH)
	{
		//Abschlußelement definieren
		abschlussEl = new Element;
		abschlussEl->copy(*newEl);

		//Abschlußelementid zurückrechnen
		int abId = translateElementIdFromComboBox(elId);

		abschlussEl->set_id(abId);
		abschlussEl->set_laenge(0);  
		abschlussEl->set_abstRel(0);
		abschlussEl->set_anschlussDN(newEl->get_dn());

		//Element als Rohrstutzen
		newEl->set_id(ROHR);
	}

	int bSprinklerFound = 0;
	for(int i=0; i < m_pElementArray->GetSize(); i++)
	{
		Element *elAv = m_pElementArray->get_element_at(i);
		if(elAv->get_id() == SPRINKLERSCHELLE_VIROTEC)
		{
			bSprinklerFound++;
		}
	}

	//getStammdaten().getText("IDS_SPRINKLER_GEWAEHLT").c_str();
	if(newEl->get_id() == SPRINKLERSCHELLE_VIROTEC &&  bSprinklerFound == 0 && AfxMessageBox(getStammdaten().getText("IDS_SPRINKLER_GEWAEHLT").c_str(), MB_ICONEXCLAMATION | MB_YESNO) == IDNO)
	{
		return false;	
	}

	//Element in das Array einfügen
	if (index < 0 || index >= m_pElementArray->GetSize())
		m_pElementArray->Add(newEl);
	else
		m_pElementArray->InsertAt(index, newEl);

	//evt. Abschlußelement setzen
	if (abschlussEl)
	{
		index++;
		if (index < 0 || index >= m_pElementArray->GetSize())
			m_pElementArray->Add(abschlussEl);
		else
			m_pElementArray->InsertAt(index, abschlussEl);
	}

	Strangrohr *rohr = dynamic_cast<Strangrohr*>(get_data()->get_werkauftrag());
	if(rohr)
	{
		if(newEl->get_id() == SPRINKLERSCHELLE_VIROTEC)
		{	
			for (int i=0; i<Strangrohr::MAX_ANZAHL_STDELEMENTE; i++)
			{				
				rohr->set_muffenRelAbst( i, 0);
			}
		}
	}

	//Abstände neu setzen
	if (changeRelAbst)
		 set_elemAbstRel(index);
	else	
		set_elemAbstAbs();



	//Prüfen ob neue Elemente alle Bedingungen erfüllen
	if (!check_plausis() && !forceinsert)
	{
		//Element(e) wieder aus der Liste löschen
		deleteElement(index);

		return false;
	}

	return true;
}


void ElementeEingabeForm::onElemNew()
{
	/**
	Neues Element einfügen 
	Eine Spezialbehandlung erfolgt bei ROHRNUT. Dies ist ein 'Makro' für das Einfügen von Rohrstutzen mit Nut.
	*/
	Rohrleitung *rohr = dynamic_cast<Rohrleitung*>(get_data()->get_werkauftrag());
	XASSERT(rohr);
	if (!rohr)
		return;		

	UpdateData();

	//Absolutabstände berechnen
	if (!set_elemAbstAbs())
		return;

	int select = list.getSelectedItem();

	//in Arrayindex umrechnen
	int index = translateSelectedIndexToArrayIndex(select,true);

	if (index >= m_pElementArray->GetSize()){
		XASSERT(index);
		return;
	}

	// Absoluten Abstand vom letzten Element berechnen
	if (index >= 0 && changeRelAbst) //???
	{
		Element *prev = m_pElementArray->get_element_at(index);
		XASSERT(prev);
		if (prev) 
			abstAbs = prev->get_abstAbs() + abstRel; 	
	}
	
	Element *ele = new Element;
	getAttributes(ele);


	//Element in Array einfügen
	if (!insertElement(index+1,id,ele))
	{
		//Fokus wieder auf Eingabe Button
		//CButton *bt = (CButton*)GetDlgItem(IDS_ELEM_NEW);
		//bt->SetFocus();
	}
	else
		show_elements(select+1);		

	doUpdate();
}

///Werte uebergeben
void ElementeEingabeForm::setAttributes(Element *ele)
{
	if (!ele)
		return;
	pos				= ele->get_pos();
	id				= ele->get_id(); 
	abstAbs			= ele->get_abstAbs(); 
	abstRel			= ele->get_abstRel(); 
	anschlussDN		= ele->get_anschlussDN(); 
	dn				= ele->get_dn(); 
	laenge			= ele->get_laenge(); 
	durchmesser		= ele->get_durchmesser();
	winkel			= ele->get_winkel(); 
	makeAnlage		= ele->get_makeAnlage();
}

///Werte uebergeben
void ElementeEingabeForm::getAttributes(Element *ele)
{
	ele->set_abstAbs(abstAbs);
	ele->set_abstRel(abstRel);
	ele->set_anschlussDN(anschlussDN);
	ele->set_dn(dn);
	ele->set_id(id);
	ele->set_laenge(laenge);
	ele->set_pos(pos);
	ele->set_winkel(winkel);
	ele->set_makeAnlage(makeAnlage);
	ele->set_durchmesser(durchmesser);
}

//Indicates that the user has clicked the left mouse button in the control
void ElementeEingabeForm::onClickElemdaten(NMHDR* pNMHDR, LRESULT* pResult)
{
	UNREFERENCED_PARAMETER(pNMHDR);
	*pResult = 1;

	elem_setFields();
}

//Indicates that a key has been pressed
void ElementeEingabeForm::onKeydownElemdaten(NMHDR* pNMHDR, LRESULT* pResult)
{
	UNREFERENCED_PARAMETER(pNMHDR);
	*pResult = 1;

	PostMessage(WM_COMMAND, WM_UPDATE_FIELDS);
}


//Indicates the edit control is losing the input focus
void ElementeEingabeForm::onKillfocusElement_abstrel()
{
	UpdateData();	
	set_elemAbstAbs();
	UpdateData();
}

///Enablen/Disablen von Eingabefeldern je nach ElementID
void ElementeEingabeForm::enableDisableFields()
{
	int elemId = id;

	///Umschaltung Feld Durchmesser(Bohrung)/Laenge(andere)
	CString str; str = getStammdaten().getText("IDS_LAENGE_TEXT").c_str();
	SetDlgItemText(IDC_LAENGE_DURCHMESSER, str);
	GetDlgItem(IDC_ELEMENTE_DURCHMESSER)->ModifyStyle(WS_VISIBLE, NULL);
	GetDlgItem(IDC_ELEMENTE_LAENGE)->ModifyStyle(NULL, WS_VISIBLE);

	
	if (elemId == NUTST || elemId == STDSTUTZEN || elemId == GWRST || elemId == MUFFE || elemId == STDMUFFE || elemId == ROHRNIPPEL) 
	{
		GetDlgItem(IDC_ELEMENTE_ABSTREL)->EnableWindow(TRUE);
		GetDlgItem(IDC_ELEMENTE_ELEDN  )->EnableWindow(TRUE);
		GetDlgItem(IDC_ELEMENTE_LAENGE )->EnableWindow(FALSE);
		GetDlgItem(IDC_ELEMENTE_WINKEL )->EnableWindow(TRUE);
	} 
	else if ( elemId == BOHR50 || elemId == BOHR24 || elemId == KNICK) 
	{
		GetDlgItem(IDC_ELEMENTE_ABSTREL)->EnableWindow(TRUE);
		GetDlgItem(IDC_ELEMENTE_ELEDN  )->EnableWindow(FALSE);
		GetDlgItem(IDC_ELEMENTE_LAENGE )->EnableWindow(FALSE);
		GetDlgItem(IDC_ELEMENTE_WINKEL )->EnableWindow(TRUE);

		if (elemId == KNICK)
			str = getStammdaten().getText("IDS_KNICK_TEXT").c_str();
		else
			str = getStammdaten().getText("IDS_DURCHMESSER_TEXT").c_str();
		SetDlgItemText(IDC_LAENGE_DURCHMESSER, str);
		GetDlgItem(IDC_ELEMENTE_DURCHMESSER)->ModifyStyle(NULL, WS_VISIBLE);
		GetDlgItem(IDC_ELEMENTE_LAENGE)->ModifyStyle(WS_VISIBLE, NULL);
	} 
	else if (elemId == KRED || elemId == EINZ) 
	{
		GetDlgItem(IDC_ELEMENTE_ABSTREL)->EnableWindow(TRUE);
		GetDlgItem(IDC_ELEMENTE_ELEDN  )->EnableWindow(TRUE);
		GetDlgItem(IDC_ELEMENTE_LAENGE )->EnableWindow(FALSE);
		GetDlgItem(IDC_ELEMENTE_WINKEL )->EnableWindow(FALSE);
	}
	else if (elemId == ERED) 
	{
		GetDlgItem(IDC_ELEMENTE_ABSTREL)->EnableWindow(TRUE);
		GetDlgItem(IDC_ELEMENTE_ELEDN  )->EnableWindow(TRUE);
		GetDlgItem(IDC_ELEMENTE_LAENGE )->EnableWindow(FALSE);
		GetDlgItem(IDC_ELEMENTE_WINKEL )->EnableWindow(TRUE);
	}
	else if (elemId == ELEMENT_BOGEN || elemId == BOG2S) 
	{ 
		GetDlgItem(IDC_ELEMENTE_ABSTREL)->EnableWindow(TRUE);
		GetDlgItem(IDC_ELEMENTE_ELEDN  )->EnableWindow(FALSE);
		GetDlgItem(IDC_ELEMENTE_LAENGE )->EnableWindow(FALSE);
		GetDlgItem(IDC_ELEMENTE_WINKEL )->EnableWindow(TRUE);
	}
	else if( elemId == SPRINKLERSCHELLE_VIROTEC)
	{
		CString str = getStammdaten().getText("IDS_BOHRUNG").c_str();
		SetDlgItemText(IDC_LAENGE_DURCHMESSER, str);
		GetDlgItem(IDC_ELEMENTE_ABSTREL)->EnableWindow(TRUE);
		GetDlgItem(IDC_ELEMENTE_ELEDN  )->EnableWindow(TRUE);
		GetDlgItem(IDC_ELEMENTE_LAENGE )->EnableWindow(TRUE);
		GetDlgItem(IDC_ELEMENTE_WINKEL )->EnableWindow(FALSE);
	}
	else 
	{
		GetDlgItem(IDC_ELEMENTE_ABSTREL)->EnableWindow(TRUE);
		GetDlgItem(IDC_ELEMENTE_ELEDN  )->EnableWindow(TRUE);
		GetDlgItem(IDC_ELEMENTE_LAENGE )->EnableWindow(TRUE);
		GetDlgItem(IDC_ELEMENTE_WINKEL )->EnableWindow(TRUE);
	}
}

//Indicates the combo box is losing the input focus
void ElementeEingabeForm::onKillfocusElement_eledn()
{
	UpdateData();	

	Rohrleitung *rohr = dynamic_cast<Rohrleitung*>(get_data()->get_werkauftrag());
	XASSERT(rohr);
	if (!rohr)
		return;

	set_aktDnvl(rohr->get_dnvl());
	
	// bei Verkettung anschlussDN = dn von vorherigen Element 
	if ( !GetDlgItemInt(IDC_ELEMENTE_ABSTREL, 0, false) && m_pElementArray->GetSize() > 1) 
	{
		// letzten Datensatz holen
		Element *ele = m_pElementArray->get_element_at(m_pElementArray->GetSize()-1);  
		XASSERT(ele);
		if (ele && ele->get_winkel() == winkel)
			SetDlgItemInt(IDC_ELEMENTE_ANSDN, ele->get_dn());
		else
			SetDlgItemInt(IDC_ELEMENTE_ANSDN, aktDnvl);
	} 
	else 
		SetDlgItemInt(IDC_ELEMENTE_ANSDN, aktDnvl);

	if (id != ROHR && id != GLATT && id != NUT && id != Element::LEER) 
	{
		get_Stammdaten();		
		SetDlgItemInt(IDC_ELEMENTE_LAENGE, laenge, false);
	}

	UpdateData();	
}

void ElementeEingabeForm::onRclickElemdaten(NMHDR* pNMHDR, LRESULT* pResult)
{
	// Pointer auf NMHDR ist generisch: Er muss auf die richtige Struktur
	// gecastet werden. 
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	// Der Punkt im Clientwindow ist in ptAction gespeichert
	CPoint point( pNMListView->ptAction.x, pNMListView->ptAction.y);

	// Wenn nicht auf ein Item geklickt wurde, ist iItem gleich -1
	// Menü soll nur angezeigt werden, wenn ein Item angeklickt wurde
	if (pNMListView->iItem == -1) 
		return;

	// relative Koordinaten auf absolute umrechnen
	list.ClientToScreen(&point);

	menu_items items[] = 
	{
		ADD_MENU_ITEM(IDS_ELEM_NEW)
		ADD_MENU_ITEM(IDS_ELEM_CHANGE)
		ADD_MENU_ITEM(IDS_ELEM_DELETE)
		{-1, ""},
	};

	createPopupMenu(point, items);
	*pResult = 1;
}

//Indicates the combo box is losing the input focus
void ElementeEingabeForm::onSelChangeElement_id()
{
	UpdateData();	

	CDataExchange cb(this, false);

	
	Rohrleitung *rohr = dynamic_cast<Rohrleitung*>(get_data()->get_werkauftrag());
	XASSERT(rohr);
	if (!rohr)
		return;

	Strangrohr *strang = dynamic_cast<Strangrohr*>(get_data()->get_werkauftrag());
	// Hinweis bogen
	Element ele;
	ele.set_id(id);
	if (strang && ele.isEleBogen())
	{
		CString str; str = getStammdaten().getText("IDS_BOGEN_NO_ELE").c_str();
		AfxMessageBox(str, MB_OK | MB_ICONINFORMATION);
	}

	set_aktDnvl(rohr->get_dnvl());
	anschlussDN = aktDnvl;

	if (id == FFL || id == AFL || id == SFL  || id == VFL10  || id == VFL10NEU || id == VFL16NEU 
	 || id == VFL16 || id == NUT   || id == GEW || id == GLATT) 
	{
		///Relativer Abstand bei Endeelement immer 0
		abstRel = 0;

		///AnschlussDN ermitteln
		// Datensatz makiert, dann diesen nehmen
		int index = list.getSelectedItem();

		//in Arrayindex umrechnen
		index = translateSelectedIndexToArrayIndex(index,false);


		if (index >= m_pElementArray->GetSize()){
			XASSERT(index);
			return;
		}		

		if (index >= 0)
		{
			Element *ele = m_pElementArray->get_element_at(index);
			XASSERT(ele);
			if (ele) 
				anschlussDN = ele->get_dn(); 
		}
		// sonst letzten Datensatz holen
		else if (m_pElementArray->GetSize() > 0) 
		{
			Element *ele = m_pElementArray->get_element_at(m_pElementArray->GetSize()-1);  
			XASSERT(ele);
			if (ele) 
				anschlussDN = ele->get_dn(); 
		} 

		///Laenge/Durchmesser des Elements aus den Stammdaten holen
		get_Stammdaten();		
		
		SetDlgItemInt(IDC_ELEMENTE_ABSTREL, abstRel,     false);
		SetDlgItemInt(IDC_ELEMENTE_LAENGE,  laenge,      false);
		SetDlgItemInt(IDC_ELEMENTE_ANSDN,   anschlussDN, false);
	} 
	else if (id == NUTST || id == GWRST || id == MUFFE || id == ROHRNIPPEL) 
	{
		get_Stammdaten();		
		
		SetDlgItemInt(IDC_ELEMENTE_ANSDN,  anschlussDN, false);
		SetDlgItemInt(IDC_ELEMENTE_LAENGE, laenge,      false);
	} 
	else if (id == BOHR50 || id == BOHR24) 
	{
		dn = 0;
		get_Stammdaten();		
		
		//SetDlgItemInt(IDC_ELEMENTE_ELEDN,  dn,			false);
		ddx_cbdata(&cb, IDC_ELEMENTE_ELEDN, dn); //Wert nicht über SetDlgItemInt, da Combobox vom Typ Dropdown-Listenfeld ist
		SetDlgItemInt(IDC_ELEMENTE_ANSDN,  anschlussDN, false);
		SetDlgItemInt(IDC_ELEMENTE_LAENGE, laenge,      false);
	}
	else if (id == KRED || id == ERED || id == EINZ) 
	{
		winkel      = 0;
		get_Stammdaten();	
		if (id == EINZ)
			laenge = 0;
		SetDlgItemInt(IDC_ELEMENTE_WINKEL, winkel,      false);
		SetDlgItemInt(IDC_ELEMENTE_ANSDN,  anschlussDN, false);
		SetDlgItemInt(IDC_ELEMENTE_LAENGE, laenge,      false);
	} 
	else if (id == ELEMENT_BOGEN || id == BOG2S) 
	{ 
		dn          = aktDnvl;
		get_Stammdaten();		

		//SetDlgItemInt(IDC_ELEMENTE_ELEDN,  dn,          false);
		ddx_cbdata(&cb, IDC_ELEMENTE_ELEDN, dn); //Wert nicht über SetDlgItemInt, da Combobox vom Typ Dropdown-Listenfeld ist
		SetDlgItemInt(IDC_ELEMENTE_ANSDN,  anschlussDN, false);
		SetDlgItemInt(IDC_ELEMENTE_LAENGE, laenge,      false);

	} 
	else 
	{
		SetDlgItemInt(IDC_ELEMENTE_ANSDN,  anschlussDN, false);
	}

	///Enablen/Disablen von Eingabefeldern je nach ElementID
	enableDisableFields();
}

/**
Rechnet den Listenindex auf den Arrayindex um.
Da in der Liste Rohrstutzen + Abschlußelement zusammengefaßt werden
\param index: Index der ComboBox
\param indexAufAbschlussEl: true = Wenn index auf einer Kombination Rohrstutzen + Abschlußelement steht wird der Index
								   des Abschlußelementes im Array zurückgegeben
						    false = Wenn index auf einer Kombination Rohrstutzen + Abschlußelement steht wird der Index
							        index des Rohrstutzen zurückgegeben
*/
int ElementeEingabeForm::translateSelectedIndexToArrayIndex(int index,bool indexAufAbschlussEl)
{
	if (index < 0)
		return -1;

	int iCount = 0;
	for (int i=0; i<m_pElementArray->GetSize(); i++)
	{
		Element *ele = getElement(i);  
		if (!ele) 
			continue;
		
		
		//Index des Arrays zurückgeben Variante 1
		if (!indexAufAbschlussEl)
		{
			if (iCount == index)
				return i;
		}

		//prüfen ob nächstes El Abschlußelement ist
		if (i+1 < m_pElementArray->GetSize())
		{
			Element *eleAb = getElement(i+1);  
			if (!eleAb) 
				continue;
			if (eleAb->isEleAbgEnde())
				i++;
		}
		
		//Index des Arrays zurückgeben Variante 2
		if (indexAufAbschlussEl)
		{
			if (iCount == index)
				return i;
		}
		
		
		iCount++;
	}

	return m_pElementArray->GetSize();
}


Element* ElementeEingabeForm::getElement(int index)
{
	if (index < 0 || index >= m_pElementArray->GetSize())
		return NULL;


	Element *ele = m_pElementArray->get_element_at(index);  
	XASSERT(ele);

	return ele;
}

/**
Berechnet zu der ID eines Abschlußelementes die ID für Rohrstutzen + Abschlußelement für die 
Combobox
*/
int ElementeEingabeForm::translateElementIdToComboBox(int elemId)
{
	return (ROHR * OFFSETID + elemId);
}

/**
Rechnet aus der ID der Combobox das Abschlußelement aus
*/
int ElementeEingabeForm::translateElementIdFromComboBox(int elemId)
{
	//Wenn elemID kein Rohr + Abschlußelement ist, elemId zurückgeben
	if (elemId < OFFSETID)
		return elemId;

	return (elemId - ROHR * OFFSETID);
}
