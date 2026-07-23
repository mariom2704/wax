#include "stdafx.h"
#include "regparameter.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


/// Konstruktor
RegParameter::RegParameter(int parakey, const string& regordner, const string& regentry, const string& def_str, bool do_load) : Parameter(parakey)
{
	/**
	Lädt einen Parameter aus der Registry. Bei abgeleiteten Klassen muss load = false übergeben werden
	da virtuelle Methoden aus dem Konstruktor nicht richtig aufgelöst werden!
	*/
	ordner = regordner;
	entry = regentry;
	def_val_str = def_str;
	def_val_int = atoi(def_str.c_str());
	if (do_load)
		load();
	nmspace = USER;
}

/// Konstruktor
RegParameter::RegParameter(int parakey, const string& regordner, const string& regentry, int def_int, bool do_load) : Parameter(parakey)
{
	ordner = regordner;
	entry = regentry;
	def_val_int = def_int;
	char tmp[50];
	sprintf(tmp, "%i", def_int);
	def_val_str = tmp;
	is_string = false;
	if (do_load)
		load();
	nmspace = USER;
}


RegParameter::~RegParameter()
{
}

int RegParameter::valid_namespaces() const
{
	/**
	Registryparameter sind immer nur für den user gültig
	*/
	return USER;
}

void RegParameter::load()
{

	if (is_string)
	{
		val_str = (LPCSTR)AfxGetApp()->GetProfileString(ordner.c_str(), entry.c_str(), def_val_str.c_str());
	//	TRACE("LADEN Reg: %s = %s\n", entry.c_str(), val_str.c_str());
	}
	else
	{
		val_int = AfxGetApp()->GetProfileInt(ordner.c_str(), entry.c_str(), def_val_int);
	//	TRACE("LADEN Reg: %s = %i\n", entry.c_str(), val_int);
	}
}


void RegParameter::save()
{
	if (is_string)
	{
		AfxGetApp()->WriteProfileString(ordner.c_str(), entry.c_str(), val_str.c_str());
		//TRACE("SPEICHERN Reg: %s = %s\n", entry.c_str(), val_str.c_str());
	}
	else
	{
		AfxGetApp()->WriteProfileInt(ordner.c_str(), entry.c_str(), val_int);
		//TRACE("SPEICHERN Reg: %s = %i\n", entry.c_str(), val_int);
	}
}


bool RegParameter::is_readonly() const
{
	return false;
}



