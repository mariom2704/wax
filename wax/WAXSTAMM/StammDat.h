#pragma once

#include <string>

#define DECLARE_COL_LANGUAGE_WAX(type, name, tab) type##Col name##(WaxSys::getLanguage().c_str()); tab.attach(&name##); 
#define DECLARE_COL_LANGUAGE_WAX_EXT(type, name, tab) type##Col name##((CString)_T(#name) + (CString)WaxSys::getLanguage().c_str()); tab.attach(&name##); 

/**
Basisklasse für Stammdatentabellen
Überschrieben werden muss die load Methode. Durch einen Aufruf von get_database()
wird das statische Datenbankobjekt erzeugt. Wird die Stammdatenbank nicht mehr 
gebraucht, muss reset_database() aufgerufen werden. 
*/
class WAXSTAMMDLL StammDat 
{
public:
	//Tabelleninitialisierung wird in abgeleiteten Klassen überschrieben
	 StammDat(const std::string& table, const std::string& sel, const std::string& ord);

 	//Destruktor löscht die Objektliste
	virtual ~StammDat();

	/// Liefert ein Handle auf die Stammdatenbank
	static DWORD get_database(const std::string& name);

	/// Löscht das statische Datenbankobjekt
	static void reset_database();
	
	const std::string& get_tbl() const;

	void set_tbl(const std::string& tblname);

	//Stammdaten löschen
	void unload();

	virtual void load(const std::string& db);
	
	const CObList& get_lst() const;

	CObList& get_lst();

protected:
	const std::string& get_select() const;

	const std::string& get_order() const;

	CObList lst;

private:

	/// Statischer Zeiger auf ein Datenbankobjekt
	static DWORD db;

	std::string tbl;

	std::string select;

	std::string order;
};
