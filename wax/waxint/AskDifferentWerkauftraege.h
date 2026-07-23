#pragma once

#include "ServerDBOpen.h"
#include <mxcontrols/medit.h>

class CObList;

/// Laden unter Verwendung eines PSP Elements. Es wird aus der Datenbank eine 
/// Projektdatei generiert.
class WAXINT AskDifferentWerkauftraege 
: public ServerDBOpen
{
private:
	Leitkarte lt;

	/// zeigt an, dass für das Projekt die Projektleitkarte erstellt werden soll. 
	/// Kann u.U. Probleme bereiten, da die Daten vererbt werden.
	BOOL projektLeitkarteErstellen;

	int iSearchOption;

	/// zeigt an ob die letzten drei Ziffern genau übereinstimmen müssen
	BOOL checkLetzteZiffern;

	long lWanr;
	__int64 SDAuftragsNr;


	/// zeigt an ob die Produktinie geprüft werden soll
	BOOL alleProduktlinien;

	CComboBox nlkennung;

	FlexMaskedEdit edpspelement;

public:
	/// Speichert das Projekt
	virtual bool serialize_project(CObList* project);

	/// Dialogelemente initialisieren
	virtual BOOL OnInitDialog();

	AskDifferentWerkauftraege();

	virtual ~AskDifferentWerkauftraege();

	virtual BOOL DestroyWindow();

	virtual void DoDataExchange(CDataExchange* pDX);

	/// Import starten
	virtual void OnOK();

protected:

	void EnableRadioPspElement(BOOL bEnable);
	void EnableRadioWerkauftrag(BOOL bEnable);
	void EnableRadioSDAuftrag(BOOL bEnable);

	virtual const CString createSQLSelect() const;

	virtual const CString createSQLSort(CString tablename) const;

	/// Indicates a new combo box list item is selected
	 void onSelchangeNl();
	/// Indicates the user has changed text in the edit control
	 void onEditchangeNl();
	/// Indicates the user clicked a button
	 void onClickedSpez();
	/// Indicates the edit control is losing the input focus
	 void onFormatPspelement();
	
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedRadioPspElement();
	afx_msg void OnBnClickedRadioWerkauftrag();
	afx_msg void OnBnClickedRadioSDAuftrag();
	afx_msg void OnBnClickedLetzteZiffernDrei();
	afx_msg void OnBnClickedLetzteZiffernVier();
};
