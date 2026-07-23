#include "stdafx.h"
#include "IniParameter.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


/// Konstruktor
IniParameter::IniParameter(const string& inifile, int parakey, const string& entry, const string& def_str) 
: RegParameter(parakey, "SysParam", entry, def_str, false)
{
	ini = inifile + ".ini";
	loaded = false;
	load();
	nmspace = MACHINE;
}


IniParameter::~IniParameter()
{
}

int IniParameter::valid_namespaces() const
{
	/**
	Gültigkeit ist MACHINE, ein Eintrag in der Inidatei gilt immer für alle Benutzer eines Rechners
	*/
	return MACHINE;
}

bool IniParameter::is_loaded() const
{
	return loaded;
}

void IniParameter::load()
{
	char val[255];
	DWORD ret = ::GetPrivateProfileString(ordner.c_str(), entry.c_str(), "", val, 255, ini.c_str());
	if (ret)
	{
		val_str = val;
		//TRACE("LADEN %s.ini: %s = %s\n", ini.c_str(), entry.c_str(), val_str.c_str());
	}
	loaded = ret > 0 ? true : false;
}


void IniParameter::save()
{
	::WritePrivateProfileString(ordner.c_str(), entry.c_str(), val_str.c_str(), ini.c_str());
	//TRACE("SPEICHERN %s.ini: %s = %s\n", ini.c_str(), entry.c_str(), val_str.c_str());
}


