#include "stdafx.h"

#include "parameter.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


/// Konstruktor
Parameter::Parameter(int para_key)
{
	is_string = true;
	val_int = UNDEFINED;
	def_val_int = 0;
	key = para_key;

	// Default ist ein Parameter ungültig
	nmspace = INVALID;
	av = false;
}

Parameter::~Parameter()
{
}


string Parameter::get_val_str() const
{
	/**
	Liefert den Wert als String. Falls der Parameter ein Integer ist, wird vorher 
	ein String erzeugt
	*/
	if (!is_string)
	{
		char conv[255];
		sprintf(conv, "%i", val_int);
		val_str = conv;
	}
	return val_str.length() ? val_str : get_def_str();
}

int Parameter::get_val_int() const
{
	if (is_string)
	{
		val_int = atoi(get_val_str().c_str());
	}
	return val_int == UNDEFINED ? get_def_int() : val_int;
}


/// Setzt den aktuell möglichen Gültigkeitsbereich
bool Parameter::set_valid_namespace(enum VALID_NAMESPACE namesp)
{
	/**
	Setzen des Gültigkeitsbereichs, nur möglich, wenn namesp auch ein 
	möglicher Wert ist. Ein Registryparameter darf natürlich nicht auf LOCATION gesetzt werden
	*/
	if (!(namesp & valid_namespaces()))
		return false;
	nmspace = namesp;
	return true;
}



string Parameter::getBemerkung() const
{
	CString id;
	id.Format("WAX_PARAM_%i", key);

	string txt = TextDataList::getInstance()->getText(id.GetBuffer());

	return txt;
}
