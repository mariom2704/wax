#if !defined __IniParameter
#define __IniParameter

#pragma once

#include "regparameter.h"


/**
Parameter aus der Inidatei. Benutzt wird die uralte Funktion Write/GetPrivateProfileInt/String.
Eine Inidatei wird zuerst im Programmverzeichnis gesucht und dann im Windowsverzeichnis.
*/
class IniParameter : public RegParameter
{
public:
	///Konstruktor für einen Stringparameter
	IniParameter(const string& inifile, int parakey, const string& entry, const string& def_str );
	
	///Destruktor
	~IniParameter();

	/// Laden des Parameter, woher auch immer
    virtual void load();

	/// Speichern des Parameters
	virtual void save();

	bool is_loaded() const;

	/// Mögliche Gültigkeitsbereiche
	virtual int valid_namespaces() const;

private:
	/// Name der Inidatei im Windows oder Programmverzeichnis
	string ini;

	/// wird auf true gesetzt, wenn der Parameter tatsächlich in der Ini gefunden wurde
	bool loaded;
};
#endif 

