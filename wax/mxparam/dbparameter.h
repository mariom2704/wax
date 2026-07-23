#if !defined __DbParameter
#define __DbParameter

#pragma once

#include "parameter.h"
class MXDatabase;

class DbParameter : public Parameter
{
public:
	///Konstruktor für einen Stringparameter
	DbParameter(MXDatabase *userdb, const string& cur_user, int parakey, const string& entry, const string& def_str);
	
	///Destruktor
	~DbParameter();

	/// Laden des Parameter, woher auch immer
    virtual void load();

	/// Speichern des Parameters
	virtual void save();

	/// Liefert true wenn der Parameter nicht bearbeitet werden darf
	virtual bool is_readonly() const;

	/// Mögliche Gültigkeitsbereiche
	virtual int valid_namespaces() const;

private :
	MXDatabase *db;

	/// Spalte Benutzer in Systemparameter
	string user;
};
#endif 

