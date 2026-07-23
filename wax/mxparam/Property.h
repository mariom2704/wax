///  Property.h : Header-Datei
/// 
///  Diese Klasse definiert ein benutzerdefiniertes modales Eigenschaftsblatt 
///  Property.
 
#ifndef __PROPERTY_H__
#define __PROPERTY_H__

#include "PropPageAllgemein.h"
#include "PropPageAnsicht.h"
#include "PropPageGewinderohr.h"
#include "PropPageGraphik.h"
#include "PropPageFreigabe.h"
#include "PropPageProjekt.h"
#include "PropPageSysAll.h"
#include "PropPageUser.h"
#include "mxparameter.h"
#include <afxdlgs.h>

/**
Einstellungsdialoge 
*/
class Property : public CPropertySheet
{
	DECLARE_MESSAGE_MAP()
private:
	bool check_passwd;

	MxParameter* parameter;
public:
	/// Tabnummerierung, zur Auswahl eines bestimmten Tabs
	static const int MAX_PAGES = 10;	

	Property(CWnd* pWndParent, MxParameter* para, bool check_passwd = true);

	virtual ~Property();

	bool check_password();

	/// Tabnummerierung, zur Auswahl eines bestimmten Tabs
	enum SHEETS
	{  
		ALLGEMEIN = 0,	
		ANSICHT = 1,
		PROJEKT = 2,
		GEWINDEROHR = 3,
		GRAPHIK = 4,
		FREIGABE = 5,
		USER = 6,
		SYSTEM = 7,
	};		
private:

	/// Defaultwerte des ausgewählten Tabs setzen
	void set_default();

    /// Allgemein, nicht zuordbar
	PropPageAllgemein ppAllgemein;
	
	/// Ansicht
	PropPageAnsicht ppAnsicht;

	/// Freigabe, Status, Email
	PropPageFreigabe ppFreigabe;

	/// Gewinderohr
	PropPageGewinderohr ppGewinderohr;

	/// Graphik
	PropPageGraphik ppGraphik;

	/// Projekt
	PropPageProjekt ppProjekt;

	/// System
	//PropPageSystem ppSystem;

	/// System Alle
	PropPageSysAll ppSysall;

	/// User
	PropPageUser ppUser;



	CButton default_button;
protected:
	virtual BOOL OnInitDialog( );

	
};


#endif
