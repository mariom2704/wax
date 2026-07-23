#pragma once

#include "Spanx/SpanxPropPage.h"

class PropPageSystem : public SpanxPropPage
{
	DECLARE_DYNCREATE(PropPageSystem)
	DECLARE_MESSAGE_MAP()
public:
	PropPageSystem();


	~PropPageSystem();

	virtual BOOL OnApply();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    ///  DDX/DDV-Unterstützung
	virtual BOOL OnInitDialog();

	void onStammDBpfad(); 

private:
	/// Stammdatenbank
	CString paraStammDB;

	/// Serverdatenbank
	CString paraServerDB;

	/// Maximale Anzahl Positionen pro Sammelauftrag
	int paraMaxPosPerSA;

	/// Maximale Anzahl Positionen pro HTML-Seite
	int paraMaxPosPerSite;

	/// AV-Version
	bool paraAV;

	/// AV-Reiter in NL-Version
	bool paraDebugNL;

	/// Default-Werte setzen
	virtual void setDefault();

	/// Parameter auf Existenz checken
	void checkPara();

	/// Datei Dialog
	bool selectPfad(CString& pfad);

};



