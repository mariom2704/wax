#pragma once

#include "parameter.h"
#include "regparameter.h"
#include <map>
#include <vector>

class CDatabase;

class MxParameter
{
private:
	MxParameter();
public:
	//Konstruktor
	MxParameter(const std::string& userdb);
	
	//Destruktor
	~MxParameter();

	///Fuellen der Liste mit angegebenen Parametern
	void load();

	/// Default-Wert fuer Parameter, liefert std::string
	CString def_val_str(int key) const; 

	/// Default-Wert fuer Parameter, liefert Integer
	int def_val_int(int key) const; 
	
	/// Einstiegsmethode zum Finden eines Parameters, liefert Integer
	int val_int(int key)const; 

	/// Einstiegsmethode zum Finden eines Parameters, liefert std::string
	CString val_str(int key) const; 

	

	template <class T>
	void save_para(int key, const T& val);

	/// Sucht das Parameterobjekt in der Parametermap
	const Parameter *get_para_const(int key) const;

	/// Sucht das Parameterobjekt in der Parametermap
	Parameter *get_para(int key);

	inline CDatabase* get_db();

	/// Liefert das tempor�re Verzeichnis
	const std::string get_tempdir(const std::string& appname) const;
	
	/// Liefert den Login des angemeldeten Users
	const std::string get_username() const;

	/// Liefert eine Liste mit Userlogins
	const std::vector<std::string> get_userlst();

	/// Pr�ft, ob der Userlogin noch nicht in der Benutzertabelle eingetragen ist
	bool is_new_user(const std::string& login);

	/// Benutzer hinzuf�gen, Systemparameter erzeugen
	void add_user(const std::string& login);

	/// Passwortabfrage
	std::string get_passwd() const;

	std::string getBemerkung(int key) const;


	std::string getEmail();
	std::string getEmailFromDefaultUser();
	std::string getNLFromDefaultUser();

private:
	friend class PropPageSysAll;
	/// Parameterliste als map, Liste mit initialisierten Objekten
	typedef std::map<int, Parameter*> ParameterMap;

	ParameterMap pmap;

	///Loescht die Objekte und die Pointer aus der Liste
	void clear_list();

	/// Parameter aus Inidatei bzw. Userdatenbank laden
	Parameter* insert_para(int key, const std::string& entry, const std::string& def_str, bool av = false);
	
	/// Erzeugen und Hinzufuegen eines Parameters in die Liste
	void add_paralist(int parameter);

	/// Name der Userdatenbank mit den Systemparametern
	std::string db_name;

	/// Name des Users, dessen Systemparameter geladen werden sollen. Ist nur f�r 
	/// Parameter aus der Userdatenbank relevant
	std::string user;

	/// Userdatenbank
	CDatabase *db;

	// Nur damit das Programm im Fehlerfall nicht abst�rzt
	RegParameter *dummy_para;
};

template <class T>
void MxParameter::save_para(int key, const T& val)
{
	Parameter *para = get_para(key);
	para->set_val(val);
	para->save();
}

inline CDatabase* MxParameter::get_db()
{
	return db;
}
	