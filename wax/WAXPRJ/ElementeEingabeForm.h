#pragma once

#include "WaxPartForm.h"
#include <mxcontrols/listctrl.h>
#include <mxcontrols/floatedit.h>
#include <mxcontrols/CtrlComboBoxDn.h>


class WAXPRJDLL ElementeEingabeForm : public WaxPartForm  
{
	DECLARE_MESSAGE_MAP()
public:
	
	ElementeEingabeForm(UINT idd = 0, const CString& chapter = "");

	virtual ~ElementeEingabeForm();

	virtual BOOL OnInitDialog();
		
	void onElemChange();

	void onElemDelete();

	void onElemNew();

	


protected:

	EnContainerRohrElemente* m_pElementArray;	// Element Array  wird in dieser Klasse für einen  Zeiger auf ein  ElementArray benutzt 
												// ElementeEingabeForm::OnInitDialog() => &get_rohrleitung()->getElementArray(); <=
												// im
												// AvElementeEingabe::OnInitDialog() => &get_rohrleitung()->get_avelementArray(); <=

	FloatEditCtrl ed_durchmesser;

	CtrlComboBoxDn dnComboBox;

	double durchmesser; 

	bool changeRelAbst;

	void set_elemAbstRel(int &index);

	int abstAbs;

	int abstRel;

	int aktDnvl;

	int anschlussDN;

	int dn;

	int id;

	int laenge;

	int pos;

	bool makeAnlage;

	int winkel;

	ListCtrl list;

	UINT m_iIDCNew;

	/// Enablen/Disablen von Eingabefeldern je nach ElementID
	virtual void enableDisableFields();

	/// Werte uebergeben
	void setAttributes(Element *ele);

	/// Werte holen
	void getAttributes(Element *ele);
	
	virtual void DoDataExchange(CDataExchange* pDX);

	

	virtual void doInsert(Element* ele, int index);

	virtual bool deleteElement(int index);							///  Element aus der Liste löschen

	virtual bool insertElement(int index, int id, Element* newEl, bool forceinsert = false);  ///  neues Element in das Array einfügen

	virtual void elem_setFields();

	/// Zeigt die Elemente in der Liste an
	virtual void show_elements(int select);

	/// Laenge/Durchmesser des Elements aus den Stammdaten holen
	void get_Stammdaten();

	virtual void set_aktDnvl(int dnvl);

	bool set_elemAbstAbs();


	int translateSelectedIndexToArrayIndex(int index,bool indexAufAbschlussEl);

	int translateElementIdToComboBox(int elemId);
	int translateElementIdFromComboBox(int elemId);
	
	Element* getElement(int index);

	/// Indicates the combo box is losing the input focus
	void onSelChangeElement_id();
	
	/// Indicates that the user has clicked the right mouse button in the control
	void onRclickElemdaten(NMHDR* pNMHDR, LRESULT* pResult);
	
	/// Indicates the combo box is losing the input focus
	void onKillfocusElement_eledn();
	
	/// Indicates the edit control is losing the input focus
	void onKillfocusElement_abstrel();
	
	/// Indicates that a key has been pressed
	void onKeydownElemdaten(NMHDR* pNMHDR, LRESULT* pResult);
	
	/// Indicates that the user has clicked the left mouse button in the control
	void onClickElemdaten(NMHDR* pNMHDR, LRESULT* pResult);
	
	/// Called after the size of CWnd has changed.
	void OnSize(UINT nType, int cx, int cy);
	
	enum COLUMNS 
	{
		LST_POSNR_ABK = 0,
		LST_ID = 1,
		LST_ANSCHLUSSDN = 2,
		LST_DN = 3,
		LST_LAENGE = 4,
		LST_WINKEL = 5, 
		LST_RELABST	= 6,
		LST_ABSABST	= 7,
		LST_ANLAGE = 8
	};

	enum DEF
	{
		OFFSETID	= 100  ///  Multiplikator für RohrstutzenID + Abschlußelement
	};

};
