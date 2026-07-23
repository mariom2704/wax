#pragma once

#include "ServerDBOpen.h"
#include <mxcontrols/medit.h>

class CObList;

/// Laden unter Verwendung eines PSP Elements. Es wird aus der Datenbank eine 
/// Projektdatei generiert.
class WAXINT ServerDBPSPElement 
: public ServerDBOpen
{
private:
	Leitkarte lt;

	/// zeigt an, dass für das Projekt die Projektleitkarte erstellt werden soll. 
	/// Kann u.U. Probleme bereiten, da die Daten vererbt werden.
	BOOL projektLeitkarteErstellen;

	/// zeigt an ob die letzten drei Ziffern genau übereinstimmen müssen
	BOOL checkLetzteZiffern;

	/// zeigt an ob die Produktinie geprüft werden soll
	BOOL alleProduktlinien;

	CComboBox nlkennung;

	FlexMaskedEdit edpspelement;

public:
	/// Speichert das Projekt
	virtual bool serialize_project(CObList* project);

	/// Dialogelemente initialisieren
	virtual BOOL OnInitDialog();

	ServerDBPSPElement();

	virtual ~ServerDBPSPElement();

	virtual BOOL DestroyWindow();

	virtual void DoDataExchange(CDataExchange* pDX);


protected:
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
	afx_msg void OnBnClickedLetzteZiffernVier();
	afx_msg void OnBnClickedLetzteZiffernDrei();
};
